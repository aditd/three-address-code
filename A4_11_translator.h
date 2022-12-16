#ifndef __PARSER_H

#define __PARSER_H
#define NSYMS 20

// structure of a symbol
typedef struct _symbol {
    char* name;
    char* value; // values saved as string to handle string, char, int
} symbol;

// possible values for an operation type
typedef enum {
    PLUS = 1,
    MINUS,
    MULT,
    MOD,
    DIV,
    LT,
    GT,
    LET,
    GET,
    OR,
    AND,
    JUMP,
    JLT,
    JGT,
    JLET,
    JGET,
    COPY
} opType;

// structure of a quad
typedef struct _quad {
    opType op;
    int index;
    char* result;
    char* arg1; // this can take the place of the relop thing for conditional jump
    char* arg2; // this can be the jumpto location. the exit for jump
} quad;

typedef struct _node {
    int val;
    struct node* next;
} node;

typedef struct _list {
    node* head;
    node* tail;
} list;

struct boolean {
    list* truelist;
    list* falselist;
};

symbol symboltable[NSYMS]; // array of symbols, i.e. symboltable
quad* qArray[NSYMS]; // array of quads
int quadPtr = 0; // pointer to quad index


// all following functions are defined in A4_11_translator.c
symbol* symlook(char* name);
symbol* gentemp();

node* create(int value);
list* makelist(int i);
list* merge_lists(list* l1, list* l2);
void backpatch(list* l, int m);
void print_list(list* li);

void print_quad(quad* q);
void new_quad_binary(opType op, char* result, char* arg1, char* arg2);
void new_quad_unary(opType op, char* result, char* arg1);
void new_quad_conditional_jump(opType op, char* left, char* right, char* go);
void new_quad_jump(char* go);


#endif // __PARSER_H