#include <stdio.h>
// ===> stack dalis
#define SIZE 5
    class Stack {	// CLASS !
        char stack[SIZE];
        int top;

        void init()       { top = 0; }       
        int isEmpty()     { return 0 == top; }
        int isFull()      { return SIZE == top; }
        void push(char c) {
            if (!isFull()) stack[top++] = c;
        }
        char pop() {
            if (!isEmpty()) return stack[--top];
            return '\0';
        }
    };
// ===> user dalis
    int main(void) {
        char c;
        Stack st1, st2;	// "nebetampome" žodelių 'struct' ar 'class'
        st1.init();	
        while ((!st1.isFull()) && ('\n' != (c = getchar()))) st1.push(c);
        while (!st1.isEmpty()) putchar(st1.pop());
        putchar('\n');
        st2.init();
        while ((!st2.isFull()) && ('\n' != (c = getchar()))) st2.push(c);
        while (!st2.isEmpty()) putchar(st2.pop());
        putchar('\n');
        return 0;
    }
