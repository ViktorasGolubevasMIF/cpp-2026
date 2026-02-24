#include <iostream>
#include "stack.h"
// std:: – naudojame pilną vardą

int main(void) {
    Stack s1;
    Stack s2("C++");
    s1.push('C');
    while (!s1.isEmpty())
        std::cout << s1.pop();
    std::cout << '\n';
    while (!s2.isEmpty())
        std::cout << s2.pop();
    std::cout << '\n';
    return 0;
}