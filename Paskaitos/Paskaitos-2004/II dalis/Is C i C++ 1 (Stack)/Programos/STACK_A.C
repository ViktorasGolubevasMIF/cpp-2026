#include <stdio.h>
#include "stack_a.h"
void reset(struct stack *id)
{
id->top = 0;
}
void push(struct stack *id, char c)
{
/*
  if (top < stack_size) stack[top++] = c;
*/

  if (id->top < STACK_SIZE) id->stack[id->top++] = c;

}
char pop(struct stack *id)
{
  if (id->top > 0) return id->stack[--id->top];
  else return '\0';
}

void print_pop(struct stack *id)
{
int i;
printf("pop  %d > ",id->top);
for (i=--id->top; i>=0 ;--i)
    putchar(id->stack[i]);
putchar('\n');
}

void print_push(struct stack *id)
{
int i;
printf("push %d > ",id->top);
for (i=0; i<id->top ;++i)
    putchar(id->stack[i]);
putchar('\n');
}