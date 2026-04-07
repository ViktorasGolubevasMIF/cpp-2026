// Date.cpp 
#include <iostream.h>
#include "Date.h"

Date::Date (int yy, int mm, int dd) {
	set_date(yy,mm,dd);
	cout << "Constructor Date(int,int,int) : " << y << "." << m << "." << d << " at " << this << endl;
}

Date::Date (const Date& x) {
	y=x.y; m=x.m; d=x.d;
	cout << "Copy Constructor Date(const Date&) : " << y << "." << m << "." << d << " at " << this << endl;
}
Date::~Date () {
	cout << "Destructor ~Date() : " << y << "." << m << "." << d << " at " << this << endl;
}

void Date::set_date (int yy, int mm, int dd) {
	set_year(yy);set_month(mm);set_day(dd);
}
void Date::set_year(int yy) {
	y=yy;
}
void Date::set_month(int mm) {
	m=mm;
}
void Date::set_day(int dd) {
	d=dd;
}

void Date::get_date (int& yy, int& mm, int& dd)  const {
yy=y;mm=m;dd=d;
}

int Date::get_year()  const {
	return y;
}
int Date::get_month()  const {
	return m;
}
int Date::get_day()  const {
	return d;
}

Date& Date::add_year_ref (int n) {
	this->y+=n;
	return *this;
}

Date Date::add_year_val (int n) {
	this->y+=n;
	return *this;
}
