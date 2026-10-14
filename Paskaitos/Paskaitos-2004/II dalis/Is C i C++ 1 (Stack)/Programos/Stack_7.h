
class stack {
 private:
  char* stackas;
  int  top;
  int  size;
 public:
  stack(int);
  void reset();
  void push(char);
  char pop();
  int is_empty ();
  int is_full ();
  int get_size ();
  ~stack();
};


