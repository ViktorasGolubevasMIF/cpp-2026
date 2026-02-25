#define STACK_SIZE 100
struct stack {
   char stack[STACK_SIZE];
   int top;
};

void reset(struct stack *id);
void push(struct stack *id, char c);
char pop(struct stack *id);
void print_push(struct stack *id);
void print_pop(struct stack *id);

/*
const int stack_size = 100;
*/