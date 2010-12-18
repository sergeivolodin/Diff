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
const char* MATH_BR1="(";
const char* MATH_BR2=")";
const char* MATH_SIN="sin";
const char* MATH_COS="cos";
const char* MATH_TG="tg";
const char* MATH_ERF="erf";
const char* MATH_EXP="exp";
const char* MATH_NADD="%2B";
const char* MATH_PI="pi";
const char* MATH_LN="ln";
const char* MATH_SQRT="sqrt";
const char* MATH_MUL="*";
const char* MATH_DIV="/";
const char* MATH_ADD="+";
const char* MATH_SUB="-";
const char* MATH_POW="^";
const char* MATH_POW1="_";
const char* MATH_ARCSIN="arcsin";
const char* MATH_ARCCOS="arccos";
const char* MATH_ARCTG="arctg";
const char* MATH_DEFDIFF="x";
const char* MATH_0="0";
const char* MATH_1="1";
const char* MATH_2="2";
const int TREE_DMATH=0;
const int TREE_DTREE=1;
const int TREE_DNIGMA=2;
const int TREE_DDEFAULT=TREE_DMATH;
const char* TREE_NIGMA="http://math3.nigma.ru/maxima2/drivermath.php?render=$";
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
        int temp=static_cast<int>(a);
        if(temp==a) {
                sprintf(h,"%d",temp);
        }
        else {
                sprintf(h,"%f",a);
        }
        return(decimal_point(h));
}
char* stradd(char* l, const char* r){
        const unsigned int offset=strlen(l);
        const unsigned int maxindex=offset+strlen(r);
        char* d=new char[maxindex+1];
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
                                tmp_=new char[strlen(value)-1];
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
                char* display_tree(int level=0) {
                        const char* sym=":";
                        char* res;
                        char* nl=new char[0];
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
                tree(){}
                //returns pointer to other but same tree
                tree* copymem() {
                        tree* nt;
                        if(leaf==true) {
                                nt=new tree(value);

                        }
                        else {
                                if(b==NULL) {
                                        nt=new tree(value,a->copymem());
                                }
                                else {
                                        nt=new tree(a->copymem(),value,b->copymem());
                                }
                        }
                        return(nt);
                }
                ~tree() {
                        //not working?
                        /*if(leaf==false) {
                                delete *a;
                                delete *b;
                        }*/
                }
                bool contents(const char* src) {
                    if(leaf) {
                        return(str(src,value));
                    }
                    else {
                        if(b==NULL) {
                            return(a->contents(value));
                        }
                        else {
                            return(a->contents(src)||b->contents(src));
                        }
                    }
                }
                tree* diff(const char* base) {
                        tree* nt;
                        nt=NULL;
                        if(leaf==true) {
                                if(str(value,base)) {
                                        nt=new tree(MATH_1);
                                }
                                else {
                                        nt=new tree(MATH_0);
                                }
                        }
                        else {
                                if(b==NULL) {
                                        if(str(value,MATH_SUB)) {
                                                nt=new tree(MATH_SUB,a->diff(base));
                                        }
                                        if(str(value,MATH_LN)) {
                                                nt=new tree(a->diff(base),MATH_MUL,new tree(new tree(MATH_1),MATH_DIV,a));
                                        }
                                        if(str(value,MATH_SIN)) {
                                                nt=new tree(a->diff(base),MATH_MUL,new tree(MATH_COS,a));
                                        }
                                        if(str(value,MATH_COS)) {
                                                nt=new tree(a->diff(base),MATH_MUL,new tree(MATH_SUB,new tree(MATH_SIN,a)));
                                        }
                                        if(str(value,MATH_ARCSIN)) {
                                                nt=new tree(a->diff(base),MATH_MUL,new tree(new tree(MATH_1),MATH_DIV,new tree(MATH_SQRT,new tree(new tree(MATH_1),MATH_SUB,new tree(a,MATH_POW,new tree(MATH_2))))));
                                        }
                                        if(str(value,MATH_ARCCOS)) {
                                                nt=new tree(MATH_SUB,new tree(a->diff(base),MATH_MUL,new tree(new tree(MATH_1),MATH_DIV,new tree(MATH_SQRT,new tree(new tree(MATH_1),MATH_SUB,new tree(a,MATH_POW,new tree(MATH_2)))))));
                                        }
                                        if(str(value,MATH_ARCTG)) {
                                                nt=new tree(a->diff(base),MATH_MUL,new tree(new tree(MATH_1),MATH_DIV,new tree(new tree(MATH_1),MATH_ADD,new tree(a,MATH_POW,new tree(MATH_2)))));
                                        }
                                        if(str(value,MATH_TG)) {
                                                nt=new tree(a->diff(base),MATH_MUL,new tree(new tree(MATH_1),MATH_DIV,new tree(new tree(MATH_COS,a),MATH_POW,new tree(MATH_2))));
                                        }
                                        if(str(value,MATH_EXP)) {
                                                nt=new tree(a->diff(base),MATH_MUL,new tree(MATH_EXP,a));
                                        }
                                        if(str(value,MATH_SQRT)) {
                                                nt=new tree(a->diff(base),MATH_MUL,new tree(new tree(MATH_1),MATH_DIV,new tree(new tree(MATH_2),MATH_MUL,new tree(MATH_SQRT,a))));
                                        }
                                        if(str(value,MATH_ERF)) {
                                                nt=new tree(a->diff(base),MATH_MUL,new tree(new tree(new tree(MATH_2),MATH_DIV,new tree(MATH_SQRT,MATH_PI)),MATH_MUL,new tree(MATH_EXP,new tree(MATH_SUB,new tree(base,MATH_POW,MATH_2)))));
                                        }
                                        if(str(nt->value,"???")) {
                                                cerr << "Unknown function" << endl;
                                        }
                                }
                                else {
                                        if(str(value,MATH_DIV)) {
                                                nt=new tree(new tree(new tree(a->diff(base),MATH_MUL,b),MATH_SUB,new tree(a,MATH_MUL,b->diff(base))),MATH_DIV,new tree(b,MATH_POW,new tree(MATH_2)));
                                        }
                                        if(str(value,MATH_ADD)||str(value,MATH_SUB)) {
                                                nt=new tree(a->diff(base),value,b->diff(base));
                                        }
                                        if(str(value,MATH_MUL)) {
                                            if(a==NULL||b==NULL) {
                                                cerr << "ERROR";
                                            }
                                                nt=new tree(new tree(a,MATH_MUL,b->diff(base)),MATH_ADD,new tree(a->diff(base),MATH_MUL,b));
                                        }
                                        if(str(value,MATH_POW)) {
                                            tree* tmp;
                                            if(b->contents(base)||!(a->leaf&&str(a->value,base))) {
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
                    }
                    else {
                        if(b==NULL) {
                            if(str(value,MATH_SUB)) {
                                rt=new tree(MATH_SUB,a->integral(base));
                            }
                            if(str(a->value,base)) {
                                if(str(value,MATH_SIN)) {
                                    rt=new tree(MATH_SUB,new tree(MATH_COS,a));
                                }
                                if(str(value,MATH_COS)) {
                                    rt=new tree(MATH_SIN,a);
                                }
                            }
                        }
                        else {
                            if(str(value,MATH_ADD)||str(value,MATH_SUB)) {
                                ta=a->integral(base);
                                tb=b->integral(base);
                                if((ta!=NULL)&&(tb!=NULL)) {
                                    rt=new tree(ta,value,tb);
                                }
                            }
                            if(str(value,MATH_POW)) {
                                if(str(a->value,base)&&!b->contents(base)&&b->leaf) {
                                    if(str(b->value,"-1")) {
                                        rt=new tree(MATH_LN,base);
                                    }
                                    else {
                                        rt=new tree(new tree(new tree(base),MATH_POW,new tree(b,MATH_ADD,new tree(MATH_1))),MATH_DIV,new tree(b,MATH_ADD,new tree(MATH_1)));
                                    }
                                }
                            }
                            if(str(value,MATH_MUL)) {
                                if(!(a->contents(value))) {
                                    tb=b->integral(base);
                                    if(tb!=NULL) {
                                        rt=new tree(a,MATH_MUL,tb);
                                    }
                                }
                                if(!(b->contents(value))) {
                                    ta=a->integral(base);
                                    if(ta!=NULL) {
                                        rt=new tree(ta,MATH_MUL,b);
                                    }
                                }
                            }
                        }
                    }
                    return(rt);
                }

                tree* easy() {
                        tree *nt,*rt=NULL;
                        bool ok=false;
                        if(!leaf) {
                                if(b==NULL) {
                                        nt=a->easy();
                                        if(str(value,MATH_SUB)) {
                                                rt=new tree(MATH_SUB,nt);
                                                ok=true;
                                                //rt=new tree(new tree(MATH_0),value,nt);
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
                                        if(!ok) {
                                                rt=new tree(value,nt);
                                        }
                                }
                                else {
                                        nt=new tree(a->easy(),value,b->easy());
                                        rt=nt;
                                        if(str(value,MATH_POW)) {
                                                if(nt->a->leaf&&nt->b->leaf) {
                                                        if(char_isnum(nt->a->value[0])&&char_isnum(nt->b->value[0])) {
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
                                                        if(char_isnum(nt->a->value[0])&&char_isnum(nt->b->value[0])) {
                                                                if(atoi(nt->b->value)==0) {
                                                                        cout << "Division by zero." << endl;
                                                                }
                                                                else if(((atoi(nt->a->value)*10) % atoi(nt->b->value))==0) {
                                                                        char* h=print_num(atof(nt->a->value)/atof(nt->b->value));
                                                                        rt=new tree(h);
                                                                }
                                                        }
                                                }
                                                if(atof(nt->a->value)==0&&char_isnum(nt->a->value[0])) {
                                                        rt=new tree(MATH_0);
                                                }
                                                if(atof(nt->b->value)==0&&char_isnum(nt->b->value[0])) {
                                                        cout << "Division by zero.";
                                                        rt=new tree(MATH_SUB);
                                                }
                                                if(atof(nt->b->value)==1&&char_isnum(nt->b->value[0])) {
                                                        rt=nt->a->copymem();
                                                }
                                        }
                                        if(str(value,MATH_ADD)) {
                                                if((*(nt->a))==(*(nt->b))) {
                                                    //cerr << nt->a->display() << value << nt->b->display() << endl;
                                                    rt=new tree(new tree(MATH_2),MATH_MUL,nt->a);
                                                }
                                                if(atof(nt->a->value)==0&&char_isnum(nt->a->value[0])) {
                                                        rt=nt->b->copymem();
                                                }
                                                if(atof(nt->b->value)==0&&char_isnum(nt->b->value[0])) {
                                                        rt=nt->a->copymem();
                                                }
                                                if(nt->a->leaf&&nt->b->leaf) {
                                                        if(char_isnum(nt->a->value[0])&&char_isnum(nt->b->value[0])) {
                                                                char* h=print_num(atof(nt->a->value)+atof(nt->b->value));
                                                                rt=new tree(h);
                                                        }
                                                }
                                        }
                                        if(str(value,MATH_SUB)) {
                                                //dont change order!
                                                if(atof(nt->a->value)==0&&char_isnum(nt->a->value[0])) {
                                                        rt=new tree(MATH_SUB,nt->b);
                                                }
                                                if(atof(nt->b->value)==0&&char_isnum(nt->b->value[0])) {
                                                        rt=nt->a->copymem();
                                                }
                                                if(char_isnum(nt->a->value[0])&&char_isnum(nt->b->value[0])) {
                                                        char* h=print_num(atof(nt->a->value)-atof(nt->b->value));
                                                        rt=new tree(h);
                                                }
                                                if(*(nt->a)==*(nt->b)) {
                                                    rt=new tree(MATH_0);
                                                }
                                        }
                                        if(str(value,MATH_MUL)) {
                                                if((nt->a->leaf&&str(nt->a->value,MATH_0))||(nt->b->leaf&&str(nt->b->value,MATH_0))) {
                                                        rt=new tree(MATH_0);
                                                }
                                                if(atof(nt->a->value)==1&&char_isnum(nt->a->value[0])) {
                                                        rt=nt->b;
                                                }
                                                if(atof(nt->b->value)==1&&char_isnum(nt->b->value[0])) {
                                                        rt=nt->a;
                                                }
                                                if(char_isnum(nt->a->value[0])&&char_isnum(nt->b->value[0])) {
                                                        //cerr << nt->a->value << " ";
                                                        //cerr << atof(nt->a->value) << " " << atof(nt->a->value)*atof(nt->b->value) << endl;
                                                        char* h=print_num(atof(nt->a->value)*atof(nt->b->value));
                                                        rt=new tree(h);
                                                }
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
lexer_answer lexer(char* str) {
        //print_str(str);
        int i=0;
        int len=strlen(str);

        int pos=0;
        char* temp=NULL,*aa=NULL;
        //char* bb;
        char current;
        int laststate=-1,currentstate;
        token* result=new token[len]; //char count more than tokens count.
        lexer_answer res;
        while(laststate!=CHAR_TEND) {
                if(len==pos) {
                        current='\0';
                        currentstate=CHAR_TEND;
                }
                else {
                        current=str[pos];
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
                            aa=new char[1];
                            aa[0]=current;
                            aa[1]='\0';
                            temp=stradd(temp,aa);
                        }
                        else {
                                if(laststate!=-1) {
                                        if(current=='_') {
                                                current='^';
                                        }
                                        result[i].c=new char[strlen(temp)-1];
                                        result[i].t=laststate;
                                        strcpy(result[i].c,temp);
                                        result[i].c=result[i].c;
                                        //result[i].c[strlen(temp)]='\0';
                                        aa=NULL;
                                        temp=NULL;
                                        i++;
                                }
                                if(current!='\0') {
                                        temp=new char[1];
                                        temp[0]=current;
                                        temp[1]='\0';
                                        aa=NULL;
                                }
                        }
                }
                laststate=currentstate;
                pos++;
        }
        res.max=i-1;
        res.result=result;
        res.errors=false;
        return(res);
}


int op_prio(char* a) {
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
        return(0);
}
parser_answer parser(lexer_answer src,int pos=0,bool binary=true,int parent_prio=0,bool sub_allow=true) {
        tree* rtree=NULL;
        parser_answer result,tmpres,tmpres1,tmpres2;
        result.pos=pos;
        bool ok=false;
        result.tr=NULL;
        if(binary) {
                //first operand
                tmpres=parser(src,pos,false);
                if(src.result[tmpres.pos].t==CHAR_TOP) {
                        //its binary
                        if(op_prio(src.result[tmpres.pos].c)>=parent_prio) {
                                //if current prio is bigger than parent prio
                                tmpres1=parser(src,tmpres.pos+1,true,op_prio(src.result[tmpres.pos].c),false);
                                if((src.result[tmpres1.pos].t==CHAR_TOP)&&sub_allow) {
                                        //if sub_recursion allowed AND next sym is operation
                                        tmpres2=parser(src,tmpres1.pos+1,true);
                                        rtree=new tree(new tree(tmpres.tr,src.result[tmpres.pos].c,tmpres1.tr),src.result[tmpres1.pos].c,tmpres2.tr);
                                        result.pos=tmpres2.pos;
                                        ok=true;
                                }
                                else {
                                        rtree=new tree(tmpres.tr,src.result[tmpres.pos].c,tmpres1.tr);
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
        if(src.result[pos].t==CHAR_TBR1&&!ok) {
                ok=true;
                tmpres=parser(src,pos+1);
                if(src.result[tmpres.pos].t==CHAR_TBR2) {
                        result.pos=tmpres.pos+1;
                        rtree=tmpres.tr;
                }
                else {
                        cerr << "Closing bracket not found" << endl;
                        src.errors=true;
                        return(result);
                }
        }
        //unary
        if(src.result[pos].t==CHAR_TOP&&!ok) {
                ok=true;
                tmpres=parser(src,pos+1,false);
                if(str(src.result[pos].c,MATH_SUB)&&tmpres.tr!=NULL) {
                        rtree=new tree(MATH_SUB,tmpres.tr);
                        result.pos=tmpres.pos;
                }
                else {
                        cerr << pos << ": unary \"" << src.result[pos].c << "\" not allowed" << endl;
                        src.errors=true;
                        result.pos=tmpres.pos;
                }
        }
        //functions
        if(src.result[pos].t==CHAR_TLETT&&!ok) {
                if(src.result[pos+1].t==CHAR_TBR1) {
                        ok=true;
                        tmpres=parser(src,pos+2);
                        result.pos=tmpres.pos+1;
                        if(src.result[tmpres.pos].t==CHAR_TBR2) {
                                //cerr << "Function parse called, new position is " << result.pos << " (it contents [" << src.result[tmpres.pos+1].c  << "])" << endl;
                                rtree=new tree(src.result[pos].c,tmpres.tr);
                        }
                        else {
                                src.errors=true;
                                cerr << tmpres.pos << ": function closing bracket expected, but '" << src.result[tmpres.pos-1].c << "' found." << endl;
                        }
                }
        }
        //atomic
        if((src.result[pos].t==CHAR_TNUM||src.result[pos].t==CHAR_TLETT)&&!ok) {
                ok=true;
                result.pos=pos+1;
                rtree=new tree(src.result[pos].c);
        }
        if(src.errors) {
                result.tr=NULL;
        }
        else {
                result.tr=rtree;
        }
        return(result);
}
tree* parse(char* src) {
        lexer_answer a;
        parser_answer b;
        //cerr << "==========LEXER============" << endl;
        a=lexer(src);
        /*int i=0;
        while(i<=a.max) {
            //var_dump(a.result[i].c);
            print_str(a.result[i].c);
            i++;
        }
        cerr << "==========/LEXER============" << endl;
        cerr << "==========PARSER============" << endl;*/
        b=parser(a);
        //cerr << "==========/PARSER============" << endl;
        if(b.tr==NULL||a.errors) {
                return(NULL);
        }
        return(b.tr);
}
char* display(char* src) {
        tree* tree2=parse(src);
        return(tree2==NULL?NULL:tree2->display());
}
char* nigma(char* src) {
        tree* tree2=parse(src);
        return(tree2==NULL?NULL:tree2->display(TREE_DNIGMA));
}
char* easy(char* src) {
        tree* tree2=parse(src);
        return(tree2==NULL?NULL:tree2->easy()->display());
}
char* integral(char* src,const char* base=MATH_DEFDIFF) {
        tree* tree2=parse(src);
        tree* integral=(tree2==NULL?NULL:((tree2->integral(base))));
        //char* add="+C";
        return(integral==NULL?NULL:integral->easy()->display());
}
char* diff(char* src,const char* base=MATH_DEFDIFF) {
        tree* tree2=parse(src);
        return(tree2==NULL?NULL:((tree2->easy()->diff(base))->easy())->display());
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
        a="Error.";
    }
    else {
        a=result;
    }
    ui->line_dest->setPlainText(a);
}
void Diff::on_button_integral_clicked() {
    QString a;
    char* result=integral(ui->line_src->toPlainText().toAscii().data(),ui->line_base->text().toAscii().data());
    if(result==NULL) {
        a="Error.";
    }
    else {
        a=result;
    }
    ui->line_dest->setPlainText(a);
}
void Diff::on_button_easy_clicked() {
    QString a;
    char* result=easy(ui->line_src->toPlainText().toAscii().data());
    if(result==NULL) {
        a="Error.";
    }
    else {
        a=result;
    }
    ui->line_dest->setPlainText(a);
}
void Diff::on_button_show_clicked() {
    QString a;
    char* result=nigma(ui->line_src->toPlainText().toAscii().data());
    if(result==NULL) {
        a="Error.";
    }
    else {
        a=result;
    }
//    ui->webView->setUrl();
    //QUrl aa=a;
    //ui->webView->setUrl(aa);
    ui->line_dest->setPlainText(a);
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