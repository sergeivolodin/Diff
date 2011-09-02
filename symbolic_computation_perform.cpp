#include "symbolic_computation.h"
#include <vector>
#include <iostream>
#include <string.h>
#include <map>
using std::vector;
using std::string;
using std::map;
using std::cerr;
using std::endl;
bool symbolic_computation::set_contains(string set,string needle) {
    return(set.find(needle)!=string::npos);
}

bool symbolic_computation::replacer_add(symbolic_computation::replacers* a,string x,tree *y) {
    if(a->find(x)==a->end()) {
        ((*a)[x])=y;
        return(true);
    }
    return(false);
}

bool symbolic_computation::replacer_add(symbolic_computation::replacers* a,string x,tree::tree_leaf_t y) {
    if(a->find(x)==a->end()) {
        tree* aa=new tree(y);
        (*a)[x]=*aa;
        delete aa;
        return(true);
    }
    return(false);
}

bool symbolic_computation::pattern(tree *stree, tree *spattern,string base,symbolic_computation::replacers *d) {
    tree::tree_mathtype_t ttype=stree->mathtype(this);
    tree::tree_mathtype_t ptype=spattern->mathtype(this);
    if(ptype==ttype) {
        if(ttype==tree::TREE_MNUMBER) {
            if(stree->getvalue().compare(spattern->getvalue())) {
                return(false);
            }
        }
        if(ttype==tree::TREE_MVARIABLE) {
            if(set_contains(set_all,spattern->getvalue())) {
                if(!replacer_add(d,spattern->getvalue(),stree)) {
                    return(false);
                }
            }
            else if(set_contains(set_variables,spattern->getvalue())) {
                if(!stree->getvalue().compare(base)) {
                    if(!replacer_add(d,spattern->getvalue(),stree)) {
                        return(false);
                    }
                }
                else return(false);
            }
            else if(set_contains(set_numbers,spattern->getvalue())) {
                if(stree->getvalue().compare(base)) {
                    if(!replacer_add(d,spattern->getvalue(),stree)) {
                        return(false);
                    }
                }
                else return(false);
            }
            else if(stree->getvalue().compare(spattern->getvalue())) {
                return(false);
            }
        }
        if(ttype==tree::TREE_MFUNCTION) {
            if(set_contains(set_all,spattern->getvalue())) {
                if(!replacer_add(d,spattern->getvalue(),stree->getvalue())) {
                    return(false);
                }
                if(!pattern(stree->geta(),spattern->geta(),base,d)) {
                    return(false);
                }
            }
            else if(!spattern->getvalue().compare(stree->getvalue())) {
                 if(!pattern(stree->geta(),spattern->geta(),base,d)) {
                    return(false);
                }
            }
            else return(false);
        }
        if(ttype==tree::TREE_MUNARY) {
            if(!stree->getvalue().compare(spattern->getvalue())) {
                if(!pattern(stree->geta(),spattern->geta(),base,d)) {
                    return(false);
                }
            }
            else return(false);
        }
        if(ttype==tree::TREE_MBINARY) {
            if(!stree->getvalue().compare(spattern->getvalue())) {
                if(!pattern(stree->geta(),spattern->geta(),base,d)) {
                    return(false);
                }
                if(!pattern(stree->getb(),spattern->getb(),base,d)) {
                    return(false);
                }
            }
            else return(false);
        }
    }
    else {

        if(ptype==tree::TREE_MVARIABLE) {

            if(set_contains(set_all,spattern->getvalue())) {
                if(!replacer_add(d,spattern->getvalue(),stree)) {
                    return(false);
                }
            }
            else if(set_contains(set_numbers,spattern->getvalue())) {
                if(ttype==tree::TREE_MBINARY) {
                    if((stree->geta()->contains(base))||(stree->getb()->contains(base))) {
                        return(false);
                    }
                    if(!replacer_add(d,spattern->getvalue(),stree)) {
                        return(false);
                    }
                }
                if(ttype==tree::TREE_MUNARY||ttype==tree::TREE_MFUNCTION) {
                    if(stree->geta()->contains(base)) return(false);
                    if(!replacer_add(d,spattern->getvalue(),stree)) {
                        return(false);
                    }
                }
                if(ttype==tree::TREE_MNUMBER) {
                    if(!replacer_add(d,spattern->getvalue(),stree)) {
                        return(false);
                    }
                }
            }
            else return(false);
        }
        else return(false);
    }
    return(true);
}

tree* symbolic_computation::getreplacer(symbolic_computation::replacers* where,string what) {
    replacers::iterator it;
    if(where) {
        it=where->find(what);
        if(it!=where->end()) {
            return(it->second.copy());
        }
        return(NULL);
    }
    return(NULL);
}

tree* symbolic_computation::replace(tree* spattern,symbolic_computation::replacers* srepl) {
    if(!spattern) return(NULL);
    if(!srepl) return(spattern);
    tree::tree_mathtype_t stype=spattern->mathtype(this);
    tree *rtree=NULL,*tmp=NULL;
    if(stype==tree::TREE_MVARIABLE) {
        srepl->find(spattern->getvalue());
        rtree=getreplacer(srepl,spattern->getvalue());
        if(!rtree) return(spattern);
    }
    else if(stype==tree::TREE_MNUMBER) rtree=spattern->copy();
    else if(stype==tree::TREE_MUNARY) {
        rtree=new tree(spattern->getvalue(),replace(spattern->geta(),srepl));
    }
    else if(stype==tree::TREE_MFUNCTION) {
        if((tmp=getreplacer(srepl,spattern->getvalue()))) {
            rtree=new tree(tmp->getvalue(),replace(spattern->geta(),srepl));
            delete tmp;
        }
        else {
            rtree=new tree(spattern->getvalue(),replace(spattern->geta(),srepl));
        }
    }
    else if(stype==tree::TREE_MBINARY) {
        rtree=new tree(replace(spattern->geta(),srepl),spattern->getvalue(),replace(spattern->getb(),srepl));
    }
    return(rtree);
}

tree* symbolic_computation::operate(tree* src,symbolic_computation::rules* crules,string operation,string base,bool rmode) {
    replacers* tmprepl=new replacers;
    tree* res;
    string tmp;
    if(!rmode) {
        for(rules::iterator it=crules->begin();it!=crules->end();it++) {
            if(pattern(src,&(it->first),base,tmprepl)) {
                res=replace(
                     replace(
                      operate(
                       replace(&(it->second.destination),tmprepl),
                      crules,operation,base,true),
                     &(it->second.post_replace)),
                    tmprepl);
                tmprepl->clear();
                delete tmprepl;
                return(res);
            }
        }
        return(NULL);
    }
    else {
        tree::tree_mathtype_t type=src->mathtype(this);
        if(type==tree::TREE_MUNARY) {
            return(new tree(src->getvalue(),operate(src->geta(),crules,operation,base,true)));
        }
        if(type==tree::TREE_MBINARY) {
            return(new tree(operate(src->geta(),crules,operation,base,true),
             src->getvalue(),operate(src->getb(),crules,operation,base,true)));
        }
        if(type==tree::TREE_MVARIABLE||type==tree::TREE_MNUMBER) {
            return(src);
        }
        if(type==tree::TREE_MFUNCTION) {
            //diff(...) in source
            if(!src->getvalue().compare(operation)) {
                return(operate(src->geta(),crules,operation,base));
            }

            //diffx(...) in source
            if((src->getvalue().length()==(operation.length()+1))&&
             (src->getvalue().find(operation)!=string::npos)) {
                tmp=src->getvalue().substr(operation.length(),1);
                return(operate(src->geta(),crules,operation,tmp));
            }

            //other
            return(new tree(src->getvalue(),operate(src->geta(),crules,operation,base,true)));
        }
    }
    return(NULL);
}

void symbolic_computation::print_replacers(symbolic_computation::replacers* source) {
    for(replacers::iterator it=source->begin();it!=source->end();it++) {
        cerr << it->first << ": " << it->second.display(this) << endl;
    }
}
