#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "A4_11_translator.h"

extern void yyerror(char* s);
extern int Parse_main();

// array of symbols, i.e. symboltable
symbol symboltable[NSYMS];
// array of quads
quad* qArray[NSYMS];
// pointer to quad index
int quadPtr = 0;

symbol* symlook(char* name) {
    symbol* sp;
    for (sp = symboltable; sp < &symboltable[NSYMS]; sp++) {
        // current row has a symbol name and it is same as name
        if (sp->name && !strcmp(sp->name, name)) {
            return sp; // return pointer to symbol
        }
        // current row does not have a symbol name
        if (!sp->name) {
            sp->name = strdup(name); // add symbol to current row
            return sp; // return pointer to symbol
        }
        // continue to next row
    }
    // finished iterating through entire table, new symbol can't be added as no space
    yyerror("Too many symbols");
    exit(1);
}

symbol* gentemp() {
    static int c = 0; // counter for temporary variables
    char str[10]; // store name of temporary variable
    sprintf(str, "t%02d", c++); // create name of temporary variable
    return symlook(str); // add it to symbol table
}

void print_quad(quad* q) {
    if ((q->op >= PLUS) && (q->op <= AND)) { // binary operation
        printf("[%d] %s = %s ", q->index, q->result, q->arg1);
        switch (q->op) {
            case PLUS:
                printf("+");
                break;
            case MINUS:
                printf("-");
                break;
            case MULT:
                printf("*");
                break;
            case MOD:
                printf("%%");
                break;
            case DIV:
                printf("/");
                break;
            case LT:
                printf("<");
                break;
            case GT:
                printf(">");
                break;
            case LTE:
                printf("<=");
                break;
            case GTE:
                printf(">=");
                break;
            case OR:
                printf("||");
                break;
            case AND:
                printf("&&");
                break;
            default:
                break;
        }
        printf(" %s\n", q->arg2);
    } else if (q->op == JUMP) {
        printf("[%d] goto %s\n", q->index, q->result);
    } else if (q->op >= JLT && q->op <= JGTE) {
        printf("[%d] if %s ", q->index, q->arg1);
        switch (q->op) {
            case JLT:
                printf("<");
                break;
            case JGT:
                printf(">");
                break;
            case JLTE:
                printf("<=");
                break;
            case JGTE:
                printf(">=");
                break;
            default:
                break;
        }
        printf(" %s goto %s\n", q->arg2, q->result);
    } else if (q->op == COPY) {
        printf("[%d] %s = %s\n", q->index, q->result, q->arg1);
    } else {
        printf("[%d] %s = - %s\n", q->index, q->result, q->arg1);
    }
}

void print_quads() {
    for (int i = 0; i < quadPtr; i++) {
        print_quad(qArray[i]);
    }
}

void new_quad_binary(opType op, char* result, char* arg1, char* arg2) {
    quad* q = (quad*)malloc(sizeof(quad)); // create new quad
    // assign quad attributes
    q->op = op;
    q->result = strdup(result);
    q->arg1 = strdup(arg1);
    q->arg2 = strdup(arg2);
    q->index = quadPtr;
    qArray[quadPtr++] = q; // add quad to quad array and then increment index
}

void new_quad_unary(opType op1, char* s1, char* s2) {
    quad* q = (quad*)malloc(sizeof(quad)); // create new quad
    // assign quad attributes
    q->op = op1;
    q->result = strdup(s1);
    q->arg1 = strdup(s2);
    q->arg2 = NULL;
    q->index = quadPtr;
    qArray[quadPtr++] = q; // add quad to quad array and then increment index
}

void new_quad_conditional_jump(opType op, char* left, char* right, char* go) {
    quad* q = (quad*)malloc(sizeof(quad)); // create new quad
    // assign quad attributes
    q->op = op;
    q->result = strdup(go);
    q->arg1 = strdup(left);
    q->arg2 = strdup(right);
    q->index = quadPtr;
    qArray[quadPtr++] = q; // add quad to quad array and then increment index
}

void new_quad_jump(char* go) {
    quad* q = (quad*)malloc(sizeof(quad)); // create new quad
    // assign quad attributes
    q->op = JUMP;
    q->result = strdup(go);
    q->arg1 = NULL;
    q->arg2 = NULL;
    q->index = quadPtr;
    qArray[quadPtr++] = q; // add quad to quad array and then increment index
}

node* create(int value) {
    node* n = (node*)malloc(sizeof(node)); // create new node
    n->val = value; // assign value
    n->next = NULL; // set next to null
    return n;
}

list* makelist(int i) {
    list* l = (list*)malloc(sizeof(list)); // create new list
    node* n = create(i);
    l->head = n;
    l->tail = n;
    return l;
}

list* merge_lists(list* l1, list* l2) {
    if (l1 == NULL) {
        return l2;
    }
    if (l2 == NULL) {
        return l1;
    }
    l1->tail->next = l2->head;
    l1->tail = l2->tail;
    free(l2);
    return l1;
}

void print_list(list* l) {
    node* temp = l->head;
    while (temp != l->tail) { // run until temp reaches last node
        printf("%d->", temp->val);
        temp = temp->next;
    }
    printf("%d\n", temp->val);
}

void backpatch(list* l, int m) {
    printf("bacpack\n");
    node* temp = l->head;
    // only one node
    if (l->head == l->tail) {
        int quadno = temp->val;
        printf("the quad number to be changed is %d\n", quadno);
        printf("current jump is %s\n", qArray[quadno]->result);
        //printf("\t%d: if %s OPERATOR %d %s goto  %s (backpatched)\n",quadno, qArray[quadno]->arg1, qArray[quadno]->op, qArray[quadno]->arg2, qArray[quadno]->result);
    } else {
        while (temp != l->tail) {
            //printf("%d->",temp->val);
            int quadno = temp->val;
            // int m to temp char*
            sprintf(qArray[quadno]->result, "%d", m);
            temp = temp->next;
            printf("\t%d: if %s OPERATOR %d %s goto  %s (backpatched)\n", quadno, qArray[quadno]->arg1, qArray[quadno]->op, qArray[quadno]->arg2, qArray[quadno]->result);
        }
    }
}

int main(int argc, char const* argv[]) {
    return Parse_main();
}
