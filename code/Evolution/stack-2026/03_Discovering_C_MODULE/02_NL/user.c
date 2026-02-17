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

