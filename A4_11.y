%{
    /* C Declarations and Definitions */
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include "A4_11_translator.h"

    extern int yylex(); // Generated by Flex
    void yyerror(char *s);
%}

%union { // Placeholder for a value
	int intval; // integer value
	symbol *sym; // pointer to a symbol
}