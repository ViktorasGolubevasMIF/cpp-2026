#include <iostream>
using namespace std;
// ===> stack dalis
#define SIZE 5
class Stack {
private:
    char stack[SIZE];
    int top;
public:
    Stack()  {                              // ← konstruktorius: tas pats vardas kaip klasė, be grąžinamo tipo
        top = 0;
        cout << "[CTOR] Stack sukurtas\n";
    }
    ~Stack() {                              // ← destruktorius: ~ + klasės vardas
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
// ===> user dalis
int main(void) {
    char c;
    cout << "--- prieš { - scope (bloko) pradžią ---\n";
    {
        cout << "--- prieš Stack s; ---\n";
        Stack s;                            // ← CTOR iškviečiamas čia
        while (!s.isFull() && cin.get(c) && c != '\n')
            s.push(c);
        while (!s.isEmpty())
            cout << s.pop();
        cout << '\n';
        cout << "--- prieš } - scope pabaigą ---\n";
    }                                       // ← DTOR iškviečiamas čia automatiškai!
    cout << "--- po scope pabaigos ---\n";
    return 0;
}