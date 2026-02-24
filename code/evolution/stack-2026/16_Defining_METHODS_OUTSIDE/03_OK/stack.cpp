#include <iostream>
#include "stack.h"

Stack::Stack()              { top = 0; }
Stack::Stack(const char* s) { top = 0; while (*s && !isFull()) stack[top++] = *s++; }
Stack::~Stack()             { }
bool Stack::isEmpty()       { return 0 == top; }
bool Stack::isFull()        { return SIZE == top; }
void Stack::push(char c)    { if (!isFull()) stack[top++] = c; }
char Stack::pop()           { if (!isEmpty()) return stack[--top]; return '\0'; }