#define STACK_SIZE 10

typedef struct {
  char stack[STACK_SIZE];
  int  top;
} stack;

void reset(stack *);
void push(stack *, char);
char pop(stack *);

