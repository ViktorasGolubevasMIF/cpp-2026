#include <stdio.h>

// ===> stack dalis
#define SIZE 5

struct Stack{
  static char stack[SIZE];
  static int top;
};

static void reset(struct Stack st) {
    st.top = 0;
}

void init(struct Stack st) {
    st.top = 0;
}

int isEmpty(struct Stack st) {
    return 0 == st.top;
}

int isFull(struct Stack st) {
    return SIZE == st.top;
}

void push(struct Stack st, char c) {
    if (!isFull(st)) {
        st.stack[st.top++] = c;
    }
}

char pop(struct Stack st) {
    if (!isEmpty(st)) {
        return st.stack[--st.top];
    }
    return '\0';
}

// ===> user dalis
int main(void) {
    char c;

    struct Stack st1, st2;

    init(st1);

    while ( (!isFull(st1)) && ('\n' != (c = getchar())) ) push(st1,c);

    while (!isEmpty(st1)) putchar(pop(st1));

    putchar('\n');

    return 0;
}

/*
gcc 05_nx_usestack.c -o 05_nx_usestack
*/

/*
05_nx_usestack.c:7:3: error: expected specifier-qualifier-list before 'static'
    7 |   static char stack[SIZE];
      |   ^~~~~~
*/

/*
"expected specifier-qualifier-list" yra sintaksinės analizės klaida, kai kompiliatorius netikėtai sutinka static struktūros nario deklaracijoje.

C kalba draudžia bet kokius storage-class specifikatorius (static, extern, auto, register) struktūros nariams.
*/

/*
clang 05_nx_usestack.c -o 05_nx_usestack
05_nx_usestack.c:7:3: error: type name does not allow storage class to be specified
    7 |   static char stack[SIZE];
      |   ^
05_nx_usestack.c:8:3: error: type name does not allow storage class to be specified
    8 |   static int top;
      |   ^
2 errors generated.

*/