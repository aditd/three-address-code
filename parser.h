#ifndef __PARSER_H 
#define __PARSER_H
/* Symbol Table Entry */ 
typedef struct symtab {
    char *name;
    int value; 
} symboltable;
/* Look-up Symbol Table */ 
symboltable *symlook(char *);
/* Generate temporary variable */ 
symboltable *gentemp();
/* Output 3-address codes */
/* if s3 != 0 ==> Assignment with Binary operator */
/* if s3 == 0 && c != 0 ==> Assignment with Unary operator */ /* if s3 == 0 && c == 0 ==> Simple Assignment */
void emit_bin(char *s1, char *s2, char c, char *s3);
void emit_assign(char *s1, char *s2);
void emit_un(char *s1, char *s2, char c);
#endif // __PARSER_H