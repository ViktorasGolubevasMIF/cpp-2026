#include <stdio.h>
#include "stack_o.h"
int main(void)
{
int i;
char test[]="modulis";
stack stackas1;

stackas1.reset();
for (i=0; test[i] ; ++i) stackas1.push(test[i]);
stackas1.print_push();
stackas1.print_pop();

return 0;
}