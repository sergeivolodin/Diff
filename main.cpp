#include <iostream>
#include <iomanip>
#include <string.h>
#include <strings.h>
using namespace std;
bool str(char* a,char* b) {
	return(strcmp(a,b)==0?true:false);
}
inline bool char_isnum(char sym) {
	return((sym>='0'&&sym<='9')||sym=='.');
}
inline bool char_islett(char sym) {
	return((sym>='A'&&sym<='Z')||(sym>='a'&&sym<='z'));
}
inline bool char_isop(char current) {
	return((current=='+'||current=='-'||current=='*'||current=='/'||current=='^'));
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
struct lekser_answer {
	token* result;
	int max;
};
const int TREE_DMATH=0;
const int TREE_DTREE=1;
const int TREE_DDEFAULT=TREE_DMATH;
void repeat(char* str,int count=1) {
	int i=1;
	while(i<=count) {
		cout << str;
		i++;
	}
}

class tree {
	private:
		void display_math() {
			//display pointers?
			bool showpntr=false;
			bool tmp;
			if(leaf==true) {
				cout << value;
			}
			else if(b!=NULL) {
				if((tmp=((b->leaf==false) && (b->leaf==false)))) {
					if(showpntr==true) {
						cout << " this: " << this << " a:" << a << " b:" << b <<  " (";
					}
					else {
						cout << "(";
					}
				}
				a->display_math();
				if(tmp) {
					cout << ")";
				}
				cout << value;
				if(tmp) {
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
			nt=new tree();
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
					nt=new tree("function error");
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
					rt=new tree(value,a->easy());
				}
				else {
					nt=new tree(a->easy(),value,b->easy());
					rt=nt;
					if(str(value,"+")) {
						if(str(nt->a->value,"0")) {
							rt=nt->b->copymem();
						}
						if(str(nt->b->value,"0")) {
							rt=nt->a->copymem();
						}
					}
					if(str(value,"-")) {
						//dont change order!
						if(str(nt->a->value,"0")) {
							rt=new tree("-",nt->b);
						}
						if(str(nt->b->value,"0")) {
							rt=nt->a->copymem();
						}
					}
					if(str(value,"*")) {
						if((nt->a->leaf==true&&str(nt->a->value,"0"))||(nt->b->leaf==true&&str(nt->b->value,"0"))) {
							rt=new tree("0");
						}
						if(str(nt->a->value,"1")) {
							rt=nt->b;
						}
						if(str(nt->b->value,"1")) {
							rt=nt->a;
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
	tree* tr;
};
lekser_answer lekser(char* str) {
	int i=0;
	int len=strlen(str);

	int pos=0;
	char* temp="",*a,*b;
	char current;
	int laststate=-1,currentstate;
	token* result=new token[len]; //char count more than tokens count.
	lekser_answer res;
	while(pos<=len) {
		current=str[pos];
		currentstate=char_state(current);
		if(currentstate==0) {
			cout << "Wrong symbol at " << pos << endl;
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
parser_answer parser(lekser_answer src,int pos=0) {
	tree* rtree=NULL;
	parser_answer result,tmpres,tmpres1;
	result.pos=pos;
	bool ok=false;
	result.tr=NULL;
	if(src.result[pos].t==CHAR_TBR1) {
		ok=true;
		tmpres=parser(src,pos+1);
		if(src.result[tmpres.pos+1].t==CHAR_TBR2) {
			result.pos=tmpres.pos+2;
			rtree=tmpres.tr;
		}
		else {
			cout << "Closing bracket not found" << endl;
			return(result);
		}
	}
	if(src.result[pos].t==CHAR_TOP) {
		ok=true;
		tmpres=parser(src,pos+1);
		if(str(src.result[pos].c,"-")) {
			rtree=new tree("-",tmpres.tr);
			result.pos=tmpres.pos;
		}
		else {
			cout << "Unary \"" << src.result[pos].c << "\" not allowed" << endl;
			return(result);
		}
	}
	if(src.result[pos].t==CHAR_TLETT) {
		if(src.result[pos+1].t==CHAR_TBR1) {
			ok=true;
			tmpres=parser(src,pos+2);
			//&&tmpres.tr!=NULL
			if(src.result[tmpres.pos].t==CHAR_TBR2&&tmpres.tr!=NULL) {
				rtree=new tree(src.result[pos].c,tmpres.tr);
			}
			else {
				cout << "Function closing bracket not found at position " << tmpres.pos << endl;
				return(result);
			}
		}
	}
	if((src.result[pos].t==CHAR_TNUM||src.result[pos].t==CHAR_TLETT)&&!ok) {
		ok=true;
		result.pos=pos+1;
		rtree=new tree(src.result[pos].c);
	}
	result.tr=rtree;
	return(result);
}
int main() {
	char* in=new char[100];
	cout << "Enter equal: ";
	cin >> in;
	parser_answer b;
	b=parser(lekser(in));
	if(b.tr==NULL) {
		cout << "Wrong equal." << endl;
		main();
	}
	tree tree2=*b.tr;
	cout << "Equal:" << endl;
	tree2.display();
	cout << endl <<  "Easy equal:" << endl;
	tree2.easy()->display();
	cout << endl <<	 "Diff:" << endl;
	(tree2.easy())->diff("x")->display();
	cout << endl << "Easy diff:" << endl;
	(((tree2.easy())->diff("x"))->easy())->display();
	cout << endl << "==================" << endl;
	main();
}
