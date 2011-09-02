#include "symbolic_computation.h"
#include "tree.h"
#include <iostream>
#include <string>
#include <vector>

using std::cerr;
using std::endl;
using std::vector;

int main(int argc, char *argv[]) {
    symbolic_computation computation;
    tree* stree;
    tree* spattern;
    symbolic_computation::replacers* repl=new symbolic_computation::replacers;
    //stree=computation.parse_string("sin(x)+cos(x)+d");
    //spattern=computation.parse_string("f(g)+h");
    computation.read_settings();
    computation.write_settings();
    /*while(true) {
        computation.pattern(stree,spattern,"x",repl);
        computation.print_replacers(repl);
        repl->clear();
    }*/
    //return a.exec();
}
