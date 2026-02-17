#define SIZE 5

struct Stack{
  char stack[SIZE];
  int top;
};

void init(struct Stack *);
int isEmpty(struct Stack *);
int isFull(struct Stack *);
void push(struct Stack *, char);
char pop(struct Stack *);
