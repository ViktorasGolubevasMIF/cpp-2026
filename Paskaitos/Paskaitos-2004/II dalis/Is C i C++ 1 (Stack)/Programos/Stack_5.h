const int STACK_SIZE = 10;

struct stack{
  char stackas[STACK_SIZE];
  int  top;
  void reset();
  void push(char);
  char pop();
  int is_empty ();
  int is_full ();
};


