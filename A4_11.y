%{
    /* C Declarations and Definitions */
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include "A4_11_translator.h"

    void yyerror(char *s);
    int Parse_main();

    extern quad* qArray[NSYMS];
    extern int quadPtr;
    extern int yylex();
    // extern void print_quad(quad* q);
%}

%union { // Placeholder for a value
	int intval; // integer value
	symbol *sym; // pointer to a symbol
}
%type <sym> expression

%token <sym> IDENTIFIER
%token <sym> I_CONSTANT
%token C_CONSTANT
%token STRING_LIT
%token INT RETURN VOID TYPEDEF
%token CHAR ELSE FOR IF
%token AND_OP
%token PTR_OP
%token OR_OP
%token LE_OP
%token GE_OP
%token EQ_OP
%token NE_OP

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%%
/* Grammar Rules */
// simple arithmetic with tac
list_of_statements: statement
;

list_of_statements: list_of_statements statement
;

statement: IDENTIFIER '=' expression ';'
                                    {
                                        new_quad_unary(COPY, $1->name, $3->name);
                                    }
;

expression: expression '+' expression
                                    {
                                        $$ = gentemp();
                                        new_quad_binary(PLUS, $$->name, $1->name, $3->name);
                                    }
            | expression '-' expression
                                    {
                                        $$ = gentemp();
                                        new_quad_binary(MINUS, $$->name, $1->name, $3->name);
                                    }
            | expression '*' expression
                                    {
                                        $$ = gentemp();
                                        new_quad_binary(MULT, $$->name, $1->name, $3->name);
                                    }
            | expression '/' expression
                                    {
                                        $$ = gentemp();
                                        new_quad_binary(DIV, $$->name, $1->name, $3->name);
                                    }
            | '(' expression ')'    { $$ = $2; }
            | '-' expression %prec UMINUS
                                    {
                                        $$ = gentemp();
                                        new_quad_unary(UNARYMINUS, $$->name, $2->name);
                                    }
            | I_CONSTANT            {
                                        $$ = gentemp();
                                        char num_s[10];
                                        sprintf(num_s, "%d", $1);
                                        new_quad_unary(COPY, $$->name, num_s);
                                    }
            | IDENTIFIER            { $$ = $1; }
;
%%
/* C Code */
void yyerror(char *s){
    fprintf(stderr, "Error: %s", s);
}

int Parse_main(){
    // for(int i = 0; i < quadPtr; i++){
    //     print_quad(qArray[i]);
    // }

    return yyparse();
}
