#define STACK_SIZE 100

class stack {
  private:
/*
const int stack_size = 100;
*/
char stack[STACK_SIZE];
int  top;

  public:
void reset(void);
void push(char c);
char pop(void);
void print_pop(void);
void print_push(void);
};
