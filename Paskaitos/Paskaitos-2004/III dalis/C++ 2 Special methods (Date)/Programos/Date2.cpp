// Date2.cpp

#include <iostream.h>

class Date {
	int y,m,d;
public:
	Date (int yy=1, int mm=1, int dd=1);
	~Date ();
};

Date::Date (int yy, int mm, int dd) {
  y=yy;m=mm;d=dd;
  cout <<"Object "<<y<<"."<<m<<"."<<d<<" at "<<this<<" is constructed"<<endl;
}
Date::~Date () {
  cout <<"Object "<<y<<"."<<m<<"."<<d<<" at "<<this<<" is destructed"<<endl;
}

//--------------------------------------------------------------------

//Date staticD; // objektas statineje atmintyje

void foo() {
Date autoD(2001); // objektas automatineje atmintyje 
} // Cia naikinamas

int main() {
//  foo(); 

//  Date* p_dynamicD = new Date(2001,3,20); // objektas dinamineje atmintyje
//  delete p_dynamicD;

  Date aautoD[2]={Date(2001,3,20),Date(2002,3,20)};
  Date *p_adynamicD = new Date[2]; // objektu masyvas dinamineje atmintyje

  delete [] p_adynamicD;
  return 0;
}

/*
Object 1.1.1 at 004279D8 is condtructed
Object 2000.1.1 at 0012FF50 is condtructed
Object 2000.1.1 at 0012FF50 is destructed
Object 2000.3.29 at 008B3EC8 is condtructed
Object 2000.3.29 at 008B3EC8 is destructed
Object 1.1.1 at 004279D8 is destructed
*/

/*
Object 1.1.1 at 0012FF74 is condtructed
Object 1.1.1 at 0012FF80 is condtructed
Object 1.1.1 at 008B4384 is condtructed
Object 1.1.1 at 008B4390 is condtructed
Object 1.1.1 at 008B4390 is destructed
Object 1.1.1 at 008B4384 is destructed
Object 1.1.1 at 0012FF80 is destructed
Object 1.1.1 at 0012FF74 is destructed
*/