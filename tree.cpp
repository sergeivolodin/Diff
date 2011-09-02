#include "tree.h"
#include "symbolic_computation.h"
#include <iostream>
using std::cerr;
using std::endl;
tree::tree(tree_leaf_t new_value) {
    value=new_value;
    a=NULL;
    b=NULL;
    leaf=true;
}

tree::tree(tree* new_left,tree_leaf_t new_operation,tree* new_right) {
#ifdef TREEDEBUG
    assert(new_left);
    assert(new_right);
#endif
    a=new_left;
    b=new_right;
    value=new_operation;
    leaf=false;
}

tree::tree(tree_leaf_t new_operation,tree* new_subtree) {
#ifdef TREEDEBUG
    assert(new_subtree);
#endif
    a=new_subtree;
    b=NULL;
    value=new_operation;
    leaf=false;
}

tree::tree() {
    leaf=false;
    a=NULL;
    b=NULL;
}

tree::~tree() {
    if(!leaf) {
        if(a) {
            leaf=true;
            delete a;
            a=NULL;
        }
        if(b) {
            leaf=true;
            delete b;
            b=NULL;
        }
    }
}

tree::tree(const tree& x) {
    if(x.a) a=x.a->copy();
    else a=NULL;

    if(x.b) b=x.b->copy();
    else b=NULL;

    leaf=x.leaf;
    value=x.value;
}

void tree::operator=(tree* x) {
    if(x->a) a=x->a->copy();
    else a=NULL;

    if(x->b) b=x->b->copy();
    else b=NULL;

    leaf=x->leaf;
    value=x->value;
}

/*void tree::erase_subtrees() {
    if(!leaf) {
        if(a) {
            erase_subtrees(a);
        }
        if(b) {
            erase_subtrees(b);
        }
    }
}*/

tree* tree::copy() {
    tree* nt=NULL;
    if(leaf) nt=new tree(value);
    else {
        if(!b) nt=new tree(value,a->copy());
        else nt=new tree(a->copy(),value,b->copy());
    }
    return(nt);
}

bool tree::operator==(tree x) {
    tree_treetype_t ctype=treetype();
    if(x.treetype()!=ctype) return(false);
    else {
        if(ctype==TREE_TLEAF) return(!value.compare(x.value));
        else if(ctype==TREE_TUNARY) return(!value.compare(x.value)&&(*a==*(x.a)));
        else if(ctype==TREE_TBINARY) return(!value.compare(x.value)&&(*a==*(x.a))&&(*b==*(x.b)));
    }
    return(false);
}

bool tree::operator!=(tree x) {
    return(!(x==*this));
}

tree* tree::geta() {
    return(a);
}

tree* tree::getb() {
    return(b);
}

tree::tree_leaf_t tree::getvalue() {
    return(value);
}

unsigned int tree::varcount(symbolic_computation* c) {
    tree_mathtype_t ttype=mathtype(c);
    if(ttype==TREE_MUNARY||ttype==TREE_MFUNCTION) return(a->varcount(c));
    else if(ttype==TREE_MBINARY) return(a->varcount(c)+b->varcount(c));
    else if(ttype==TREE_MVARIABLE) return(1);
    else return(0);
}

tree::tree_treetype_t tree::treetype() {
    if(leaf) return(TREE_TLEAF);
    else {
        if(!b) return(TREE_TUNARY);
        else return(TREE_TBINARY);
    }
    return(TREE_TINVALID);
}

tree::tree_mathtype_t tree::mathtype(symbolic_computation* c) {
    if(leaf) {
        if(c->stringtype(value)==symbolic_computation::SVARIABLE) {
            return(TREE_MVARIABLE);
        }
        else return(TREE_MNUMBER);
    }
    else {
        if(!b) {
            if(c->stringtype(value)==symbolic_computation::SOPERATION) {
                return(TREE_MUNARY);
            }
            else return(TREE_MFUNCTION);
        }
        else return(TREE_MBINARY);
    }
    return(TREE_MINVALID);
}

bool tree::contains(tree_leaf_t what) {
    if(leaf) {
        return(!value.compare(what));
    }
    else {
        if(!b) return(a->contains(what));
        else return(a->contains(what)||b->contains(what));
    }
}

unsigned int tree::length() {
    tree_treetype_t ttype=treetype();
    if(ttype==TREE_TLEAF) return(1);
    else if(ttype==TREE_TUNARY) return(1+a->length());
    else if(ttype==TREE_TBINARY) return(1+a->length()+b->length());
    return(0);
}

unsigned int tree::opcount(symbolic_computation* c) {
    tree_mathtype_t ttype=mathtype(c);
    if(ttype==TREE_MUNARY) return(1+a->opcount(c));
    else if(ttype==TREE_MBINARY) return(2+a->opcount(c)+b->opcount(c));
    else if(ttype==TREE_MVARIABLE) return(1);
    else return(0);
}

string tree::display(symbolic_computation* c,tree_display_t display_type) {
    bool tmp;
    tree_mathtype_t ctype=mathtype(c);
    string res;
    if(ctype==TREE_MNUMBER||ctype==TREE_MVARIABLE) res=value;
    if(ctype==TREE_MUNARY||ctype==TREE_MFUNCTION) {
        res=value;
        if((tmp=((ctype==TREE_MFUNCTION
                  ||(a->mathtype(c)==TREE_MBINARY&&ctype==TREE_MUNARY))))) res.append(c->BR1);
        res.append(a->display(c));
        if(tmp) res.append(c->BR2);
    }
    if(ctype==TREE_MBINARY) {
        if((tmp=((display_type==TREE_DBRACKETS&&a->treetype()!=TREE_TLEAF)
                 ||(a->mathtype(c)==TREE_MBINARY&&(c->op_prio(a->value)<c->op_prio(value)))))) {
            res=c->BR1;
        }
        res.append(a->display(c));
        if(tmp) res.append(c->BR2);
        res.append(value);
        if((tmp=((display_type==TREE_DBRACKETS&&a->treetype()!=TREE_TLEAF)
                 ||(b->mathtype(c)==TREE_MBINARY&&(c->op_prio(b->value)<c->op_prio(value)))))) {
            res.append(c->BR1);
        }
        res.append(b->display(c));
        if(tmp) res.append(c->BR2);
    }
    return(res);
}