// Driver.cpp 
#include <iostream.h>
#include "Person3.h"
//#include "Date.h"	//daugybinis deklaravimas


int main() {
//	Person p0;	// be Date KPN 
/*
Cannot find default constructor to initialize member 'birthdate' 
in function Person::Person(int,int,int,char *)
*/

//	Person p0;	// su KPN tiek Person, tiek Date
/*
Constructor Date(int,int,int) : 1.1.1 at 0012FF7C
Constructor Person(int,int,int,char*) : JK was born in 1.1.1 at 0012FF7C
Destructor ~Person() : JK at 0012FF7C bye-bye...
Destructor ~Date() : 1.1.1 at 0012FF7C
*/
Date SomeDay(1964,9,19);
Person p1(SomeDay,"V.G.");
Person p2(SomeDay,"S.B.");
/*
Constructor Date(int,int,int) : 1999.4.30 at 0012FF48
Copy Constructor Date(const Date&) : 1999.4.30 at 0012FF7C
Constructor Person(Date,char*) : V.G. was born in 1999.4.30 at 0012FF7C
Destructor ~Date() : 1999.4.30 at 0012FF48
Destructor ~Person() : V.G. at 0012FF7C bye-bye...
Destructor ~Date() : 1999.4.30 at 0012FF7C
*/
	return 0;
}
