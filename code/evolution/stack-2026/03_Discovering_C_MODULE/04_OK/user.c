#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;

    init();
    while ( (!isFull()) && ('\n' != (c = getchar())) ) push(c);
    while (!isEmpty()) putchar(pop());

    putchar('\n');
    return 0;
}

/*
C:\Repos\Github\ViktorasGolubevas\cpp2026-test\src\evolution\stack-2026\03_header_mistakes\04_ok>gcc stack.c user.c -o use_stack

C:\Repos\Github\ViktorasGolubevas\cpp2026-test\src\evolution\stack-2026\03_header_mistakes\04_ok>use_stack.exe             
123456
54321
*/