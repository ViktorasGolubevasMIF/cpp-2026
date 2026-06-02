#include <stdio.h>
#include "stack_7.h"

stack::stack (int sz) {stackas = new char[size=sz]; reset();}
stack::~stack () {delete [] stackas;}
void stack::reset() { top=0; }
int stack::get_size () {return size;}
int stack::is_empty () {return (0 == top);}
int  stack::is_full () {return (size == top);}

void stack::push(char c) {
  if (!is_full()) stackas[top++] = c;
  else  printf("Stekas pilnas\n");
}
char stack::pop() {
  if (!is_empty()) return stackas[--top];
  printf("\nStekas tuscias\n");
  return '\n';
}