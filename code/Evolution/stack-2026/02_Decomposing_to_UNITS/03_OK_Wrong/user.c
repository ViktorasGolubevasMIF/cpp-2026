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

/*
C:\Repos\Github\ViktorasGolubevas\cpp2026-test\code\evolution\stack-2026\02_separating_Files\03_OK_Wrong>gcc user.c -o app

C:\Repos\Github\ViktorasGolubevas\cpp2026-test\code\evolution\stack-2026\02_separating_Files\03_OK_Wrong>app.exe
123
321
*/