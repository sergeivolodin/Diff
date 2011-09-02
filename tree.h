#ifndef TREE_H
#define TREE_H
#define TREEDEBUG
#include <iostream>
#include <string.h>

#ifdef TREEDEBUG
    #include <assert.h>
#endif

using std::string;

class symbolic_computation;

class tree {
    friend class symbolic_computation;
public:
    typedef string tree_leaf_t;
    enum tree_treetype_t {TREE_TUNARY,TREE_TBINARY,TREE_TLEAF,TREE_TINVALID};
    enum tree_mathtype_t {TREE_MUNARY,TREE_MBINARY,TREE_MFUNCTION,TREE_MVARIABLE,TREE_MNUMBER,TREE_MINVALID};
    enum tree_display_t {TREE_DBRACKETS,TREE_DNORMAL};

    tree();
    ~tree();
    void erase_subtrees();
    tree(tree_leaf_t); //leaf
    tree(tree*,tree_leaf_t,tree*); //binary
    tree(tree_leaf_t,tree*); //unary

    tree(const tree&);
    void operator=(tree*);

    tree* copy(); //returns new tree, recursive

    bool operator==(tree); //same content?
    bool operator!=(tree);
    string display(symbolic_computation*,tree_display_t display_type=TREE_DNORMAL);

    tree* geta();
    tree* getb();
    tree::tree_leaf_t getvalue();

    bool contains(tree_leaf_t); //contains _value_?

    tree::tree_treetype_t treetype(); //UNARY/BINARY/LEAF
    tree::tree_mathtype_t mathtype(symbolic_computation*); //UNARY/BINARY/NUMBER/VARIABLE/FUNCTION

    unsigned int varcount(symbolic_computation*);
    unsigned int length();
    unsigned int opcount(symbolic_computation*);
private:
    tree_leaf_t value;
    bool leaf;
    tree *a, *b;

};

#endif // TREE_H
