// Driver.cpp 
#include <iostream.h>
#include "Date.h"

void print_date_ref (Date& dx) {
   cout 	
	<< "Printing by reference"
	<< " : " 
	<< dx.get_year() 
	<< "." 
	<< dx.get_month() 
	<< "." 
	<< dx.get_day() 
	<< " at " 
	<< &dx 
	<< endl;
}
void print_date_val (Date dx) {
	int y,m,d;
	dx.get_date(y,m,d);
   cout 	
	<< "Printing by value"
	<< " : " 
	<< y 
	<< "." 
	<< m 
	<< "." 
	<< d 
	<< " at " 
	<< &dx 
	<< endl;
}

int main() {

	Date d1(2001,03,27);
	Date d2=d1;
	d2.add_year_val(1).add_year_val(1).add_year_val(1);
//	d2.add_year_ref(1).add_year_ref(1).add_year_ref(1);
	cout << d2.get_year() << endl;
	print_date_val(d2);
//	print_date_ref(d2);
	return 0;
}
/* Su add_year1() ir print_date1()
//--------------------------------------------------------
Constructor Date(int,int,int) : 1999.4.30 at 0012FF80
Copy Constructor Date(const Date&) : 1999.4.30 at 0012FF74
2002
0012FF80 : 1999.4.30
Destructor ~Date() : 2002.4.30 at 0012FF74
Destructor ~Date() : 1999.4.30 at 0012FF80
//--------------------------------------------------------
*/

/* Su add_year1() ir print_date1()
//--------------------------------------------------------
Constructor Date(int,int,int) : 1999.4.30 at 0012FF80
Copy Constructor Date(const Date&) : 1999.4.30 at 0012FF74
Copy Constructor Date(const Date&) : 2000.4.30 at 0012FF68
Copy Constructor Date(const Date&) : 2001.4.30 at 0012FF5C
Copy Constructor Date(const Date&) : 2002.4.30 at 0012FF50
Destructor ~Date() : 2002.4.30 at 0012FF50
Destructor ~Date() : 2002.4.30 at 0012FF5C
Destructor ~Date() : 2001.4.30 at 0012FF68
2000
Copy Constructor Date(const Date&) : 2000.4.30 at 0012FF1C
0012FF1C : 2000.4.30
Destructor ~Date() : 2000.4.30 at 0012FF1C
Destructor ~Date() : 2000.4.30 at 0012FF74
Destructor ~Date() : 1999.4.30 at 0012FF80
//--------------------------------------------------------
*/
