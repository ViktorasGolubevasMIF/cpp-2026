#include <iostream>
using namespace std;
#define SIZE 5
class Stack {
private:
    char stack[SIZE];
    int top;
public:
    Stack() {
        top = 0;
        cout << "[CTOR] Stack sukurtas (tuščias)\n";
    }
    ~Stack() {
        cout << "[DTOR] Stack sunaikintas\n";
    }
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
int main(void) {
    Stack s1;               // ← default: tuščias stekas
    s1.push('C');
    s1.push('+');
    s1.push('+');
    while (!s1.isEmpty())
        cout << s1.pop();
    cout << '\n';
    return 0;
}