// StudentDriver5UpcastSlice.cpp 

#include <iostream.h>
#include "Student.h"
void p_print(Person* p) {
	p->print_info("Person or  Person's child: ");cout << endl;}
void r_print(Person& p) {
	p.print_info("Person or Person's child: ");cout << endl;}
void v_print(Person p) {
	p.print_info("Person or  Person's child: ");cout << endl;}

int main() {
//	Student s3(1964,9,19,"VG",1982,9,1,"VU");
//	Person* p_Person=&s3;
//	Person& r_Person=s3;
//	p_print(&s3);
//	r_print(s3);
/*
 Date(int,int,int)		1964.9.19       	: 0012FF6C
 Person(int,int,int,char*)	1964.9.19 VG    	: 0012FF6C
 Date(int,int,int)		1982.9.1        	: 0012FF7C
 Student(int,int,int,char*,int,int,int,char*)
				1964.9.19 VG 1982.9.1 VU: 0012FF6C
Person or Person's child: 1964.9.19 VG
Person or  Person's child: 1964.9.19 VG
~Student()		        1964.9.19 VG 1982.9.1 VU: 0012FF6C
~Date()			        1982.9.1	        : 0012FF7C
~Person()		        1964.9.19 VG    	: 0012FF6C
~Date()			        1964.9.19		: 0012FF6C
*/

//	Person p0=s3;
//	v_print(p0);
//	v_print(s3);
//"Person::Person(const Person &)' is not accessible in function main()

// Papildzius Person kopijos konstruktoriumi
/*
Person::Person (const Person& p) : birthdate(p.birthdate) {
 name = new char[strlen(p.name)+1];
 memcpy(name,p.name,strlen(p.name)+1);
 print_info(" Person(const Person&)\n\t");
 cout <<  "\t: " << this << " copy of " << (void*) &p << endl;
}
*/
// isitikiname, kad upcasting'as veikia ir tiesiog objektams, o ne
// vien rodyklems ar nuorodoms. Taciau svarbu pastebeti, kad skirtingai
// nuo rodykliu, kur priskiriami adresai, Student perduotas pagal
// reiksme kaip Person yra "apkarpomas" (slice) iki Person!
//	Student s4(Date(1980,7,15),"FX",Date(1998,9,1),"VU");
//	Person p0=s4;
//	v_print(p0);
//	v_print(s4);
/*

 Date(int,int,int)        	1998.9.1        	: 0012FF24
 Date(int,int,int)        	1980.7.15       	: 0012FF14
 Date(const Date&)        	1980.7.15       	: 0012FECC copy of 0012FF14
 Date(const Date&)        	1980.7.15       	: 0012FF6C copy of 0012FECC
 Person(Date,char*)       	1980.7.15 FX   		: 0012FF6C
~Date()				1980.7.15       	: 0012FECC
 Date(const Date&)        	1998.9.1        	: 0012FF7C copy of 0012FF24
 Student(Date,char*,Date,char*) 1980.7.15 FX 1998.9.1 VU: 0012FF6C
~Date()        			1980.7.15       	: 0012FF14
~Date()        			1998.9.1        	: 0012FF24
 Date(const Date&)        	1980.7.15       	: 0012FF5C copy of 0012FF6C
 Person(const Person&)        	1980.7.15 FX   		: 0012FF5C copy of 0012FF6C
 Date(const Date&)        	1980.7.15       	: 0012FF24 copy of 0012FF5C
 Person(const Person&)        	1980.7.15 FX   		: 0012FF24 copy of 0012FF5C

Person or  Person's child: 1980.7.15 FX

~Person()        		1980.7.15 FX   		: 0012FF24
~Date()        			1980.7.15       	: 0012FF24
 Date(const Date&)        	1980.7.15       	: 0012FF24 copy of 0012FF6C
 Person(const Person&)        	1980.7.15 FX   		: 0012FF24 copy of 0012FF6C

Person or  Person's child: 1980.7.15 FX

~Person()        		1980.7.15 FX   		: 0012FF24
~Date()        			1980.7.15       	: 0012FF24
~Person()        		1980.7.15 FX   		: 0012FF5C
~Date()        			1980.7.15       	: 0012FF5C
~Student()        		1980.7.15 FX 1998.9.1 VU: 0012FF6C
~Date()        			1998.9.1        	: 0012FF7C
~Person()        		1980.7.15 FX   		: 0012FF6C
~Date()        			1980.7.15       	: 0012FF6C
*/

	Student s5(Date(1980,7,15),"FX",Date(1998,9,1),"VU");
	Student s6=s5;
//'Student::Student(const Student &)' is not accessible in function main()
	r_print(s5);
	r_print(s6);
//Privalome realizuoti Student kopijos konstruktoriu, tarkime taip:
/*
Student::Student (const Student& s) : enterdate(s.enterdate) {
 college = new char[strlen(s.college)+1];
 memcpy(college,s.college,strlen(s.college)+1);
 print_info(" Student(const Student&)\n\t");
 cout <<  "\t: " << this << " copy of " << (void*) &s << endl;
*/
//Bet tada spausdins
/*
Person or Person's child: 1980.7.15 FX
Person or Person's child: 1900.1.1 JK
*/
// Suvokiame, kad kopijos konstruktoriuje uzmirsome inicijuoti 
// Person poobjekti.Teisingam kopijos konstruktorius reikalingas
// nedidelis triukas - ir tai ne kas kita o pasinaudojimas upcasting'u!
// Student::Student (const Student& s) :Person(s),enterdate(s.enterdate) {
// Dabar spausdins
/*
Person or Person's child: 1980.7.15 FX
Person or Person's child: 1900.1.1 JK
*/
// Ir vis viena islieka problema. upcating'as gerai, bet po upcasting'o
// studentas neturetu nustoti buti studentu ir pasizymeti tik tuo kuo
// pasizymi "banali" asmenybe. Juk visi musu 'spausdinimai' - tai
// Person print_info(), o ne Student.
// Reikalingas mechanizmas(priemones) kartu su objektu nurodyti ir jo
// priklausomybe tipui - virtualios funkcijos - polimorfizmo C++
// nesejos.
}
