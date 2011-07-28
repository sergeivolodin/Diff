#include <iostream>
#include "default.cpp"
#include "funcs.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#define BUFLEN 1000
#define CONFFILE ".diff_program"

long long int stime1=0;
using namespace std;
/*
  упрощение
*/
int DEBUG=0; //0-silent;1-some;2-full
bool FULLEASY=true;
int MAXITER=100;
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
const char* MATH_CALC="calc";
//наборы
char* MATH_NUMS="abcde";
char* MATH_VARS="xyz";
//MATH_OPS дальше
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
const int TREE_DMATHB=10;
const int TREE_DTREE=1;
const int TREE_ADEF=0;
const int TREE_ASYM=1;
const int TREE_TNUM=0;
const int TREE_TVAR=4;
const int TREE_TUNARY=1;
const int TREE_TBINARY=2;
const int TREE_TFUNCTION=3;
struct operation {
    char* a;
    int p;
};
struct operations {
    operation** a;
    int max;
};
operations* MATH_OPS=NULL;
struct token {
    char* c;
    int t;
};
struct lexer_answer {
    token** result;
    int max;
    bool errors;
};
const int TREE_DDEFAULT=TREE_DMATHB;
inline bool char_isnum(char sym) {
    return((sym>='0'&&sym<='9')||sym=='.'||sym==',');
}
inline bool char_isnum_m(char sym) {
    return((sym>='0'&&sym<='9')||sym=='.'||sym==','||sym=='-');
}
inline bool char_islett(char sym) {
    return((sym>='A'&&sym<='Z')||(sym>='a'&&sym<='z'));
}
bool char_isop(char current) {
    int i=0;
    while(i<=MATH_OPS->max) {
        if(MATH_OPS->a[i]->a[0]==current) return(true);
        i++;
    }
    return(false);
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
    while(i<=MATH_OPS->max) {
        if(str(MATH_OPS->a[i]->a,a)) {
            return(MATH_OPS->a[i]->p);
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
        if(value==NULL) return(NULL);
        if(type()==TREE_TNUM||type()==TREE_TVAR) {
            res=strcp(value);
        }
        if(type()==TREE_TUNARY||type()==TREE_TFUNCTION) {
            res=strcp(value);
            if(tmp=((type()==TREE_TFUNCTION||(a->type()==TREE_TBINARY&&type()==TREE_TUNARY)))) res=stradd(res,MATH_BR1);
            res=stradd(res,a->display_math(brackets));
            if(tmp) res=stradd(res,MATH_BR2);
        }
        if(type()==TREE_TBINARY) {
            if(tmp=((brackets&&a->type()!=TREE_TNUM&&a->type()!=TREE_TVAR)||(a->type()==TREE_TBINARY&&(op_prio(a->value)<op_prio(value))))) res=strcp(MATH_BR1);
            else res="";
            res=stradd(res,a->display_math(brackets));
            if(tmp) res=stradd(res,MATH_BR2);
            res=stradd(res,value);
            if(tmp=((brackets&&b->type()!=TREE_TVAR&&b->type()!=TREE_TNUM)||(b->type()==TREE_TBINARY&&(op_prio(b->value)<op_prio(value))))) res=stradd(res,strcp(MATH_BR1));
            res=stradd(res,b->display_math(brackets));
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
    bool operator!=(tree x) {
        return(!(x==*this));
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
        if(leaf) return(str(src,value));
        else {
            if(b==NULL) return(a->contains(src));
            else return(a->contains(src)||b->contains(src));
        }
    }
    bool test() {
        if(leaf) {
            if(value==NULL) return(false);
        }
        else {
            if(a==NULL) return(false);
            if(b==NULL) return(a->test());
            else return(a->test()&&b->test());
        }
        return(true);
    }
    int type() {
        if(leaf) {
            if(char_isnum_m(value[0])) return(TREE_TNUM);
            else return(TREE_TVAR);
        }
        else {
            if(b==NULL) {
                if(char_islett(value[0])) return(TREE_TFUNCTION);
                else return(TREE_TUNARY);
            }
            else return(TREE_TBINARY);
        }
    }
    int varcount() {
        int ttype=type();
        if(ttype==TREE_TUNARY||ttype==TREE_TFUNCTION) return(a->varcount());
        else if(ttype==TREE_TBINARY) return(a->varcount()+b->varcount());
        else if(ttype==TREE_TVAR) return(1);
        else return(0);
    }
    int lenght() {
        int ttype=type();
        if(ttype==TREE_TVAR||ttype==TREE_TNUM) return(1);
        else if(ttype==TREE_TFUNCTION||ttype==TREE_TUNARY) return(1+a->lenght());
        else if(ttype==TREE_TBINARY) return(1+a->lenght()+b->lenght());
        else return(0);
    }
    int opcount() {
        int ttype=type();
        if(ttype==TREE_TUNARY||ttype==TREE_TFUNCTION) return(1+a->opcount());
        else if(ttype==TREE_TBINARY) return(2+a->opcount()+b->opcount());
        else if(ttype==TREE_TVAR) return(1);
        else return(0);
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
        if(dtype==TREE_DMATH) return(display_math());
        else if(dtype==TREE_DTREE) return(display_tree());
        else if(dtype==TREE_DMATHB) return(display_math(true));
        else return(display(TREE_DDEFAULT));
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
            cerr << "Warning (lexer): wrong symbol '" << c << "' at " << i << endl;
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
        src->r[src->maxindex]->r=which;
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
//ВНИМАНИЕ: функции сравнивают указатели, а не их содержимое!
//возвращает -1, если что-то пошло не так.
//расстояние от корня до нужного указателя
bool contains(tree* where,tree* what,bool pointer=true) {
    if((pointer&&where==what)||(*where==*what&&!pointer)) return(true);
    if(where->type()==TREE_TBINARY) return(contains(where->geta(),what,pointer)||contains(where->getb(),what,pointer));
    if(where->type()==TREE_TFUNCTION||where->type()==TREE_TUNARY) return(contains(where->geta(),what,pointer));
    return(false);
}
int length(tree* where, tree* a,bool addleft=false,bool addright=false) {
    if(where==a) return(0);
    int wtype=where->type(),tlen;
    if(wtype==TREE_TFUNCTION||wtype==TREE_TUNARY) {
        tlen=length(where->geta(),a);
        if(tlen!=-1) return(1+tlen);
    }
    if(wtype==TREE_TBINARY) {
        tlen=length(where->geta(),a);
        if(tlen!=-1) return(addleft?2+tlen:1+tlen);
        tlen=length(where->getb(),a);
        if(tlen!=-1) return(addright?2+tlen:1+tlen);
    }
    return(-1);
}
//расстояние между двумя
int length(tree* where,tree* a,tree* b) {
    int wtype=where->type(),tlen1,tlen2;
    if(wtype==TREE_TFUNCTION||wtype==TREE_TUNARY) {
        return(length(where->geta(),a,b));
    }
    if(wtype==TREE_TBINARY) {
        bool aca=contains(where->geta(),a);
        bool acb=contains(where->geta(),b);
        bool bca=contains(where->getb(),a);
        bool bcb=contains(where->getb(),b);
        if(aca&&acb) return(length(where->geta(),a,b));
        if(bca&&bcb) return(length(where->getb(),a,b));
        if(aca&&bcb) {
            if(where->geta()==a&&where->getb()==b) return(0);
            tlen1=length(where->geta(),a);
            tlen2=length(where->getb(),b);
            if(tlen1!=-1&&tlen2!=-1) return(1+tlen1+tlen2);
        }
        if(acb&&bca) {
            tlen1=length(where->geta(),b);
            tlen2=length(where->getb(),a);
            if(tlen1!=-1&&tlen2!=-1) return(2+tlen1+tlen2);
        }
    }
    return(-1);
}
//без замен!
struct treerepls {
    tree* t;
    replacers* r;
};
treerepls* rule_apply(tree* src,rule* r,char* base,replacers* repls) {
    treerepls* res=new treerepls;
    res->r=pattern(src,r->src,base,repls);
    if(!res->r) return(NULL);
    res->t=replace(r->dest,res->r);
    return(res);
}
tree* rule_apply(tree* src,rule* r,char* base) {
    replacers* a=pattern(src,r->src,base);
    if(!a) return(NULL);
    return(replace(r->dest,a));
}
struct treemin {
    tree* t;
    int l;
};
//"вытягивает" наверх what из where
treemin* tree_join(tree* where,tree* a,bool addleft=false,bool addright=false,treemin* w=NULL) {
    if(!(addleft||addright)) {
        treemin* x=tree_join(where,a,false,true,w),*y=tree_join(where,a,true,false,w);
        if(x->l<y->l) return(x);
        return(y);
    }
    treemin* res;
    tree* ttree;
    int tlen;
    if(where==a||!contains(where,a)) {
        res=new treemin;
        res->l=length(where,a,addleft,addright);
        res->t=where;
        return(res);
    }
    //cerr << "TJ called: where=" << where->display() << " what=" << a->display() << " al=" << addleft << " ar=" << addright << " tmin=?" << (w!=NULL) << endl;
    if(where->type()==TREE_TUNARY||where->type()==TREE_TFUNCTION) {
        if(contains(where->geta(),a)) where=new tree(where->getvalue(),tree_join(where->geta(),a)->t);
    }
    if(where->type()==TREE_TBINARY) {
        if(contains(where->geta(),a)) where=new tree(tree_join(where->geta(),a)->t,where->getvalue(),where->getb());
        if(contains(where->getb(),a)) where=new tree(where->geta(),where->getvalue(),tree_join(where->getb(),a)->t);
    }
    if(w) res=w;
    else {
        res=new treemin;
        res->l=length(where,a,addleft,addright);
        res->t=where;
    }
    int i=0;
    while(i<=FRULES->maxindex) {
        if(FRULES->r[i]->op==0&&(ttree=rule_apply(where,FRULES->r[i],MATH_DEFDIFF))) {
            tlen=length(ttree,a,addleft,addright);
            if(tlen<res->l) {
                //cerr << "where=" << where->display(TREE_DMATHB) << " applying rule #" << i << ": " << FRULES->r[i]->src->display(TREE_DMATHB) << " => " << ttree->display(TREE_DMATHB) << " l=" << tlen << "/" << res->l << endl;
                res->l=tlen;
                res->t=ttree;
                tree_join(ttree,a,false,false,res);
                break;
            }
        }
        i++;
    }
    return(res);
}
treemin* tree_join_full(tree* where,tree* a,bool addleft,bool addright) {
    return(tree_join(tree_join(where,a)->t,a,addleft,addright));
}
tree* tree_join(tree* where,tree* a,tree* b) {
    if(!a||!b) return(NULL);
    int wtype=where->type();
    tree* ttree,*ttree1,*ttree2;
    tree* wsrc=where->copymem();
    if(wtype==TREE_TBINARY) {
        bool aca=contains(where->geta(),a);
        bool acb=contains(where->geta(),b);
        bool bca=contains(where->getb(),a);
        bool bcb=contains(where->getb(),b);
        if(aca&&acb) {
            ttree=tree_join(where->geta(),a,b);
            if(ttree) where=new tree(ttree,where->getvalue(),where->getb());
            else return(NULL);
        }
        if(bca&&bcb) {
            ttree=tree_join(where->getb(),a,b);
            if(ttree) where=new tree(where->geta(),where->getvalue(),ttree);
            else return(NULL);
        }
        //здесь обязательно явно указывать "направление" дерева!
        if(aca&&bcb) where=new tree(tree_join(where->geta(),a,true,false)->t,where->getvalue(),tree_join(where->getb(),b,false,true)->t);
        if(bca&&acb) where=new tree(tree_join(where->geta(),b,true,false)->t,where->getvalue(),tree_join(where->getb(),a,false,true)->t);
    }
    //if(*wsrc!=*where) cerr << "TJ-mod: " << wsrc->display() << " => " << where->display() << endl;
    int i=0;
    int slen=length(where,a,b),dlen,dlen1;
    if(!slen) return(where);
    while(i<=FRULES->maxindex) {
        if(FRULES->r[i]->op==0&&(ttree1=rule_apply(where,FRULES->r[i],MATH_DEFDIFF))) {
            dlen=length(ttree1,a,b);
            //cerr << "TJ1: where=" << where->display() << " a=" << a->display() << " b=" << b->display() << " (" << slen << ") trying rule #" << i << " " << FRULES->r[i]->src->display() << " => " << ttree1->display() << " (" << dlen << ")" << endl;
            if(dlen<slen) {
                if(!dlen) return(ttree1);
                ttree2=tree_join(ttree1,a,b);
                if(ttree2) {
                    dlen1=length(ttree2,a,b);
                    if(!dlen1) return(ttree2);
                }
            }
        }
        i++;
    }
    return(NULL);
}
tree* rreplace(tree* where,rule* r,char* base,bool* replaced) {
    tree *tmp=NULL,*tmp1=NULL;
    if(tmp1=rule_apply(where,r,base)) {
        *replaced=true;
        return(tmp1);
    }
    int wtype=where->type();
    if(wtype==TREE_TUNARY||wtype==TREE_TFUNCTION) return(new tree(where->getvalue(),rreplace(where->geta(),r,base,replaced)));
    if(wtype==TREE_TBINARY) return(new tree(rreplace(where->geta(),r,base,replaced),where->getvalue(),rreplace(where->getb(),r,base,replaced)));
    return(where);
}
replacers* copy_replacers(replacers* src) {
    replacers* res=new replacers;
    res->maxindex=src->maxindex;
    res->r=new replacer*[res->maxindex+1];
    int i=0;
    while(i<=src->maxindex) {
        res->r[i]=new replacer;
        res->r[i]->l=strcp(src->r[i]->l);
        res->r[i]->r=src->r[i]->r->copymem();
        i++;
    }
    return(res);
}
tree* find_next(tree* where,tree* spattern,trees* nolist,char* base,replacers* repl=NULL) {
    replacers* wr=NULL,*wrt=NULL,*wrt1=NULL;
    tree* tr=NULL;
    if(repl==NULL) {
        wr=new replacers;
        wr->maxindex=-1;
        wr->r=new replacer*[pattern_count_funcs(where)+1];
    }
    else wr=repl;
    int wtype=where->type();
    bool inlist=false;
    int i=0;
    int si=0;
    /*while(si<=wr->maxindex) {
        cerr << si << ": " << wr->r[si]->l << "=" << wr->r[i]->r->display(TREE_DMATHB) << endl;
        si++;
    }*/
    wrt1=copy_replacers(wr);
    if((wrt=pattern(where,spattern,base,wrt1))) {
        //if((wrt=pattern(where,spattern,base))) {
        while(i<=nolist->max) {
            if(nolist->t[i]==where) {
                inlist=true;
            }
            i++;
        }
        if(!inlist) {
            wr=wrt;
            return(where);
        }
        /*else {
            //wr=wrt1;
            //cerr << "inlist: " << where->display() << " rule=" << spattern->display() << endl;
        }*/
    }
    /*else {
        cerr << "!pattern " << where->display() << " " << spattern->display() << endl;
    }*/
    if(wtype==TREE_TUNARY||wtype==TREE_TFUNCTION) {
        return(find_next(where->geta(),spattern,nolist,base,wr));
    }
    if(wtype==TREE_TBINARY) {
        tr=find_next(where->geta(),spattern,nolist,base,wr);
        if(tr) return(tr);
        else return(find_next(where->getb(),spattern,nolist,base,wr));
    }
    return(NULL);
}
trees* find_all(tree* where,tree* spattern,char* base,replacers* repl=NULL,trees* ut=NULL) {
    trees* res=NULL;
    if(ut==NULL) {
        res=new trees;
        res->max=-1;
        res->t=new tree*[where->varcount()*2];
    }
    else res=ut;
    replacers* wr=NULL,*wr1,*wr2;
    if(repl==NULL) {
        wr=new replacers;
        wr->maxindex=-1;
        wr->r=new replacer*[pattern_count_funcs(where)+1];
    }
    else wr=repl;
    int wtype=where->type();
    wr1=copy_replacers(wr);
    if(repl!=NULL&&(wr2=pattern(where,spattern,base,wr1))) {
        wr=wr2;
        res->max++;
        res->t[res->max]=where;
    }
    if(wtype==TREE_TBINARY) {
        find_all(where->geta(),spattern,base,wr,res);
        find_all(where->getb(),spattern,base,wr,res);
    }
    if(wtype==TREE_TUNARY||wtype==TREE_TFUNCTION) find_all(where->geta(),spattern,base,wr,res);
    return(res);
}
void print_trees(trees* a) {
    int i=0;
    while(i<=a->max) {
        cerr << "trees[" << i << "]: " << a->t[i]->display(TREE_DMATHB) << endl;
        i++;
    }
    cerr << "ok" << endl;
}
tree* tree_replace_r(tree* src,tree* a,tree* b,rule* trule,bool one=true) {
    if(DEBUG>1) cerr << "a=" << a->display() << " b=" << b->display() << endl;
    trees* l1=find_all(src,a,MATH_DEFDIFF),*l2=find_all(src,b,MATH_DEFDIFF),*tt;
    if(DEBUG>1) {
        cerr << "==l1==" << endl;
        print_trees(l1);
        cerr << "==l2==" << endl;
        print_trees(l2);
    }
    replacers* trepl=NULL;
    tree* tmp,*ta,*a1=NULL,*a2=NULL;
    bool tbool;
    int i=0,si;
    if(DEBUG>1) cerr << l1->max << " " << l2->max << endl;
    if(l1->max<0||l2->max<0) return(NULL);
    while(i<=l1->max) {
        si=0;
        while(si<=l2->max) {
            if(l1->t[i]&&l2->t[si]&&(l1->t[i]!=l2->t[si])) {
                stime1-=mtime();
                tmp=tree_join(src,l1->t[i],l2->t[si]);
                stime1+=mtime();
                if(tmp) {
                    if(DEBUG>1) cerr << "join ok - " << tmp->display(TREE_DMATH) << "(" << length(src,l1->t[i],l2->t[si]) << ")" << endl;
                    if(one) {
                        tbool=false;
                        
                        tmp=rreplace(tmp,trule,MATH_DEFDIFF,&tbool);
                        
                        if(tbool) {return(tmp);}
                    }
                    else {
                        tt=find_all(src,trule->src,MATH_DEFDIFF);
                        if(tt->max!=-1) {return(tmp);}
                    }
                }
                else if(DEBUG>1) {
                    cerr << "join failed " << l1->t[i]->display() << " " << l2->t[si]->display() << endl;
                }
            }
            si++;
        }
        i++;
    }
    /*tmp=src;
    if(l1->max==-1&&a->type()==TREE_TBINARY) {
        tmp=tree_replace_r(tmp,a->geta(),a->getb(),trule,false);
        if(tmp==NULL) return(NULL);
    }
    if(l2->max==-1&&b->type()==TREE_TBINARY) tmp=tree_replace_r(tmp,b->geta(),b->getb(),trule,false);
    if(tmp&&!(*tmp==*src)) {
        return(tree_replace_r(tmp,a,b,trule));
    }*/
    return(NULL);
}
tree* easy_one(tree* src) {
    int i=0;
    tree* tmp=NULL,*res=src,*a,*b;
    int min=src->opcount();
    bool tbool,ok;
    while(i<=FRULES->maxindex) {
        if(FRULES->r[i]->op==0) {
            if(FRULES->r[i]->src->opcount()>FRULES->r[i]->dest->opcount()||(FRULES->r[i]->dest->type()==TREE_TFUNCTION&&str(MATH_CALC,FRULES->r[i]->dest->getvalue()))) {
                //упрощающее правило, либо calc
                tbool=false;
                tmp=rreplace(src,FRULES->r[i],MATH_DEFDIFF,&tbool);
                if(DEBUG>1) cerr << " TRYING [" << FRULES->r[i]->src->display(TREE_DMATHB) << "]: ";
                if(tbool) {
                    if(DEBUG>1) cerr << "OK";
                    return(tmp);
                }
                else if((FRULES->r[i]->src->type()==TREE_TBINARY)&&FULLEASY) {
                    //бинарная операция в шаблоне, разбиваем на части и тянем друг к другу
                    if(DEBUG>1) cerr << "BINARY... ";
                    a=FRULES->r[i]->src->geta();
                    b=FRULES->r[i]->src->getb();
                    if(tmp=tree_replace_r(src,a,b,FRULES->r[i])) {
                        if(DEBUG>1) cerr << "OK";
                        return(tmp);
                    }
                    else if(DEBUG>1) cerr << "FAILED";
                }
                else if(DEBUG>1) cerr << "FAILED";
            }
        }
        i++;
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
    if(strs->max==-1) return(NULL);
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
    if(c>=0) res->maxindex=c-1;
    return(res);
}
operations* string_to_operations(char* src) {
    operations* res=new operations;
    res->max=-1;
    strings* s=explode(src,' ');
    int i=0,si;
    i=0;
    res->a=new operation*[strlen(src)];
    while(i<=s->max) {
        si=0;
        while(si<strlen(s->strs[i])) {
            res->max++;
            res->a[res->max]=new operation;
            res->a[res->max]->p=i;
            res->a[res->max]->a=new char[2];
            res->a[res->max]->a[0]=s->strs[i][si];
            res->a[res->max]->a[1]=0;
            //cerr << "Writing operation: " << s->strs[i][si] << " prio=" << i << endl;
            si++;
        }
        i++;
    }
    return(res);
}
char* operations_to_string(operations* a) {
    int i=0;
    char* res="";
    int prev;
    while(i<=a->max) {
        if(i>0&&a->a[i]->p!=prev) res=stradd(res," ");
        res=stradd(res,a->a[i]->a);
        prev=a->a[i]->p;
        i++;
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
        fputs(operations_to_string(MATH_OPS),file);
        fputs("\t",file);
        int i=0;
        while(i<=MATH_OPLIST->max) {
            fputs(MATH_OPLIST->strs[i],file);
            if(i<MATH_OPLIST->max) fputs(" ",file);
            i++;
        }
        fputs("\t",file);
        fputs(print_num(MAXITER),file);
        fputs("\t",file);
        fputs(print_num(DEBUG),file);
        fputs("\t",file);
        fputs(print(FULLEASY),file);
        fputs("\n",file);
        fputs(print_num(R->maxindex),file);
        fputs("\n",file);
        i=0;
        while(i<=R->maxindex) {
            buf=R->r[i]->src->display(TREE_DMATHB);
            buf=stradd(buf,MATH_CCOMMOND);
            buf=stradd(buf,print_num(R->r[i]->op));
            buf=stradd(buf,MATH_CCOMMOND);
            buf=stradd(buf,R->r[i]->dest->display(TREE_DMATHB));
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
    if(file==NULL) read_ok=false;
    else {
        char* buf=new char[BUFLEN];
        rule* tr=NULL;
        FRULES=new rules;
        FRULES->maxindex=-1;
        buf=fgets(buf,BUFLEN,file);
        if(buf!=NULL&&buf[0]!=0) {
            strings* a=explode(buf,'\t');
            if(a->max==9) {
                remove_nl(a->strs[9]);
                MATH_VARS=a->strs[0];
                MATH_FUNCS=a->strs[1];
                MATH_NUMS=a->strs[2];
                MATH_REPLS=a->strs[3];
                MATH_DEFDIFF=a->strs[4];
                MATH_OPS=string_to_operations(a->strs[5]);
                MATH_OPLIST=explode(a->strs[6],' ');
                MAXITER=atoi(a->strs[7]);
                DEBUG=atoi(a->strs[8]);
                FULLEASY=atob(a->strs[9]);
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
        else read_ok=false;
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
        if(*(a->t[i])==*b) return(true);
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
    else res=write;
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
    if(write==NULL) return(res);
    return(NULL);
}
tree* easy_all(tree* src) {
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
        if(str(src->getvalue(),MATH_CALC)) {
            return(rcalc(src->geta()));
        }
        return(calc(new tree(src->getvalue(),rcalc(src->geta()))));
    }
    return(src);
}
tree* easy(tree* src) {
    long long int alltime=mtime();
    stime1=0;

    unsigned int i=0;
    tree* tmp=src->copymem(),*tmp1=new tree("");
    do {
        tmp1=tmp->copymem();
        tmp=rcalc(easy_one(rcalc(tmp)));
        if(DEBUG) cerr << "easy iter#" << i << "  " << tmp->display() << endl;
        i++;
    }
    while(i<MAXITER&&*tmp!=*tmp1);

    alltime=mtime()-alltime;
    if(DEBUG) cerr << "sum: " << 100.0*stime1/alltime << "% (" << stime1 << ") of " << alltime << endl;
    return(tmp);
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
            return(NULL);
        }
        return(NULL);
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
