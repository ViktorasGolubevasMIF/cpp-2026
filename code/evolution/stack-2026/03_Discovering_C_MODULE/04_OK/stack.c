#include "stack.h"

#define SIZE 5

char stack[SIZE];
int top;

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
