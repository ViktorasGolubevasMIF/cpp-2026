#include "stack_m.h"
int main(void)
{
int i;
char test[]="modulis";
for (i=0; test[i] ; ++i) push(test[i]);
print_push();
print_pop();
return 0;
}
