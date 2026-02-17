#include <stdio.h>
#include "stack.c"

int main(void) {
    char c;

    init();

    /* Patobuliname: Tik jei stekas neužpildytas, skaitome simbolius iki '\n' */
    while ( (!isFull()) && ('\n' != (c = getchar())) ) push(c);

    while (!isEmpty()) putchar(pop());

    putchar('\n');
    return 0;
}
