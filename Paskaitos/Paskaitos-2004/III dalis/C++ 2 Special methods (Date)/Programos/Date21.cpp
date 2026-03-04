// Date2.c 

// 1.Konstruktorius
// 2.Kada konstruktoriai kvieciami
// 3.Netiesiogiai apibreztas konstruktorius
// 4.Konstruktoriai. Konstruktoriai su parametrais. Konstruktoriu
//   perkrovimas. Objektu kurimo ivairove.
// 5.Konstruktorius pagal nutylejima.
// 6."Universalus" konstruktorius pagal nutylejima
// 7.Destruktorius.
// 8.Kada destruktorius kvieciamas
// 9.Destruktoriu iskvietimo tvarka.

#include <iostream.h>

struct Date {
	int y,m,d;

	void add_year (int n);
//	void add_month (int n);
//	void add_day (int n);

//	void init_date (int yy, int mm, int dd);
/*
	Date (int yy, int mm, int dd);
	Date (int yy);
	Date ();
*/
	Date (int yy=1, int mm=1, int dd=1);
	~Date ();
};

void Date::add_year (int n) {d+=n;}
//void Date::add_month (int n) {}
//void Date::add_day (int n) {}

/*
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
*/
Date::Date (int yy, int mm, int dd) {
	y=yy;m=mm;d=dd;
	cout << "Constructor Date(int yy=1, int mm=1, int dd=1) : " << y << "." << m << "." << d << " at " << this << endl;
}
Date::~Date () {
	cout << "Destructor ~Date() : " << y << "." << m << "." << d << " at " << this << endl;
}


//--------------------------------------------------------------------

Date D1(1999,9,9);

int main() {
// NAK
Date d1;

// K. su parametrais
Date d21(1999,3,31);
Date d22=Date(1999,3,31);

// K. perkrovimas
Date d31(2000);
Date d32=Date(2000);
Date d33=2000;

// Nebeturim NAK, bandomas kviesti
// vienas is tiesiogiai aprasytu konstruktoriu
// 
/* Date D41; */

// KPN
Date d42;
Date d43=Date();

// "Universalus" KPN
return 0;
}
