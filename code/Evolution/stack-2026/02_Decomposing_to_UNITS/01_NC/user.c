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











/*
>gcc 02_n_stack.c 02_nx_user.c -o 02_nx_use_stack

02_nx_user.c: In function 'main':
02_nx_user.c:6:5: warning: implicit declaration of function 'init' [-Wimplicit-function-declaration]
    6 |     init();
      |     ^~~~
02_nx_user.c:7:37: warning: implicit declaration of function 'push' [-Wimplicit-function-declaration]
    7 |     while ('\n' != (c = getchar())) push(c);
      |                                     ^~~~
02_nx_user.c:8:13: warning: implicit declaration of function 'isEmpty' [-Wimplicit-function-declaration]
    8 |     while (!isEmpty()) putchar(pop());
      |             ^~~~~~~
02_nx_user.c:8:32: warning: implicit declaration of function 'pop'; did you mean 'popen'? [-Wimplicit-function-declaration]
    8 |     while (!isEmpty()) putchar(pop());
      |                                ^~~
*/

/*
>clang 02_n_stack.c 02_nx_user.c -o 02_nx_use_stack

02_nx_user.c:6:5: error: call to undeclared function 'init'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
    6 |     init();
      |     ^
02_nx_user.c:7:37: error: call to undeclared function 'push'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
    7 |     while ('\n' != (c = getchar())) push(c);
      |                                     ^
02_nx_user.c:8:13: error: call to undeclared function 'isEmpty'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
    8 |     while (!isEmpty()) putchar(pop());
      |             ^
02_nx_user.c:8:32: error: call to undeclared function 'pop'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
    8 |     while (!isEmpty()) putchar(pop());
      |                                ^
4 errors generated.
*/

/*
C:\Repos\Github\ViktorasGolubevas\desktop-tutorial\src\evolution\stackext>gcc 02_n_stack.c 02_nx_user.c -o 02_nx_use_stack -Werror=implicit-function-declaration
02_nx_user.c: In function 'main':
02_nx_user.c:6:5: error: implicit declaration of function 'init' [-Werror=implicit-function-declaration]
    6 |     init();
      |     ^~~~
02_nx_user.c:7:37: error: implicit declaration of function 'push' [-Werror=implicit-function-declaration]
    7 |     while ('\n' != (c = getchar())) push(c);
      |                                     ^~~~
02_nx_user.c:8:13: error: implicit declaration of function 'isEmpty' [-Werror=implicit-function-declaration]
    8 |     while (!isEmpty()) putchar(pop());
      |             ^~~~~~~
02_nx_user.c:8:32: error: implicit declaration of function 'pop'; did you mean 'popen'? [-Werror=implicit-function-declaration]
    8 |     while (!isEmpty()) putchar(pop());
      |                                ^~~

*/