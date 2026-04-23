// Student.cpp
#include <iostream.h>
#include "Student.h"

Student::Student (int yb,int mb,int db,char* n,int ye,int me,int de,char* c) : 
	Person(yb,mb,db,n),enterdate(ye,me,de) {
 college = new char[strlen(c)+1];
 memcpy(college,c,strlen(c)+1);
 print_info(" Student(int,int,int,char*,int,int,int,char*)\n\t");
 cout <<  "\t: " << this << endl;
}
Student::Student (Date dxb,char* n,Date dxe,char* c): 
	Person(dxb,n),enterdate(dxe) {
 college = new char[strlen(c)+1];
 memcpy(college,c,strlen(c)+1);
 print_info(" Student(Date,char*,Date,char*)\n\t");
 cout <<  "\t: " << this << endl;
}
Student::~Student () {
 print_info("~Student()\n\t");
 cout <<  "\t: " << this << endl;
 delete [] college;
}
Student::Student (const Student& s) : Person(s),enterdate(s.enterdate) {
 college = new char[strlen(s.college)+1];
 memcpy(college,s.college,strlen(s.college)+1);
 print_info(" Student(const Student&)\n\t");
 cout <<  "\t: " << this << " copy of " << (void*) &s << endl;
}

void Student::print_info(char* intro) const {
 cout << intro;
 print_this();
}
void Student::print_this() const {
 Person::print_this();
 cout << " ";
 enterdate.print_this(); 
 cout << " " << college;
}
void Student::print_test() const {
// cout << "Mano vardas " << name << endl;
// Error: 'Person::name' is not accessible in function Student::print_test() const
// Kadangi get_name() yra paveldetas is 'Person' metodas, galima
// cout << "Mano vardas " << get_name() << endl;
// Mazas nesusipratimas - 'Student' i savo varda turi tiek pat teisiu
// kiek isoriniai objektai/funkcijos.
// Tinkamas sprendimas - 'protected' priejimo specifikatorius.

cout << "Mano vardas " << name << endl;

// Tas pats su 'private' 'birthdate'
// Error 'Person::birthdate' is not accessible in function Student::print_test() const
cout << "Kai man buvo " << enterdate.get_year()-birthdate.get_year() << "..." << endl;
}