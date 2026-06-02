// Person.cpp
#include <iostream.h>
#include "Person2.h"

Person::Person (Date rd,char* n) : birthdate(rd) {
	name = new char[strlen(n)+1];
	memcpy(name,n,strlen(n)+1);
   cout << "Constructor Person(Date,char*) : "
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

