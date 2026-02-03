#include <stdio.h>
#include "stack.h"

extern char stack[]; // +
extern int top;      // +

int main(void) {
    char c;

    init();
    while ( (!isFull()) && ('\n' != (c = getchar())) ) push(c);

    stack[top++] = '!'; // attack non-hiiden data

    while (!isEmpty()) putchar(pop());
    putchar('\n');

    return 0;
}

/*
C:\Repos\Github\ViktorasGolubevas\cpp2026-test\src\evolution\stack-2026\03_header_mistakes\05_ok_z>gcc stack.c user.c -o use_stack

C:\Repos\Github\ViktorasGolubevas\cpp2026-test\src\evolution\stack-2026\03_header_mistakes\05_ok_z>use_stack.exe
123456
!54321
*/