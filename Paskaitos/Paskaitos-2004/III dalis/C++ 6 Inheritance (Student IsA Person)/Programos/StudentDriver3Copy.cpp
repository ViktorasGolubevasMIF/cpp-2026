// StudentDriver3Copy.cpp 

#include <iostream.h>
#include "Student.h"

int main() {
	Student s31(1980,7,15,"FX",1999,9,1,"VU");
	Student s32=s31;
/*
Error StudentDriver3Copy.cpp : 'Student::Student(const Student &)' 
is not accessible in function main()
*/
//Privalome realizuoti Student kopijos konstruktoriu, tarkime taip:
/*
Student::Student (const Student& s) : enterdate(s.enterdate) {
 college = new char[strlen(s.college)+1];
 memcpy(college,s.college,strlen(s.college)+1);
 print_info(" Student(const Student&)\n\t");
 cout <<  "\t: " << this << " copy of " << (void*) &s << endl;
*/
	s31.print_this(); cout << endl;
	s32.print_this(); cout << endl;
//Bet tada spausdins
/*
 Date(int,int,int)
        1980.7.15       : 0012FF6C
 Person(int,int,int,char*)
        1980.7.15 FX    : 0012FF6C
 Date(int,int,int)
        1999.9.1        : 0012FF7C
 Student(int,int,int,char*,int,int,int,char*)
        1980.7.15 FX 1999.9.1 VU        : 0012FF6C
 Date(int,int,int)
        1900.1.1        : 0012FF4C
 Person(int,int,int,char*)
        1900.1.1 JK     : 0012FF4C
 Date(const Date&)
        1999.9.1        : 0012FF5C copy of 0012FF7C
 Student(const Student&)
        1900.1.1 JK 1999.9.1 VU : 0012FF4C copy of 0012FF6C
1980.7.15 FX 1999.9.1 VU
1900.1.1 JK 1999.9.1 VU
~...
*/
// Suvokiame, kad kopijos konstruktoriuje uzmirsome inicijuoti 
// Person poobjekti.Teisingam kopijos konstruktoriui reikalingas
// nedidelis triukas - pasinaudojimas upcasting'o savybe!
// Student::Student (const Student& s) :Person(s),enterdate(s.enterdate) {
// Bet tokiu atveju reikalingas Person kopijos konstruktorius.
//
/*
Error Student.cpp: 'Person::Person(const Person &)' is not 
accessible in function Student::Student(const Student &)
*/
// Jeigu ji idesime 'protected' dalyje gausime linker'io klaida.
/*
Unresolved external 'Person::Person(const Person&)' 
referenced from STUDENT.OBJ
*/
// Realizuojame 'public' kopijos konstruktoriu (esam "priversti", 
// nes Person objektas naudoja dinamine atminti.
/*
Person::Person (const Person& p) : birthdate(p.birthdate) {
 name = new char[strlen(p.name)+1];
 memcpy(name,p.name,strlen(p.name)+1);
 print_info(" Person(const Person&)\n\t");
 cout <<  "\t: " << this << " copy of " << (void*) &p << endl;
}
*/

// Dabar spausdins
/*
 Date(int,int,int)
        1980.7.15       : 0012FF6C
 Person(int,int,int,char*)
        1980.7.15 FX    : 0012FF6C
 Date(int,int,int)
        1999.9.1        : 0012FF7C
 Student(int,int,int,char*,int,int,int,char*)
        1980.7.15 FX 1999.9.1 VU        : 0012FF6C
 Date(const Date&)
        1980.7.15       : 0012FF4C copy of 0012FF6C
 Person(const Person&)
        1980.7.15 FX    : 0012FF4C copy of 0012FF6C
 Date(const Date&)
        1999.9.1        : 0012FF5C copy of 0012FF7C
 Student(const Student&)
        1980.7.15 FX 1999.9.1 VU        : 0012FF4C copy of 0012FF6C
1980.7.15 FX 1999.9.1 VU
1980.7.15 FX 1999.9.1 VU
~...
*/
}
