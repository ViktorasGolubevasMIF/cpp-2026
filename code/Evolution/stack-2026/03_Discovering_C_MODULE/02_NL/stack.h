#define SIZE 100

char stack[SIZE];
int top=0;

void init(void);
int isEmpty(void);
int isFull(void);
void push(char c);
char pop(void);