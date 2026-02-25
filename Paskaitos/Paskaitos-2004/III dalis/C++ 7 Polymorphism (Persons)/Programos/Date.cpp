// Date.cpp 
#include <iostream.h>
#include "Date.h"

Date::Date (int yy, int mm, int dd) : y(yy),m(mm),d(dd) {
 print_info(" Date(int,int,int)\n\t");
 cout <<  "\t: " << this << endl;
}
Date::Date (const Date& dx) : y(dx.y),m(dx.m),d(dx.d) {
 print_info(" Date(const Date&)\n\t");
 cout <<  "\t: " << this << " copy of " << (void*) &dx << endl;
  
}
Date::~Date () {
 print_info("~Date()\n\t");
 cout <<  "\t: " << this << endl;
}

Date& Date::set_year(int yy) {y=yy;return *this;}
Date& Date::set_month(int mm) {m=mm;return *this;}
Date& Date::set_day(int dd) {d=dd;return *this;}

int Date::get_year() const {return y;}
int Date::get_month() const {return m;}
int Date::get_day() const {return d;}

void Date::print_info(char* intro) const {
  cout << intro;
  print_this();
}
void Date::print_this() const {
  cout << y << "." << m << "." << d;
}
