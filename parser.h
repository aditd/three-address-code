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
    DIV
} opcodeType;

typedef enum { 
    ASSIGN = 0,
    UNARY, 
    BINARY
} operatortype;

typedef struct quad {
    opcodeType op;
    operatortype type;
    char * result;
    char * arg1;
    char * arg2;
    // ONLY arg1 and result are there if COPY
    // ONLY arg1, op and result are there if UNARY
    
} quad;

// quad(op=MINUS,type=BIN,$$->name,$1->name,$1->name)
char print_operator(opcodeType op) {
    switch (op) {
        case PLUS:
            return '+';
            break;
        case MINUS:
            printf("-");
            return '-';
            break;
        case MULT:
            printf("*");
            return '*';
            break;
        case DIV:
            printf("/");
            return '/';
            break;
        case MOD:
            printf("%");
            return '%';
            break;
    }
} 
    


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
#endif // __PARSER_H