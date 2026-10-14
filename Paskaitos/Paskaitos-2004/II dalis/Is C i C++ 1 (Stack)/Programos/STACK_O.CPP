#include <stdio.h>
#include "stack_o.h"

void stack::reset(void)
{
top = 0;
}

void stack::push(char c)
{
/*
  if (top < stack_size) stack[top++] = c;
*/
  if (top < STACK_SIZE) stack[top++] = c;
}

char stack::pop(void)
{
  if (top > 0) return stack[--top];
  else return '\0';
}

void stack::print_pop(void)
{
int i;
printf("pop  %d > ",top);
for (i=--top; i>=0 ;--i)
    putchar(stack[i]);
putchar('\n');
}

void stack::print_push(void)
{
int i;
printf("push %d > ",top);
for (i=0; i<top ;++i)
    putchar(stack[i]);
putchar('\n');
}
