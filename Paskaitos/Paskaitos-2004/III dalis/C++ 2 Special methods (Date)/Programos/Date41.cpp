// Date4.c 
#include <iostream.h>

struct Date {
	int y,m,d;

	void add_year (int n);
//	void add_month (int n);
//	void add_day (int n);

//	void init_date (int yy, int mm, int dd) {y=yy;m=mm;d=dd;}
	Date ();
	Date (int yy);
	Date (int yy, int mm, int dd);
	Date (Date& d);
	Date (const Date& d);
};

void Date::add_year (int n) {d+=n;}
//void Date::add_month (int n) {}
//void Date::add_day (int n) {}

Date::Date () {
	cout << "Constructor Date() : " << y << "." << m << "." << d << " at " << this << endl;
}
Date::Date (int yy) {
	y=yy;
	cout << "Constructor Date(int yy) : " << y << "." << m << "." << d << " at " << this << endl;
}
Date::Date (int yy, int mm, int dd) {
	y=yy;m=mm;d=dd;
	cout << "Constructor Date(int yy, int mm, int dd) : " << y << "." << m << "." << d << " at " << this << endl;
}
Date::Date (const Date& x) {
	y=x.y;
	m=x.m;
	d=x.d;
	cout << "Copy Constructor Date(const Date& d) : " << y << "." << m << "." << d << " at " << this << endl;
}
Date::Date (Date& x) {
	y=x.y;
	m=x.m;
	d=x.d;
	cout << "Copy Constructor Date(Date& d) : " << y << "." << m << "." << d << " at " << this << endl;
}

int main() {
//Date d1;
//Date d2(1999);
//Date d3(1999,3,31);
const Date d4=Date(1999,3,31);
Date d5=d4;
Date d6=1998;
d6 = Date(1999,3,31);
//cout << &d1 << endl;
//cout << &d2 << endl;
//cout << &d3 << endl;
//cout << &d4 << endl;
//cout << &d5 << endl;
//cout << &d6 << endl;
return 0;
}
