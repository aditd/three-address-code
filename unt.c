#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_QUADS 100

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

void print_operator(opcodeType op);


int num_quads=0;

// Define the structure for a quad
typedef struct {
  opcodeType op;
  char arg1[32];
  char arg2[32];
  char result[32];
} Quad;

// Declare the array of quads
Quad quads[MAX_QUADS];

// Function to add a new quad to the array
void add_quad(char *result, char *arg1, opcodeType operator, char *arg2) {
  // Set the values for the new quad
  quads[num_quads].op=operator;
  strcpy(quads[num_quads].arg1, arg1);
  strcpy(quads[num_quads].arg2, arg2);
  strcpy(quads[num_quads].result, result);
  // Increment the count of quads
  printf("\t%d: %s = %s ",num_quads,quads[num_quads].result, quads[num_quads].arg1);
  //printf("%s\n",quads[num_quads].arg2);
  print_operator(quads[num_quads].op);
  printf(" %s\n",quads[num_quads].arg2);
  num_quads=num_quads+1;
  
}

int main() {
  // Add some quads to the array
  //("+", "y", "z", "x");
  add_quad("x", "y", MULT,"z");
  printf("quadd ");
  // Print the quads of the array
  //num_quads=num_quads+1;
  printf("%d\n",num_quads);
  for (int i = 0; i < num_quads; i++) {
    //printf("(%s, %s, %s, %s)\n", print_operator(quads[i].op), quads[i].arg1, quads[i].arg2, quads[i].result);
    printf("\t%d: %s = %s ",num_quads,quads[i].result, quads[i].arg1);
    //printf("%s\n",quads[num_quads].arg2);
    print_operator(quads[i].op);
    printf(" %s\n",quads[i].arg2);
  }

  return 0;
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