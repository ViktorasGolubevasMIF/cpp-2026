// Date4.cpp 
#include <iostream.h>

class Date {
	int y,m,d;
public:
	Date (int yy=1, int mm=1, int dd=1);
	Date (const Date& d);
	Date (Date& d);
};

Date::Date (int yy, int mm, int dd) {
  y=yy;m=mm;d=dd;
  cout <<"Object "<<y<<"."<<m<<"."<<d<<" at "<<this
       <<" is condtructed"<<endl;
}


Date::Date (const Date& x) {
  y=x.y;  m=x.m;  d=x.d;
  cout <<"Object "<<y<<"."<<m<<"."<<d<<" at "<<this
       <<" is condtructed as copy of "<<&x<< " (const) "<<endl;
}


Date::Date (Date& x) {
  y=x.y;  m=x.m;  d=x.d;
  cout <<"Object "<<y<<"."<<m<<"."<<d<<" at "<<this
       <<" is condtructed as copy of "<<&x <<endl;
}


int main() {

Date d(2001,3,20);
const Date constd;

Date d2=constd;

Date d3;
d3 = constd= d;
/*
Date d(2001,3,20);
const Date constd;
Date d1=d;
Date d2=constd;

Date d3;
d3 = d2 = d;
*/
// Jei nebutu KK su const forma d2 nebutu sukurtas
/*
Could not find a match for 'Date::Date(const Date)' in function main()
*/

// Jei butu tik KK su const forma jis butu panaudotas ir d1, ir d2 
return 0;
}
/*
Object 2000.3.29 at 0012FF80 is condtructed
Object 1.1.1 at 0012FF74 is condtructed
Object 2000.3.29 at 0012FF68 is condtructed as copy of 0012FF80
Object 1.1.1 at 0012FF5C is condtructed as copy of 0012FF74 (const)
Object 1.1.1 at 0012FF50 is condtructed
Object 2000.3.29 at 0012FF80 is assigned to 0012FF50
*/