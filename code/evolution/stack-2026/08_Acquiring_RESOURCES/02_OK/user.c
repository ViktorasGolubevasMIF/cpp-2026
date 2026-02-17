#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;

    struct Stack *pst1, *pst2;

    pst1 = create();
    pst2 = create();

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
C:\Repos\Github\ViktorasGolubevas\cpp2026-test\code\evolution\stack-2026\08_Acquiring_RESOURCES\02_OK>gcc stack.c user.c -o app

C:\Repos\Github\ViktorasGolubevas\cpp2026-test\code\evolution\stack-2026\08_Acquiring_RESOURCES\02_OK>app.exe
Input 1st Stack:
1234
4321
1st Stack is empty

Input 2nd Stack:
4567
7654
2nd Stack is empty
*/