#ifndef __PARSER_H 
#define __PARSER_H
/* Symbol Table Entry */ 
typedef struct symtab {
    char *name;
    int value; 
} symboltable;

typedef enum { 
    PLUS = 1,
    MINUS, 
    MULT,
    MOD,
    DIV,
    LESS,
    MORE,
    LE,
    GE,
    OR,
    AND,
    JUMP,
    COPY
} opcodeType;



// we might need to remove operatortype because it is supposed to be a quad
typedef struct quad {
    opcodeType op;
    char * result; 
    char * arg1; // this can take the place of the relop thing for conditional jump
    char * arg2; // this can be the jumpto location. the exit for jump
} quad;


struct boo {
    int quadLoc;
    int truelist[10];
    int falselist[10];
};

// we can have the quad operator type set to jump
// operatortype can be if
// for if, have the arg

void makelist(int *arr,int i);
int * mergelist(int *l1,int *l2);
void backpatch(int *lis, int i);
// quad(op=MINUS,type=BIN,$$->name,$1->name,$1->name)
char * print_operator(opcodeType op);

/* Look-up Symbol Table */ 
symboltable *symlook(char *);
/* Generate temporary variable */ 
symboltable *gentemp();
/* Output 3-address codes */
/* if s3 != 0 ==> Assignment with Binary operator */
/* if s3 == 0 && c != 0 ==> Assignment with Unary operator */ /* if s3 == 0 && c == 0 ==> Simple Assignment */
quad emit_bin(char *result, char *arg1,opcodeType operator, char *arg2);
quad emit_assign(char *result, char *arg1);
quad emit_un(char *result, char *arg1, opcodeType operator);
quad emit_jump(char *go);
void emit_jump_cond(char *left, opcodeType operator,char *right, char *go);
#endif // __PARSER_H