#ifndef __PARSER_H

#define __PARSER_H
#define NSYMS 20

// possible values for an operation
typedef enum {
    PLUS,
    MINUS,
    MULT,
    MOD,
    DIV,
    LT,
    GT,
    EQ,
    NE,
    LTE,
    GTE,
    OR,
    AND,
    JUMP,
    JLT,
    JGT,
    JLTE,
    JGTE,
    PTR,
    COPY,
    UNARYMINUS
} opType;

// structure of a symbol
typedef struct _symbol {
    char* name;
    char* value; // values saved as string to handle string, char, int
} symbol;

// structure of a quad
typedef struct _quad {
    opType op;
    int index;
    char* result;
    char* arg1; // this can take the place of the relop thing for conditional jump
    char* arg2; // this can be the jumpto location. the exit for jump
} quad;

// structure of a node
typedef struct _node {
    int val;
    struct _node* next;
} node;

// structure of a list
typedef struct _list {
    node* head;
    node* tail;
} list;

// structure of a boolean variable
typedef struct _boolean {
    list* truelist;
    list* falselist;
} boolean;

// all following functions are defined in A4_11_translator.c

// create or find symbol with given name
symbol* symlook(char* name);
// generate a temporary variable
symbol* gentemp();
// create a node
node* create(int value);
// create a list
list* makelist(int i);
// merge 2 lists
list* merge_lists(list* l1, list* l2);
// print a list
void print_list(list* li);
// backpatch
void backpatch(list* l, int m);
// print a quad
void print_quad(quad* q);
// print all quads
void print_quads();
// create a new quad with a binary operation
void new_quad_binary(opType op, char* result, char* arg1, char* arg2);
// create a new quad with a unary operation
void new_quad_unary(opType op, char* result, char* arg1);
// create a new quad for a conditional jump
void new_quad_conditional_jump(opType op, char* left, char* right, char* go);
// create a new quad for an unconditional jump
void new_quad_jump(char* go);

#endif // __PARSER_H