#include <stdio.h>
#include "stack_5.h"

int main(void) {
  int c;
  stack st1, st2;

  st1.reset(); 
  while ( '\n' != (c=getchar()) ) st1.push(c);
  while ( '\n' != (c=st1.pop()) )  putchar(c);

  st2.reset(); 
  while ( '\n' != (c=getchar()) ) st2.push(c);
  while ( '\n' != (c=st2.pop()) )  putchar(c);

  st1.stackas[st1.top++] = '!';
  putchar(st1.pop());
  return 0;
}
