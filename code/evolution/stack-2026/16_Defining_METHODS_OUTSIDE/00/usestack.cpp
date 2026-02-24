#include <iostream>
using namespace std;
#define SIZE 5
class Stack {
private:
    char stack[SIZE];
    int top;
public:
    Stack() {                               // ← default konstruktorius
        top = 0;
        cout << "[CTOR] Stack sukurtas (tuščias)\n";
    }
    Stack(const char* str) {               // ← parametrinis konstruktorius
        top = 0;
        while (*str && !isFull())
            stack[top++] = *str++;
        cout << "[CTOR] Stack sukurtas su duomenimis\n";
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
    Stack s1;               // ← default konstruktorius
    Stack s2("C++");      // ← parametrinis konstruktorius

    cout << "s1: ";
    s1.push('C');
    while (!s1.isEmpty())
        cout << s1.pop();
    cout << '\n';

    cout << "s2: ";
    while (!s2.isEmpty())
        cout << s2.pop();
    cout << '\n';
    return 0;
}