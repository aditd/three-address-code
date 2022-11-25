#include "A3_11.tab.c"

void yyerror(char *s) { printf ("error %s\n", s);
}
int main() {
    yyparse(); 
}
