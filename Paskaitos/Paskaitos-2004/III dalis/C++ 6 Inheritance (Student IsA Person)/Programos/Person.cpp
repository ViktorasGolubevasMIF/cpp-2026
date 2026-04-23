// Person.cpp
#include <iostream.h>
#include "Person.h"

Person::Person (int yy, int mm, int dd, char* n): birthdate(yy,mm,dd) {
 name = new char[strlen(n)+1];
 memcpy(name,n,strlen(n)+1);
 print_info(" Person(int,int,int,char*)\n\t");
 cout <<  "\t: " << this << endl;
}
Person::Person (Date dx, char* n): birthdate(dx) {
 name = new char[strlen(n)+1];
 memcpy(name,n,strlen(n)+1);
 print_info(" Person(Date,char*)\n\t");
 cout <<  "\t: " << this << endl;
}
Person::~Person () {
 print_info("~Person()\n\t");
 cout <<  "\t: " << this << endl;
 delete [] name;
}
Person::Person (const Person& p) : birthdate(p.birthdate) {
 name = new char[strlen(p.name)+1];
 memcpy(name,p.name,strlen(p.name)+1);
 print_info(" Person(const Person&)\n\t");
 cout <<  "\t: " << this << " copy of " << (void*) &p << endl;
}
Person& Person::set_name(char* n) {
 delete [] name;
 name = new char[strlen(n)+1];
 memcpy(name,n,strlen(n)+1);
 return *this;
}
char* Person::get_name() const {
 return name;
}

Person& Person::set_birthyear (int yy) {
// birthdate.y=yy;
//Error : 'Date::y' is not accessible in function Person::set_birthyear(int) 
 birthdate.set_year(yy);return *this;}
Person& Person::set_birthmonth (int mm) {
 birthdate.set_month(mm);return *this;}
Person& Person::set_birthday (int dd) {
 birthdate.set_day(dd);return *this;}
int Person::get_birthyear () const {return birthdate.get_year();}
int Person::get_birthmonth () const {return birthdate.get_month();}
int Person::get_birthday () const {return birthdate.get_day();}

void Person::print_info(char* intro) const {
 cout << intro;
 print_this();
}
void Person::print_this() const {
 birthdate.print_this(); //tik per Date interface'a
 cout << " " << name ;
}