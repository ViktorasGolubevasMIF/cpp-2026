#include <stdio.h>
#include "stack_m.h"
/*
static char stack[stack_size];
*/

static char stack[STACK_SIZE];

static int  top = 0;
void push(char c)
{
/*
  if (top < stack_size) stack[top++] = c;
*/

  if (top < STACK_SIZE) stack[top++] = c;

}
char pop(void)
{
  if (top > 0) return stack[--top];
  else return '\0';
}

void print_pop(void)
{
int i;
printf("pop  %d > ",top);
for (i=--top; i>=0 ;--i)
    putchar(stack[i]);
putchar('\n');
}

void print_push(void)
{
int i;
printf("push %d > ",top);
for (i=0; i<top ;++i)
    putchar(stack[i]);
putchar('\n');
}