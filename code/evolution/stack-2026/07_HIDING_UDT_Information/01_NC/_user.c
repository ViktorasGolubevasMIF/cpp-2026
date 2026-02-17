#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;

    struct Stack st1, st2;

    printf("Ivesk st1\n");
    init(&st1);
    while ( (!isFull(&st1)) && ('\n' != (c = getchar())) ) push(&st1,c);
    while (!isEmpty(&st1)) putchar(pop(&st1));
    putchar('\n');
    printf("st1 pabaiga\n");

    printf("Ivesk st2\n");
    init(&st2);
    while ( (!isFull(&st2)) && ('\n' != (c = getchar())) ) push(&st2,c);
    while (!isEmpty(&st2)) putchar(pop(&st2));
    putchar('\n');
    printf("st2 pabaiga\n");

    return 0;
}


/*
C:\Repos\Github\ViktorasGolubevas\desktop-tutorial\src\evolution\stackext>gcc 07_n_stack.c 07_nx_user.c -o 07_nx_use_stack
07_nx_user.c: In function 'main':
07_nx_user.c:7:18: error: storage size of 'st1' isn't known
    7 |     struct Stack st1, st2;
      |                  ^~~
07_nx_user.c:7:23: error: storage size of 'st2' isn't known
    7 |     struct Stack st1, st2;
      |                       ^~~
*/

/*
C:\Repos\Github\ViktorasGolubevas\desktop-tutorial\src\evolution\stackext>clang 07_n_stack.c 07_nx_user.c -o 07_nx_use_stack
07_nx_user.c:7:18: error: variable has incomplete type 'struct Stack'
    7 |     struct Stack st1, st2;
      |                  ^
./07_stack.h:3:8: note: forward declaration of 'struct Stack'
    3 | struct Stack;
      |        ^
07_nx_user.c:7:23: error: variable has incomplete type 'struct Stack'
    7 |     struct Stack st1, st2;
      |                       ^
./07_stack.h:3:8: note: forward declaration of 'struct Stack'
    3 | struct Stack;
      |        ^
2 errors generated.
*/