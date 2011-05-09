#include "libdiff.cpp"
int main() {
	char* buf=new char[500];
	cout << "Enter: ";
	cin >> buf;
	settings_from_file();
	tree* src=parser(lexer(buf)),*dest=NULL;
	if(src!=NULL) {
		dest=operate(src,1,MATH_DEFDIFF);
		if(dest!=NULL) {
			cout << easy(dest)->display() << endl;
		}
	}
}
