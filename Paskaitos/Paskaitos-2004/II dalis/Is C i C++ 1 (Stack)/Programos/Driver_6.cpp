#include <stdio.h>
#include "stack_6.h"

int main(void) {
  int c;
  stack st1, st2;

  while ( '\n' != (c=getchar()) ) st1.push(c);
  while ( '\n' != (c=st1.pop()) )  putchar(c);

  while ( '\n' != (c=getchar()) ) st2.push(c);
  while ( '\n' != (c=st2.pop()) )  putchar(c);

 // st1.stackas[st1.top++] = '!';
 // putchar(st1.pop());
  return 0;
}
