#include <iostream>
using namespace std;
#define SIZE 5
class Stack {
private:
    char stack[SIZE];
    int top;
public:
    Stack()              { top = 0; }
    Stack(const char* s) { top = 0; while (*s && !isFull()) stack[top++] = *s++; }
    ~Stack()             { }
    bool isEmpty()       { return 0 == top; }
    bool isFull()        { return SIZE == top; }
    void push(char c)    { if (!isFull()) stack[top++] = c; }
    char pop()           { if (!isEmpty()) return stack[--top]; return '\0'; }
};
int main(void) {
    Stack s1;
    Stack s2("C++");
    s1.push('C');
    while (!s1.isEmpty())
        cout << s1.pop();
    cout << '\n';
    while (!s2.isEmpty())
        cout << s2.pop();
    cout << '\n';
    return 0;
}