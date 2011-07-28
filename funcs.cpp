#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sys/timeb.h>
using namespace std;
struct strings {
    char** strs;
    int max;
};
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
unsigned long long int mtime() {
    static timeb* tp=new timeb;ftime(tp);
    return(tp->time*1000+tp->millitm-1906000000);
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
char* repeat(const char* str,int count=1) {
    int i=1;
    char* res=NULL;
    while(i<=count) {
        res=stradd(res,str);
        i++;
    }
    return(res);
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
char* strings_merge(strings* a,const char *del=" ") {
    int i=0;
    char* res="";
    while(i<=a->max) {
        res=stradd(res,a->strs[i]);
        if(i<a->max) {
            res=stradd(res,del);
        }
        i++;
    }
    return(res);
}
const char* print(bool a) {
    if(a) return("1");
    return("0");
}
bool atob(const char* a) {
    if(a&&str(a,"1")) return(true);
    return(false);
}
