// PersonDriver.cpp 

#include <iostream.h>
#include "Person.h"

int main() {

	Person p1;	// Kai Person turi KPN
	Person p2(1964,9,19,"VG");
	Person p3(Date(2000,1,1),"CC");
	p3.set_birthmonth(9).set_birthday(19);
}
/*
 Date(int,int,int)
        1900.1.1        : 0012FF7C
 Person(int,int,int,char*)
        1900.1.1 JK     : 0012FF7C
 Date(int,int,int)
        1964.9.19       : 0012FF6C
 Person(int,int,int,char*)
        1964.9.19 VG    : 0012FF6C
 Date(int,int,int)
        2000.1.1        : 0012FF24
 Date(const Date&)
        2000.1.1        : 0012FF5C copy of 0012FF24
 Person(Date,char*)
        2000.1.1 CC     : 0012FF5C
~Date()
        2000.1.1        : 0012FF24
~Person()
        2000.9.19 CC    : 0012FF5C
~Date()
        2000.9.19       : 0012FF5C
~Person()
        1964.9.19 VG    : 0012FF6C
~Date()
        1964.9.19       : 0012FF6C
~Person()
        1900.1.1 JK     : 0012FF7C
~Date()
        1900.1.1        : 0012FF7C
*/