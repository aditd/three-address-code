%{
    /* C Declarations and Definitions */
    # include "A3_11.tab.h"
    # include <stdio.h>
    #include "A3_11.tab.h"
    #include "parser.h"
%}


/* Regular Expression Definitions */
/* HELPERS */
/* all letters */
BACKSLASH  [\][^\abfnrtv\'\"]
LETTERS    [a-zA-Z]
/* all digits */
DIGITS     [0-9]
/* \ followed by escape character letters */
ESCSEQ     [\][abfnrtv\'\"]

SLCOMMENT  "//".*[\n]
MLCOMMENT  "/*"(.|[\n])*"*/"
/* SLCOMMENT or MLCOMMENT or blank space, tab, carriage return, or newline */
BLANK      {SLCOMMENT}|{MLCOMMENT}
/* 1 or 0 + or -, followed by 1 1-9, followed by 0 or more DIGITS */
SNOPT      [\+\-]?[1-9]{DIGITS}*
/* 0 or more of anything but a \n, ', or \, surrounded by ' */
CHARCONST  [']({ESCSEQ}|[^\n'{BACKSLASH}])*[']

PUNC       "["|"]"|"("|")"|"{"|"}"|"&"|"*"|"/"|"%"|"!"|"?"|"<"|">"|"="|":"|";"|","|"+"|"-"
/* TOKENS */
CHAR "char"
ELSE "else"
FOR "for"
IF "if"
INT "int"
RETURN "return"
VOID "void"
TYPEDEF "typedef"
/* 1 LETTERS or _, followed by 0 or more LETTERS, DIGITS, or _ */
IDENTIFIER ({LETTERS}|[\_])({LETTERS}|{DIGITS}|[\_])*
/* 0, SNOPT, or CHARCONST */
CONST      [0]|{SNOPT}|{CHARCONST}
/* 0 or more of ESCSEQ or anything but a \n or \ or ", surrounded by " */
STRLIT     [\"]({ESCSEQ}|[^\n\"{BACKSLASH}])*[\"]
/* IMPORTANT */
/* STRLIT if defined as neccesary termination by \0: [\"]({ESCSEQ}|[^\\n\\\""])*"\0"[\"]  */
WS  [ \t\v\n\f\r]
/* Definitions of Rules & Actions */
%%
{VOID}                  {return VOID;}
{ELSE}                  {return ELSE;}
{FOR}                   {return FOR;}
{IF}                    {return IF;}
{INT}                   {return INT;}
{CHAR}                  {return CHAR;}
{RETURN}                {return RETURN;}
{TYPEDEF}               {return TYPEDEF;}

{SNOPT}                 {yylval.symp = symlook(yytext); return I_CONSTANT;}
{CHARCONST}             {return C_CONSTANT; } // print constant
{STRLIT}                {return STRING_LIT; } // print string literal //{PUNC}       { printf("<PUNCTUATOR, %s>\n", yytext);     } // print punctuators

{IDENTIFIER}            {yylval.symp = symlook(yytext);return IDENTIFIER;     } // print identifier
{PUNC}                  {return yytext[0]; }
"&&"                    { return AND_OP; }
"||"					{ return OR_OP; }
"<="					{ return LE_OP; }
">="					{return GE_OP; }
"=="					{ return EQ_OP; }
"!="					{ return NE_OP; }
"->"                    { return PTR_OP;}
{BLANK}                 {} // ignore comments and whitespace
{WS}+					{/* whitespace separates tokens */ }
.					    {/* discard bad characters */ }
%%



