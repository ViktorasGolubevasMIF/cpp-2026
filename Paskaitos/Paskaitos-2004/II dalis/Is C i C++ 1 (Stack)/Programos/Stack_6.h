const int STACK_SIZE = 10;

class stack {
 private:
  char stackas[STACK_SIZE];
  int  top;
 public:
  stack();
  void reset();
  void push(char);
  char pop();
  int is_empty ();
  int is_full ();
};


