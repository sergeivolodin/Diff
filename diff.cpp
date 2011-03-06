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
  lexer returns wrong-allocated strings (result.src)
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
const char* MATH_NO="-";
//math variables for rules
char* MATH_NUMS="abcde";
char* MATH_VARS="xy";
char* MATH_FUNCS="fgh";
//constans
const char* MATH_DEFDIFF="x";
const int MATH_TDIFF=1;
const char MATH_CPOSTREPL='|';
const char MATH_CCOMMOND=' ';
const char MATH_CLINESD='\n';
char** MATH_OPLIST=NULL;
const char MATH_CPOSTREPLD=',';
const char MATH_CPOSTREPLDC='=';
const int MATH_TINTEGRAL=2;
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
const int TREE_ADEF=0;
const int TREE_ASYM=1;
const int TREE_TNUM=0;
const int TREE_TVAR=4;
const int TREE_TUNARY=1;
const int TREE_TBINARY=2;
const int TREE_TFUNCTION=3;
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
void var_dump(const char* a) {
    cerr << "String (" << strlen(a) << "): [" << a << "]" << endl;
}
/*void var_dump(int a) {
        cerr << "Int: [" << a << "]" << endl;
}*/
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
            //&&b->b!=NULL
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
        /*delete *a;
        delete *b;*/
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
lexer_answer* lexer(char* str) {
    //print_str(str);
    int i=0;
    int len=strlen(str);
    int pos=0;
    char* temp=NULL,*aa=NULL;
    //char* bb;
    char current;
    int laststate=-1,currentstate;
    token* result=new token[len+2]; //char count more than tokens count.
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
                //b=a->easy();
                b=a;
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
bool pattern(tree* stree, tree* spattern,const char* base=MATH_DEFDIFF) {
    cerr << "pattern from [" << stree->display() << "] to [" << spattern->display() << "]" << endl;
    cerr << "funcs=" << MATH_FUNCS << endl;
    cerr << "vars=" << MATH_VARS << endl;
    cerr << "nums=" << MATH_NUMS << endl;
    //need in sin(x) with sin(x), not f
    if((*stree)==(*spattern)) {
        return(true);
    }
    int ttype=stree->type();
    int ptype=spattern->type();
    //tree types:num/var/unary/binary/function
    if(ptype==ttype) {
        cerr << "pattern type ==" << endl;
        //types equal, so lets test sub-trees.
        if(ttype==TREE_TNUM) {
            cerr << "pattern num ";
            //current is num. pattern can be [axf]
            if(strchar(MATH_NUMS,spattern->getvalue())||strchar(MATH_VARS,spattern->getvalue())||strchar(MATH_FUNCS,spattern->getvalue())) {
                cerr << "ok" << endl;
                return(true);
            }
        }
        if(ttype==TREE_TVAR) {
            //cerr << "var";
            //current is var. pattern can be [var or function]
            cerr << "pattern var ";
            if(strchar(MATH_VARS,spattern->getvalue())||strchar(MATH_FUNCS,spattern->getvalue())) {
                if(str(stree->getvalue(),base)) {
                    cerr << "ok (pattern: var/funcs)" << endl;
                    return(true);
                }
            }
            //for 'y' when base is x
            if(strchar(MATH_NUMS,spattern->getvalue())) {
                if(!str(stree->getvalue(),base)) {
                    cerr << "ok (pattern: othervar)" << endl;
                    return(true);
                }
            }
        }
        if(ttype==TREE_TFUNCTION) {
            //current is function. pattern can be [number, var or function]
            cerr << "pattern function ";
            if((str(spattern->getvalue(),stree->getvalue()))||strchar(MATH_FUNCS,spattern->getvalue())) {
                //same function OR 'f' pattern, go inside
                cerr << "ok" << endl;
                return(pattern(stree->geta(),spattern->geta(),base));
            }
            else {
                cerr << "[" <<  MATH_FUNCS << ", " <<  spattern->getvalue() << "]" << endl ;
            }
        }
        if(ttype==TREE_TUNARY) {
            cerr << "pattern unary ";
            if(str(stree->getvalue(),spattern->getvalue())) {
                //-(x),-(f)
                //same operation, go inside
                cerr << "inside..." << endl;
                return(pattern(stree->geta(),spattern->geta(),base));
            }
        }
        if(ttype==TREE_TBINARY) {
            //commutative operations support will be added later
            //2-level commutative changing will be unuseful here
            cerr << "pattern binary ";
            if(str(stree->getvalue(),spattern->getvalue())) {
                //x+y,f+g
                cerr << "inside..." << endl;
                return(pattern(stree->geta(),spattern->geta(),base)&&pattern(stree->getb(),spattern->getb(),base));
            }
        }
    }
    else {
        cerr << "pattern type !=" << endl;
        if(ptype==TREE_TVAR) {
            cerr << "var patern... ";
            if(strchar(MATH_FUNCS,spattern->getvalue())) {
                cerr << "ok (pattern=function)" << endl;
                return(true);
            }
            if(strchar(MATH_NUMS,spattern->getvalue())) {
                cerr << "pattern=num. testing stree: ";
                if(ttype==TREE_TBINARY) {
                    cerr << "binary tree. go inside... " << endl;
                    //a and b contains ONLY numbers
                    return((!stree->geta()->contains(base))&&(!stree->getb()->contains(base)));
                }
                if(ttype==TREE_TUNARY||ttype==TREE_TFUNCTION) {
                    cerr << "function/unary tree. go inside... " << endl;
                    //b contains ONLY numbers
                    return(!(stree->geta()->contains(base)));
                }
                if(ttype==TREE_TNUM) {
                    cerr << "num tree. ok" << endl;
                    return(true);
                }
            }
        }
    }
    return(false);
}
unsigned int pattern_count_funcs(tree* p) {
    //WARNING: if in pattern exists more than one same replacer, it will not be counted once
    int ptype=p->type();
    if(ptype==TREE_TVAR) {
        if(strchar(MATH_FUNCS,p->getvalue())) {
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
tree* getreplacer(replacers* src,char* a,int offset=0) {
    int i=0;
    if(src!=NULL&&a!=NULL) {
        if(src->r!=NULL) {
            while(i<=(src->maxindex+offset)) {
                if(src->r[i]!=NULL) {
                    if((src->r[i]->l!=NULL)&&(src->r[i]->r!=NULL)) {
                        if(str(src->r[i]->l,a)) {
                            return(src->r[i]->r);
                        }
                    }
                }
                i++;
            }
        }
    }
    return(NULL);
}

replacers* getreplacers(tree* stree,tree* spattern,const char* base=MATH_DEFDIFF,replacers* areplacers=NULL) {
    replacers* crepl=NULL;
    int fcount=pattern_count_funcs(spattern);
    if(fcount==0||(!pattern(stree,spattern,base))) {
        cerr << "getreplacers: !pattern";
        return(crepl);
    }
    if(areplacers==NULL) {
        crepl=new replacers;
        crepl->maxindex=0;
        crepl->r=new replacer*[fcount];
    }
    else {
        crepl=areplacers;
    }
    int ttype=stree->type();
    int ptype=spattern->type();
    //
    if(ptype==TREE_TBINARY) {
        cerr << "getreplacers_binary from" << spattern->getvalue() << endl;
        //pattern matches tree, so only extracting variables
        getreplacers(stree->geta(),spattern->geta(),base,crepl);
        getreplacers(stree->getb(),spattern->getb(),base,crepl);
    }
    if(ptype==TREE_TUNARY) {
        cerr << "getreplacers_unary from" << spattern->getvalue() << endl;
        getreplacers(stree->geta(),spattern->geta(),base,crepl);
    }
    if(ptype==TREE_TFUNCTION) {
        //f(g)=>any function from anything
        //if f then write f=[fvalue] else only inside
        cerr << "getreplacers_function from" << spattern->getvalue() << endl;
        if(strchar(MATH_FUNCS,spattern->getvalue())&&(getreplacer(crepl,spattern->getvalue(),-1)==NULL)) {
            crepl->r[crepl->maxindex]=new replacer;
            crepl->r[crepl->maxindex]->l=strcp(spattern->getvalue());
            cerr << "!!!![adding from function " << spattern->getvalue() << "=" << stree->getvalue() << "]" << endl;
            crepl->r[crepl->maxindex]->r=new tree(strcp(stree->getvalue()));
            crepl->maxindex++;
        }
        getreplacers(stree->geta(),spattern->geta(),base,crepl);
    }
    if(ptype==TREE_TVAR) {
        cerr << "getreplacers_var from" << spattern->getvalue() << endl;
        if(strchar(MATH_FUNCS,spattern->getvalue())&&(getreplacer(crepl,spattern->getvalue(),-1)==NULL)) {
            crepl->r[crepl->maxindex]=new replacer;
            crepl->r[crepl->maxindex]->l=strcp(spattern->getvalue());
            crepl->r[crepl->maxindex]->r=stree->copymem();
            crepl->maxindex++;
        }
        //t=sin(z) by z && p=sin(x) THEN match
        if(strchar(MATH_VARS,spattern->getvalue())&&(getreplacer(crepl,spattern->getvalue(),-1)==NULL)) {
            if(ttype==TREE_TVAR||ttype==TREE_TNUM) {
                crepl->r[crepl->maxindex]=new replacer;
                crepl->r[crepl->maxindex]->l=strcp(spattern->getvalue());
                crepl->r[crepl->maxindex]->r=stree->copymem();
                crepl->maxindex++;
            }
        }
    }
    //
    if(areplacers==NULL) {
        crepl->maxindex--;
        return(crepl);
    }
    else {
        return(NULL);
    }
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
        cerr << "replace-binary-b=" << spattern->getb()->display();
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
//MATH_CCOMMOND MATH_CLINESD MATH_CPOSTREPLD MATH_CPOSTREPL
rule* parsestr(char* str) {
    strings* t=explode(str,MATH_CPOSTREPL);
    //a n b
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
        cerr << "parsestr_nopostrepl (" << str << ") l-max [" << l->max << "]" << endl;
    }
    else if(t->max==1) {
        l=explode(t->strs[0],MATH_CCOMMOND);
        r=explode(t->strs[1],MATH_CPOSTREPLD);
        if(l==NULL||r==NULL||l->max==-1||r->max==-1) {
            return(NULL);
        }
                cerr << "parsestr_postrepl (" << str << ") l-max=" << l->max << ", " << "r-max=" << r->max << endl;
        if(r->max>=0) {
            prepl=new replacers;
            prepl->r=new replacer*[r->max+1];
            //prepl - replacers pointer for all postrepl
            while(i<=r->max) {
                tmp=explode(r->strs[i],MATH_CPOSTREPLDC);
                if(tmp->max==1) {
                    prepl->r[rcount]=new replacer; //current replacer "y=1"
                    prepl->r[rcount]->l=tmp->strs[0];
                    ttree=parse(tmp->strs[1]);
                    if(ttree!=NULL) {
                        prepl->r[rcount]->r=ttree;
                        cerr << "adding " << prepl->r[rcount]->l << "=" << prepl->r[rcount]->r->display() << endl;
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
        if(res->src==NULL) {
            cerr << "tree from res->src (" << l->strs[0] << ") == NULL" << endl;
        }
        if(res->dest==NULL) {
            cerr << "tree from res->src (" << l->strs[3] << ") == NULL" << endl;
        }
        if(res->src==NULL||res->dest==NULL) {
            cerr << "NULL parse tree" << endl;
            return(NULL);
        }
        cerr << "tree ok" << endl;
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
            if(tempr->postrepl!=NULL&&tempr->postrepl->r[0]!=NULL) {
                cerr << "0repl " << tempr->postrepl->r[0]->l << "=" << tempr->postrepl->r[0]->r->display() << endl;
            }
            c++;
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
tree* operate(tree* src,int operation,const char* base=MATH_DEFDIFF,rules* crules=FRULES,bool rmode=false) {
    int i=0,type=0;
    char* name=NULL, *tmp=new char[2]; tmp[1]='\n';
    replacers* tmprepl=NULL;
    if(!rmode) {
        if(FRULES!=NULL) {
            while(i<=FRULES->maxindex) {
                if(pattern(src,FRULES->r[i]->src,base)&&FRULES->r[i]->op==operation) {
                    tmprepl=getreplacers(src,FRULES->r[i]->src,base);
                    //return(replace(replace(operate(replace(FRULES->r[i]->dest,tmprepl),operation,base,crules,true),FRULES->r[i]->postrepl),tmprepl));
                    return(replace(operate(replace(FRULES->r[i]->dest,tmprepl),operation,base,crules,true),FRULES->r[i]->postrepl));
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
            /*while(i<strlen(MATH_VARS)) {
                tmp=new char[2];
                tmp[0]=MATH_VARS[i];
                tmp[1]='\n';
                name=MATH_OPLIST[operation-1];
                //name=stradd(name,tmp);
                cerr << name << endl;
                if(str(name,src->getvalue())) {
                    return(operate(src->geta(),operation,"y",crules));
                }
                i++;
            }
            */return(new tree(src->getvalue(),operate(src->geta(),operation,base,crules,true)));
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
    MATH_OPLIST=new char*[2];
    MATH_OPLIST[0]="diff";
    MATH_OPLIST[1]="integral";
    tree* src=parse(ui->line_src->toPlainText().toAscii().data());
    tree* dest=NULL;
    char* res=strcp(ERROR);
    QString a;
    if(src!=NULL) {
        res="src ok";
        dest=operate(src,1,MATH_DEFDIFF);
        if(dest!=NULL) {
            res="dest ok";
            res=dest->display();
        }
    }
    a=res;
    ui->line_dest->setPlainText(a);
}
void Diff::on_button_parse_clicked() {
    //settings
    FRULES=parsestrs(ui->plainTextEdit->toPlainText().toAscii().data());
    printrules(FRULES);
//    MATH_NUMS=ui->mnums->text().toAscii().data();
 //   MATH_VARS=ui->mvars->text().toAscii().data();
   // MATH_FUNCS=ui->mall->text().toAscii().data();
}
void Diff::on_button_integral_clicked() {

}
void Diff::on_button_easy_clicked() {

}
void Diff::on_button_show_clicked() {

}
void Diff::on_pushButton_clicked() {
    QString a,b,c;
    tree* x=parse(ui->teste->toPlainText().toAscii().data()),
    *y=parse(ui->testp->toPlainText().toAscii().data()),
    *z=parse(ui->testr_2->toPlainText().toAscii().data()),
    *trepl=NULL;
    char* aa=NULL;
    if(x!=NULL&&y!=NULL) {
        cerr << "testing [" << x->display() << "] with [" << y->display() << "]" << endl;
        if(pattern(x,y)) {
            replacers* repl = getreplacers(x,y);
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
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void Diff::on_pushButton_2_clicked()
{
    while(true) {
        ui->pushButton->click();
        ui->pushButton->click();
        ui->pushButton->click();
        ui->pushButton->click();
        //sleep(1);
    }
}