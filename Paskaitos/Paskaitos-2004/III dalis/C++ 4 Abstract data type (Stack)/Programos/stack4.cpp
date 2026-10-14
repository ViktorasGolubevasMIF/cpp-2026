#include <iostream.h>


class stack {
  private:
	//const int size;
	int size;
	char* stck;
	int  top;
  public:
	stack(int s=100);
	stack(char* filler, int s=100);
	stack(const stack&);
	stack(stack&);
//	void operator=(const stack&);
	stack& operator=(const stack&);
	stack& operator=(stack&);
	~stack();
	void reset();
	int get_size() const;
	void push(char c);
	char pop();
	bool is_empty() const;
	bool is_full() const;

};

stack::stack(int s) : size(s), stck(new char[size]), top(0) { 
  cout << "Konstruktorius:" << this << endl;
}

stack::stack(char* filler, int s)  : size(s), stck(new char[size]), top(0) { 
  for (int i=0; i<strlen(filler) ;i++) push(*(filler+i));
  cout << "Konstruktorius su char:" << this << endl;
}

stack::stack(const stack& st) :  size(st.size), stck(new char[st.size]), top(st.top) {
  for (int i=0; i<size ;i++) stck[i]=st.stck[i];
  cout << "Kopijos konstruktorius konstantai:" << this << endl;
}

stack::stack(stack& st) :  size(st.size), stck(new char[st.size]), top(st.top) {
  for (int i=0; i<size ;i++) stck[i]=st.stck[i];
  cout << "Kopijos konstruktorius:" << this << endl;
}

stack& stack::operator=(const stack& st) {
  if (this!=&st) {
	delete [] stck;
	stck = new char[size=st.size];
	top=st.top;
  }
  cout << "Priskyrimo operacija konstantai:" << this << endl;
  return *this;
}

stack& stack::operator=(stack& st) {
  if (this!=&st) {
	delete [] stck;
	stck = new char[size=st.size];
	top=st.top;
  }
  cout << "Priskyrimo operacija:" << this << endl;
  return *this;
}

stack::~stack() { 
  delete [] stck;
  cout << "Destruktorius:"  << this << endl;
}

void stack::reset() {
  top = 0;
}

int stack::get_size() const{
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

bool stack::is_empty () const{
  return (0==top);
}

bool stack::is_full () const{
  return (get_size() == top);
}

int foo(stack st) {
  cout << "<<" << endl;
  return st.get_size();
}

int main(void)
{
// Be kopijos konstruktoriaus
// Protokole: 1 konstruktorius, 2 destruktoriai.
// be to, 2 naikinamas stackas1
/*
stack stackas1;
stack stackas2 = stackas1;

stackas1.push('a'); 
stackas2.push('b'); 
cout << stackas1.pop() << endl; //b
cout << stackas2.pop() << endl; //b
*/

// Su kopijos konstruktoriumi
// Protokole: 1 konstruktorius, 1 kopijos konstruktorius, 2 destruktoriai.
/*
stack stackas1;
stack stackas2 = stackas1;
stackas1.push('a'); 
stackas2.push('b'); 
cout << stackas1.pop() << endl; //a
cout << stackas2.pop() << endl; //b
*/

// Be priskyrimo operacijos
// Protokole: 2 konstruktoriai, 2 destruktoriai.
/*
stack stackas1;
stack stackas2;
// Su const int size; bus klaida, kad negali sugeneruoti operatoriaus
stackas1 = stackas2;
stackas1.push('a'); 
stackas2.push('b'); 
cout << stackas1.pop() << endl; //b
cout << stackas2.pop() << endl; //b
*/

// Idomus(?) efektas geresniame pvz., kai stack3 is pradziu apdorojamas
// o paskui juo inicijuojamas stack4
/*
stack stackas3;
stackas3.push('a'); 
stack stackas4;
stackas4 = stackas3;
stackas4.push('b'); 
cout << stackas3.pop() << endl; //a
cout << stackas4.pop() << endl; //b
*/

// Su priskyrimo operacija
// Protokole: 2 konstruktoriai, 1 priskyrimo operacija, 2 destruktoriai.
/*
stack stackas1;
stack stackas2;
stackas1 = stackas2;
//stackas1.operator=(stackas2);
stackas1.push('a'); 
stackas2.push('b'); 
cout << stackas1.pop() << endl; //a
cout << stackas2.pop() << endl; //b
*/

/*
const stack stackas3;
const stack stackas4 = stackas3; // neveiks su nekonstantiniu kopijos konstruktoriumi
stack stackas5;
stackas5 = stackas4; // neveiks su nekonstantiniu priskyrimo operacija
*/

stack stackas61("Mes laimesim"); // 
stack stackas62 = "Mes laimesim"; // 
stack stackas7;
stackas7 = "Mes laimesim"; // 
int i1=foo(stackas7);
int i2=foo("Mes laimesim");

return 0;
}