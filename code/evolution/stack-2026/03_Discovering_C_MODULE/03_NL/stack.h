#define SIZE 100

extern char stack[SIZE];
extern int top;

extern void init(void);
extern int isEmpty(void);
extern int isFull(void);
extern void push(char c);
extern char pop(void);
