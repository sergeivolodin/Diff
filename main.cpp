#include <iostream>
#include <iomanip>
using namespace std;
void repeat(char* str,int count=1) {
	int i=1;
	while(i<=count) {
		cout << str;
		i++;
	}
}
class btree {
	private:
		//Операция (если не лист)|значение (если лист)
		char* value;
		//лист это или нет
		bool leaf;
		//если не лист, то два указателя на другие деревья.
		//если унарная операция, то используется только а
		btree *a,*b;
	public:
		//лист
		btree(char* str): leaf(true),value(str) { }
		//бинарная операция с деревьями
		btree(btree *in_a,char* op,btree *in_b): leaf(false),a(in_a),b(in_b),value(op) { }
		//унарная операция с деревом
		btree(char *op,btree *in_): leaf(false),b(NULL),a(in_),value(op) {}
		//бинарная операция с листьями
		btree(char* ina, char* op, char* inb): leaf(false),value(op) {
			a=new btree(ina);
			b=new btree(inb);
		}
		//унарная операция с листом
		btree(char* op,char* in_): leaf(false),value(op),b(NULL) {
			a=new btree(in_);
		}
		btree(){}
		//возвращает указатель на такое же, но другое дерево
		btree* copymem() {
			btree* nt;
			if(leaf==true) {
				nt=new btree(value);
				
			}
			else {
				if(b==NULL) {
					nt=new btree(value,a->copymem());
				}
				else {
					nt=new btree(a->copymem(),value,b->copymem());
				}
			}
			return(nt);
		}
		~btree() {
			//почему-то не работает, а надо бы в операции присваивания
			//чтобы не занимать память a и b, которые были у объекта до нее.
			/*if(leaf==false) {
				delete *a;
				delete *b;
			}*/
		}
		btree* diff(char* base) {
			btree* nt;
			nt=new btree();
			if(leaf==true) {
				if(*value==*base) {
					nt=new btree("1");
				}
				else {
					nt=new btree("0");
				}
			}
			else {
				if(b==NULL) {
					if(value=="-") {
						nt=new btree("-",a->diff(base));
					}
					if(value=="sin") {
						nt=new btree(new btree("-",new btree("cos",a)),"*",a->diff(base));
					}
					if(value=="exp") {
						nt=new btree(new btree("exp",a),"*",a->diff(base));
					}
					if(value=="cos") {
						nt=new btree(new btree("sin",a),"*",a->diff(base));
					}
					if(value=="sqrt") {
						nt=new btree(new btree(new btree("1"),"/",new btree(new btree("2"),"*",new btree("sqrt",a))),"*",a->diff(base));
					}
				}
				else {
					if(value=="/") {
						nt=new btree(new btree(new btree(a->diff(base),"*",b),"-",new btree(a,"*",b->diff(base))),"/",new btree(b,"**",new btree("2")));
					}
					if(value=="+"|value=="-") {
						nt=new btree(a->diff(base),value,b->diff(base));
					}
					if(value=="*") {
						nt=new btree(new btree(a,"*",b->diff(base)),"+",new btree(a->diff(base),"*",b));
					}
					if((value=="^")|(value=="**")) {
						nt=new btree(b,"*",new btree(a,value,new btree(b,"-",new btree("1"))));
					}
				}
			}
			return(nt);
		}
		/*void operator=(float n){value=n;}
		void operator=(char n){operation=n;}
		void operator=(bool n){leaf=n;}*/
		void seta(btree* aset){a=aset;}
		void setb(btree* bset){b=bset;}
//присваивает текущему дереву копию другого
		void operator=(btree *src) {
			/*delete *a;
			delete *b;*/
			btree *newtree;
			newtree=src->copymem();
			leaf=newtree->leaf;
			value=newtree->value;
			value=newtree->value;
			a=newtree->a;
			b=newtree->b;
		}
		btree* easy(int level=0) {
			btree* nt;
			if(leaf==false) {
				if(b==NULL) {
					nt=new btree(value,a->easy());
				}
				else {
					nt=new btree(a->easy(),value,b->easy());
					if(value=="+"|value=="-") {
						if((a->value)=="0") {
							nt=b->easy();
						}
						if((b->value)=="0") {
							nt=a->easy();
						}
					}
					if(value=="*") {
						if((a->leaf==true&(a->value)=="0")|(b->leaf==true&(b->value)=="0")) {
							nt=new btree("0");
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
				nt=new btree(value);
			}
			if(level<=1) {
				return nt->easy(level+1);
			}
			else {
				return nt;
			}
		}

		void treeview(int level=0) {
			char* sym=":";
			//cout << "_";
			if(leaf==true) {
				repeat(sym,level);
				cout << value << endl;
			}
			else {
				if(b==NULL) {
					repeat(sym,level);
					cout << value << endl;
					a->treeview(level+1);
				}
				else {
					//repeat(sym,level);
					a->treeview(level+1);
					//cout << endl;
					repeat(sym,level);
					cout <<	value << endl;
					b->treeview(level+1);
				}
			}
		}
		//показывает дерево и указатели
		void display() {
			//показывать указатели?
			bool showpntr=false;
			bool tmp;
			if(leaf==true) {
				cout << value;
			}
			else if(b!=NULL) {
				if((tmp=((b->leaf==false) & (b->leaf==false)))) {
					if(showpntr==true) {
						cout << " this: " << this << " a:" << a << " b:" << b <<  " (";
					}
					else {
						cout << "(";
					}
				}
				//(a->easy())->display();
				a->display();
				if(tmp) {
					cout << ")";
				}
				cout << value;
				if(tmp) {
					cout << "(";
				}
				//(b->easy())->display();
				b->display();
				if(tmp) {
					cout << ")";
				}
			}
			else if(value=="exp") {
				cout << "e^(";
				a->display();
				cout << ")";
			}
			else {
				cout << value << "(";
				a->display();
				cout << ")";
			}
		}
};
int main()
{
	btree btree2(new btree("exp",new btree("2","*","x")),"/",new btree("sin",new btree(new btree("2","*","x"),"+",new btree("6"))));
	cout << "Equal:" << endl;
	btree2.treeview();
	cout << endl <<  "Easy equal:" << endl;
	btree2.easy()->treeview();
	cout << endl <<	 "Diff:" << endl;
	btree2.diff("x")->treeview();
	cout << endl << "Easy diff:" << endl;
	(((btree2.easy())->diff("x"))->easy())->treeview();
	return 0;
}
