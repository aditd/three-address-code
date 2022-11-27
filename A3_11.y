%{ /* C Declarations and Definitions */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "A3_11.tab.h"
#include "parser.h" /* wasnt there in parser*/
extern int yylex(); // Generated by Flex
void yyerror(char *s);
#define NSYMS 20 /* max # of symbols */ 
symboltable symtab[NSYMS];
quad *qArray[NSYMS];
int quadPtr = 0;
%}


%union {                 
    int intval;
    struct symtab *symp;    /* 2 */
}


%token CHAR ELSE FOR IF 
%token INT RETURN VOID TYPEDEF

%token <symp> IDENTIFIER

%token STRING_LIT
%token C_CONSTANT

%token <symp> I_CONSTANT

%token AND_OP
%token PTR_OP
%token OR_OP
%token LE_OP
%token GE_OP
%token EQ_OP
%token NE_OP
%define parse.error detailed
%type <symp> primary_expression
%type <symp> postfix_expression
%type <symp> unary_expression
%type <symp> multiplicative_expression
%type <symp> additive_expression
%start translation_unit

%%


//constant:
//C_CONSTANT |
//;

// EXPRESSIONS
primary_expression:
IDENTIFIER { $$ = $1;}| // Simple identifier  // Integer or character constant string-literal this wILLL BE PROBLEM
STRING_LIT |
I_CONSTANT { $$->name = $1->name;} |
C_CONSTANT |
'(' expression ')'
;

postfix_expression: // Expressions with postfix operators. Left assoc. in C; non-assoc. here 
primary_expression {$$ = $1;}|
postfix_expression '[' expression ']' |// 1-D array access
postfix_expression '(' argument_expression_list_opt ')' |// Function invocation
postfix_expression PTR_OP IDENTIFIER
;

argument_expression_list_opt:
%empty |
argument_expression_list
;

argument_expression_list: 
assignment_expression |
argument_expression_list ',' assignment_expression
;

unary_operator:
'&'|'*' |'+' |'-' |'!';

unary_expression: 
postfix_expression {$$ = $1;}|
'+' unary_expression {$$ = gentemp(); 
emit_un($$->name, $2->name, PLUS);}|
'-' unary_expression {$$ = gentemp();
emit_un($$->name, $2->name, MINUS);}|
unary_operator unary_expression 
;

multiplicative_expression: // Left associative operators 
unary_expression {$$ = $1;}|
multiplicative_expression '*' unary_expression {$$ = gentemp();
emit_bin($$->name, $1->name, MULT,$3->name);}|
multiplicative_expression '/' unary_expression {$$ = gentemp();
emit_bin($$->name, $1->name, DIV,$3->name);}|
multiplicative_expression '%' unary_expression {$$ = gentemp();
emit_bin($$->name, $1->name, MOD,$3->name);}
;

additive_expression: // Left associative operators 
multiplicative_expression {$$ = $1;}|
additive_expression '+' multiplicative_expression {$$ = gentemp();
emit_bin($$->name, $1->name, PLUS,$3->name);}|
additive_expression '-' multiplicative_expression {$$ = gentemp();
emit_bin($$->name, $1->name, MINUS,$3->name);}
;




relational_expression: // Left associative operators 
additive_expression |
relational_expression '<' additive_expression |
relational_expression '>' additive_expression |
relational_expression LE_OP additive_expression |
relational_expression GE_OP additive_expression 
;


equality_expression: // Left associative operators 
relational_expression |
equality_expression EQ_OP relational_expression |
equality_expression NE_OP relational_expression
;

logical_AND_expression: // Left associative operators 
equality_expression |
logical_AND_expression AND_OP equality_expression
;

logical_OR_expression: // Left associative operators 
logical_AND_expression |
logical_OR_expression OR_OP logical_AND_expression
;

conditional_expression: // Right associative operator 
logical_OR_expression |
logical_OR_expression '?' expression ':' conditional_expression
;

assignment_expression: // Right associative operator 
conditional_expression |
unary_expression '=' assignment_expression
;

expression:
assignment_expression
;

//Declarations

declaration: // Simple identifier, 1-D array or function declaration of built-in type 
type_specifier init_declarator ';'
;

init_declarator:
declarator |// Simple identifier, 1-D array or function declaration
declarator '=' initializer
;

type_specifier: // Built-in types 
VOID |
CHAR |
INT
;

declarator:
pointer_opt direct_declarator
;

direct_declarator:
IDENTIFIER | // Simple identifier 
IDENTIFIER '[' I_CONSTANT ']' |
IDENTIFIER '(' parameter_list_opt ')' 
;


pointer: '*';

pointer_opt:
%empty |
pointer
;

parameter_list: 
parameter_declaration|
parameter_list ',' parameter_declaration
;

parameter_list_opt:
%empty |
parameter_list
;


parameter_declaration:
type_specifier pointer_opt identifier_opt
;

identifier_opt:
%empty |
IDENTIFIER
;

initializer:
assignment_expression;
// same as an expression



//initializer: assignment_expression;

// STATEMENT
statement:
compound_statement |// Multiple statements and / or nest block/s
expression_statement |// Any expression or null statements
selection_statement |// if or if_else
iteration_statement |// for
jump_statement
;

compound_statement:
'{' block_item_list_opt '}'
;

block_item_list_opt:
%empty |
block_item_list
;


block_item_list:
block_item |
block_item_list block_item
;

block_item:
declaration |
statement
;

expression_statement:
expressionopt ';'
;

selection_statement:
IF '(' expression ')' statement |
IF '(' expression ')' statement ELSE statement
;

expressionopt:
%empty |
expression
;

// fix this
iteration_statement:
FOR '(' expressionopt ';' expressionopt ';' expressionopt ')' statement 
;

jump_statement:
RETURN expressionopt ';'
;

// idxwond

translation_unit: // Single source file containing main() 
function_definition |
declaration
;

function_definition:
type_specifier declarator declaration_list_opt compound_statement 
;

declaration_list:
declaration |
declaration_list declaration
;

declaration_list_opt:
%empty |
declaration_list
;

%%
struct symtab * symlook(char *s){
    char *p; 
    struct symtab *sp;

    for(sp = symtab; sp < &symtab[NSYMS]; sp++) { 
        /* is it already here? */
        if(sp->name && !strcmp(sp->name, s)){
            return sp;
        }
        /* is it free */ 
        if(!sp->name) {
            sp->name = strdup(s); 
            return sp;
        }
        /* otherwise continue to next */
    }
    yyerror("Too many symbols");
    exit(1);   /* cannot continue */ 
    
} /* symlook */

/* Generate temporary variable */ 
symboltable *gentemp() {
    static int c = 0; /* Temp counter */ 
    char str[10]; /* Temp name */
    /* Generate temp name */ 
    sprintf(str, "t%02d", c++);
    /* Add temporary to symtab */
    return symlook(str); 
}

quad emit_bin(char *result, // Result
char *arg1, // Arg 1
opcodeType operator, // Operator
char *arg2) // Arg 2
{   
    quadPtr++;
    quad qt;
    qt.result = result;
    qt.arg1 = arg1;
    qt.arg2 = arg2;
    qt.op = operator;
    qt.type = BINARY;
    /* Assignment with Binary operator */
    printf("\t%d: %s = %s %c %s\n",quadPtr,result, arg1, 'o', arg2);
return qt;

}

quad emit_un(char *result, char *arg1,opcodeType operator) // Operator
{
/* Assignment with Unary operator */
    quadPtr++;
    quad qt;
    qt.result = result;
    qt.arg1 = arg1;
    qt.op = operator;
    qt.type = UNARY;
    printf("\t%d: %s = %c %s\n",quadPtr, qt.result, 'o', arg1);
    return qt;
}

quad emit_assign(char *result, char *arg1)
{
    /* Simple Assignment */
    quadPtr++;
    quad qt;
    qt.result = result;
    qt.arg1 = arg1;
    qt.type = ASSIGN;
    printf("\t%d: %s = %s\n",quadPtr,result, arg1);
    return qt;
}

/*
void yyerror(char *s) { printf ("error %s\n", s);
}

int main() {
    yyparse(); 
}
*/