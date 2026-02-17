#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;

    struct Stack st1, st2;

    printf("Input 1st Stack:\n");
    init(&st1);
    while ( (!isFull(&st1)) && ('\n' != (c = getchar())) ) push(&st1,c);
    while (!isEmpty(&st1)) putchar(pop(&st1));
    putchar('\n');
    printf("1st Stack is empty\n\n");

    printf("Input 2nd Stack:\n");
    init(&st2);
    while ( (!isFull(&st2)) && ('\n' != (c = getchar())) ) push(&st2,c);
    //// ATTACK!
    st2.stack[st2.top++] = '!';
    ////
    while (!isEmpty(&st2)) putchar(pop(&st2));
    putchar('\n');
    printf("2nd Stack is empty\n\n");   

    return 0;
}

/*
C:\Repos\Github\ViktorasGolubevas\cpp2026-test\code\evolution\stack-2026\07_HIDING_UDT_Information\01_NC>gcc stack.c user_attack.c -o app
user_attack.c: In function 'main':
user_attack.c:7:18: error: storage size of 'st1' isn't known
    7 |     struct Stack st1, st2;
      |                  ^~~
user_attack.c:7:23: error: storage size of 'st2' isn't known
    7 |     struct Stack st1, st2;
      |                       ^~~
*/

//// !!!!
/*
C:\Repos\Github\ViktorasGolubevas\cpp2026-test\code\evolution\stack-2026\07_HIDING_UDT_Information\01_NC>clang stack.c user_attack.c -o app
user_attack.c:7:18: error: variable has incomplete type 'struct Stack'
    7 |     struct Stack st1, st2;
      |                  ^
./stack.h:3:8: note: forward declaration of 'struct Stack'
    3 | struct Stack;
      |        ^
user_attack.c:7:23: error: variable has incomplete type 'struct Stack'
    7 |     struct Stack st1, st2;
      |                       ^
./stack.h:3:8: note: forward declaration of 'struct Stack'
    3 | struct Stack;
      |        ^
2 errors generated.
*/