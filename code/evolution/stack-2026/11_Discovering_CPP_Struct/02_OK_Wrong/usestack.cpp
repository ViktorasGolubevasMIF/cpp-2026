#include <stdio.h>
// ===> stack dalis
#define SIZE 5
    struct Stack {
        char stack[SIZE];
        int top;
        static void reset(struct Stack *pst) { pst->top = 0; }
        void init(struct Stack *pst)         { pst->top = 0; }
        int isEmpty(struct Stack *pst)       { return 0 == pst->top; }
        int isFull(struct Stack *pst)        { return SIZE == pst->top; }
        void push(struct Stack *pst, char c) {
            if (!isFull(pst)) pst->stack[pst->top++] = c;
        }
        char pop(struct Stack *pst) {
            if (!isEmpty(pst)) return pst->stack[--pst->top];
            return '\0';
        }
    };
// ===> user dalis
    int main(void) {
        char c;
        struct Stack st1, st2;
        st1.init(&st1);   // ← objekto sintaksė
        while ((!st1.isFull(&st1)) && ('\n' != (c = getchar()))) st1.push(&st1, c);
        while (!st1.isEmpty(&st1)) putchar(st1.pop(&st1));
        putchar('\n');
        st2.init(&st2);
        while ((!st2.isFull(&st2)) && ('\n' != (c = getchar()))) st2.push(&st2, c);
        while (!st2.isEmpty(&st2)) putchar(st2.pop(&st2));
        putchar('\n');
        return 0;
    }