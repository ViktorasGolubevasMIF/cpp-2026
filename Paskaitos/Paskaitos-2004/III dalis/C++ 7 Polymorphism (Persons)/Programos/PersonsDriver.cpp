// PersonsDriver.cpp 

#include <iostream.h>
#include "Student.h"

int main() {

	Person* ap_Person[2];
	ap_Person[0] = new Person(1999,5,12,"CC");
	ap_Person[1] = new Student(1980,7,15,"FX",1999,9,1,"VU");

	ap_Person[0] -> print_info (" Person: "); cout << endl;
	ap_Person[1] -> print_info (" Student: "); cout << endl;

	delete ap_Person[0];
	delete ap_Person[1];
	return 0;
}
/*
 Date(int,int,int)
        1999.5.12       : 008B3EB8
 Person(int,int,int,char*)
        1999.5.12 CC    : 008B3EB8
 Date(int,int,int)
        1980.7.15       : 008B4384
 Person(int,int,int,char*)
        1980.7.15 FX    : 008B4384
 Date(int,int,int)
        1999.9.1        : 008B4394
 Student(int,int,int,char*,int,int,int,char*)
        1980.7.15 FX 1999.9.1 VU        : 008B4384
 Person: 1999.5.12 CC
 Student: 1980.7.15 FX
~Person()
        1999.5.12 CC    : 008B3EB8
~Date()
        1999.5.12       : 008B3EB8
~Person()
        1980.7.15 FX    : 008B4384
~Date()
        1980.7.15       : 008B4384
*/