#include <stdio.h>
#include "stack_6.h"

stack::stack () {top=0;}
int stack::is_empty () {return (0 == top);}
int  stack::is_full () {return (STACK_SIZE == top);}

void stack::push(char c) {
  if (!is_full()) stackas[top++] = c;
  else  printf("Stekas pilnas\n");
}
char stack::pop() {
  if (!is_empty()) return stackas[--top];
  printf("\nStekas tuscias\n");
  return '\n';
}