#include <iostream>
using namespace std;
// ===> stack dalis
#define SIZE 5
class Stack {
private:
    char stack[SIZE];
    int top;
public:
    bool isEmpty() { return 0 == top; }
    bool isFull()  { return SIZE == top; }
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
    Stack s;
    while (!s.isFull() && cin.get(c) && c != '\n')
        s.push(c);
    while (!s.isEmpty())
        cout << s.pop();
    cout << '\n';
    return 0;
}