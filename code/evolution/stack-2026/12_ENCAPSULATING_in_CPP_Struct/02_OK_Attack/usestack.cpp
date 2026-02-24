#include <stdio.h>
// ===> stack dalis
#define SIZE 5
    struct Stack {
        char stack[SIZE];
        int top;

        void init()       { top = 0; }            // ← pst - išmestas, top - tiesiogiai
        int isEmpty()     { return 0 == top; }
        int isFull()      { return SIZE == top; }
        void push(char c) {
            if (!isFull()) stack[top++] = c;       // ← stack, top – tiesiogiai
        }
        char pop() {
            if (!isEmpty()) return stack[--top];
            return '\0';
        }
    };
// ===> user dalis
    int main(void) {
        char c;
        struct Stack st1;
        Stack st2;	// "nebetampome" etikėtės 'struct'
        st1.init();	// ← nebėra (&st) !
        while ((!st1.isFull()) && ('\n' != (c = getchar()))) st1.push(c);
        while (!st1.isEmpty()) putchar(st1.pop());
        putchar('\n');
        st2.init();
        while ((!st2.isFull()) && ('\n' != (c = getchar()))) st2.push(c);
    //// ATTACK!
    st2.stack[st2.top++] = '!';
    ////
        while (!st2.isEmpty()) putchar(st2.pop());
        putchar('\n');
        return 0;
    }
