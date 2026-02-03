#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main(void) {
    char c;

    struct Stack *pst1, *pst2; 

    pst1 = calloc(sizeof(struct Stack), 1); // !
    pst2 = (struct Stack *) calloc(sizeof(struct Stack), 1); // !

    printf("Input 1st Stack:\n");
    init(pst1);
    while ( (!isFull(pst1)) && ('\n' != (c = getchar())) ) push(pst1,c);
    while (!isEmpty(pst1)) putchar(pop(pst1));
    putchar('\n');
    printf("1st Stack is empty\n\n");

    printf("Input 2nd Stack:\n");
    init(pst2);
    while ( (!isFull(pst2)) && ('\n' != (c = getchar())) ) push(pst2,c);
    while (!isEmpty(pst2)) putchar(pop(pst2));
    putchar('\n');
    printf("2nd Stack is empty\n\n");   

    return 0;
}


/*
C:\Repos\Github\ViktorasGolubevas\cpp2026-test\code\evolution\stack-2026\08_Acquiring_RESOURCES\01_NC>gcc stack.c user.c
 -o app
user.c: In function 'main':
user.c:10:26: error: invalid application of 'sizeof' to incomplete type 'struct Stack'
   10 |     pst1 = calloc(sizeof(struct Stack), 1); // !
      |                          ^~~~~~
user.c:11:43: error: invalid application of 'sizeof' to incomplete type 'struct Stack'
   11 |     pst2 = (struct Stack *) calloc(sizeof(struct Stack), 1); // !
      |                                           ^~~~~~

C:\Repos\Github\ViktorasGolubevas\cpp2026-test\code\evolution\stack-2026\08_Acquiring_RESOURCES\01_NC>clang stack.c user.c -o app
user.c:10:19: error: invalid application of 'sizeof' to an incomplete type 'struct Stack'
   10 |     pst1 = calloc(sizeof(struct Stack), 1); // !
      |                   ^     ~~~~~~~~~~~~~~
./stack.h:3:8: note: forward declaration of 'struct Stack'
    3 | struct Stack;
      |        ^
user.c:11:36: error: invalid application of 'sizeof' to an incomplete type 'struct Stack'
   11 |     pst2 = (struct Stack *) calloc(sizeof(struct Stack), 1); // !
      |                                    ^     ~~~~~~~~~~~~~~
./stack.h:3:8: note: forward declaration of 'struct Stack'
    3 | struct Stack;
      |        ^
2 errors generated.
*/