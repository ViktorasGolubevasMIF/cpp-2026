#include <iostream.h>
#include "stack_7.h"

int main(void) {
  int c;
  stack st1(2000), st2(2001);

  cout << st1.get_size();
  while ( '\n' != (c=getchar()) ) st1.push(c);
  while ( '\n' != (c=st1.pop()) )  putchar(c);

  while ( '\n' != (c=getchar()) ) st2.push(c);
  while ( '\n' != (c=st2.pop()) )  putchar(c);

  stack st;
  return 0;
}
