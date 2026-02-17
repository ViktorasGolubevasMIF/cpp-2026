#include "stack.h"

#define SIZE 5

static char stack[SIZE];    // !
static int top;             // !

static void reset(void) {   // !
    top = 0;
}

void init(void) {
    top = 0;
}

int isEmpty(void) {
    return 0 == top;
}

int isFull(void) {
    return SIZE == top;
}

void push(char c) {
    if (!isFull()) {
        stack[top++] = c;
    }
}

char pop(void) {
    if (!isEmpty()) {
        return stack[--top];
    }
    return '\0';
}
