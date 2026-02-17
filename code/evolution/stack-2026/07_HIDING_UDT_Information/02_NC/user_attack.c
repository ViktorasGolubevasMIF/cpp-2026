#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;

    struct Stack *pst1, *pst2; // !

    printf("Input 1st Stack:\n");
    init(pst1);
    while ( (!isFull(pst1)) && ('\n' != (c = getchar())) ) push(pst1,c);
    while (!isEmpty(pst1)) putchar(pop(pst1));
    putchar('\n');
    printf("1st Stack is empty\n\n");

    printf("Input 2nd Stack:\n");
    init(pst2);
    while ( (!isFull(pst2)) && ('\n' != (c = getchar())) ) push(pst2,c);
    //// ATTACK!
    pst2->stack[pst2->top++] = '!';
    ////
    while (!isEmpty(pst2)) putchar(pop(pst2));
    putchar('\n');
    printf("2nd Stack is empty\n\n");   

    return 0;
}

/*
gcc -c user_attack.c

user_attack.c: In function 'main':
user_attack.c:20:9: error: invalid use of undefined type 'struct Stack'
   20 |     pst2->stack[pst2->top++] = '!';
      |         ^~
user_attack.c:20:21: error: invalid use of undefined type 'struct Stack'
   20 |     pst2->stack[pst2->top++] = '!';
      |                     ^~
*/

/*
clang -c user_attack.c
user_attack.c:20:9: error: incomplete definition of type 'struct Stack'
   20 |     pst2->stack[pst2->top++] = '!';
      |     ~~~~^
./stack.h:3:8: note: forward declaration of 'struct Stack'
    3 | struct Stack;
      |        ^
user_attack.c:20:21: error: incomplete definition of type 'struct Stack'
   20 |     pst2->stack[pst2->top++] = '!';
      |                 ~~~~^
./stack.h:3:8: note: forward declaration of 'struct Stack'
    3 | struct Stack;
      |        ^
2 errors generated.
*/