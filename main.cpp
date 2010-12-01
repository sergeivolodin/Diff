#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <strings.h>
/*
 f(x)^g(x)=exp(ln(f(x))*g(x))
 */
using namespace std;
bool str(char* a,char* b) {
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
};
const int TREE_DMATH=0;
const int TREE_DTREE=1;
const int TREE_DDEFAULT=TREE_DMATH;
void repeat(char* str,int count=1,bool tcerr=false) {
	int i=1;
	while(i<=count) {
		if(tcerr) {
			cerr << str;
		}
		else {
			cout << str;
		}
		i++;
	}
}

class tree {
	private:
		void display_math() {
			bool tmp;
			if(leaf==true) {
				cout << value;
			}
			else if(b!=NULL) {
				if(tmp=(!a->leaf)) {
					cout << "(";
				}
				a->display_math();
				if(tmp) {
					cout << ")";
				}
				cout << value;
				if(tmp=(!b->leaf)) {
					cout << "(";
				}
				b->display_math();
				if(tmp) {
					cout << ")";
				}
			}
			else if(str(value,"exp")) {
				cout << "e^(";
				a->display_math();
				cout << ")";
			}
			else {
				cout << value << "(";
				a->display_math();
				cout << ")";
			}
		}
		void display_tree(int level=0) {
			char* sym=":";
			if(leaf==true) {
				repeat(sym,level);
				cout << value << endl;
			}
			else {
				if(b==NULL) {
					repeat(sym,level);
					cout << value << endl;
					a->display_tree(level+1);
				}
				else {
					//repeat(sym,level);
					a->display_tree(level+1);
					//cout << endl;
					repeat(sym,level);
					cout <<	value << endl;
					b->display_tree(level+1);
				}
			}
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
		tree(char* str): value(str),leaf(true) { }
		//binary with trees
		tree(tree *in_a,char* op,tree *in_b): value(op),leaf(false),a(in_a),b(in_b) {}
		//unary with tree
		tree(char *op,tree *in_): value(op),leaf(false),a(in_),b(NULL) {}
		//binary with leaves
		tree(char* ina, char* op, char* inb): value(op),leaf(false) {
			a=new tree(ina);
			b=new tree(inb);
		}
		//unary with leaf
		tree(char* op,char* in_): value(op),leaf(false),b(NULL) {
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
		tree* diff(char* base) {
			tree* nt;
			nt=new tree("???");
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
						nt=new tree(a->diff(base),"*",new tree("-",new tree("cos",a)));
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
					if(str(value,"cos")) {
						nt=new tree(a->diff(base),"*",new tree("sin",a));
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
						nt=new tree(b,"*",new tree(a,value,new tree(b,"-",new tree("1"))));
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
		tree* easy() {
			tree *nt,*rt;
			if(leaf==false) {
				if(b==NULL) {
					nt=a->easy();
					if(str(value,"-")) {
						rt=new tree(new tree("0"),value,nt);
					}
					else {
						rt=new tree(value,nt);
					}
				}
				else {
					nt=new tree(a->easy(),value,b->easy());
					rt=nt;
					if(str(value,"^")) {
						if(nt->a->leaf&&nt->b->leaf) {
							if(char_isnum(nt->a->value[0])&&char_isnum(nt->b->value[0])) {
								char* h=new char[100];
								sprintf(h, "%f", pow(atof(nt->a->value),atof(nt->b->value)));
								rt=new tree(h);
							}
						}
					}
					if(str(value,"/")) {
						if(nt->a->leaf&&nt->b->leaf) {
							if(char_isnum(nt->a->value[0])&&char_isnum(nt->b->value[0])) {
								if(((atoi(nt->a->value)*10) % atoi(nt->b->value))==0) {
									char* h=new char[100];
									sprintf(h, "%f", atof(nt->a->value)/atof(nt->b->value));
									rt=new tree(h);
								}
							}
						}
						if(atof(nt->a->value)==0&&char_isnum(nt->a->value[0])) {
							rt=new tree("0");
						}
						if(atof(nt->b->value)==0&&char_isnum(nt->b->value[0])) {
							cerr << "Division by zero.";
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
								char* h=new char[100];
								sprintf(h, "%f", atof(nt->a->value)+atof(nt->b->value));
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
							char* h=new char[100];
							sprintf(h, "%f", atof(nt->a->value)-atof(nt->b->value));
							rt=new tree(h);
						}
					}
					if(str(value,"*")) {
						if((nt->a->leaf==true&&str(nt->a->value,"0"))||(nt->b->leaf==true&&str(nt->b->value,"0"))) {
							rt=new tree("0");
						}
						if(atof(nt->a->value)==1&&char_isnum(nt->a->value[0])) {
							rt=nt->b;
						}
						if(atof(nt->b->value)==1&&char_isnum(nt->b->value[0])) {
							rt=nt->a;
						}
						if(char_isnum(nt->a->value[0])&&char_isnum(nt->b->value[0])) {
							char* h=new char[100];
							sprintf(h, "%f", atof(nt->a->value)*atof(nt->b->value));
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
		void display(int type=TREE_DDEFAULT) {
			bool inrange=false;
			if(type==TREE_DMATH) {
				display_math();
				inrange=true;
			}
			if(type==TREE_DTREE) {
				display_tree();
				inrange=true;
			}
			if(!inrange) {
				display(TREE_DDEFAULT);
			}
		}
};
struct parser_answer {
	int pos;
	int state;
	tree* tr;
};
lexer_answer lexer(char* str) {
	int i=0;
	int len=strlen(str);

	int pos=0;
	char* temp="",*a,*b;
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
				temp=new char[0];
				temp[0]=current;
			}
		}
		laststate=currentstate;
		pos++;
	}
	res.max=i-1;
	res.result=result;
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
	return(-1);
}
//parser_answer parser(lexer_answer src,int pos=0,bool binary=true,int prev_binary=-2,int level=0) {
parser_answer parser(lexer_answer src,int pos=0,int binary_a=-1) {
	tree* rtree=NULL;
	parser_answer result,tmpres,tmpres1,tmpres2;
	result.pos=pos;
	bool ok=false;
	result.tr=NULL;
	result.state=0; //1 for 
	char* sym=" ";
	//binary operations
	/*cerr << endl;
	repeat(sym,level,true);
	cerr << pos << "[" << src.result[pos].c << "]: ";
	if(binary) {
		cerr << "b+ ";
		//parse first operand which excludes binary operations (but brackets allowed)
		tmpres=parser(src,pos,false,-2,level+1);
		//if next sym is operation
		if(src.result[tmpres.pos].t==CHAR_TOP) {
			//(for recursion) if prev. operation's prio is smaller than current, 
			//tmpres.pos - operator
			if(prev_binary==-2) {
				repeat(sym,level,true);
				cerr << "prev: clean ";
			}
			if(op_prio(src.result[tmpres.pos].c)>prev_binary) {
				if(prev_binary!=-2) {
					repeat(sym,level,true);
					cerr << "prev: smaller ";
				}
				tmpres1=parser(src,tmpres.pos+1,true,op_prio(src.result[tmpres.pos].c),level+1);
				//if inner exits 'cause my prio is bigger
				if(tmpres1.state==1) {
					if(src.result[tmpres1.pos].t==CHAR_TOP) {
						tmpres2=parser(src,tmpres1.pos+1,true,-2,level+1);
						rtree=new tree(new tree(tmpres.tr,src.result[tmpres.pos].c,tmpres1.tr),src.result[tmpres1.pos].c,tmpres2.tr);
						ok=true;
						result.pos=tmpres2.pos;
					}
				}
				else {
					rtree=new tree(tmpres.tr,src.result[tmpres.pos].c,tmpres1.tr);
					result.pos=tmpres1.pos;
					ok=true;
				}
			}
			else {
				repeat(sym,level,true);
				cerr << "prev: bigger";
				//returns only one
				//5*3+7
				//  ^
				tmpres1=parser(src,pos,false,-2,level+1);
				rtree=tmpres1.tr;
				result.pos=tmpres1.pos;
				ok=true;
				result.state=1;
			}
		}
	}
	else {
		cerr << "b- ";
	}
	cerr << endl;
	ok=false;*/
	if(pos!=binary_a) {
		tmpres=parser(src,pos,pos);
		if(src.result[tmpres.pos].t==CHAR_TOP) {
			tmpres1=parser(src,tmpres.pos+1);
			if(pos!=binary_a) {
				rtree=new tree(tmpres.tr,src.result[tmpres.pos].c,tmpres1.tr);
				result.pos=tmpres1.pos;
				ok=true;
			}
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
			return(result);
		}
	}
	//unary
	if(src.result[pos].t==CHAR_TOP&&!ok) {
		ok=true;
		tmpres=parser(src,pos+1);
		if(str(src.result[pos].c,"-")) {
			rtree=new tree("-",tmpres.tr);
			result.pos=tmpres.pos;
		}
		else {
			cerr << pos << ": unary \"" << src.result[pos].c << "\" not allowed" << endl;
			return(result);
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
				cerr << tmpres.pos << ": function closing bracket expected, but '" << src.result[tmpres.pos-1].c << "' found." << endl;
				return(result);
			}
		}
	}
	//atomic
	if((src.result[pos].t==CHAR_TNUM||src.result[pos].t==CHAR_TLETT)&&!ok) {
		ok=true;
		result.pos=pos+1;
		rtree=new tree(src.result[pos].c);
	}
	result.tr=rtree;
	return(result);
}
int main() {
	char* in=new char[10000];
	//char* in="sin(x/cos(x*tg(10*x^(cos(exp(x))))))";
	cout << "Enter equal: ";
	cin >> in;
	//cerr << strlen(in);
	parser_answer b;
	b=parser(lexer(in));
	if(b.tr==NULL) {
		cerr << "Wrong equal." << endl;
	//	main();
	}
	tree tree2=*b.tr;
	cout << "Equal:" << endl;
	tree2.display();
	cout << endl <<  "Easy equal:" << endl;
	tree2.easy()->display();
	cout << endl <<	 "Diff:" << endl;
	(tree2.easy())->diff("x")->display();
	cout << endl << "Easy diff:" << endl;
	(((tree2.easy())->diff("x"))->easy())->easy()->display();
	cout << endl << "==================" << endl;
//	main();
}
