#include "A3_11.tab.c"
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"


int quadPtr = 0;
// array of symbols
symboltable symtab[NSYMS];
// array of pointers to structure of objects
quad *qArray[NSYMS];
// return a symbol
/*
 THE SYMBOL TABLE
 ============================================
*/
struct symtab * symlook(char *s){
    char *p; 
    struct symtab *sp;
    // go through the symbol table array as long as the memory address of the current symbol table
    // is less than the memory address of the last symbol in the table
    for(sp = symtab; sp < &symtab[NSYMS]; sp++) { 
        /* is it already here? */
        if(sp->name && !strcmp(sp->name, s)){
            return sp;
        }
        /* is it free */
        // if there is available space then dump the name of the variable 
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



void emit_bin(char *result, char *arg1, opcodeType operator, // Operator
char *arg2) // Arg 2
{   
    quad *qt = (quad*)malloc(sizeof(quad));
    qt->result = strdup(result);
    qt->arg1 = strdup(arg1);
    qt->arg2 = strdup(arg2);
    qt->op = operator;
    qArray[quadPtr] = qt;
    /* Assignment with Binary operator */
    printf("[%d] %s = %s ", quadPtr, qt->result, qt->arg1);
    print_operator(qt->op);
    printf(" %s\n", qt->arg2);
    quadPtr++;
    //printf("\t%d: %s = %s %s %s\n",quadPtr,qArray[quadPtr]->result, qArray[quadPtr]->arg1, print_operator(qArray[quadPtr]->op), qArray[quadPtr]->arg2);
    //return qt;

}

void emit_jump(char *go) // Operator
{
    //quadPtr++;
    quad *qt = (quad*)malloc(sizeof(quad));
    qt->result = strdup(go);
    qt->op = JUMP;
    qArray[quadPtr] = qt;
    printf("\t%d: goto  %s\n", quadPtr, qArray[quadPtr]->result);
    quadPtr++;
}

void emit_jump_cond(char *left, opcodeType operator,char *right, char *go) // Operator
{
/* Assignment with Unary operator */
    //quadPtr++;
    quad *qt = (quad*)malloc(sizeof(quad));
    qt->arg1 = strdup(left);
    qt->arg2 = strdup(right);
    qt->op = operator;
    qt->result = strdup(go);
    qArray[quadPtr] = qt;
    printf("[%d] if %s ", quadPtr, qt->arg1);
    print_operator(qArray[quadPtr]->op);
    printf(" %s goto %s\n", qt->arg2, qt->result);
    //printf("\t%d: if %s %s %s goto  %s\n",quadPtr, qArray[quadPtr]->arg1, print_operator(qArray[quadPtr]->op), qArray[quadPtr]->arg2, qArray[quadPtr]->result);
    //return qt;
    quadPtr++;
    
}

void emit_un(char *result, char *arg1, opcodeType operator) // Operator
{
/* Assignment with Unary operator */
    quad *qt = (quad*)malloc(sizeof(quad));
    qt->result = strdup(result);
    qt->arg1 = strdup(arg1);
    qt->op = strdup(operator);
    qArray[quadPtr] = qt;
    printf("[%d] %s = %s\n", quadPtr, qt->result, qt->arg1);
    //printf("\t%d: %s = %s %s\n",quadPtr, qArray[quadPtr]->result, print_operator(qArray[quadPtr]->op), qArray[quadPtr]->arg1);
    
    //return qt;
    quadPtr++;
}



void emit_assign(char *result, char *arg1)
{
    /* Simple Assignment */
    quad *qt = (quad*)malloc(sizeof(quad));
    qt->result = strdup(result);
    qt->arg1 = strdup(arg1);
    qt->op = COPY;


    qArray[quadPtr] = qt;
    //quadPtr++;
    printf("\t%d: %s = %s\n",quadPtr,qArray[quadPtr]->result, qArray[quadPtr]->arg1);
    //printf("\t%d: %s \n",0,qArray[0]->result);
    //return qt;
    quadPtr++;
}
/*
 BOOLEAN FUNCTION
*/




// i is the current quad instruction
list* makelist(int i){
    // we are given the quad number and we need to create boolean struct
    //int arr[10];
    list *li= malloc(sizeof(list));
    node *gen = create(i);
    // gen is the address of the node
    li->head = gen;
    li->tail = gen;
    return li;
}

list * merge_lists(list *l1, list *l2) {
    list *li = malloc(sizeof(list));
    li->head = l1->head;
    l1->tail = l2->head;
    li->tail = l2->tail;
    free(l1);
    free(l2);

    return li;
}

node* create(int value) {
    node *genesis = malloc(sizeof(node));
    genesis->val= value;
    genesis->next = NULL;
    return genesis;
}

void backpatch(list * li, int m){
    printf("bacpack\n");
    node *temp = li->head;
    // only one node
    if (li->head==li->tail){
        int quadno = temp->val;
        printf("the quad number to be changed is %d\n",quadno);
        printf("current jump is %s\n",qArray[quadno]->result);
        //printf("\t%d: if %s %s %s goto  %s (backpatched)\n",quadno, qArray[quadno]->arg1, print_operator(qArray[quadno]->op), qArray[quadno]->arg2, qArray[quadno]->result);
    } else {
        while(temp!=li->tail){
            //printf("%d->",temp->val);
            int quadno = temp->val;
            qArray[quadno]->result = atoi(m);
            temp = temp->next;
            //printf("\t%d: if %s %s %s goto  %s (backpatched)\n",quadno, qArray[quadno]->arg1, print_operator(qArray[quadno]->op), qArray[quadno]->arg2, qArray[quadno]->result);
        }
    }
}

void print_list(list * li){
    printf("printing list\n");
    node *temp = li->head;
    //printf("head %d->",temp->val);
    while(temp!=li->tail){
        printf("%d->",temp->val);
        temp = temp->next;
    }
    printf("%d\nlist has been printed\n",temp->val);
}

void print_operator(opcodeType op) {
    switch (op) {
        case PLUS:
            printf("+");
            break;
        case MINUS:
            //printf("-");
            printf("-");
            break;
        case MULT:
            //printf("*");
            printf("*");
            break;
        case DIV:
            //printf("/");
            printf("/");
            break;
        case MOD:
            //printf("%");
            printf("%");
            break;
        case LE:
            //printf("%");
            printf("<=");
            break;
        case GE:
            //printf("%");
            printf(">=");
            break;
        case OR:
            //printf("%");
            printf("||");
            break;
        case AND:
            //printf("%");
            printf("&&");
            break;
        case LESS:
            //printf("%");
            printf("<");
            break;     
        case MORE:
            //printf("%");
            printf(">");
            break;
        default:
            break;
    }
}


// you will need to handle unary operators
void print_quadtbl() {
    printf("the quadptr is %d\n",quadPtr);
    /* printf("\t[%d]: %s",0,qArray[0]->result);
    printf("\t[%d]: %s",1,qArray[1]->result);
    printf("\t[%d]: %s",2,qArray[2]->result); */
    /* for(int i=0;i<quadPtr;i++){
        printf("\t[%d]: %s\n",i,qArray[i]->result);
    } */
    
     for(int i=0;i<quadPtr;i++){
        quad *qt = qArray[i];
        if (qt->op <= DIV) {
            printf("\t[%d] %s = %s ", i, qt->result, qt->arg1);
            print_operator(qt->op);
            printf(" %s\n", qt->arg2);
        } else if (qt->op <=GE) {
            printf("\t[%d] if %s ", i, qt->arg1);
            print_operator(qt->op);
            printf(" %s goto %s\n", qt->arg2, qt->result);
        } else if (qt->op == JUMP) {
            printf("\t[%d] goto  %s\n", i, qt->result);
        } else if (qt->op == COPY) {
            printf("\t[%d] %s = %s\n",i,qt->result, qt->arg1);
        } else {
            printf("\n");
        }
    } 

    printf("done\n");
}

void yyerror(char *s) { printf ("error %s\n", s);
}

int main() {
    //print_quadtbl();
    return yyparse(); 
}
