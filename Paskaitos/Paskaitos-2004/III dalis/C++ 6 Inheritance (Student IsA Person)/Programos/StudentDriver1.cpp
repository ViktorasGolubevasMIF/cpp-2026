// StudentDriver1.cpp 

#include <iostream.h>
#include "Student.h"

int main() {

//	Student s0;	// Kai Student turi KPN
	Student s1(1964,9,19,"VG",1982,9,1,"VU");

	s1.print_test();

	cout << "Tavo vardas " << s1.get_name() << endl;
	cout << "Ka darei, kai tau buvo " 
             << s1.get_enteryear()-s1.get_birthyear() << "?" << endl;

}
/*
 Date(int,int,int)
        1964.9.19       : 0012FF6C
 Person(int,int,int,char*)
        1964.9.19 VG    : 0012FF6C
 Date(int,int,int)
        1982.9.1        : 0012FF7C
 Student(int,int,int,char*,int,int,int,char*)
        1964.9.19 VG 1982.9.1 VU        : 0012FF6C
Mano vardas VG
Kai man buvo 18...
Tavo vardas VG
Ka darei, kai tau buvo 18?
~Student()
        1964.9.19 VG 1982.9.1 VU        : 0012FF6C
~Date()
        1982.9.1        : 0012FF7C
~Person()
        1964.9.19 VG    : 0012FF6C
~Date()
        1964.9.19       : 0012FF6C
*/