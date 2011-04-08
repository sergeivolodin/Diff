#include "diff.h"
#include "ui_diff.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <strings.h>
using namespace std;
/*
  segfault: lexer_normal, parser (из-за lexerа)
  parser: sin(cos(x))+10*8*x распознается как (sin(cos(x))+10*8)*x
//
  API, документация
*/
unsigned int sw=0;
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
const char* MATH_INTEGRAL_ADDC="+C";
const char* MATH_NO="-";
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
const int MATH_MAXREPL=100;
char** MATH_OPLIST=NULL;
const char MATH_CPOSTREPLD=',';
const char MATH_CPOSTREPLDC='=';
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
bool is_integer(const float a) {
    int temp=static_cast<int>(a);
    if(temp==a) {
        return(true);
    }
    return(false);
}
void print_str(const char* str) {
    if(str!=NULL) {
        int i=0;
        while(str[i]!='\0') {
            cerr << str[i];
            i++;
        }
        cerr << "... END AT " << i << " (" << strlen(str) <<")" << endl;
    }
}
bool str(const char* a,const char* b) {
    if(a==NULL||b==NULL) {
        return(false);
    }
    else {
        return(strcmp(a,b)==0);
    }
}
struct strings {
    char** strs;
    int max;
};
char* strcp(const char* src) {
    if(src==NULL){return(NULL);}
    char* destination=new char[strlen(src)+1];
    unsigned int i=0;
    while(src[i]!='\0') {
        destination[i]=src[i];
        i++;
    }
    destination[strlen(src)]='\0';
    return(destination);
}
char* stradd(char* l, const char* r){
    if(l==NULL) {return(strcp(r));}
    const unsigned int offset=strlen(l);
    const unsigned int maxindex=offset+strlen(r);
    char* d=new char[maxindex+2];
    unsigned int i=0;
    bool l_end=false;
    bool r_end=false;
    while(!r_end) {
        if(l[i]=='\0') {
            l_end=true;
        }

        if(l_end) {
            if(r[i-offset]!='\0') {
                d[i]=r[i-offset];
            }
            else {
                r_end=true;
            }
        }
        else  {
            d[i]=l[i];
        }
        i++;
    }
    d[maxindex]='\0';
    return(d);
}
strings* explode(const char* src,char sym=' ') {
    unsigned int i=0;
    strings* res=new strings;
    res->max=-1;
    unsigned int smax=strlen(src)-1;
    //cerr << "[" << smax << "]";
    if(smax==0||src==NULL||strlen(src)<=1) {
        return(res);
    }
    res->max=0;
    res->strs=new char*[smax];
    char* prevs=NULL,*temp=NULL;
    while(i<=(smax+1)) {
        if(src[i]==sym||i>smax) {
            if(prevs!=NULL) {
                res->strs[(res->max)++]=prevs;
                prevs=NULL;
            }
        }
        else {
            temp=new char[2];
            temp[0]=src[i];
            temp[1]='\0';
            prevs=stradd(prevs,temp);
        }
        i++;
    }
    res->max--;
    return(res);
}
bool strchar(const char* h,const char* n) {
    if(strlen(n)>1) {
        return(false);
    }
    unsigned int i=0;
    while((h[i]!='\0')&&(h[i]!=n[0])) {
        i++;
    }
    return(i!=strlen(h));
}
bool strchar(const char* h, char n) {
    char* a=new char[2];
    a[0]=n;
    a[1]=0;
    return(strchar(h,a));
}
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
    if(char_isnum(in)) {
        return(CHAR_TNUM);
    }
    if(char_islett(in)) {
        return(CHAR_TLETT);
    }
    if(char_isop(in)) {
        return(CHAR_TOP);
    }
    if(in==')') {
        return(CHAR_TBR2);
    }
    if(in=='(') {
        return(CHAR_TBR1);
    }
    if(in=='\0') {
        return(CHAR_TEND);
    }
    return(0);
}
struct token {
    char* c;
    int t;
};
struct lexer_answer {
    token* result;
    int max;
    bool errors;
};
const int TREE_DDEFAULT=TREE_DMATH;
char* char_replace(char* src,char a,char b) {
    char* result=new char[strlen(src)];
    int i=0;
    while(src[i]!='\0') {
        if(src[i]==a) {
            result[i]=b;
        }
        else {
            result[i]=src[i];
        }
        i++;
    }
    result[strlen(src)]='\0';
    return(result);
}
char* decimal_point(char* src) {
    return(char_replace(src,'.',','));
}
char* print_num(float a) {
    char* h=new char[15];
    if(is_integer(a)) {
        sprintf(h,"%d",static_cast<int>(a));
    }
    else {
        sprintf(h,"%f",a);
    }
    return(decimal_point(h));
}
void var_dump(const char* a) {
    cerr << "String (" << strlen(a) << "): [" << a << "]" << endl;
}
/*void var_dump(int a) {
        cerr << "Int: [" << a << "]" << endl;
}*/
int op_prio(char* a) {
    if(a==NULL) {
        return(-1);
    }
    int i=0;
    while(i<strlen(MATH_OPS)) {
        if(MATH_OPS[i]==a[0]) {
            return(i+1);
        }
        i++;
    }
    cerr << "Warning (prio): unexpected operator [" << a << "]" << endl;
    return(0);
}
char* repeat(const char* str,int count=1) {
    int i=1;
    char* res=NULL;
    while(i<=count) {
        res=stradd(res,str);
        i++;
    }
    return(res);
}
class tree {
private:
    char* display_math() {
        bool tmp;
        char* res=NULL;
        char* tmp_=NULL,*tmp2=NULL;
        if(value==NULL) {return(NULL);}
        if(leaf==true) {
            res=strcp(value);
        }
        else if(b!=NULL) {
            if(a==NULL) {return(NULL);}
            res=NULL;
            if((tmp=(!a->leaf))) {
                res=strdup(MATH_BR1);
                tmp_=a->display_math();
                if(tmp_==NULL) {
                    return(NULL);
                }
                res=stradd(res,tmp_);
            }
            else {
                tmp_=a->display_math();
                if(tmp_==NULL) {
                    return(NULL);
                }
                res=tmp_;
            }
            if(tmp) {
                res=stradd(res,MATH_BR2);
            }
            res=stradd(res,value);
            if((tmp=(!b->leaf))) {
                res=stradd(res,MATH_BR1);
            }
            tmp_=b->display_math();
            if(tmp_==NULL) {
                return(NULL);
            }
            res=stradd(res,tmp_);
            if(tmp) {
                res=stradd(res,MATH_BR2);
            }
        }
        else {
            tmp_=new char[strlen(value)];
            strcpy(tmp_,value);
            res=tmp_;
            res=stradd(res,MATH_BR1);
            tmp2=a->display_math();
            if(tmp2==NULL) {
                return(NULL);
            }
            res=stradd(res,tmp2);
            res=stradd(res,MATH_BR2);
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
            cerr << "!=";
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
            return(1+a->opcount()+b->opcount());
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
struct parser_answer {
    int pos;
    tree* tr;
};
struct trees {
    tree** t;
    int max;
};
lexer_answer* lexer(char* str) {
    int i=0;
    int len=strlen(str);
    int pos=0;
    char* temp=NULL,*aa=NULL;
    char current;
    int laststate=-1,currentstate;
    token* result=new token[len+2];
    lexer_answer* res=new lexer_answer;
    while(laststate!=CHAR_TEND) {
        if(len==pos) {
            current='\0';
            currentstate=CHAR_TEND;
        }
        else {
            current=tolower(str[pos]);
            if(current=='.') {
                current=',';
            }
            currentstate=char_state(current);
        }
        if(currentstate==0) {
            cerr << "Warning (lexer): wrong symbol at " << pos << endl;
            pos++;
            continue;
        }
        else {
            if((currentstate!=CHAR_TBR2)&&(currentstate!=CHAR_TBR1)&&(currentstate!=CHAR_TOP)&&(laststate==currentstate)) {
                aa=new char[2];
                aa[0]=current;
                aa[1]='\0';
                temp=stradd(temp,aa);
            }
            else {
                if(laststate!=-1) {
                    result[i].t=laststate;
                    result[i].c=strcp(temp);
                    aa=NULL;
                    temp=NULL;
                    i++;
                }
                if(current!='\0') {
                    temp=new char[2];
                    temp[0]=current;
                    temp[1]='\0';
                    aa=NULL;
                }
            }
        }
        laststate=currentstate;
        pos++;
    }
    res->max=i-1;
    res->result=result;
    res->errors=false;
    return(res);
}
lexer_answer* lexer_normal(lexer_answer* src) {
    int i=0,ri=0;
    lexer_answer* res=new lexer_answer;
    res->result=new token[((src->max)*2)+1];
    res->errors=src->errors;
    char* pow=strcp(MATH_POW),*mul=strcp(MATH_MUL);
    while(i<=src->max) {
        res->result[ri].c=src->result[i].c;
        res->result[ri].t=src->result[i].t;
        ri++;
        if(i<src->max) {
            if(src->result[i].t==CHAR_TNUM&&(src->result[i].c)[0]!='-'&&src->result[i+1].t==CHAR_TLETT) {
                res->result[ri].t=CHAR_TOP;
                res->result[ri].c=mul;
                ri++;
                i++;
                res->result[ri].c=src->result[i].c;
                res->result[ri].t=src->result[i].t;
                ri++;
                if(src->result[i+2].t==CHAR_TNUM&&(src->result[i+2].c)[0]!='-') {
                    res->result[ri].t=CHAR_TOP;
                    res->result[ri].c=pow;
                    ri++;
                    i++;
                    res->result[ri].c=src->result[i].c;
                    res->result[ri].t=src->result[i].t;
                    ri++;
                }
            }
            //repeat.
            if(src->result[i].t==CHAR_TLETT&&src->result[i+1].t==CHAR_TNUM&&(src->result[i+1].c)[0]!='-') {
                res->result[ri].t=CHAR_TOP;
                res->result[ri].c=pow;
                ri++;
                i++;
                res->result[ri].c=src->result[i].c;
                res->result[ri].t=src->result[i].t;
                ri++;
            }
        }
        i++;
    }
    res->max=ri-1;
    return(res);
}
parser_answer parser(lexer_answer *src,int pos=0,bool binary=true,int parent_prio=0,bool sub_allow=true,bool minus_to_plus=false) {
    tree* rtree=NULL;
    parser_answer result,tmpres,tmpres1,tmpres2;
    result.pos=pos;
    bool ok=false;
    result.tr=NULL;
    if(binary) {
        tmpres=parser(src,pos,false);
        if(src->result[tmpres.pos].t==CHAR_TOP) {
            if(minus_to_plus&&src->result[tmpres.pos].c==MATH_SUB) {
                src->result[tmpres.pos].c=strcp(MATH_ADD);
            }
            if(op_prio(src->result[tmpres.pos].c)>parent_prio) {
                tmpres1=parser(src,tmpres.pos+1,true,op_prio(src->result[tmpres.pos].c),false);
                if((src->result[tmpres1.pos].t==CHAR_TOP)&&sub_allow) {
                    tmpres2=parser(src,tmpres1.pos+1,true,0,true,false);
                    rtree=new tree(new tree(tmpres.tr,src->result[tmpres.pos].c,tmpres1.tr),src->result[tmpres1.pos].c,tmpres2.tr);
                    result.pos=tmpres2.pos;
                    ok=true;
                }
                else {
                    rtree=new tree(tmpres.tr,src->result[tmpres.pos].c,tmpres1.tr);
                    result.pos=tmpres1.pos;
                    ok=true;
                }
            }
        }
        else {
            ok=true;
            rtree=tmpres.tr;
            result.pos=tmpres.pos;
        }
    }
    if(src->result[pos].t==CHAR_TBR1&&!ok) {
        ok=true;
        tmpres=parser(src,pos+1);
        if(src->result[tmpres.pos].t==CHAR_TBR2) {
            result.pos=tmpres.pos+1;
            rtree=tmpres.tr;
        }
        else {
            cerr << "Warning (parser): Closing bracket not found" << endl;
            src->errors=true;
            return(result);
        }
    }
    if(src->result[pos].t==CHAR_TOP&&!ok) {
        ok=true;
        tmpres=parser(src,pos+1,false);
        tree* a;
        if(tmpres.tr!=NULL) {
            rtree=new tree(MATH_SUB,tmpres.tr);
            a=tmpres.tr;
            result.pos=tmpres.pos;
        }
    }
    if(src->result[pos].t==CHAR_TLETT&&!ok) {
        if(src->result[pos+1].t==CHAR_TBR1) {
            ok=true;
            tmpres=parser(src,pos+2);
            result.pos=tmpres.pos+1;
            if(src->result[tmpres.pos].t==CHAR_TBR2) {
                rtree=new tree(src->result[pos].c,tmpres.tr);
            }
            else {
                src->errors=true;
            }
        }
    }
    if((src->result[pos].t==CHAR_TNUM||src->result[pos].t==CHAR_TLETT)&&!ok) {
        ok=true;
        result.pos=pos+1;
        rtree=new tree(src->result[pos].c);
    }
    if(src->errors) {
        result.tr=NULL;
    }
    else {
        result.tr=rtree;
    }
    return(result);
}
tree* parse(char* src) {
    if(str(src,"")) {
        return(NULL);
    }
    lexer_answer* a;//,*a_src;
    parser_answer b;
    //a_src=lexer(src);
    //a=lexer_normal(a_src);
    a=lexer(src);
    b=parser(a);
    if(b.tr==NULL||a->errors) {
        return(NULL);
    }
    if(!(b.tr->test())) {
        return(NULL);
    }
    return(b.tr->copymem());
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
        cerr << "[pattern:" << stree->display() << "@" << spattern->display() << "]";
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
        return(res);
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
                if(a->r[i]->op==2) {
                    cerr << "integral";
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
rules* FRULES=NULL;
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
    replacers* repl=NULL,*tmp=NULL;
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
        cerr << i << " of " << vars->max << ": " << vars->t[i]->display() << " (" << opc << ")" << endl;
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
    bool set=false;
    tree* res=src;
    if(src->type()==TREE_TBINARY) {
        cerr << "calc " << src->display() << ": ";
        if(src->geta()->isleaf()&&src->getb()->isleaf()) {
            cerr << "a+b ";
            if(char_isnum(src->geta()->getvalue()[0])&&char_isnum(src->getb()->getvalue()[0])) {
                l=atof(src->geta()->getvalue());
                r=atof(src->getb()->getvalue());
                set=true;
            }
        }
        else if((!src->geta()->isleaf())&&(!src->getb()->isleaf())&&str(src->geta()->getvalue(),"-")&&str(src->getb()->getvalue(),"-")&&src->geta()->geta()->isleaf()&&src->getb()->geta()->isleaf()) {
            l=-atof(src->geta()->geta()->getvalue());
            r=-atof(src->getb()->geta()->getvalue());
            set=true;
        }
        if(set) {
            if(str(src->getvalue(),"+")) {
                cerr << "+";
                res=new tree(print_num(l+r));
            }
            if(str(src->getvalue(),"-")) {
                cerr << "-";
                res=new tree(print_num(l-r));
            }
            if(str(src->getvalue(),"*")) {
                cerr << "*";
                res=new tree(print_num(l*r));
            }
            if(str(src->getvalue(),"/")) {
                tmp_float=l/r;
                if(is_integer(tmp_float)) {
                    cerr << "/";
                    res=new tree(print_num(tmp_float));
                }
            }
            if(res->isleaf()&&res->getvalue()[0]=='-') {
                cerr << "[-a]=>-[a]";
                res=new tree("-",print_num(-atof(res->getvalue())));
            }
        }
        cerr << endl;
    }
    return(res);
}
tree* easy(tree* src) {
    cerr << "called easy of " << src->display() << endl;
    int i=0,opc=0;
    tree* res=src,*tmp;
    int min=src->varcount();
    if(src->type()==TREE_TBINARY) {
        res=new tree(easy(src->geta()),src->getvalue(),easy(src->getb()));
    }
    if(src->type()==TREE_TUNARY||src->type()==TREE_TFUNCTION) {
        res=new tree(src->getvalue(),easy(src->geta()));
    }
    trees* vars=getvariants(res,false);
    while(i<=vars->max) {
        tmp=calc(vars->t[i]);
        opc=tmp->varcount();
        cerr << i << " of " << vars->max << ": " << vars->t[i]->display() << " (" << opc << ")" << endl;
        if(opc<min) {
            min=opc;
            res=tmp;
        }
        i++;
    }
    return(calc(res));
}
tree* operate(tree* src,int operation,const char* base=MATH_DEFDIFF,rules* crules=FRULES,bool rmode=false) {
    int i=0,type=0;
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
            if(str(MATH_OPLIST[operation-1],src->getvalue())) {
                return(operate(src->geta(),operation,base,crules));
            }
            tmp1=MATH_VARS;
            tmp1=stradd(tmp1,MATH_REPLS);
            while(i<strlen(tmp1)) {
                tmp=new char[2];
                tmp[0]=tmp1[i];
                tmp[1]='\0';
                name=MATH_OPLIST[operation-1];
                name=stradd(name,tmp);
                if(str(name,src->getvalue())) {
                    return(operate(src->geta(),operation,tmp,crules));
                }
                i++;
            }
            return(new tree(src->getvalue(),operate(src->geta(),operation,base,crules,true)));
        }
    }
}
char* display(char* src,int type=TREE_DMATH) {
    tree* tree2=parse(src);
    return(tree2==NULL?NULL:(tree2->test()?tree2->display(type):NULL));
}
Diff::Diff(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::Diff)
{
    ui->setupUi(this);
}

Diff::~Diff()
{
    delete ui;
};
void Diff::on_button_diff_clicked() {
    tree* src=parse(ui->line_src->toPlainText().toAscii().data());
    tree* dest=NULL;
    char* res=strcp(ERROR);
    const char* base=strcp(ui->line_base->text().toAscii().data());
    QString a;
    if(src!=NULL) {
        res="src ok";
        dest=operate(src,1,base);
        if(dest!=NULL) {
            res="dest ok";
            res=easy(dest)->display();
            //res=dest->display();
        }
    }
    a=res;
    ui->line_dest->setPlainText(a);
}
void Diff::update_settings() {
    FRULES=parsestrs(ui->plainTextEdit->toPlainText().toAscii().data());
    MATH_VARS=strcp(ui->mvars->text().toAscii().data());
    MATH_FUNCS=strcp(ui->mall->text().toAscii().data());
    MATH_REPLS=strcp(ui->mrepl->text().toAscii().data());
    MATH_DEFDIFF=strcp(ui->mvar0->text().toAscii().data());
    MATH_OPS=strcp(ui->mops1->text().toAscii().data());
    int i=0;
    strings* tmp=explode(strcp(ui->mops->text().toAscii().data()),' ');
    if(tmp!=NULL) {
        if(tmp->max>=0) {
            MATH_OPLIST=new char*[tmp->max+1];
            while(i<=tmp->max) {
                MATH_OPLIST[i]=tmp->strs[i];
                i++;
            }
        }
    }
}
void Diff::on_button_parse_clicked() {
    update_settings();
}
void Diff::on_button_integral_clicked() {
}
void Diff::on_button_easy_clicked() {
    tree* src=parse(ui->line_src->toPlainText().toAscii().data());
    tree* dest=NULL;
    char* res=strcp(ERROR);
    QString a;
    if(src!=NULL) {
        res="src ok";
        dest=easy(src);
        if(dest!=NULL) {
            res=dest->display();
        }
    }
    a=res;
    ui->line_dest->setPlainText(a);
}
void Diff::on_button_show_clicked() {
    tree* src=parse(ui->line_src->toPlainText().toAscii().data());
    tree* dest=NULL;
    char* res=strcp(ERROR);
    QString a;
    if(src!=NULL) {
        res=src->display();
    }
    a=res;
    ui->line_dest->setPlainText(a);
}
void Diff::on_pushButton_clicked() {
    QString a,b,c;
    tree* x=parse(ui->teste->toPlainText().toAscii().data()),
    *y=parse(ui->testp->toPlainText().toAscii().data()),
    *z=parse(ui->testr_2->toPlainText().toAscii().data()),
    *trepl=NULL;
    replacers* repl=NULL;
    char* aa=NULL;
    if(x!=NULL&&y!=NULL) {
        cerr << "testing [" << x->display() << "] with [" << y->display() << "]" << endl;
        if(repl=pattern(x,y)) {
            int i=0;
            while(repl!=NULL&&i<=repl->maxindex) {
                aa=stradd(aa,repl->r[i]->l);
                aa=stradd(aa,"=");
                aa=stradd(aa,repl->r[i]->r->display());
                aa=stradd(aa,"\n");
                i++;
            }
            b=aa;
            if(z->test()) {
                trepl=replace(z,repl);
            }
            else {
                trepl=new tree("FALSE");
            }
            c=trepl->display();
            a="yes";
            cerr << "[yes]";
        }
        else {
            a="no";
            cerr << "[no]";
        }
    }
    else {
        a="error";
    }
    ui->mreplacers->setPlainText(b);
    ui->testr->setPlainText(a);
    ui->testr1->setPlainText(c);
}
void Diff::changeEvent(QEvent *e)
{
    if(sw<=1) {
        if(sw==1) {
            update_settings();
        }
        sw++;
    }
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void Diff::on_testv_button_clicked()
{
    tree* a=parse(strcp(ui->testv_src->text().toAscii().data()));
    trees* b=getvariants(a);
    char* d="";
    int i=0;
    while(i<=b->max) {
        d=stradd(d,b->t[i]->display());
        d=stradd(d,"\n");
        i++;
    }
    QString d1;
    d1=d;
    ui->testv_dest->setPlainText(d1);
}