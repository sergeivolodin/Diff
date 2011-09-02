#ifndef SYMBOLIC_COMPUTATION_H
#define SYMBOLIC_COMPUTATION_H
#include <string.h>
#include <vector>
#include <map>
#include "tree.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

class symbolic_computation
{
    friend class tree;

private:
    enum stringtype_t {SVARIABLE,SNUMBER,SOPERATION,SBR1,SBR2,SOTHER};
    enum debuglevel_t {DNONE,DEASY,DALL};

public: typedef map<string,tree> replacers;
private:
    struct arithmetic_operation {
        string name;
        unsigned int priority;
    };

    struct token {
        string content;
        stringtype_t type;
    };

    struct lexer_answer {
        vector<token> tokens;
        bool errors;
    };

    struct rule_contents {
        tree destination;
        replacers post_replace;
    };

    typedef pair<tree,rule_contents> rule;

    typedef vector<rule> rules;

    typedef map<string,rules> rules_set;

    //settings
    string BR1;
    string BR2;
    debuglevel_t debug_level;
    vector<arithmetic_operation> arithmetic_operations;
    string set_all;
    string set_numbers;
    string set_variables;

    string settings_name_value_separator;
    char settings_comment_symbol;

    string settings_filename;

    unsigned int op_prio(string which);
    stringtype_t stringtype(string which);

    //analyzer (lexer&parser)
    lexer_answer lexer(string);
    bool testbrackets(lexer_answer src,int pmin=0,int pmax=-1);
    unsigned int minopprio(lexer_answer src,int pmin=0,int pmax=-1);
    int getlast(lexer_answer src,int type,int pmin=0,int i=-1,bool notinsidebrackets=false,bool nounary=false);
    bool brackets_nospace(lexer_answer src,int left,int right);
    tree* parser_atomic(lexer_answer src,int pmin,int pmax);
    tree* parser_brackets(lexer_answer src,int pmin,int pmax);
    tree* parser_unary(lexer_answer src,int pmin,int pmax);
    tree* parser_function(lexer_answer src,int pmin,int pmax);
    tree* parser(lexer_answer src,int pmin=0,int pmax=-1,bool binary_allowed=true);

    //perform
    bool replacer_add(replacers* a,string x,tree* y);
    bool replacer_add(replacers* a,string x,tree::tree_leaf_t);
    bool set_contains(string set,string needle);
    tree* getreplacer(replacers* where,string what);
public:
    symbolic_computation();

    //analyzer (lexer&parser)
    tree* parse_string(string source);

    //perform
    bool pattern(tree* stree, tree* spattern,string base,replacers* d=NULL);
    void print_replacers(replacers* source);
    tree* replace(tree* spattern,replacers* srepl);
    tree* operate(tree* src,symbolic_computation::rules* crules,string operation,string base,bool rmode=false);

    //settings
    void read_settings();
    void write_settings();
};

#endif // SYMBOLIC_COMPUTATION_H
