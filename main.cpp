#include <iostream>
#include <iomanip>
#include <string.h>
#include <strings.h>
using namespace std;
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
			//показывать указатели?
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
			else if(value=="exp") {
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
		//Операция (если не лист)||значение (если лист)
		char* value;
		//лист это или нет
		bool leaf;
		//если не лист, то два указателя на другие деревья.
		//если унарная операция, то используется только а
		tree *a,*b;
	public:
		//лист
		tree(char* str): value(str),leaf(true) { }
		//бинарная операция с деревьями
		tree(tree *in_a,char* op,tree *in_b): value(op),leaf(false),a(in_a),b(in_b) {}
		//унарная операция с деревом
		tree(char *op,tree *in_): value(op),leaf(false),a(in_),b(NULL) {}
		//бинарная операция с листьями
		tree(char* ina, char* op, char* inb): value(op),leaf(false) {
			a=new tree(ina);
			b=new tree(inb);
		}
		//унарная операция с листом
		tree(char* op,char* in_): value(op),leaf(false),b(NULL) {
			a=new tree(in_);
		}
		tree(){}
		//возвращает указатель на такое же, но другое дерево
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
			//почему-то не работает, а надо бы в операции присваивания
			//чтобы не занимать память a и b, которые были у объекта до нее.
			/*if(leaf==false) {
				delete *a;
				delete *b;
			}*/
		}
		tree* diff(char* base) {
			tree* nt;
			nt=new tree();
			if(leaf==true) {
				if(*value==*base) {
					nt=new tree("1");
				}
				else {
					nt=new tree("0");
				}
			}
			else {
				if(b==NULL) {
					if(value=="-") {
						nt=new tree("-",a->diff(base));
					}
					if(value=="ln") {
						nt=new tree(a->diff(base),"*",new tree(new tree("1"),"/",a));
					}
					if(value=="sin") {
						nt=new tree(a->diff(base),"*",new tree("-",new tree("cos",a)));
					}
					if(value=="arcsin") {
						nt=new tree(a->diff(base),"*",new tree(new tree("1"),"/",new tree("sqrt",new tree(new tree("1"),"-",new tree(a,"^",new tree("2"))))));
					}
					if(value=="arccos") {
						nt=new tree("-",new tree(a->diff(base),"*",new tree(new tree("1"),"/",new tree("sqrt",new tree(new tree("1"),"-",new tree(a,"^",new tree("2")))))));
					}
					if(value=="arctg") {
						nt=new tree(a->diff(base),"*",new tree(new tree("1"),"/",new tree(new tree("1"),"+",new tree(a,"^",new tree("2")))));
					}
					if(value=="tg") {
						nt=new tree(a->diff(base),"*",new tree(new tree("1"),"/",new tree(new tree("cos",a),"^",new tree("2"))));
					}
					if(value=="exp") {
						nt=new tree(a->diff(base),"*",new tree("exp",a));
					}
					if(value=="cos") {
						nt=new tree(a->diff(base),"*",new tree("sin",a));
					}
					if(value=="sqrt") {
						nt=new tree(a->diff(base),"*",new tree(new tree("1"),"/",new tree(new tree("2"),"*",new tree("sqrt",a))));
					}
				}
				else {
					if(value=="/") {
						nt=new tree(new tree(new tree(a->diff(base),"*",b),"-",new tree(a,"*",b->diff(base))),"/",new tree(b,"^",new tree("2")));
					}
					if(value=="+"||value=="-") {
						nt=new tree(a->diff(base),value,b->diff(base));
					}
					if(value=="*") {
						nt=new tree(new tree(a,"*",b->diff(base)),"+",new tree(a->diff(base),"*",b));
					}
					if(value=="^") {
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
		//присваивает текущему дереву копию другого
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
					if(value=="+") {
						if((nt->a->value)=="0") {
							rt=nt->b->copymem();
						}
						if((nt->b->value)=="0") {
							rt=nt->a->copymem();
						}
					}
					if(value=="-") {
						//не менять порядок!
						if((nt->a->value)=="0") {
							rt=new tree("-",nt->b);
						}
						if((nt->b->value)=="0") {
							rt=nt->a->copymem();
						}
					}
					if(value=="*") {
						if((nt->a->leaf==true&&nt->a->value=="0")||(nt->b->leaf==true&&nt->b->value=="0")) {
							rt=new tree("0");
						}
						if((nt->a->value)=="1") {
							rt=nt->b;
						}
						if((nt->b->value)=="1") {
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
inline bool char_isnum(char sym) {
	return((sym>='0'&&sym<='9')||sym=='.');
}
inline bool char_islett(char sym) {
	return((sym>='A'&&sym<='Z')||(sym>='a'&&sym<='z'));
}
inline bool char_isop(char current) {
	return((current=='('||current==')'||current=='+'||current=='-'||current=='*'||current=='/'||current=='^'));
}
int char_state(char in) {
	if(char_isnum(in)) {
		return(1);
	}
	if(char_islett(in)) {
		return(2);
	}
	if(char_isop(in)) {
		return(3);
	}
	if(in=='\0') {
		return(4);
	}
	return(0);
}
struct lekser_answer {
	char** result;
	int max;
};
lekser_answer lekser(char* str) {
	int i=0;
	int len=strlen(str);

	int pos=0;
	char* temp="",*a,*b;
	char current;
	int laststate=-1,currentstate;
	char** result=new char*[100];
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
			if((currentstate!=3)&&(laststate==currentstate)) {
				strcpy(a,temp);
				temp=new char[strlen(temp)+1];
				b=new char[0];
				b[0]=current;
				temp=strcat(a,b);
			}
			else {
				if(laststate!=-1) {
					result[i]=new char[strlen(temp)+1];
					strcpy(result[i],temp);
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
int main() {
	/*
//	tree tree2(new tree("exp",new tree("2","*","x")),"/",new tree("sin",new tree(new tree("2","*","x"),"+",new tree("6"))));
	tree tree2("tg",new tree("x","*","2"));
	cout << "Equal:" << endl;
	tree2.display();
	cout << endl <<  "Easy equal:" << endl;
	tree2.easy()->display();
	cout << endl <<	 "Diff:" << endl;
	tree2.diff("x")->display();
	cout << endl << "Easy diff:" << endl;
	(((tree2.easy())->diff("x"))->easy())->display();
	cout << endl;
	return 0;
	*/
	char* in=new char[100];
	cin >> in;
	lekser_answer a;
	a=lekser(in);
	int i=0;
	while(i<=a.max) {
		cout << a.result[i] << "_";
		i++;
	}
	cout << endl;
	return 0;
}
