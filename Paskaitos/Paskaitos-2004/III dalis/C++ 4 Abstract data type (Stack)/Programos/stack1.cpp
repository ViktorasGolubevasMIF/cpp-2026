#include <iostream.h>

const int size = 2;

class stack {
  private:
	// const int size;
	char stck[size];
	int  top;
  public:
	stack();
	void reset();
	int get_size();
	void push(char c);
	char pop();
	bool is_empty();
	bool is_full();
};

stack::stack() { 
  reset();
  cout << "Konstruktorius" << endl;
}

void stack::reset() {
  top = 0;
}

int stack::get_size() {
  return size;
}

void stack::push (char c) {
  if (!is_full()) stck[top++]=c; 
  else cout << "Jau pilnas" << endl; 
}

char stack::pop () {
  if (!is_empty()) return stck[--top];
  else cout << "Jau tuscias" << endl; 
}

bool stack::is_empty () {
  return (0==top) ? true : false;
}

bool stack::is_full () {
  return (get_size() == top) ? true : false;
}

int main(void)
{
stack stackas;

char c1='a', c2;
stackas.push(c1); 
c2=stackas.pop(); 
if (c1!=c2) cout << "Negali buti" << endl;

return 0;
}