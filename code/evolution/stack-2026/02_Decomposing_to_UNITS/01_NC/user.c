#include <stdio.h>

int main(void) {
    char c;

    init();
    while ('\n' != (c = getchar())) push(c);
    while (!isEmpty()) putchar(pop());
    putchar('\n');
    return 0;
}

// gcc -c stack.c
// gcc -c user.c
// gcc stack.o user.o -o app