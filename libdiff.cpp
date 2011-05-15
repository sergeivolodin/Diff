#include <iostream>
#include <assert.h>
#include "default.cpp"
#include "strs.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <strings.h>
#define BUFLEN 1000

#define CONFFILE ".diff_program"
using namespace std;
/*
  упрощение
*/
const int CHAR_TNUM=1; //numbers: 0-9.
const int CHAR_TLETT=2; //letters a-zA-Z
const int CHAR_TOP=3; //operations: +-*/^
const int CHAR_TEND=4; //end of string
const int CHAR_TBR1=5; //opening bracket
const int CHAR_TBR2=6; //closing bracket
//строки
const char* ERROR="Error.";
const char* MATH_BR1="(";
const char* MATH_BR2=")";
//наборы
char* MATH_NUMS="abcde";
char* MATH_VARS="xyz";
char* MATH_OPS="+-*/^";
char* MATH_FUNCS="fgh";
char* MATH_REPLS="mnop";
//константы
char* MATH_DEFDIFF="x";
const int MATH_TDIFF=1;
const char MATH_CPOSTREPL='|';
const char MATH_CCOMMOND=' ';
const char MATH_CLINESD='\n';
const char MATH_CPOSTREPLD=',';
const char MATH_CPOSTREPLDC='=';
const int MATH_MAXREPL=100;
strings* MATH_OPLIST=NULL;
const int MATH_TINTEGRAL=2;
//операции
const char* MATH_MUL="*";
const char* MATH_DIV="/";
const char* MATH_ADD="+";
const char* MATH_SUB="-";
const char* MATH_POW="^";
const char* MATH_POW1="_";
//константы к классу tree
const int TREE_DMATH=0;
const int TREE_DTREE=1;
const int TREE_ADEF=0;
const int TREE_ASYM=1;
const int TREE_TNUM=0;
const int TREE_TVAR=4;
const int TREE_TUNARY=1;
const int TREE_TBINARY=2;
const int TREE_TFUNCTION=3;
struct token {
    char* c;
    int t;
};
struct lexer_answer {
    token** result;
    int max;
    bool errors;
};
const int TREE_DDEFAULT=TREE_DMATH;
inline bool char_isnum(char sym) {
    return((sym>='0'&&sym<='9')||sym=='.'||sym==',');
}
inline bool char_isnum_m(char sym) {
    return((sym>='0'&&sym<='9')||sym=='.'||sym==','||sym=='-');
}
inline bool char_islett(char sym) {
    return((sym>='A'&&sym<='Z')||(sym>='a'&&sym<='z'));
}
inline bool char_isop(char current) {
    return(strchar(MATH_OPS,current));
}
int char_state(char in) {
    if(char_isnum(in)) return(CHAR_TNUM);
    if(char_islett(in)) return(CHAR_TLETT);
    if(char_isop(in)) return(CHAR_TOP);
    if(in==')') return(CHAR_TBR2);
    if(in=='(') return(CHAR_TBR1);
    if(in=='\0') return(CHAR_TEND);
    return(0);
}
int op_prio(char* a) {
    if(a==NULL) {
        return(-1);
    }
    unsigned int i=0;
    while(i<strlen(MATH_OPS)) {
        if(MATH_OPS[i]==a[0]) {
            return(i+1);
        }
        i++;
    }
    cerr << "Warning (prio): unexpected operator [" << a << "]" << endl;
    return(0);
}
class tree {
private:
    char* display_math(bool brackets=false) {
        bool tmp;
        char* res=NULL;
        char* tmp_=NULL,*tmp2=NULL;
        if(value==NULL) {return(NULL);}
        if(type()==TREE_TNUM||type()==TREE_TVAR) {
            res=strcp(value);
        }
        if(type()==TREE_TUNARY||type()==TREE_TFUNCTION) {
            res=strcp(value);
            if(tmp=(brackets||(a->type()==TREE_TBINARY||type()==TREE_TFUNCTION||type()==TREE_TUNARY))) res=stradd(res,MATH_BR1);
            res=stradd(res,a->display_math());
            if(tmp) res=stradd(res,MATH_BR2);
        }
        if(type()==TREE_TBINARY) {
            if(tmp=(brackets||(a->type()==TREE_TBINARY&&(op_prio(a->value)<op_prio(value))))) res=strcp(MATH_BR1);
            else res="";
            res=stradd(res,a->display_math());
            if(tmp) res=stradd(res,MATH_BR2);
            res=stradd(res,value);
            if(tmp=(brackets||(b->type()==TREE_TBINARY&&(op_prio(b->value)<op_prio(value))))) res=stradd(res,strcp(MATH_BR1));
            res=stradd(res,b->display_math());
            if(tmp) res=stradd(res,MATH_BR2);
        }
        return(res);
    }
    char* display_tree(int level=0) {
        const char* sym=":";
        char* res;
        char* nl=new char[1];
        nl[0]=10;
        if(leaf==true) {
            if(value==NULL) {
                return(NULL);
            }
            res=repeat(sym,level);
            res=stradd(res,value);
            res=stradd(res,nl);
        }
        else {
            if(b==NULL) {
                res=repeat(sym,level);
                res=stradd(res,value);
                res=stradd(res,nl);
                res=stradd(res,a->display_tree(level+1));
            }
            else {
                res=a->display_tree(level+1);
                res=stradd(res,repeat(sym,level));
                res=stradd(res,value);
                res=stradd(res,nl);
                res=stradd(res,b->display_tree(level+1));
            }
        }
        if(level==0) {
            char* res_=new char[strlen(res)];
            res[strlen(res)-1]=NULL;
            strcpy(res_,res);
            res=res_;
        }
        return(res);
    }
    char* value;
    bool leaf;
    tree *a,*b;
        public:
    tree(const char* str): value(strcp(str)),leaf(true) {}
    tree(tree *in_a,const char* op,tree *in_b): value(strcp(op)),leaf(false),a(in_a),b(in_b) {}
    tree(const char *op,tree *in_): value(strcp(op)),leaf(false),a(in_),b(NULL) {}
    tree(const char* ina, const char* op, const char* inb): value(strcp(op)),leaf(false) {
        a=new tree(ina);
        b=new tree(inb);
    }
    tree(const char* op,const char* in_): value(strcp(op)),leaf(false),b(NULL) {
        a=new tree(in_);
    }
    tree():value(NULL),leaf(false),a(NULL),b(NULL){}
    tree* copymem() {
        tree* nt;
        char* s=strcp(value);
        if(leaf==true) {
            nt=new tree(s);

        }
        else {
            if(b==NULL) {
                nt=new tree(s,a->copymem());
            }
            else {
                nt=new tree(a->copymem(),s,b->copymem());
            }
        }
        return(nt);
    }
    bool operator==(tree x) {
        int ctype=type();
        if(x.type()!=ctype) {
            return(false);
            //cerr << "!=";
        }
        else {
            if(ctype==TREE_TVAR||ctype==TREE_TNUM) {
                return(str(value,x.value));
            }
            else if(ctype==TREE_TFUNCTION||ctype==TREE_TUNARY) {
                return(str(value,x.value)&&(*a==*(x.a)));
            }
            else if(ctype==TREE_TBINARY) {
                return(str(value,x.value)&&(*a==*(x.a))&&(*b==*(x.b)));
            }
            return(false);
        }
    }
    inline bool isleaf() {
        return(leaf);
    }
    tree* geta() {
        return(a);
    }
    tree* getb() {
        return(b);
    }
    char* getvalue() {
        return(value);
    }
    bool contains(const char* src) {
        if(leaf) {
            return(str(src,value));
        }
        else {
            if(b==NULL) {
                return(a->contains(src));
            }
            else {
                return(a->contains(src)||b->contains(src));
            }
        }
    }
    bool test() {
        if(leaf) {
            if(value==NULL) {
                return(false);
            }
        }
        else {
            if(a==NULL) {
                return(false);
            }
            if(b==NULL) {
                return(a->test());
            }
            else {
                return(a->test()&&b->test());
            }
        }
        return(true);
    }
    int type() {
        if(leaf) {
            if(char_isnum_m(value[0])) {
                return(TREE_TNUM);
            }
            else {
                return(TREE_TVAR);
            }
        }
        else {
            if(b==NULL) {
                if(char_islett(value[0])) {
                    return(TREE_TFUNCTION);
                }
                else {
                    return(TREE_TUNARY);
                }
            }
            else {
                return(TREE_TBINARY);
            }
        }
    }
    int varcount() {
        int ttype=type();
        if(ttype==TREE_TUNARY||ttype==TREE_TFUNCTION) {
            return(a->varcount());
        }
        else if(ttype==TREE_TBINARY) {
            return(a->varcount()+b->varcount());
        }
        else {
            return(1);
        }
    }
    int opcount() {
        int ttype=type();
        if(ttype==TREE_TUNARY||ttype==TREE_TFUNCTION) {
            return(1+a->opcount());
        }
        else if(ttype==TREE_TBINARY) {
            return(2+a->opcount()+b->opcount());
        }
        else if(ttype==TREE_TVAR) {
            return(1);
        }
        else {
            return(0);
        }
    }
    void operator=(tree *src) {
        tree *newtree;
        newtree=src->copymem();
        leaf=newtree->leaf;
        value=newtree->value;
        a=newtree->a;
        b=newtree->b;
    }
    char* display(int dtype=TREE_DDEFAULT) {
        if(dtype==TREE_DMATH) {
            return(display_math());
        }
        else if(dtype==TREE_DTREE) {
            return(display_tree());
        }
        else {
            return(display(TREE_DDEFAULT));
        }
    }
};
struct replacer {
    char* l;
    tree* r;
};
struct replacers {
    replacer** r;
    int maxindex;
};
struct trees {
    tree** t;
    int max;
};
lexer_answer* lexer(char* cstr) {
    int i=0,cstate=0,pstate=0;
    int len=strlen(cstr);
    char c,*tmp;
    lexer_answer* res=new lexer_answer;
    res->result=new token*[len];
    res->max=-1;
    while(i<len) {
        c=cstr[i];
        if(c=='.') c=',';
        cstate=char_state(c);
        if(cstate==0) {
            cerr << "Warning (lexer): wrong symbol '" << c << "' at" << i << endl;
        }
        else {
            tmp=new char[2];
            tmp[0]=c;
            tmp[1]=0;
            if(res->max==-1||cstate!=pstate||cstate==CHAR_TBR2||cstate==CHAR_TBR1||cstate==CHAR_TOP) {
                res->result[++(res->max)]=new token;
                res->result[res->max]->t=cstate;
                res->result[res->max]->c=tmp;
            }
            else {
                res->result[res->max]->c=stradd(res->result[res->max]->c,tmp);
            }
        }
        pstate=cstate;
        i++;
    }
    res->errors=false;
    return(res);
}
void perr(const char* a) {
    cout << "Warning (parser/NULL): " << a << endl;
}
bool testbrackets(lexer_answer* src,int pmin=0,int pmax=-1) {
    if(pmax==-1) pmax=src->max;
    int bc1=0,bc2=0;
    while(pmin<=pmax) {
        if(src->result[pmin]->t==CHAR_TBR1) {
            bc1++;
        }
        if(src->result[pmin]->t==CHAR_TBR2) {
            bc2++;
        }
        pmin++;
    }
    return(bc1==bc2);
}
int getlast(lexer_answer* src,int type,int pmin=0,int i=-1,bool notinsidebrackets=false,bool nounary=false) {
    if(i==-1) i=src->max;
    int bc=0;;
    while(i>=pmin) {
        if(src->result[i]->t==CHAR_TBR2) bc++;
        if(src->result[i]->t==CHAR_TBR1) bc--;
        if(src->result[i]->t==type) {
            if((!notinsidebrackets)||(bc==0)) {
                if(!nounary) {
                    return(i);
                }
                else {
                    if(i>pmin) {
                        if(src->result[i-1]->t!=CHAR_TBR1&&src->result[i-1]->t!=CHAR_TOP) {
                            return(i);
                        }
                    }
                    //"^-3$"
                }
            }
        }
        i--;
    }
    return(-1);
}
bool brackets_nospace(lexer_answer* src,int left,int right) {
    if(src->result[left]->t==CHAR_TBR1&&src->result[right]->t==CHAR_TBR2) {
        int br=1;
        left++;
        while(left<right) {
            if(src->result[left]->t==CHAR_TBR1) br++;
            if(src->result[left]->t==CHAR_TBR2) br--;
            if(br==0) return(false);
            left++;
        }
    }
    else {
        return(false);
    }
    return(true);
}
int minopprio(lexer_answer* src,int pmin=0,int pmax=-1) {
    if(pmax==-1) pmax=src->max;
    int minprio=100;
    int bc=0;
    while(pmax>=pmin) {
        if(src->result[pmax]->t==CHAR_TBR2) bc++;
        if(src->result[pmax]->t==CHAR_TBR1) bc--;
        if(src->result[pmax]->t==CHAR_TOP) {
            if(bc==0) {
                minprio=min(minprio,op_prio(src->result[pmax]->c));
            }
        }
        pmax--;
    }
    return(minprio);
}
tree* parser(lexer_answer* src,int pmin=0,int pmax=-1,bool binary_allowed=true);
tree* parser_atomic(lexer_answer* src,int pmin,int pmax) {
    if(pmin==pmax) {
        return(new tree(src->result[pmin]->c));
    }
    return(NULL);
}
tree* parser_brackets(lexer_answer* src,int pmin,int pmax) {
    if((pmax-pmin)>=2&&src->result[pmin]->t==CHAR_TBR1&&src->result[pmax]->t==CHAR_TBR2&&brackets_nospace(src,pmin,pmax)) {
        return(parser(src,pmin+1,pmax-1));
    }
    return(NULL);
}
tree* parser_unary(lexer_answer* src,int pmin,int pmax) {
    if((pmax-pmin)>=1&&src->result[pmin]->t==CHAR_TOP) {
        return(new tree(src->result[pmin]->c,parser(src,pmin+1,pmax)));
    }
    return(NULL);
}
tree* parser_function(lexer_answer* src,int pmin,int pmax) {
    if((pmax-pmin)>=3&&src->result[pmin]->t==CHAR_TLETT&&src->result[pmin+1]->t==CHAR_TBR1&&src->result[pmax]->t==CHAR_TBR2&&brackets_nospace(src,pmin+1,pmax)) {
        return(new tree(src->result[pmin]->c,parser(src,pmin+2,pmax-1)));
    }
    return(NULL);
}
tree* parser(lexer_answer* src,int pmin,int pmax,bool binary_allowed) {
    //разрешено парсить [pmin,pmax]
    if(pmax==-1) pmax=src->max;
    if(pmax<pmin) return(NULL);
    int tmp;
    /*if(!testbrackets(src,pmin,pmax)) {
        tmp=getlast(src,CHAR_TBR2,pmin,pmax)-1;
        if(tmp!=-1) {
            pmax=min(pmax,tmp);
        }
    }*/
    //pmin...op_pos...pmax
    int op_pos=getlast(src,CHAR_TOP,pmin,pmax,true,true);
    tree* r1=NULL,*r2=NULL;
    if(op_pos!=-1) {
        if(binary_allowed) {
            bool btest=false;
            while(!btest) {
                if(op_pos!=-1) {
                    if((r1=parser(src,pmin,op_pos-1,false))!=NULL) {
                        btest=true;
                    }
                    else {
                        if(minopprio(src,pmin,op_pos-1)>=op_prio(src->result[op_pos]->c)) {
                            btest=true;
                            r1=parser(src,pmin,op_pos-1);
                        }
                    }
                    if(btest) {
                        if((r2=parser(src,op_pos+1,pmax,false))!=NULL) {
                        }
                        else {
                            r2=parser(src,op_pos+1,pmax);
                        }
                    }
                    else {
                        op_pos=getlast(src,CHAR_TOP,pmin,op_pos-1,true,true);
                        continue;
                    }
                    if(btest&&r1!=NULL&&r2!=NULL) {
                        return(new tree(r1,src->result[op_pos]->c,r2));
                    }
                    else {
                        return(NULL);
                    }
                }
                else {
                    break;
                }
            }
        }
    }
    else {
        if((r1=parser_function(src,pmin,pmax))!=NULL) return(r1);
        if((r1=parser_unary(src,pmin,pmax))!=NULL) return(r1);
        if((r1=parser_brackets(src,pmin,pmax))!=NULL) return(r1);
        if((r1=parser_atomic(src,pmin,pmax))!=NULL) return(r1);
    }
    return(NULL);
}
tree* parse(char* src) {
    if(str(src,"")) {
        return(NULL);
    }
    lexer_answer* a=lexer(src);
    if(a!=NULL) {
        tree* b=parser(a);
        if(b!=NULL&&b->test()) {
            return(b);
        }
    }
    return(NULL);
}
unsigned int pattern_count_funcs(tree* p) {
    int ptype=p->type();
    if(ptype==TREE_TVAR) {
        if(strchar(MATH_FUNCS,p->getvalue())||strchar(MATH_NUMS,p->getvalue())) {
            return(1);
        }
    }
    if(ptype==TREE_TUNARY||ptype==TREE_TFUNCTION) {
        if(strchar(MATH_FUNCS,p->getvalue())) {
            return(pattern_count_funcs(p->geta())+1);
        }
        else {
            return(pattern_count_funcs(p->geta()));
        }
    }
    if(ptype==TREE_TBINARY) {
        return(pattern_count_funcs(p->geta())+pattern_count_funcs(p->getb()));
    }
    return(0);
}
tree* getreplacer(replacers* src,char* a) {
    int i=0;
    if(src!=NULL&&a!=NULL) {
        if(src->r!=NULL) {
            while(i<=src->maxindex) {
                if(src->r[i]!=NULL) {
                    if((src->r[i]->l!=NULL)&&(src->r[i]->r!=NULL)) {
                        if(str(src->r[i]->l,a)) {
                            if(src->r[i]->r->test()) {
                                return(src->r[i]->r);
                            }

                        }
                    }
                }
                i++;
            }
        }
    }
    return(NULL);
}
bool replacer_add(replacers* src,char* a,tree* which) {
    if(a==NULL||src==NULL||which==NULL) {
        return(false);
    }
    /*if(!which->test()) {
        return(false)
    }*/
    tree* repl=getreplacer(src,a);
    if(repl==NULL) {
        src->maxindex++;
        src->r[src->maxindex]=new replacer;
        src->r[src->maxindex]->l=a;
        src->r[src->maxindex]->r=which->copymem();
        //cerr << "Notice (repl_add): adding replacer #" << src->maxindex << " " << a << "=" << which->display() << "with type=" << which->type() << endl;
    }
    else if(!(*repl==*which)) {
        return(false);
    }
    return(true);
}

replacers* pattern(tree* stree, tree* spattern,const char* base=MATH_DEFDIFF,replacers* areplacers=NULL) {
    replacers* crepl=NULL;
    int fcount=pattern_count_funcs(spattern);
    if(areplacers==NULL) {
        crepl=new replacers;
        crepl->maxindex=-1;
        crepl->r=new replacer*[fcount+1];
    }
    else {
        crepl=areplacers;
    }
    int ttype=stree->type();
    int ptype=spattern->type();
    if(ptype==ttype) {
        //        cerr << "types=";
        if(ttype==TREE_TNUM) {
            //            cerr << "num ";
            if(!str(stree->getvalue(),spattern->getvalue())) {
                //                cerr << "NOMATCH" << endl;
                return(NULL);
            }
            //            cerr << "ok" << endl;
        }
        if(ttype==TREE_TVAR) {
            //            cerr << "var ";
            if(strchar(MATH_FUNCS,spattern->getvalue())) {
                //                cerr << "ptype=funcs ";
                if(!replacer_add(crepl,spattern->getvalue(),stree)) {
                    //                    cerr << "WRONGREPLACER" << endl;
                    return(NULL);
                }
                //                cerr << "ok" << endl;
            }
            else if(strchar(MATH_VARS,spattern->getvalue())) {
                //                cerr << "ptype=vars ";
                if(str(stree->getvalue(),base)) {
                    //                    cerr << "baseok ";
                    if(!replacer_add(crepl,spattern->getvalue(),stree)) {
                        //                        cerr << "WRONGREPLACER" << endl;
                        return(NULL);
                    }
                    //                    cerr << "ok" << endl;
                }
                else {
                    //                    cerr << "WRONGBASE" << endl;
                    return(NULL);
                }
            }
            else if(strchar(MATH_NUMS,spattern->getvalue())) {
                //                cerr << "ptype=nums ";
                if(!str(stree->getvalue(),base)) {
                    //                    cerr << "baseok ";
                    if(!replacer_add(crepl,spattern->getvalue(),stree)) {
                        //                        cerr << "WRONGREPLACER" << endl;
                        return(NULL);
                    }
                    //                    cerr << "ok" << endl;
                }
                else {
                    //                    cerr << "WRONGBASE" << endl;
                    return(NULL);
                }
            }
            else if(!str(stree->getvalue(),spattern->getvalue())) {
                //                cerr << "WRONGOTHER" << endl;
                return(NULL);
            }
        }
        if(ttype==TREE_TFUNCTION) {
            //            cerr << "func ";
            if(strchar(MATH_FUNCS,spattern->getvalue())) {
                //                cerr << "f='func' ";
                if(!replacer_add(crepl,spattern->getvalue(),(new tree(strcp(stree->getvalue())))->copymem())) {
                    //                    cerr << "WRONGREPLACER" << endl;
                    return(NULL);
                }
                if(!pattern(stree->geta(),spattern->geta(),base,crepl)) {
                    //                    cerr << "WRONGARG" << endl;
                    return(NULL);
                }
                //                cerr << "ok" << endl;
            }
            else if(str(spattern->getvalue(),stree->getvalue())) {
                //                cerr << "samefunction ";
                if(!pattern(stree->geta(),spattern->geta(),base,crepl)) {
                    //                    cerr << "WRONGARG" << endl;
                    return(NULL);
                }
                //                cerr << "ok" << endl;
            }
            else {
                //                cerr << "WRONGFUNCTION" << endl;
                return(NULL);
            }
        }
        if(ttype==TREE_TUNARY) {
            //            cerr << "unary ";
            if(str(stree->getvalue(),spattern->getvalue())) {
                //                cerr << "sameoperation ";
                if(!pattern(stree->geta(),spattern->geta(),base,crepl)) {
                    //                    cerr << "WRONGARG" << endl;
                    return(NULL);
                }
                //                cerr << "ok" << endl;
            }
            else {
                //                cerr << "WRONGOPERATION" << endl;
                return(NULL);
            }
        }
        if(ttype==TREE_TBINARY) {
            //            cerr << "binary ";
            if(str(stree->getvalue(),spattern->getvalue())) {
                //                cerr << "sameoperation ";
                if(!pattern(stree->geta(),spattern->geta(),base,crepl)) {
                    //                    cerr << "WRONGARG_a" << endl;
                    return(NULL);
                }
                if(!pattern(stree->getb(),spattern->getb(),base,crepl)) {
                    //                    cerr << "WRONGARG_b" << endl;
                    return(NULL);
                }
                //                cerr << "ok" << endl;
            }
            else {
                //                cerr << "WRONGOPERATION" << endl;
                return(NULL);
            }
        }
    }
    else {
        if(ptype==TREE_TVAR) {
            //            cerr << "mixed_type=";
            if(strchar(MATH_FUNCS,spattern->getvalue())) {
                //                cerr << "funcs ";
                if(!replacer_add(crepl,spattern->getvalue(),stree)) {
                    //                    cerr << "WRONGREPL" << endl;
                    return(NULL);
                }
                //                cerr << "ok" << endl;
            }
            else if(strchar(MATH_NUMS,spattern->getvalue())) {
                //                cerr << "nums ttype=";
                if(ttype==TREE_TBINARY) {
                    //                    cerr << "binary ";
                    if((stree->geta()->contains(base))||(stree->getb()->contains(base))) {
                        //                        cerr << "ARGCONTAINSBASE" << endl;
                        return(NULL);
                    }
                    if(!replacer_add(crepl,spattern->getvalue(),stree)) {
                        //                        cerr << "WRONGREPL" << endl;
                        return(NULL);
                    }
                    //                    cerr << "ok" << endl;
                }
                if(ttype==TREE_TUNARY||ttype==TREE_TFUNCTION) {
                    //                    cerr << "unary/function ";
                    if(stree->geta()->contains(base)) {
                        //                        cerr << "ARGCONTAINSBASE" << endl;
                        return(NULL);
                    }
                    if(!replacer_add(crepl,spattern->getvalue(),stree)) {
                        //                        cerr << "WRONGREPL" << endl;
                        return(NULL);
                    }
                }
                if(ttype==TREE_TNUM) {
                    //                    cerr << "num ";
                    if(!replacer_add(crepl,spattern->getvalue(),stree)) {
                        //                        cerr << "WRONGREPL" << endl;
                        return(NULL);
                    }
                    //                    cerr << "ok" << endl;
                }
            }
            else {
                //                cerr << "WRONGMIXED" << endl;
                return(NULL);
            }
        }
        else {
            //            cerr << "WRONGMIXED" << endl;
            return(NULL);
        }
    }
    if(areplacers==NULL) {
        //cerr << "[pattern:" << stree->display() << "@" << spattern->display() << "]";
    }
    return(crepl);
}
tree* replace(tree* spattern,replacers* srepl) {
    if(spattern==NULL) {
        return(NULL);
    }
    if(srepl==NULL) {
        return(spattern);
    }
    int stype=spattern->type();
    tree* rtree=NULL,*tmp=NULL;
    if(stype==TREE_TVAR) {
        if((rtree=getreplacer(srepl,spattern->getvalue()))!=NULL) {
            return(rtree);
        }
        else {
            return(spattern);
        }
    }
    if(stype==TREE_TNUM) {
        rtree=spattern->copymem();
    }
    if(stype==TREE_TUNARY) {
        rtree=new tree(spattern->getvalue(),replace(spattern->geta(),srepl));
    }
    if(stype==TREE_TFUNCTION) {
        if((tmp=getreplacer(srepl,spattern->getvalue()))!=NULL) {
            rtree=new tree(tmp->getvalue(),replace(spattern->geta(),srepl));
        }
        else {
            rtree=new tree(spattern->getvalue(),replace(spattern->geta(),srepl));
        }
    }
    if(stype==TREE_TBINARY) {
        rtree=new tree(replace(spattern->geta(),srepl),spattern->getvalue(),replace(spattern->getb(),srepl));
    }
    return(rtree);
}
struct rule {
    tree* src;
    tree* dest;
    int op;
    replacers* postrepl;
};
struct rules {
    rule** r;
    int maxindex;
};
rules* FRULES=NULL;
rule* parsestr(char* str) {
    strings* t=explode(str,MATH_CPOSTREPL);
    strings* l=NULL;
    strings* r=NULL;
    rule* res=new rule;
    strings* tmp=NULL;
    tree* ttree;
    replacers* prepl=NULL;
    int rcount=0,i=0;
    res->postrepl=NULL;
    if(t->max==0) {
        l=explode(str,MATH_CCOMMOND);
        if(l==NULL||l->max==-1) {
            return(NULL);
        }
    }
    else if(t->max==1) {
        l=explode(t->strs[0],MATH_CCOMMOND);
        r=explode(t->strs[1],MATH_CPOSTREPLD);
        if(l==NULL||r==NULL||l->max==-1||r->max==-1) {
            return(NULL);
        }
        if(r->max>=0) {
            prepl=new replacers;
            prepl->r=new replacer*[r->max+1];
            while(i<=r->max) {
                tmp=explode(r->strs[i],MATH_CPOSTREPLDC);
                if(tmp->max==1) {
                    prepl->r[rcount]=new replacer;
                    prepl->r[rcount]->l=tmp->strs[0];
                    ttree=parse(tmp->strs[1]);
                    if(ttree!=NULL) {
                        prepl->r[rcount]->r=ttree;
                        rcount++;
                    }
                }
                i++;
            }
            if(rcount>=0) {
                prepl->maxindex=rcount-1;
                res->postrepl=prepl;
            }
        }
    }
    if(l==NULL||l->max==2) {
        res->src=parse(l->strs[0]);
        res->op=atoi(l->strs[1]);
        res->dest=parse(l->strs[2]);
        if(res->src==NULL||res->dest==NULL) {
            return(NULL);
        }
        return(res);
    }
    return(NULL);
}
rules* rules_merge(rules* a,rules* b) {
    rules* res=new rules;
    res->maxindex=-1;
    if(a!=NULL&&b!=NULL) {
        res->r=new rule*[a->maxindex+b->maxindex+3];
        int i=0;
        while(i<=a->maxindex) {
            res->r[++res->maxindex]=a->r[i];
            i++;
        }
        i=0;
        while(i<=b->maxindex) {
            res->r[++res->maxindex]=b->r[i];
            i++;
        }
    }
    return(res);
}
char* postrepls_to_string(replacers* a,bool addpipe=true) {
    int si=0;
    char* buf="";
    if(a!=NULL&&a->maxindex>=0) {
        if(addpipe) {
            buf=new char[2];
            buf[0]=MATH_CPOSTREPL;
            buf[1]=0;
        }
        while(si<=a->maxindex) {
            if(si>0) {
                buf=stradd(buf,MATH_CPOSTREPLD);
            }
            buf=stradd(buf,a->r[si]->l);
            buf=stradd(buf,MATH_CPOSTREPLDC);
            buf=stradd(buf,a->r[si]->r->display());
            si++;
        }
    }
    return(buf);
}
rules* parsestrs(char* str) {
    strings* strs=explode(str,MATH_CLINESD);
    rule* tempr=NULL;
    rules* res=new rules;

    if(strs->max==-1) {
        return(NULL);
    }
    res->r=new rule*[strs->max+2];
    int i=0,c=0;
    while(i<=strs->max) {
        tempr=parsestr(strs->strs[i]);
        if(tempr!=NULL) {
            res->r[c]=tempr;
            c++;
        }
        else {
            cerr << "Warning (rules): rule " << i << " [" << strs->strs[i] << "] is wrong!" << endl;
        }
        i++;
    }
    if(c>=0) {
        res->maxindex=c-1;
    }
    return(res);
}
void settings_to_file(rules* R=FRULES,const char* filename=CONFFILE) {
    FILE *file=fopen(filename,"w");
    fputs("",file);
    fclose(file);
    file=fopen(filename,"a");
    char* buf=NULL;
    if(R!=NULL) {
        fputs(MATH_VARS,file);
        fputs("\t",file);
        fputs(MATH_FUNCS,file);
        fputs("\t",file);
        fputs(MATH_NUMS,file);
        fputs("\t",file);
        fputs(MATH_REPLS,file);
        fputs("\t",file);
        fputs(MATH_DEFDIFF,file);
        fputs("\t",file);
        fputs(MATH_OPS,file);
        fputs("\t",file);
        int i=0;
        while(i<=MATH_OPLIST->max) {
            fputs(MATH_OPLIST->strs[i],file);
            if(i<MATH_OPLIST->max) {
                fputs(" ",file);
            }
            i++;
        }
        fputs("\n",file);
        fputs(print_num(R->maxindex),file);
        fputs("\n",file);
        i=0;
        while(i<=R->maxindex) {
            buf=R->r[i]->src->display();
            buf=stradd(buf,MATH_CCOMMOND);
            buf=stradd(buf,print_num(R->r[i]->op));
            buf=stradd(buf,MATH_CCOMMOND);
            buf=stradd(buf,R->r[i]->dest->display());
            buf=stradd(buf,postrepls_to_string(R->r[i]->postrepl));
            buf=stradd(buf,MATH_CLINESD);
            fputs(buf,file);
            delete[] buf;
            i++;
        }
    }
    fclose(file);
}
void settings_from_file(const char* filename=CONFFILE) {
    FILE *file=fopen(filename, "r");
    bool read_ok=true;
    if(file==NULL) {
        read_ok=false;
    }
    else {
        char* buf=new char[BUFLEN];
        rule* tr=NULL;
        FRULES=new rules;
        FRULES->maxindex=-1;
        buf=fgets(buf,BUFLEN,file);
        if(buf!=NULL&&buf[0]!=0) {
            strings* a=explode(buf,'\t');
            if(a->max==6) {
                remove_nl(a->strs[6]);
                MATH_VARS=a->strs[0];
                MATH_FUNCS=a->strs[1];
                MATH_NUMS=a->strs[2];
                MATH_REPLS=a->strs[3];
                MATH_DEFDIFF=a->strs[4];
                MATH_OPS=a->strs[5];
                //print_str(a->strs[6]);
                MATH_OPLIST=explode(a->strs[6],' ');
                buf=fgets(buf,BUFLEN,file);
                int i=0,m=atoi(buf);
                if(m>=0) {
                    FRULES->r=new rule*[m+1];
                    while(fgets(buf,BUFLEN,file)&&FRULES->maxindex<=m) {
                        remove_nl(buf);
                        if((tr=parsestr(buf))!=NULL) {
                            FRULES->r[++FRULES->maxindex]=tr;
                        }
                        i++;
                    }
                }
            }
            else {
                cerr << "Warning [settings]: wrong parameters count!" << endl;
                read_ok=false;
            }
        }
        else {
            read_ok=false;
        }
        fclose(file);
    }
    if(!read_ok) {
        cerr << "writing";
        file=fopen(filename,"w");
        fwrite(DRULES,1,sizeof(DRULES),file);
        fclose(file);
        settings_from_file(filename);
    }
}
void printrules(rules* a) {
    if(a!=NULL) {
        int i=0,si=0;
        while(i<=a->maxindex) {
            if(a->r[i]!=NULL) {
                if(a->r[i]->op==1) {
                    cerr << "diff";
                }
                if(a->r[i]->op==0) {
                    cerr << "easy";
                }
                cerr << "(" << a->r[i]->src->display() << ")=" << a->r[i]->dest->display() << endl;
                if(a->r[i]->postrepl!=NULL) {
                    si=0;
                    while(si<=a->r[i]->postrepl->maxindex) {
                        cerr << "  postrepl" << si << ": " << a->r[i]->postrepl->r[si]->l << "=" << a->r[i]->postrepl->r[si]->r->display() << endl;
                        si++;
                    }
                }
            }
            else {
                cerr << "WARNING: rule #" << i << "==NULL!" << endl;
            }
            i++;
        }
    }
}
bool invariants(trees* a,tree* b) {
    int i=0;
    while(i<=a->max) {
        if(*(a->t[i])==*b) {
            return(true);
        }
        i++;
    }
    return(false);
}
trees* getvariants(tree* a,bool recursive=false,trees* write=NULL) {
    trees* res=NULL,*t1=NULL,*t2=NULL;
    tree* ttree=NULL,*ttree1=NULL;
    replacers* tmp=NULL;
    int ti1=0,ti2=0;
    if(write==NULL) {
        res=new trees;
        res->t=new tree*[MATH_MAXREPL];
        res->t[0]=a;
        res->max=0;
    }
    else {
        res=write;
    }
    int i=0;
    if(recursive) {
        if(a->type()==TREE_TBINARY) {
            t1=getvariants(a->geta(),recursive);
            t2=getvariants(a->getb(),recursive);
            ti1=0;
            while(ti1<=t1->max) {
                ti2=0;
                while(ti2<=t2->max) {
                    ttree1=new tree(t1->t[ti1],a->getvalue(),t2->t[ti2]);
                    if(!invariants(res,ttree1)) {
                        if(res->max<MATH_MAXREPL) {
                            res->max++;
                            res->t[res->max]=ttree1;
                            getvariants(ttree1,recursive,res);
                        }
                        else {
                            cerr << "Warning (getvariants): maximum variants exceed (" << MATH_MAXREPL << ")" << endl;
                            return(res);
                        }
                    }
                    ti2++;
                }
                ti1++;
            }
        }
        if(a->type()==TREE_TUNARY||a->type()==TREE_TFUNCTION) {
            t1=getvariants(a->geta(),recursive);
            ti1=0;
            while(ti1<=t1->max) {
                ttree1=new tree(a->getvalue(),t1->t[ti1]);
                if(!invariants(res,ttree1)) {
                    if(res->max<MATH_MAXREPL) {
                        res->max++;
                        res->t[res->max]=ttree1;
                        getvariants(ttree1,recursive,res);
                    }
                    else {
                        cerr << "Warning (getvariants): maximum variants exceed (" << MATH_MAXREPL << ")" << endl;
                        return(res);
                    }
                }
                ti1++;
            }
        }
    }
    while(i<=FRULES->maxindex) {
        if(FRULES->r[i]->op==0&&(tmp=pattern(a,FRULES->r[i]->src))) {
            ttree=replace(FRULES->r[i]->dest,tmp);
            if(!invariants(res,ttree)) {
                if(res->max<MATH_MAXREPL) {
                    res->max++;
                    res->t[res->max]=ttree;
                    getvariants(ttree,recursive,res);
                }
                else {
                    cerr << "Warning (getvariants): maximum variants exceed (" << MATH_MAXREPL << ")" << endl;
                    return(res);
                }
            }
        }
        i++;
    }
    if(write==NULL) {
        return(res);
    }
    return(NULL);
}
tree* easy_old(tree* src) {
    int i=0,opc=0;
    tree* res=src;
    trees* vars=getvariants(src,true);
    int min=src->varcount();
    while(i<=vars->max) {
        opc=vars->t[i]->varcount();
        //cerr << i << " of " << vars->max << ": " << vars->t[i]->display() << " (" << opc << ")" << endl;
        if(opc<min) {
            min=opc;
            res=vars->t[i];
        }
        i++;
    }
    return(res);
}
tree* calc(tree* src) {
    float tmp_float,l=0,r=0;
    bool lset=false,rset=false;
    tree* res=src;
    if(src->type()==TREE_TBINARY) {
        //cerr << "calc " << src->display() << ": ";
        //src->getb()->isleaf()
        if(src->geta()->isleaf()&&char_isnum(src->geta()->getvalue()[0])) {
            //cerr << "a+b ";
            lset=true;
            l=atof(src->geta()->getvalue());
            //cerr << "{" << src->geta()->getvalue() << "}";
        }
        if(src->getb()->isleaf()&&char_isnum(src->getb()->getvalue()[0])) {
            //cerr << "a+b ";
            rset=true;
            r=atof(src->getb()->getvalue());
        }
        if((src->geta()->type()==TREE_TUNARY)&&str(src->geta()->getvalue(),MATH_SUB)&&char_isnum(src->geta()->geta()->getvalue()[0])) {
            l=-atof(src->geta()->geta()->getvalue());
            lset=true;
        }
        if((src->getb()->type()==TREE_TUNARY)&&str(src->getb()->getvalue(),MATH_SUB)&&char_isnum(src->getb()->geta()->getvalue()[0])) {
            r=-atof(src->getb()->geta()->getvalue());
            rset=true;
        }
        if(lset&&rset) {
            if(str(src->getvalue(),"+")) {
                //cerr << "[" << l << "+" << r << "]";
                res=new tree(print_num(l+r));
            }
            if(str(src->getvalue(),"-")) {
                //cerr << "-";
                res=new tree(print_num(l-r));
            }
            if(str(src->getvalue(),"*")) {
                //cerr << "*";
                res=new tree(print_num(l*r));
            }
            if(str(src->getvalue(),"/")) {
                tmp_float=l/r;
                if(is_integer(tmp_float)) {
                    //cerr << "/";
                    res=new tree(print_num(tmp_float));
                }
            }
            if(res->isleaf()&&res->getvalue()[0]=='-') {
                //cerr << "[-a]=>-[a]";
                res=new tree("-",print_num(-atof(res->getvalue())));
            }
        }
        //cerr << endl;
    }
    return(res);
}
tree* rcalc(tree* src) {
    int stype=src->type();
    if(stype==TREE_TBINARY) {
        return(calc(new tree(rcalc(src->geta()),src->getvalue(),rcalc(src->getb()))));
    }
    if(stype==TREE_TFUNCTION||stype==TREE_TUNARY) {
        return(calc(new tree(src->getvalue(),rcalc(src->geta()))));
    }
    return(src);
}

tree* easy(tree* src) {
    int i=0,opc=0;
    tree* res=src,*tmp;
    int min=src->opcount();
    if(src->type()==TREE_TBINARY) {
        res=new tree(easy(src->geta()),src->getvalue(),easy(src->getb()));
    }
    if(src->type()==TREE_TUNARY||src->type()==TREE_TFUNCTION) {
        res=new tree(src->getvalue(),easy(src->geta()));
    }
    trees* vars=getvariants(res,false);
    while(i<=vars->max) {
        tmp=calc(rcalc(vars->t[i]));
        opc=tmp->opcount();
        if(opc<min) {
            min=opc;
            res=tmp;
        }
        i++;
    }
    return(calc(res));
}
tree* operate(tree* src,int operation,const char* base=MATH_DEFDIFF,rules* crules=FRULES,bool rmode=false) {
    unsigned int i=0,type=0;
    char* name=NULL, *tmp=NULL,*tmp1=NULL;
    replacers* tmprepl=NULL,*newrepl=NULL;
    if(!str(base,MATH_DEFDIFF)) {
        newrepl=new replacers;
        newrepl->maxindex=0;
        newrepl->r=new replacer*[1];
        newrepl->r[0]=new replacer;
        newrepl->r[0]->l=MATH_DEFDIFF;
        newrepl->r[0]->r=new tree(base);
    }
    else {
        newrepl=NULL;
    }
    if(!rmode) {
        if(FRULES!=NULL) {
            while(i<=FRULES->maxindex) {
                if((tmprepl=pattern(src,replace(FRULES->r[i]->src,newrepl),base))&&FRULES->r[i]->op==operation) {
                    return(
                            replace(
                                    replace(
                                            operate(
                                                    replace(
                                                            replace(FRULES->r[i]->dest,newrepl),
                                                            tmprepl),
                                                    operation,base,crules,true),
                                            FRULES->r[i]->postrepl),
                                    tmprepl)
                            );
                }
                i++;
            }
        }
    }
    else {
        type=src->type();
        if(src->type()==TREE_TUNARY) {
            return(new tree(src->getvalue(),operate(src->geta(),operation,base,crules,true)));
        }
        if(src->type()==TREE_TBINARY) {
            return(new tree(operate(src->geta(),operation,base,crules,true),src->getvalue(),operate(src->getb(),operation,base,crules,true)));
        }
        if(src->type()==TREE_TVAR||src->type()==TREE_TNUM) {
            return(src);
        }
        if(src->type()==TREE_TFUNCTION) {
            i=0;
            if(str(MATH_OPLIST->strs[operation-1],src->getvalue())) {
                return(operate(src->geta(),operation,base,crules));
            }
            tmp1=MATH_VARS;
            tmp1=stradd(tmp1,MATH_REPLS);
            while(i<strlen(tmp1)) {
                tmp=new char[2];
                tmp[0]=tmp1[i];
                tmp[1]='\0';
                name=MATH_OPLIST->strs[operation-1];
                name=stradd(name,tmp);
                if(str(name,src->getvalue())) {
                    return(operate(src->geta(),operation,tmp,crules));
                }
                i++;
            }
            return(new tree(src->getvalue(),operate(src->geta(),operation,base,crules,true)));
        }
    }
    return(NULL);
}
char* display(char* src,int type=TREE_DMATH) {
    tree* tree2=parse(src);
    return(tree2==NULL?NULL:(tree2->test()?tree2->display(type):NULL));
}
