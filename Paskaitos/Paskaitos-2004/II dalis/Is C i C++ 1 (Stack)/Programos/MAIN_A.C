#include "stack_a.h"
int main(void)
{
int i;
struct stack stackas1, stackas2;
char test1[]="modulis";
char test2[]="tipas";

reset(&stackas1);
reset(&stackas2);


for (i=0; test1[i] ; ++i) push(&stackas1, test1[i]);
for (i=0; test2[i] ; ++i) push(&stackas2, test2[i]);
print_push(&stackas1);
print_pop(&stackas1);
print_push(&stackas2);
print_pop(&stackas2);
return 0;
}
