#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <strings.h>
using namespace std;
bool str(const char* a,const char* b) {
	return(strcmp(a,b)==0);
}
inline bool char_isnum(char sym) {
	return((sym>='0'&&sym<='9')||sym=='.');
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
const int TREE_DMATH=0;
const int TREE_DTREE=1;
const int TREE_DDEFAULT=TREE_DMATH;
char* strn(const char* src) {
	if(src[strlen(src)-1]!='\0') {
		char* b=new char[strlen(src)];
		strcpy(b,src);
		b[strlen(src)]='\0';
		return(b);
	}
	else {
		char* src_=new char[strlen(src)-1];
		strcpy(src_,src);
		return(src_);
	}
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
	return(strn(h));
}
char* stradd(char* l, const char* r){
        const unsigned int offset=strlen(l);
        const unsigned int maxindex=offset+strlen(r);
        char* d=new char[maxindex];
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
/*char* stradd(char* src,const char* add) {
	char* src_=new char[strlen(src)+strlen(add)-1];
	strcpy(src_,strn(src));
	strcat(src_,strn(add));
	return(src_);
}*/
void var_dump(char* a) {
	cerr << "String (" << strlen(a) << "): [" << a << "]" << endl;
}
void var_dump(int a) {
	cerr << "Int: [" << a << "]" << endl;
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
			char* tmp_;
			if(leaf==true) {
				tmp_=new char[strlen(value)-1];
				strcpy(tmp_,value);
				res=tmp_;
			}
			else if(b!=NULL) {
				res=NULL;
				if((tmp=(!a->leaf))) {
					res=new char[0];
					res[0]='(';
					res=stradd(res,a->display_math());
				}
				else {
					res=a->display_math();
				}
				if(tmp) {
					res=stradd(res,")");
				}
				res=stradd(res,value);
				if((tmp=(!b->leaf))) {
					res=stradd(res,"(");
				}
				res=stradd(res,b->display_math());
				if(tmp) {
					res=stradd(res,")");
				}
			}
			else {
				tmp_=new char[strlen(value)-1];
				strcpy(tmp_,value);
				res=tmp_;
				res=stradd(res,"(");
				res=stradd(res,a->display_math());
				res=stradd(res,")");
			}
			return(res);
		}
		char* display_tree(int level=0) {
			const char* sym=":";
			char* res;
			char* nl=new char[0];
			nl[0]=10;
			if(leaf==true) {
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
		const char* value;
		//leaf?
		bool leaf;
		//if leaf=>two pointers to other trees
		//if unary only "a" used
		tree *a,*b;
	public:
		//leaf
		tree(const char* str): value(str),leaf(true) { }
		//binary with trees
		tree(tree *in_a,const char* op,tree *in_b): value(op),leaf(false),a(in_a),b(in_b) {}
		//unary with tree
		tree(const char *op,tree *in_): value(op),leaf(false),a(in_),b(NULL) {}
		//binary with leaves
		tree(const char* ina, const char* op, const char* inb): value(op),leaf(false) {
			a=new tree(ina);
			b=new tree(inb);
		}
		//unary with leaf
		tree(const char* op,const char* in_): value(op),leaf(false),b(NULL) {
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
		tree* diff(const char* base) {
			tree* nt;
			nt=new tree(NULL);
			if(leaf==true) {
				if(str(value,base)) {
					nt=new tree("1");
				}
				else {
					nt=new tree("0");
				}
			}
			else {
				if(b==NULL) {
					if(str(value,"-")) {
						nt=new tree("-",a->diff(base));
					}
					if(str(value,"ln")) {
						nt=new tree(a->diff(base),"*",new tree(new tree("1"),"/",a));
					}
					if(str(value,"sin")) {
						nt=new tree(a->diff(base),"*",new tree("cos",a));
					}
					if(str(value,"cos")) {
						nt=new tree(a->diff(base),"*",new tree("-",new tree("sin",a)));
					}
					if(str(value,"arcsin")) {
						nt=new tree(a->diff(base),"*",new tree(new tree("1"),"/",new tree("sqrt",new tree(new tree("1"),"-",new tree(a,"^",new tree("2"))))));
					}
					if(str(value,"arccos")) {
						nt=new tree("-",new tree(a->diff(base),"*",new tree(new tree("1"),"/",new tree("sqrt",new tree(new tree("1"),"-",new tree(a,"^",new tree("2")))))));
					}
					if(str(value,"arctg")) {
						nt=new tree(a->diff(base),"*",new tree(new tree("1"),"/",new tree(new tree("1"),"+",new tree(a,"^",new tree("2")))));
					}
					if(str(value,"tg")) {
						nt=new tree(a->diff(base),"*",new tree(new tree("1"),"/",new tree(new tree("cos",a),"^",new tree("2"))));
					}
					if(str(value,"exp")) {
						nt=new tree(a->diff(base),"*",new tree("exp",a));
					}
					if(str(value,"sqrt")) {
						nt=new tree(a->diff(base),"*",new tree(new tree("1"),"/",new tree(new tree("2"),"*",new tree("sqrt",a))));
					}
					if(str(value,"erf")) {
						nt=new tree(a->diff(base),"*",new tree(new tree(new tree("2"),"/",new tree("sqrt","pi")),"*",new tree("exp",new tree("-",new tree("x","^","2")))));
					}
					if(str(nt->value,"???")) {
						cerr << "Unknown function" << endl;
					}
				}
				else {
					if(str(value,"/")) {
						nt=new tree(new tree(new tree(a->diff(base),"*",b),"-",new tree(a,"*",b->diff(base))),"/",new tree(b,"^",new tree("2")));
					}
					if(str(value,"+")||str(value,"-")) {
						nt=new tree(a->diff(base),value,b->diff(base));
					}
					if(str(value,"*")) {
						nt=new tree(new tree(a,"*",b->diff(base)),"+",new tree(a->diff(base),"*",b));
					}
					if(str(value,"^")) {
						//nt=new tree(b,"*",new tree(a,value,new tree(b,"-",new tree("1"))));
						//f(x)^g(x)=exp(ln(f(x))*g(x))
						tree* tmp;
						tmp=new tree("exp",new tree(new tree("ln",a),"*",b));
						nt=tmp->diff(base);
					}
				}
			}
			return(nt);
		}
		/*void operator=(float n){value=n;}
		void operator=(char n){operation=n;}
		void operator=(bool n){leaf=n;}*/
		void seta(tree* aset){a=aset;}
		void setb(tree* bset){b=bset;}
		void operator=(tree *src) {
			/*delete *a;
			delete *b;*/
			tree *newtree;
			newtree=src->copymem();
			leaf=newtree->leaf;
			value=newtree->value;
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
		tree* easy() {
			tree *nt,*rt;
			bool ok=false;
			if(leaf==false) {
				if(b==NULL) {
					nt=a->easy();
					if(str(value,"-")) {
						rt=new tree("-",nt);
						ok=true;
						//rt=new tree(new tree("0"),value,nt);
					}
					else if(str(value,"exp")) {
						if(!nt->leaf) {
							if(nt->b==NULL&&str(nt->value,"ln")) {
								rt=nt->a;
								ok=true;
							}
							else if(str(nt->value,"*")) {
								//ln(...)*...
								if(!nt->a->leaf&&str(nt->a->value,"ln")) {
									ok=true;
									rt=new tree(nt->a->a,"^",nt->b);
								}
								if(!nt->b->leaf&&str(nt->b->value,"ln")) {
									ok=true;
									rt=new tree(nt->b->a,"^",nt->a);
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
					if(str(value,"^")) {
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
					if(str(value,"/")) {
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
							rt=new tree("0");
						}
						if(atof(nt->b->value)==0&&char_isnum(nt->b->value[0])) {
							cout << "Division by zero.";
							rt=new tree("-");
						}
						if(atof(nt->b->value)==1&&char_isnum(nt->b->value[0])) {
							rt=nt->a->copymem();
						}
					}
					if(str(value,"+")) {
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
					if(str(value,"-")) {
						//dont change order!
						if(atof(nt->a->value)==0&&char_isnum(nt->a->value[0])) {
							rt=new tree("-",nt->b);
						}
						if(atof(nt->b->value)==0&&char_isnum(nt->b->value[0])) {
							rt=nt->a->copymem();
						}
						if(char_isnum(nt->a->value[0])&&char_isnum(nt->b->value[0])) {
							char* h=print_num(atof(nt->a->value)-atof(nt->b->value));
							rt=new tree(h);
						}
					}
					if(str(value,"*")) {
						if((nt->a->leaf&&str(nt->a->value,"0"))||(nt->b->leaf&&str(nt->b->value,"0"))) {
							rt=new tree("0");
						}
						if(atof(nt->a->value)==1&&char_isnum(nt->a->value[0])) {
							rt=nt->b;
						}
						if(atof(nt->b->value)==1&&char_isnum(nt->b->value[0])) {
							rt=nt->a;
						}
						if(char_isnum(nt->a->value[0])&&char_isnum(nt->b->value[0])) {
							char* h=print_num(atof(nt->a->value)*atof(nt->b->value));
							rt=new tree(h);
						}
					}
				}
			}
			else {
				rt=new tree(value);
			}
			return rt;
		}
		char* display(int type=TREE_DDEFAULT) {
			if(type==TREE_DMATH) {
				return(display_math());
			}
			if(type==TREE_DTREE) {
				return(display_tree());
			}
			return(display(TREE_DDEFAULT));
		}
};
struct parser_answer {
	int pos;
	tree* tr;
};
lexer_answer lexer(char* str) {
	int i=0;
	int len=strlen(str);

	int pos=0;
	char* temp=NULL,*a,*b;
	char current;
	int laststate=-1,currentstate;
	token* result=new token[len]; //char count more than tokens count.
	lexer_answer res;
	while(pos<=len) {
		if(len==pos) {
			current='\0';
			currentstate=10;
		}
		else {
			current=str[pos];
			currentstate=char_state(current);
		}
		if(currentstate==0) {
			cerr << "Wrong symbol at " << pos << endl;
			res.max=-1;
			return(res);
		}
		else {
			if((currentstate!=CHAR_TBR2)&&(currentstate!=CHAR_TBR1)&&(currentstate!=CHAR_TOP)&&(laststate==currentstate)) {
				a=new char[strlen(temp)-1];
				strcpy(a,temp);
				temp=new char[strlen(temp)];
				b=new char[0];
				b[0]=current;
				temp=strcat(a,b);
			}
			else {
				if(laststate!=-1) {
					if(current=='_') {
						current='^';
					}
					result[i].c=new char[strlen(temp)+1];
					result[i].t=laststate;
					strcpy(result[i].c,temp);
					i++;
				}
				if(current!='\0') {
					temp=new char[0];
					temp[0]=current;
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
		if(str(src.result[pos].c,"-")&&tmpres.tr!=NULL) {
			rtree=new tree("-",tmpres.tr);
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
	a=lexer(strn(src));
	b=parser(a);
	if(b.tr==NULL||a.errors) {
		return(NULL);
	}
	return(b.tr);
}
char* show(char* src) {
	tree* tree2=parse(src);
	return(tree2==NULL?NULL:tree2->display());
}
char* easy(char* src) {
	tree* tree2=parse(src);
	return(tree2==NULL?NULL:tree2->easy()->display());
}
char* diff(char* src) {
	tree* tree2=parse(src);
	return(tree2==NULL?NULL:((tree2->easy()->diff("x"))->easy())->display());
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
//char* in
int main(int argc,char* argv[]) {
	bool tmp;
	char* res;
	if(argc==3) {
		if((tmp=isarg(argc,argv,"easy"))) {
			res=easy(argv[tmp+1]);
		}
		else if((tmp=isarg(argc,argv,"show"))) {
			res=show(argv[tmp+1]);
		}
		else if((tmp=isarg(argc,argv,"diff"))) {
			res=diff(argv[tmp+1]);
		}
		if(res==NULL) {
			cerr << "Wrong equal" << endl;;
			return(1);
		}
		else {
			cout << res << endl;
			return(0);
		}
	}
	else if(argc==2) {
		char* in=new char[100];
		cout << "> ";
		cin >> in;
		tmp=0;
		if(isarg(argc,argv,"easy")) {
			res=easy(in);
			tmp=1;
		}
		else if(isarg(argc,argv,"show")) {
			res=show(in);
			tmp=1;
		}
		else if(isarg(argc,argv,"diff")) {
			res=diff(in);
			tmp=1;
		}
		if(tmp==0) {
			cerr << "Wrong command." << endl;
			return(1);
		}
		else { 
			if(res==NULL) {
				cerr << "Wrong equal" << endl;;
				return(1);
			}
			else {
				cout << res << endl;
				return(0);
			}
		}
	}
	else {
		cerr << "Usage: " << argv[0] << " [easy|show|diff] expr" << endl;
		return(1);
	}
}
