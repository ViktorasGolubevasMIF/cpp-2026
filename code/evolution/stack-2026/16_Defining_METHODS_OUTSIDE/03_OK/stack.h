#pragma once
#define SIZE 5

class Stack {
private:
    char stack[SIZE];
    int top;
public:
    Stack();
    Stack(const char* str);
    ~Stack();
    bool isEmpty();
    bool isFull();
    void push(char c);
    char pop();
};