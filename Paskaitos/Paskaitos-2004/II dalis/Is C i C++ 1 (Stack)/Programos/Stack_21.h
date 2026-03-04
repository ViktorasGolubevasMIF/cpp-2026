#define STACK_SIZE 10

extern char stack[STACK_SIZE]={0};
extern int  top=0;
/* extern */ void reset(void);
/* extern */ void push(char c);
/* extern */ char pop(void);



