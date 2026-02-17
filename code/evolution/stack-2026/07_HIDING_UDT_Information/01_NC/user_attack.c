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
C:\Repos\Github\ViktorasGolubevas\cpp2026-test\code\evolution\stack-2026\06_Encapsulating_UDT_in_MODULE\02_OK_Attack>gcc stack.c user_attack.c -o app

C:\Repos\Github\ViktorasGolubevas\cpp2026-test\code\evolution\stack-2026\06_Encapsulating_UDT_in_MODULE\02_OK_Attack>app.exe
Input 1st Stack:
1234
4321
1st Stack is empty

Input 2nd Stack:
4567
!7654
2nd Stack is empty
*/