#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;

    struct Stack *pst1=NULL, *pst2=NULL; // !

    pst1 = create(); // !
    pst2 = create(); // !

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

    destroy(pst1); // !
    destroy(pst2); // !

    return 0;
}


/*
gcc 077_stack.c 077_user.c -o 077_use_stack
*/

/*
C:\Repos\Github\ViktorasGolubevas\desktop-tutorial\src\evolution\stackext>077_use_stack.exe
Ivesk st1
1234567
54321
st1 pabaiga
*/