#include <stdlib.h>
#include "stack.h"

struct Stack{
  char stack[SIZE];
  int top;
};

struct Stack* create(void) {
    return (struct Stack*) calloc(sizeof(struct Stack), 1);
}

void destroy(struct Stack *pst) { // !
    free(pst);
}

static void reset(struct Stack *pst) {
    pst->top = 0;
}

void init(struct Stack *pst) {
    pst->top = 0;
}

int isEmpty(struct Stack *pst) {
    return 0 == pst->top;
}

int isFull(struct Stack *pst) {
    return SIZE == pst->top;
}

void push(struct Stack *pst, char c) {
    if (!isFull(pst)) {
        pst->stack[pst->top++] = c;
    }
}

char pop(struct Stack *pst) {
    if (!isEmpty(pst)) {
        return pst->stack[--pst->top];
    }
    return '\0';
}
