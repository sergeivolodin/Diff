#include "diff.h"
#include "ui_diff.h"
#include <iostream>
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
  API, документация
  расстановка скобок в парсере
  gui
*/
struct strings {
    char** strs;
    int max;
};
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
char* stradd(char* l,char r) {
    char* a=new char[2];
    a[0]=r;
    a[1]=0;
    return(stradd(l,a));
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
    token** result;
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
    int i=0,cstate=0,pstate=0;
    int len=strlen(str);
    char c,*tmp;
    lexer_answer* res=new lexer_answer;
    res->result=new token*[len];
    res->max=-1;
    while(i<len) {
        c=str[i];
        if(c==',') c='.';
        cstate=char_state(c);
        if(cstate==0) {
            cerr << "Warning (lexer): wrong symbol '" << c << "' at" << i << endl;
        }
        else {
            tmp=new char[2];
            tmp[0]=c;
            tmp[1]=0;
            if(res->max==-1||cstate!=pstate||cstate==CHAR_TBR2) {
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
bool ptest(parser_answer* src) {
    if(src!=NULL&&src->tr!=NULL&&src->tr->test()) {
        return(true);
    }
    return(false);
}
void perr(const char* a) {
    cout << "Warning (parser/NULL): " << a << endl;
}
parser_answer* parser(lexer_answer *src,int pos=0,bool binary=true,int parent_prio=0,bool sub_allow=true);
parser_answer* parse_atomic(lexer_answer *,int);
parser_answer* parse_function(lexer_answer *,int);
parser_answer* parse_unary(lexer_answer *,int);
parser_answer* parse_brackets(lexer_answer *,int);
parser_answer* parse_atomic(lexer_answer *src,int pos=0) {
    if((src->result[pos]->t==CHAR_TNUM||src->result[pos]->t==CHAR_TLETT)) {
        parser_answer* result=new parser_answer;
        result->pos=pos+1;
        result->tr=new tree(src->result[pos]->c);
        return(result);
    }
    return(NULL);
}
parser_answer* parse_function(lexer_answer *src,int pos=0) {
    if(src->result[pos]->t==CHAR_TLETT) {
        if(((pos+1)<=src->max)&&src->result[pos+1]->t==CHAR_TBR1) {
            parser_answer* tmpres=parser(src,pos+2);
            if(!ptest(tmpres)) {
                perr("func tmpres");
                return(NULL);
            }
            if(tmpres->pos<=src->max&&src->result[tmpres->pos]->t==CHAR_TBR2) {
                parser_answer* result=new parser_answer;
                //cerr << "parsing function " << src->result[pos]->c << "(" << tmpres->tr->display() << ")" << endl;
                result->tr=new tree(src->result[pos]->c,tmpres->tr);
                result->pos=tmpres->pos+1;
                return(result);
            }
            else {
                if(tmpres->pos<=src->max) {
                    cerr << "wrong function " << src->result[pos]->c << tmpres->tr->display() << " next=" << src->result[tmpres->pos]->c << endl;
                }
                src->errors=true;
                return(NULL);
            }
        }
    }
    return(NULL);
}
parser_answer* parse_unary(lexer_answer *src,int pos=0) {
    if(src->result[pos]->t==CHAR_TOP) {
        parser_answer* tmpres=parser(src,pos+1,false);
        if(!ptest(tmpres)) {
            perr("unary tmpres");
            return(NULL);
        }
        parser_answer* result=new parser_answer;
        result->tr=new tree(MATH_SUB,tmpres->tr);
        result->pos=tmpres->pos;
        return(result);
    }
    return(NULL);
}
parser_answer* parse_brackets(lexer_answer *src,int pos=0) {
    if(src->result[pos]->t==CHAR_TBR1) {
        parser_answer* tmpres=parser(src,pos+1);
        if(!ptest(tmpres)) {
            perr("() tmpres");
            return(NULL);
        }
        if(tmpres->pos<=src->max&&src->result[tmpres->pos]->t==CHAR_TBR2) {
            parser_answer* result=new parser_answer;
            result->pos=tmpres->pos+1;
            result->tr=tmpres->tr;
            return(result);
        }
        else {
            cerr << "Warning (parser): Closing bracket not found" << endl;
            src->errors=true;
            perr(") bracket");
            return(NULL);
        }
    }
    return(NULL);
}
//parser_answer* parser(lexer_answer *src,int pos=0,bool binary=true,int parent_prio=0,bool sub_allow=true) {
parser_answer* parser(lexer_answer *src,int pos,bool binary,int parent_prio,bool sub_allow) {
    parser_answer* result=NULL,*tmpres=NULL,*tmpres1=NULL,*tmpres2=NULL;
    result=new parser_answer;
    result->pos=pos;
    result->tr=NULL;
    if(pos>src->max||src->errors) {
        perr("pos/errors");
        return(NULL);
    }
    if(binary) {
        //первый операнд, без бинарных операций
        tmpres=parser(src,pos,false);
        if(!ptest(tmpres)) {
            perr("binary tmpres");
            return(NULL);
        }
        if(tmpres->pos<=src->max&&src->result[tmpres->pos]->t==CHAR_TOP) {
            //если следующий за первым - операция
            if(op_prio(src->result[tmpres->pos]->c)>parent_prio) {
                //если приоритет текущей операции больше приоритета предыдущей
                //парсер возвращает второй операнд
                tmpres1=parser(src,tmpres->pos+1,true,op_prio(src->result[tmpres->pos]->c));
                if(!ptest(tmpres1)) {
                    perr("binary tmpres1");
                    return(NULL);
                }
                if(tmpres1->pos<=src->max&&(src->result[tmpres1->pos]->t==CHAR_TOP)&&sub_allow) {
                    //если следующий - операция и разрешено
                    tmpres2=parser(src,tmpres1->pos+1,true,0,true);
                    if(!ptest(tmpres2)) {
                        perr("binary tmpres2");
                        return(NULL);
                    }
                    result->tr=new tree(
                            new tree(tmpres->tr,src->result[tmpres->pos]->c,tmpres1->tr),
                            src->result[tmpres1->pos]->c,tmpres2->tr);
                    result->pos=tmpres2->pos;
                    return(result);
                }
                else {
                    //иначе первый-операция-второй
                    result->tr=new tree(tmpres->tr,src->result[tmpres->pos]->c,tmpres1->tr);
                    result->pos=tmpres1->pos;
                    return(result);
                }
            }
        }
        else {
            result->tr=tmpres->tr;
            result->pos=tmpres->pos;
            return(result);
        }
    }
    if((result=parse_brackets(src,pos))!=NULL) return(result);
    if((result=parse_unary(src,pos))!=NULL) return(result);
    if((result=parse_function(src,pos))!=NULL) return(result);
    if((result=parse_atomic(src,pos))!=NULL) return(result);
    return(NULL);
}
tree* parse(char* src) {
    if(str(src,"")) {
        return(NULL);
    }
    lexer_answer* a=lexer(src);;//,*a_src;
    parser_answer* b=parser(a);
    //a_src=lexer(src);
    //a=lexer_normal(a_src)
    if(!ptest(b)) {
        return(NULL);
    }
    return(b->tr->copymem());
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
rules* parsetable(QTableWidget* a,int type) {
    int i=0;
    char* buf=NULL;
    QTableWidgetItem *i1,*i2,*i3;
    rules* res=new rules;
    int c=a->rowCount();
    res->maxindex=-1;
    res->r=new rule*[c+1];
    while(i<c) {
        if(((i1=a->item(i,0))!=NULL)&&((i2=a->item(i,1))!=NULL)) {
            buf=i1->text().toAscii().data();
            buf=stradd(buf,MATH_CCOMMOND);
            buf=stradd(buf,print_num(type));
            buf=stradd(buf,MATH_CCOMMOND);
            buf=stradd(buf,i2->text().toAscii().data());
            i3=a->item(i,2);
            if(i3!=NULL&&i3->text()!=NULL) {
                buf=stradd(buf,MATH_CPOSTREPL);
                buf=stradd(buf,i3->text().toAscii().data());
            }
            if(buf!=NULL) {
                res->r[++(res->maxindex)]=parsestr(buf);
            }
        }
        i++;
    }
    return(res);
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
rules* FRULES=NULL;
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
        int si;
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
void remove_nl(char* a) {
    if(a[strlen(a)-1]=='\n') {
       a[strlen(a)-1]=0;
    }
}
void remove_nl(strings* a) {
    int i=0;
    while(i<=a->max) {
        remove_nl(a->strs[i]);
        i++;
    }
}
void settings_from_file(Ui::Diff* UI,const char* filename=CONFFILE) {
    FILE *file=fopen(filename, "r");
    char* buf=new char[BUFLEN];
    rule* tr=NULL;
    FRULES=new rules;
    FRULES->maxindex=-1;
    buf=fgets(buf,BUFLEN,file);
    strings* a=explode(buf,'\t');
    if(a->max==6) {
        remove_nl(a->strs[6]);
        MATH_VARS=a->strs[0];
        MATH_FUNCS=a->strs[1];
        MATH_NUMS=a->strs[2];
        MATH_REPLS=a->strs[3];
        MATH_DEFDIFF=a->strs[4];
        MATH_OPS=a->strs[5];
        print_str(a->strs[6]);
        MATH_OPLIST=explode(a->strs[6],' ');
        UI->mnums->setText(QString(MATH_NUMS));
        UI->mall->setText(QString(MATH_FUNCS));
        UI->mvars->setText(QString(MATH_VARS));
        UI->mrepl->setText(QString(MATH_REPLS));
        UI->mvar0->setText(QString(MATH_DEFDIFF));
        UI->mops1->setText(QString(MATH_OPS));
        UI->mops->setText(QString(a->strs[6]));
        cerr << MATH_DEFDIFF << " " << MATH_VARS;
    }
    else {
        cerr << "Warning [settings]: wrong parameters count!" << endl;
    }
    buf=fgets(buf,BUFLEN,file);
    int i=0,m=atoi(buf);
    FRULES->r=new rule*[m+1];
    cerr << m << endl;
    while(fgets(buf,BUFLEN,file)&&FRULES->maxindex<=m) {
        remove_nl(buf);
        if((tr=parsestr(buf))!=NULL) {
            FRULES->r[++FRULES->maxindex]=tr;
        }
        i++;
    }
    fclose(file);
}
void rules_to_table(QTableWidget* a,int type,rules* R=FRULES) {
    if(R!=NULL&&a!=NULL&&R->r!=NULL) {
        if(R->maxindex>=0) {
            a->setRowCount(0);
            int i=0,si=0;
            a->setSortingEnabled(false);
            QTableWidgetItem* ti=NULL;
            while(i<=R->maxindex) {
                if(R->r[i]!=NULL&&R->r[i]->op==type) {
                    a->setRowCount(a->rowCount()+1);
                    ti=new QTableWidgetItem(QString(R->r[i]->src->display()));
                    if(ti!=NULL) {
                        a->setItem(si,0,ti);
                    }
                    ti=new QTableWidgetItem(QString(R->r[i]->dest->display()));
                    if(ti!=NULL) {
                        a->setItem(si,1,ti);
                    }
                    ti=new QTableWidgetItem(QString(postrepls_to_string(R->r[i]->postrepl,false)));
                    if(ti!=NULL) {
                        a->setItem(si,2,ti);
                        si++;
                    }
                }
                i++;
            }
        }
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
    cerr << "called easy of " << src->display() << endl;
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

Diff::~Diff() {
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
    FRULES=rules_merge(parsetable(ui->tableWidget_3,1),parsetable(ui->tableWidget_2,0));
    MATH_NUMS=strcp(ui->mnums->text().toAscii().data());
    MATH_VARS=strcp(ui->mvars->text().toAscii().data());
    MATH_FUNCS=strcp(ui->mall->text().toAscii().data());
    MATH_REPLS=strcp(ui->mrepl->text().toAscii().data());
    MATH_DEFDIFF=strcp(ui->mvar0->text().toAscii().data());
    MATH_OPS=strcp(ui->mops1->text().toAscii().data());
    MATH_OPLIST=explode(strcp(ui->mops->text().toAscii().data()),' ');
    printrules(FRULES);
}
void Diff::on_button_parse_clicked() {
    update_settings();
    if(ui->checkBox->isChecked()) {
        settings_to_file();
    }
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
void Diff::changeEvent(QEvent *e) {
    if(sw<=1) {
        if(sw==1) {
            settings_from_file(ui);
            rules_to_table(ui->tableWidget_2,0);
            rules_to_table(ui->tableWidget_3,1);
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
void Diff::on_testv_button_clicked() {
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
void Diff::on_pushButton_2_clicked() {
    ui->tableWidget_3->setRowCount(ui->tableWidget_3->rowCount()+1);
}

void Diff::on_pushButton_3_clicked() {
    ui->tableWidget_3->setRowCount(ui->tableWidget_3->rowCount()-1);
}

void Diff::on_pushButton_4_clicked() {
    ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);
}

void Diff::on_pushButton_5_clicked() {
    ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()-1);
}
