#include "diff.h"
#include "ui_diff.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <strings.h>
/*
  lexer_normal FAILS.
  diff table: functions, operations
  custom operations
  new functions (api), documenation
  */
/*
  tree::type() leaf-0,unary-1,binary-2
  diff table: add !contains (x) - for integrals, also parsing (add '!' to variant of letters;
    operator== in trees: add order change for it.
  */
/*
  segfault in (lg(x)^2)^2-3*lg(x)-1
   (lexer_normal problem)
  */
using namespace std;
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
    return((current=='_'||current=='+'||current=='-'||current=='*'||current=='/'||current=='^'));
}
const int CHAR_TNUM=1; //numbers: 0-9.
const int CHAR_TLETT=2; //letters a-zA-Z
const int CHAR_TOP=3; //operations: +-*/^
const int CHAR_TEND=4; //end of string
const int CHAR_TBR1=5; //opening bracket
const int CHAR_TBR2=6; //closing bracket
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
    char* c; //content
    int t; //type
};
struct lexer_answer {
    token* result;
    int max;
    bool errors;
};
struct strings {
    char** strs;
    int max;
};
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

//strings
const char* ERROR="Error.";
const char* MATH_BR1="(";
const char* MATH_BR2=")";
const char* MATH_INTEGRAL_ADDC="+C";
const char* MATH_NADD="%2B";
const char* MATH_NO="-";
const char* MATH_NUMS="abc";
const char* MATH_VARS="xyz";
const char* MATH_FUNCS="fgh";
//constans
const char* MATH_PI="pi";
const char* MATH_M1="-1";
const char* MATH_DEFDIFF="x";
const char* MATH_0="0";
const char* MATH_1="1";
const char* MATH_2="2";
//functions
const char* MATH_SIN="sin";
const char* MATH_COS="cos";
const char* MATH_TG="tg";
const char* MATH_ERF="erf";
const char* MATH_EXP="exp";
const char* MATH_LN="ln";
const char* MATH_SQRT="sqrt";
const char* MATH_ARCSIN="arcsin";
const char* MATH_ARCCOS="arccos";
const char* MATH_ABS="abs";
const char* MATH_ARCTG="arctg";
//operations
const char* MATH_MUL="*";
const char* MATH_DIV="/";
const char* MATH_ADD="+";
const char* MATH_SUB="-";
const char* MATH_POW="^";
const char* MATH_POW1="_";
//tree constans
const int TREE_DMATH=0;
const int TREE_DTREE=1;
const int TREE_DNIGMA=2;
const int TREE_ADEF=0;
const int TREE_ASYM=1;
const int TREE_TNUM=0;
const int TREE_TVAR=4;
const int TREE_TUNARY=1;
const int TREE_TBINARY=2;
const int TREE_TFUNCTION=3;
const char* TREE_NIGMA="http://math3.nigma.ru/maxima2/drivermath.php?render=$";
//tree settings
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
char* strcp(const char* src) {
    if(src==NULL){return(NULL);}
    char* destination=new char[strlen(src)];
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
    //right ends after left
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
    //delete[] l;
    return(d);
}
void var_dump(const char* a) {
    cerr << "String (" << strlen(a) << "): [" << a << "]" << endl;
}
/*void var_dump(int a) {
        cerr << "Int: [" << a << "]" << endl;
}*/
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
    char* display_math(bool nigma=false) {
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
                tmp_=a->display_math(nigma);
                if(tmp_==NULL) {
                    return(NULL);
                }
                res=stradd(res,tmp_);
            }
            else {
                tmp_=a->display_math(nigma);
                if(tmp_==NULL) {
                    return(NULL);
                }
                res=tmp_;
            }
            if(tmp) {
                res=stradd(res,MATH_BR2);
            }
            if(str(value,MATH_ADD)&&nigma) {
                res=stradd(res,MATH_NADD);
            }
            else {
                res=stradd(res,value);
            }
            //&&b->b!=NULL
            if((tmp=(!b->leaf))) {
                res=stradd(res,MATH_BR1);
            }
            tmp_=b->display_math(nigma);
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
            tmp2=a->display_math(nigma);
            if(tmp2==NULL) {
                return(NULL);
            }
            res=stradd(res,tmp2);
            res=stradd(res,MATH_BR2);
        }
        return(res);
    }
    int type_sym() {
        //???
        //if(type==)
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
    //Operation/value
    char* value;
    //leaf?
    bool leaf;
    //if leaf=>two pointers to other trees
    //if unary only "a" used
    tree *a,*b;
        public:
    //leaf
    tree(const char* str): value(strcp(str)),leaf(true) {}
    //binary with trees
    tree(tree *in_a,const char* op,tree *in_b): value(strcp(op)),leaf(false),a(in_a),b(in_b) {}
    //unary with tree
    tree(const char *op,tree *in_): value(strcp(op)),leaf(false),a(in_),b(NULL) {}
    //binary with leaves
    tree(const char* ina, const char* op, const char* inb): value(strcp(op)),leaf(false) {
        a=new tree(ina);
        b=new tree(inb);
    }
    //unary with leaf
    tree(const char* op,const char* in_): value(strcp(op)),leaf(false),b(NULL) {
        a=new tree(in_);
    }
    tree():value(NULL),leaf(false),a(NULL),b(NULL){}
    //returns pointer to other but same tree
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
    //bugs.
    /*~tree() {
        cerr << "DESTRUCTOR CALLED [" << display() << "]" << endl;
        if(leaf) {
            if(value!=NULL) {
                var_dump(value);
                delete[] value;
            }
        }
        else {
            if(a!=NULL) {
                delete a;
            }
            if(b!=NULL) {
                delete b;
            }
        }
    }*/
    inline bool isleaf() {
        return(leaf);
    }
    bool pattern(tree* spattern,const char* base=MATH_DEFDIFF) {
        if((*this)==(*spattern)) {
            return(true);
        }
        int ctype=type();
        int stype=spattern->type();
        //tree types:num/var/unary/binary/function
        if(stype==ctype) {
            cerr << "type==" << endl;
            //types equal, so lets test sub-trees.
            if(ctype==TREE_TNUM) {
                //current is num. pattern can be [number, var or function]
                if(strchar(MATH_NUMS,spattern->value)||strchar(MATH_VARS,spattern->value)||strchar(MATH_FUNCS,spattern->value)) {
                    return(true);
                }
            }
            if(ctype==TREE_TVAR) {
                //cerr << "var";
                //current is var. pattern can be [var or function]
                if(strchar(MATH_VARS,spattern->value)||strchar(MATH_FUNCS,spattern->value)) {
                    if(base!=NULL) {
                        if(str(value,base)) {
                            return(true);
                        }
                    }
                }
            }
            if(ctype==TREE_TFUNCTION) {
                //current is function. pattern can be [number, var or function]
                if(strchar(MATH_FUNCS,spattern->value)) {
                    return(true);
                }
            }
            if(ctype==TREE_TUNARY) {
                if(str(value,spattern->value)) {
                    //-(x),-(f)
                    return(a->pattern(spattern->a,base));
                }
            }
            if(ctype==TREE_TBINARY) {
                //commutative operations support will be added later
                //2-level commutative changing will be unuseful here
                if(str(value,spattern->value)) {
                    //x+y,f+g
                    return(a->pattern(spattern->a,base)&&b->pattern(spattern->b,base));
                }
            }
        }
        else {
            if(stype==TREE_TVAR) {
                if(strchar(MATH_FUNCS,spattern->value)) {
                    return(true);
                }
                if(strchar(MATH_NUMS,spattern->value)) {
                    tree* num=new tree("a");
                    if(ctype==TREE_TBINARY) {
                        //a and b contains ONLY numbers
                        return((!a->contains(base))&&(!b->contains(base)));
                    }
                    if(ctype==TREE_TUNARY||ctype==TREE_TFUNCTION) {
                        //b contains ONLY numbers
                        return(!(a->contains(base)));
                    }
                    if(ctype==TREE_TNUM) {
                        return(true);
                    }
                }
            }
        }
        return(false);
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
                //?x
                if(char_islett(value[0])) {
                    //f(x)
                    return(TREE_TFUNCTION);
                }
                else {
                    //[op]x
                    return(TREE_TUNARY);
                }
            }
            else {
                //x+y
                return(TREE_TBINARY);
            }
        }
    }
    tree* diff_new(const char* base) {
        tree* nt=NULL;
        int ctype=type();

        return(nt);
    }
    tree* diff(const char* base) {
        tree* nt=NULL;
        if(leaf==true) {
            if(value==NULL) {
                return(NULL);
            }
            if(str(value,base)) {
                nt=new tree(MATH_1);
            }
            else {
                nt=new tree(MATH_0);
            }
        }
        else {
            if(a==NULL) {
                return(NULL);
            }
            tree* adiff=a->diff(base);
            if(adiff==NULL) {
                return(NULL);
            }
            if(b==NULL) {
                if(str(value,MATH_ABS)) {
                    nt=new tree(MATH_SQRT,new tree(adiff,MATH_POW,new tree(MATH_2)));
                }
                if(str(value,MATH_SUB)) {
                    nt=new tree(MATH_SUB,adiff);
                }
                if(str(value,MATH_LN)) {
                    nt=new tree(adiff,MATH_MUL,new tree(new tree(MATH_1),MATH_DIV,a));
                }
                if(str(value,MATH_SIN)) {
                    nt=new tree(adiff,MATH_MUL,new tree(MATH_COS,a));
                }
                if(str(value,MATH_COS)) {
                    nt=new tree(adiff,MATH_MUL,new tree(MATH_SUB,new tree(MATH_SIN,a)));
                }
                if(str(value,MATH_ARCSIN)) {
                    nt=new tree(adiff,MATH_MUL,new tree(new tree(MATH_1),MATH_DIV,new tree(MATH_SQRT,new tree(new tree(MATH_1),MATH_SUB,new tree(a,MATH_POW,new tree(MATH_2))))));
                }
                if(str(value,MATH_ARCCOS)) {
                    nt=new tree(MATH_SUB,new tree(adiff,MATH_MUL,new tree(new tree(MATH_1),MATH_DIV,new tree(MATH_SQRT,new tree(new tree(MATH_1),MATH_SUB,new tree(a,MATH_POW,new tree(MATH_2)))))));
                }
                if(str(value,MATH_ARCTG)) {
                    nt=new tree(adiff,MATH_MUL,new tree(new tree(MATH_1),MATH_DIV,new tree(new tree(MATH_1),MATH_ADD,new tree(a,MATH_POW,new tree(MATH_2)))));
                }
                if(str(value,MATH_TG)) {
                    nt=new tree(adiff,MATH_MUL,new tree(new tree(MATH_1),MATH_DIV,new tree(new tree(MATH_COS,a),MATH_POW,new tree(MATH_2))));
                }
                if(str(value,MATH_EXP)) {
                    nt=new tree(adiff,MATH_MUL,new tree(MATH_EXP,a));
                }
                if(str(value,MATH_SQRT)) {
                    nt=new tree(adiff,MATH_MUL,new tree(new tree(MATH_1),MATH_DIV,new tree(new tree(MATH_2),MATH_MUL,new tree(MATH_SQRT,a))));
                }
                if(str(value,MATH_ERF)) {
                    nt=new tree(adiff,MATH_MUL,new tree(new tree(new tree(MATH_2),MATH_DIV,new tree(MATH_SQRT,MATH_PI)),MATH_MUL,new tree(MATH_EXP,new tree(MATH_SUB,new tree(base,MATH_POW,MATH_2)))));
                }
                if(nt==NULL) {
                    return(NULL);
                }
            }
            else {
                tree *bdiff=b->diff(base);
                if(bdiff==NULL) {
                    return(NULL);
                }
                if(str(value,MATH_DIV)) {
                    nt=new tree(new tree(new tree(adiff,MATH_MUL,b),MATH_SUB,new tree(a,MATH_MUL,bdiff)),MATH_DIV,new tree(b,MATH_POW,new tree(MATH_2)));
                }
                if(str(value,MATH_ADD)||str(value,MATH_SUB)) {
                    nt=new tree(adiff,value,bdiff);
                }
                if(str(value,MATH_MUL)) {
                    nt=new tree(new tree(a,MATH_MUL,bdiff),MATH_ADD,new tree(adiff,MATH_MUL,b));
                }
                if(str(value,MATH_POW)) {
                    tree* tmp;
                    if(b->contains(base)||!(a->leaf&&str(a->value,base))) {
                        tmp=new tree(MATH_EXP,new tree(new tree(MATH_LN,a),MATH_MUL,b));
                        nt=tmp->diff(base);
                    }
                    else {
                        nt=new tree(b,MATH_MUL,new tree(a,value,new tree(b,MATH_SUB,new tree(MATH_1))));
                    }
                }
            }
        }
        return(nt);
    }
    bool operator==(tree sec) {
        //bool tmp=false;
        /*if(value==NULL||sec.value==NULL) {
            return(false);
        }*/
        if(!str(value,sec.value)) {
            return(false);
        }
        if(leaf&&sec.leaf) {
            return(true);
        }
        if(!leaf&&!sec.leaf) {
            if(a==NULL||sec.a==NULL) {
                return(false);
            }
            if(b==NULL&&sec.b==NULL) {
                return(*a==*sec.a);
            }
            if(b!=NULL&&sec.b!=NULL) {
                if(b==NULL||sec.b==NULL) {
                    return(false);
                }
                if(str(value,MATH_MUL)||str(value,MATH_ADD)) {
                    return((((*b==*(sec.b)))&&(*a==(*(sec.a))))||(((*a==*(sec.b)))&&(*b==(*(sec.a)))));
                }
                return(((*b==*(sec.b)))&&(*a==(*(sec.a))));
            }
        }
        return(false);
    }

    void operator=(tree *src) {
        /*delete *a;
        delete *b;*/
        tree *newtree;
        newtree=src->copymem();
        leaf=newtree->leaf;
        value=newtree->value;
        a=newtree->a;
        b=newtree->b;
    }
    unsigned int size() {
        if(leaf) {
            return(0);
        }
        else {
            if(b==NULL) {
                return(a->size()+1);
            }
            else {
                return(a->size()+b->size()+1);
            }
        }
    }
    tree* integral(const char* base) {
        tree* rt=NULL,*ta=NULL,*tb=NULL;
        if(leaf) {
            if(str(value,MATH_1)) {
                rt=new tree(base);
            }
            if(str(value,base)) {
                rt=new tree(new tree(base,MATH_POW,MATH_2),MATH_DIV,new tree(MATH_2));
            }
            if(!contains(base)) {
                rt=new tree(value,MATH_MUL,base);
            }
        }
        else {
            if(b==NULL) {
                //unary minus
                if(str(value,MATH_SUB)) {
                    rt=new tree(MATH_SUB,a->integral(base));
                }
                //functions from 'x' (sin(x),cos(x)...)
                if(str(a->value,base)) {
                    if(str(value,MATH_SIN)) {
                        rt=new tree(MATH_SUB,new tree(MATH_COS,a));
                    }
                    if(str(value,MATH_COS)) {
                        rt=new tree(MATH_SIN,a);
                    }
                    if(str(value,MATH_TG)) {
                        rt=new tree(new tree(MATH_1),MATH_DIV,new tree(new tree(MATH_COS,base),MATH_POW,new tree(MATH_2)));
                    }
                    if(str(value,MATH_EXP)) {
                        rt=new tree(MATH_EXP,base);
                    }
                }
                if(!contains(base)) {
                    rt=new tree(this,MATH_MUL,new tree(base));
                }
            }
            else {
                //binary
                //add/substr.
                if(str(value,MATH_ADD)||str(value,MATH_SUB)) {
                    ta=a->integral(base);
                    tb=b->integral(base);
                    if((ta!=NULL)&&(tb!=NULL)) {
                        rt=new tree(ta,value,tb);
                    }
                }
                //power
                if(str(value,MATH_POW)) {
                    //x^a
                    if(str(a->value,base)&&!b->contains(base)&&b->leaf) {
                        if(str(b->value,"-1")) {
                            rt=new tree(MATH_LN,new tree(MATH_ABS,base));
                        }
                        else {
                            rt=new tree(new tree(new tree(base),MATH_POW,new tree(b,MATH_ADD,new tree(MATH_1))),MATH_DIV,new tree(b,MATH_ADD,new tree(MATH_1)));
                        }
                    }
                    if(!a->contains(base)&&b->contains(base)&&b->leaf) {
                        rt=new tree(new tree(a,MATH_POW,new tree(base)),MATH_DIV,new tree(MATH_LN,a));
                    }
                }
                //mult.
                if(str(value,MATH_MUL)) {
                    if(!(a->contains(base))) {
                        tb=b->integral(base);
                        if(tb!=NULL) {
                            rt=new tree(a,MATH_MUL,tb);
                        }
                    }
                    if(!(b->contains(base))) {
                        ta=a->integral(base);
                        if(ta!=NULL) {
                            rt=new tree(ta,MATH_MUL,b);
                        }
                    }
                }
                //division
                if(str(value,MATH_DIV)) {
                    if(!b->contains(base)) {
                        rt=new tree(a->integral(base),MATH_DIV,b);
                    }
                }
            }
        }
        return(rt);
    }

    tree* easy() {
        tree *nt,*rt=NULL,*nta,*ntb;
        bool ok=false;
        if(!leaf) {
            if(a==NULL) {
                return(NULL);
            }
            if(b==NULL) {
                nt=a->easy();
                rt=new tree(value,nt);
                if(nt==NULL) {
                    return(NULL);
                }
                if(str(value,MATH_SUB)) {
                    rt=new tree(MATH_SUB,nt);
                    if(!nt->leaf) {
                        if(nt->b==NULL) {
                            if(str(nt->value,MATH_SUB)) {
                                rt=nt->a;
                            }
                        }
                    }
                    ok=true;
                }
                else if(str(value,MATH_SQRT)) {
                    if(nt->leaf) {
                        float sq_res=1.1;
                        if(is_integer(sq_res=sqrt(atof(nt->value)))&&char_isnum(nt->value[0])) {
                            rt=new tree(print_num(sq_res));
                            ok=true;
                        }
                    }
                }
                else if(str(value,MATH_EXP)) {
                    if(!nt->leaf) {
                        if(nt->b==NULL&&str(nt->value,MATH_LN)) {
                            rt=nt->a;
                            ok=true;
                        }
                        else if(str(nt->value,MATH_MUL)) {
                            //ln(...)*...
                            if(!nt->a->leaf&&str(nt->a->value,MATH_LN)) {
                                ok=true;
                                rt=new tree(nt->a->a,MATH_POW,nt->b);
                            }
                            if(!nt->b->leaf&&str(nt->b->value,MATH_LN)) {
                                ok=true;
                                rt=new tree(nt->b->a,MATH_POW,nt->a);
                            }
                        }
                    }

                }
            }
            else {
                nta=a->easy();
                ntb=b->easy();
                if(nta==NULL||ntb==NULL) {
                    return(NULL);
                }
                nt=new tree(nta,value,ntb);
                rt=nt;
                if(str(value,MATH_POW)) {
                    if(nt->a->leaf&&nt->b->leaf) {
                        if(char_isnum_m(nt->a->value[0])&&char_isnum_m(nt->b->value[0])) {
                            char* h=print_num(pow(atof(nt->a->value),atof(nt->b->value)));
                            rt=new tree(h);
                        }
                    }
                    if(nt->b->leaf) {
                        if(atof(nt->b->value)==1) {
                            rt=nt->a;
                        }
                    }
                }
                if(str(value,MATH_DIV)) {
                    if(nt->a->leaf&&nt->b->leaf) {
                        if(char_isnum_m(nt->a->value[0])&&char_isnum_m(nt->b->value[0])) {
                            if(atoi(nt->b->value)==0) {
                                cout << "Division by zero." << endl;
                            }
                            else if(((atoi(nt->a->value)*10) % atoi(nt->b->value))==0) {
                                char* h=print_num(atof(nt->a->value)/atof(nt->b->value));
                                rt=new tree(h);
                            }
                        }
                    }
                    if(atof(nt->a->value)==0&&char_isnum_m(nt->a->value[0])) {
                        rt=new tree(MATH_0);
                    }
                    if(atof(nt->b->value)==0&&char_isnum_m(nt->b->value[0])) {
                        cout << "Division by zero.";
                        rt=new tree(MATH_SUB);
                    }
                    if(atof(nt->b->value)==1&&char_isnum_m(nt->b->value[0])) {
                        rt=nt->a->copymem();
                    }
                }
                if(str(value,MATH_ADD)) {
                    if(!nt->b->leaf&&nt->b->b==NULL&&str(nt->b->value,MATH_SUB)) {
                        rt=new tree(nt->a,MATH_SUB,nt->b->a);
                    }
                    if(!nt->b->leaf&&str(nt->b->value,MATH_SUB)&&nt->b->b==NULL) {
                        if((*(nt->a))==(*(nt->b->a))) {
                            rt=new tree(MATH_0);
                        }
                    }
                    if(!nt->a->leaf&&str(nt->a->value,MATH_SUB)&&nt->a->b==NULL) {
                        if((*(nt->a->a))==(*(nt->b))) {
                            rt=new tree(MATH_0);
                        }
                    }
                    if((*(nt->a))==(*(nt->b))) {
                        //cerr << nt->a->display() << value << nt->b->display() << endl;
                        rt=new tree(new tree(MATH_2),MATH_MUL,nt->a);
                    }
                    if(nt->a->leaf&&atof(nt->a->value)==0&&char_isnum_m(nt->a->value[0])) {
                        rt=nt->b->copymem();
                    }
                    if(nt->b->leaf&&atof(nt->b->value)==0&&char_isnum_m(nt->b->value[0])) {
                        rt=nt->a->copymem();
                    }
                    if(nt->a->leaf&&nt->b->leaf) {
                        if(char_isnum_m(nt->a->value[0])&&char_isnum_m(nt->b->value[0])) {
                            char* h=print_num(atof(nt->a->value)+atof(nt->b->value));
                            rt=new tree(h);
                        }
                    }
                }
                if(str(value,MATH_SUB)) {
                    //dont change order!
                    if(atof(nt->a->value)==0&&char_isnum_m(nt->a->value[0])&&nt->a->leaf) {
                        rt=new tree(MATH_SUB,nt->b);
                    }
                    if(atof(nt->b->value)==0&&char_isnum_m(nt->b->value[0])&&nt->b->leaf) {
                        rt=nt->a->copymem();
                    }
                    if(char_isnum_m(nt->a->value[0])&&char_isnum_m(nt->b->value[0])&&nt->a->leaf&&nt->b->leaf) {
                        char* h=print_num(atof(nt->a->value)-atof(nt->b->value));
                        rt=new tree(h);
                    }
                    if(*(nt->a)==*(nt->b)) {
                        rt=new tree(MATH_0);
                    }
                }
                if(str(value,MATH_MUL)) {
                    if(nt->a->leaf&&char_isnum_m(nt->a->value[0])) {
                        //3*(7*...)
                        if(!nt->b->leaf&&nt->b->a->leaf&&char_isnum_m(nt->b->a->value[0])) {
                            if(str(nt->b->value,MATH_MUL)) {
                                rt=new tree(new tree(print_num(atof(nt->a->value)*atof(nt->b->a->value))),MATH_MUL,nt->b->b);
                            }
                        }
                    }
                    //(1/x)*x
                    if(!nt->a->leaf&&str(nt->a->value,MATH_DIV)) {
                        if(*nt->a->b==*nt->b) {
                            rt=new tree(MATH_1);
                        }
                    }
                    if((nt->a->leaf&&str(nt->a->value,MATH_0))||(nt->b->leaf&&str(nt->b->value,MATH_0))) {
                        rt=new tree(MATH_0);
                    }
                    if(char_isnum_m(nt->a->value[0])&&char_isnum_m(nt->b->value[0])&&nt->a->leaf&&nt->b->leaf) {
                        char* h=print_num(atof(nt->a->value)*atof(nt->b->value));
                        rt=new tree(h);
                    }
                    if(atof(nt->a->value)==1&&char_isnum_m(nt->a->value[0])) {
                        rt=nt->b;
                    }
                    if(atof(nt->b->value)==1&&char_isnum_m(nt->b->value[0])) {
                        rt=nt->a;
                    }
                    //3*...
                }
            }
        }
        else {
            if(value==NULL) {
                rt=NULL;
            }
            else {
                rt=new tree(value);
            }
        }
        return rt;
    }
    char* display(int dtype=TREE_DDEFAULT) {
        if(dtype==TREE_DMATH) {
            return(display_math());
        }
        else if(dtype==TREE_DNIGMA) {
            char* a=strcp(TREE_NIGMA);
            char* b=display_math(true);
            a=stradd(a,b);
            a=stradd(a,"$");
            return(a);
        }
        else if(dtype==TREE_DTREE) {
            return(display_tree());
        }
        else {
            return(display(TREE_DDEFAULT));
        }
    }
};
struct parser_answer {
    int pos;
    tree* tr;
};
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
lexer_answer* lexer(char* str) {
    //print_str(str);
    int i=0;
    int len=strlen(str);
    int pos=0;
    char* temp=NULL,*aa=NULL;
    //char* bb;
    char current;
    int laststate=-1,currentstate;
    token* result=new token[len+1]; //char count more than tokens count.
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
            cerr << "Wrong symbol at " << pos << endl;
            pos++;
            //res.max=-1;
            continue;
            //res.errors=true;
            //return(res);
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
                    if(current=='_') {
                        current='^';
                    }
                    result[i].c=new char[strlen(temp)];
                    result[i].t=laststate;
                    strcpy(result[i].c,temp);
                    result[i].c=result[i].c;
                    //result[i].c[strlen(temp)]='\0';
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
int op_prio(char* a) {
    static const int def=0;
    if(a==NULL) {
        return(def);
    }
    if(a[0]=='+'||a[0]=='-') {
        return(1);
    }
    if(a[0]=='*'||a[0]=='/') {
        return(2);
    }
    if(a[0]=='^') {
        return(3);
    }
    cerr << "unexpected operator [" << a << "]" << endl;
    //its prio is smaller
    return(def);
}
parser_answer parser(lexer_answer *src,int pos=0,bool binary=true,int parent_prio=0,bool sub_allow=true,bool minus_to_plus=false) {
    tree* rtree=NULL;
    parser_answer result,tmpres,tmpres1,tmpres2;
    result.pos=pos;
    bool ok=false;
    result.tr=NULL;
    if(binary) {
        //first operand
        tmpres=parser(src,pos,false);
        if(src->result[tmpres.pos].t==CHAR_TOP) {
            if(minus_to_plus&&src->result[tmpres.pos].c==MATH_SUB) {
                src->result[tmpres.pos].c=strcp(MATH_ADD);
            }
            //its binary
            if(op_prio(src->result[tmpres.pos].c)>parent_prio) {
                //if current prio is bigger than parent prio
                tmpres1=parser(src,tmpres.pos+1,true,op_prio(src->result[tmpres.pos].c),false);
                if((src->result[tmpres1.pos].t==CHAR_TOP)&&sub_allow) {
                    //if sub_recursion allowed AND next sym is operation
                    tmpres2=parser(src,tmpres1.pos+1,true,0,true,true);
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
    //brackets
    if(src->result[pos].t==CHAR_TBR1&&!ok) {
        ok=true;
        tmpres=parser(src,pos+1);
        if(src->result[tmpres.pos].t==CHAR_TBR2) {
            result.pos=tmpres.pos+1;
            rtree=tmpres.tr;
        }
        else {
            cerr << "Closing bracket not found" << endl;
            src->errors=true;
            return(result);
        }
    }
    //unary
    if(src->result[pos].t==CHAR_TOP&&!ok) {
        ok=true;
        tmpres=parser(src,pos+1,false);
        tree* a,*b;
        if(str(src->result[pos].c,MATH_SUB)&&tmpres.tr!=NULL) {
            rtree=new tree(MATH_SUB,tmpres.tr);
            a=tmpres.tr;
            if(a->test()) {
                b=a->easy();
                if(b->isleaf()&&char_isnum(b->getvalue()[0])) {
                    rtree=new tree(print_num(-atof(b->getvalue())));
                }
            }
            result.pos=tmpres.pos;
        }
        else {
            cerr << pos << ": unary \"" << src->result[pos].c << "\" not allowed" << endl;
            src->errors=true;
            result.pos=tmpres.pos;
        }
    }
    //functions
    if(src->result[pos].t==CHAR_TLETT&&!ok) {
        if(src->result[pos+1].t==CHAR_TBR1) {
            ok=true;
            tmpres=parser(src,pos+2);
            result.pos=tmpres.pos+1;
            if(src->result[tmpres.pos].t==CHAR_TBR2) {
                //cerr << "Function parse called, new position is " << result.pos << " (it contains [" << src->result[tmpres.pos+1].c  << "])" << endl;
                rtree=new tree(src->result[pos].c,tmpres.tr);
            }
            else {
                src->errors=true;
                cerr << tmpres.pos << ": function closing bracket expected, but '" << src->result[tmpres.pos-1].c << "' found." << endl;
            }
        }
    }
    //atomic
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
    lexer_answer* a,*a_src;
    parser_answer b;
    a_src=lexer(src);
    a=lexer_normal(a_src);
    b=parser(a);
    if(b.tr==NULL||a->errors) {
        return(NULL);
    }
    return(b.tr->copymem());
}
class funcrule {
private:
    tree* src;
    int operation; //1-diff,2-integral
    tree* dest;
public:
    funcrule():src(NULL),operation(0),dest(NULL){}
    funcrule(const char* string) {
        //sin(x) 1 cos(x)
        //)
        //- if no der./int.
        strings* expl=explode(string);
        if(expl->max<2) {
            cerr << "-0-";
            operation=0;
        }
        else {
            src=parse(expl->strs[0]);
            operation=atoi(expl->strs[1]);
            dest=parse(expl->strs[2]);
            if(src==NULL||dest==NULL||operation<1||operation>2) {
                operation=0;
            }
        }
    }
    inline bool valid() {return(operation>0);}
    void print() {
        if(operation!=0) {
            if(operation==1) {
                cerr << "diff";
            }
            else {
                cerr << "integral";
            }
            cerr << "(" << src->display() << ")=" << dest->display() << endl;
        }
    }
};
class operation {
private:
    bool commutative; //x!y=x!y
    bool associative; //x!(y!z)=(x!y)!z
    char* distributive; //x!(y%z)=x!y%x!z
public:
    operation(bool commutative_in,bool associative_in,char* distributive_in):commutative(commutative_in),associative(associative_in),distributive(distributive_in) {}
};

class funcrules {
private:
    funcrule** data;
    int max;
public:
    funcrules():data(NULL),max(-1){}
    funcrules(const char* string) {
        int i=0,w=0;
        max=-1;
        strings* expl=explode(string,10);
        if(expl->max>-1) {
            max=expl->max;
            data=new funcrule*[expl->max+1];
            while(i<=expl->max) {
                //cerr << "creating " << expl->strs[i] << endl;
                data[w++]=new funcrule(expl->strs[i]);
                if(!(data[w-1]->valid())) {
                    w--;
                }
                i++;
            }
            max=w-1;
        }
    }
    void printall() {
        int i=0;
        while(i<=max) {
            data[i]->print();
            i++;
        }
    }
};
/*sin,cos,tg,erf,exp,ln,sqrt,arcsin,arccos,abs,arctg,*/
funcrules* FUNCRULES;
char* display(char* src,int type=TREE_DMATH) {
    tree* tree2=parse(src);
    return(tree2==NULL?NULL:(tree2->test()?tree2->display(type):NULL));
}
char* easy(char* src) {
    tree* tree2=parse(src);
    tree* easy=(tree2==NULL?NULL:tree2->easy());
    return(easy==NULL?NULL:easy->display());
}
char* integral(char* src,const char* base=MATH_DEFDIFF) {
    tree* tree2=parse(src);
    tree* integral=(tree2==NULL?NULL:(tree2->test()?tree2->easy()->integral(base):NULL));
    return(integral==NULL?NULL:integral->easy()->display());
}
char* diff(char* src,const char* base=MATH_DEFDIFF) {
    tree* tree2=parse(src);
    tree* diff=(tree2==NULL?NULL:(tree2->test()?tree2->easy()->diff(base):NULL));
    return(diff==NULL?NULL:diff->easy()->display());
}
int isarg(int argc,char* argv[],const char* need) {
    unsigned int i=argc-1;
    while(i>0) {
        if(str(argv[i],need)) {
            return(i);
        }
        i--;
    }
    return(0);
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
    QString a;
    char* result=diff(ui->line_src->toPlainText().toAscii().data(),ui->line_base->text().toAscii().data());
    if(result==NULL) {
        a=ERROR;
    }
    else {
        a=result;
    }
    ui->line_dest->setPlainText(a);
}
void Diff::on_button_parse_clicked() {
    FUNCRULES=new funcrules(ui->plainTextEdit->toPlainText().toAscii().data());
    FUNCRULES->printall();
}
void Diff::on_button_integral_clicked() {
    QString a;
    char* result=integral(ui->line_src->toPlainText().toAscii().data(),ui->line_base->text().toAscii().data());
    if(result==NULL) {
        a=ERROR;
    }
    else {
        a=result;
        a=a+MATH_INTEGRAL_ADDC;
    }
    ui->line_dest->setPlainText(a);
}
void Diff::on_button_easy_clicked() {
    //loop diff for tests
    /*int i=0;
    while(true) {
        cerr << i;
        ui->button_diff->click();
        i++;
    }*/
    QString a;
    char* result=easy(ui->line_src->toPlainText().toAscii().data());
    if(result==NULL) {
        a=ERROR;
    }
    else {
        a=result;
    }
    ui->line_dest->setPlainText(a);
}
void Diff::on_button_show_clicked() {
    QString a;
    char* result=display(ui->line_src->toPlainText().toAscii().data());
    if(result==NULL) {
        a=ERROR;
    }
    else {
        a=result;
    }
    ui->line_dest->setPlainText(a);
}
void Diff::on_pushButton_clicked() {
    QString a;
    tree* x=parse(ui->teste->toPlainText().toAscii().data()),*y=parse(ui->testp->toPlainText().toAscii().data());
    if(x!=NULL&&y!=NULL) {
        cerr << "testing [" << x->display() << "] with [" << y->display() << "]" << endl;
        if(x->pattern(y)) {
            a="yes";
        }
        else {
            a="no";
        }
    }
    else {
        a="error";
    }
    ui->testr->setPlainText(a);
}
void Diff::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
