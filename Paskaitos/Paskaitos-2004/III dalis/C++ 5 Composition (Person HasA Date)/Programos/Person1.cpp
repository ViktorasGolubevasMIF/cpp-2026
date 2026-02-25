// Person1.cpp
#include <iostream.h>
#include "Person1.h"
// 1 : 'Date' turi KPN
//Person::Person (int yy, int mm, int dd, char* n) {
//	birthdate.y=yy;birthdate.m=mm;birthdate.d=dd;
/*
'Date::y' is not accessible in function Person::Person(int,int,int,char *)
*/

// 2 : 'Date' turi KPN
//Person::Person (int yy, int mm, int dd, char* n) {
//	birthdate.set_date(yy,mm,dd);

// 3 : 'Date' turi KPN, bet 'birthdate' yra const.
Person::Person (int yy, int mm, int dd, char* n) {
	birthdate.set_date(yy,mm,dd);
/*
Warning Person1.cpp 17: Non-const function Date::set_date(int,int,int) 
called for const object in function Person::Person(int,int,int,char *)
*/

// 4 : 'Date' gali netureti KPN, o 'birthdate' gali buti const
//Person::Person (int yy, int mm, int dd, char* n): birthdate(yy,mm,dd) {

 name = new char[strlen(n)+1];
 memcpy(name,n,strlen(n)+1);
 cout << "Constructor Person(int,int,int,char*) : "
      << name << " was born in "
      << birthdate.get_year() << "."
      << birthdate.get_month() << "."
      << birthdate.get_day()
      << " at " << this << endl;
}
Person::~Person () {
 cout << "Destructor ~Person() : "
      << name
      << " at " << this << " bye-bye..." << endl;
 delete [] name;
}

void Person::set_name(char* n) {
 delete [] name;
 name = new char[strlen(n)+1];
 memcpy(name,n,strlen(n)+1);
}
char* Person::get_name() const {
 return name;
}

void Person::set_birthdate (int yy,int mm,int dd) {
 birthdate.set_date(yy,mm,dd);
}
void Person::get_birthdate (int& yy,int& mm,int& dd) const {
 birthdate.get_date(yy,mm,dd);
}

void Person::set_birthyear (int yy) {birthdate.set_year(yy);}
void Person::set_birthmonth (int mm) {birthdate.set_month(mm);}
void Person::set_birthday (int dd) {birthdate.set_day(dd);}
int Person::get_birthyear () const {return birthdate.get_year();}
int Person::get_birthmonth () const {return birthdate.get_month();}
int Person::get_birthday () const {return birthdate.get_day();}
