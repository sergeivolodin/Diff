#include <iostream>
#include <iomanip>
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
		tree(tree *in_a,char* op,tree *in_b): value(op),leaf(false),a(in_a),b(in_b) { }
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
					if(value=="sin") {
						nt=new tree(new tree("-",new tree("cos",a)),"*",a->diff(base));
					}
					if(value=="exp") {
						nt=new tree(new tree("exp",a),"*",a->diff(base));
					}
					if(value=="cos") {
						nt=new tree(new tree("sin",a),"*",a->diff(base));
					}
					if(value=="sqrt") {
						nt=new tree(new tree(new tree("1"),"/",new tree(new tree("2"),"*",new tree("sqrt",a))),"*",a->diff(base));
					}
				}
				else {
					if(value=="/") {
						nt=new tree(new tree(new tree(a->diff(base),"*",b),"-",new tree(a,"*",b->diff(base))),"/",new tree(b,"**",new tree("2")));
					}
					if(value=="+"||value=="-") {
						nt=new tree(a->diff(base),value,b->diff(base));
					}
					if(value=="*") {
						nt=new tree(new tree(a,"*",b->diff(base)),"+",new tree(a->diff(base),"*",b));
					}
					if((value=="^")||(value=="**")) {
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
		tree* easy(int level=0) {
			tree* nt;
			if(leaf==false) {
				if(b==NULL) {
					nt=new tree(value,a->easy());
				}
				else {
					nt=new tree(a->easy(),value,b->easy());
					if((value=="+")||(value=="-")) {
						if((a->value)=="0") {
							nt=b->easy();
						}
						if((b->value)=="0") {
							nt=a->easy();
						}
					}
					if(value=="*") {
						if((a->leaf==true&&(a->value)=="0")||(b->leaf==true&&(b->value)=="0")) {
							nt=new tree("0");
						}
						if((a->value)=="1") {
							nt=b->easy();
						}
						if((b->value)=="1") {
							nt=a->easy();
						}
					}
				}
			}
			else {
				nt=new tree(value);
			}
			if(level<=1) {
				return nt->easy(level+1);
			}
			else {
				return nt;
			}
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
int main()
{
//	tree tree2(new tree("exp",new tree("2","*","x")),"/",new tree("sin",new tree(new tree("2","*","x"),"+",new tree("6"))));
	tree tree2("x");
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
}
