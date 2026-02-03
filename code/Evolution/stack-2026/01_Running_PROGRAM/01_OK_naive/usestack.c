#include <stdio.h>

#define SIZE 5

char stack[SIZE];
int top = 0;

/* Inicializavimas/nustatymas - reset() */
void init(void) {
    top = 0;
}

/* Ar stekas tuščias */
int isEmpty(void) {
    return 0 == top;
}

/* Ar stekas pilnas */
int isFull(void) {
    return SIZE == top;
}

/* Įdėti elementą į steką */
void push(char c) {
    if (!isFull()) {
        stack[top++] = c;
    }
}

/* Išimti elementą iš steko */
char pop(void) {
    if (!isEmpty()) {
        return stack[--top];
    }
    return '\0';
}

// ===

int main(void) {
    char c;

    init();

    /* Skaitome simbolius iki '\n' */
    while ((c = getchar()) != '\n') {
        push(c);
    }

    /* Kol stekas netuščias, spausdiname išimdami po simbolį (elementą) */
    while (!isEmpty()) {
        putchar(pop());
    }

    putchar('\n');
    return 0;
}

/*
>gcc usestack.c -o usestack
*/

