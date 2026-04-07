// Driver.cpp 
#include <iostream.h>
#include "Person1.h"
//#include "Date.h"	//daugybiniu include'u (aprasu) pavojus!

int main() {
// Kai Person ir Date turi KPN
//	Person p1;
/*
Constructor Date(int,int,int) : 1.1.1 at 0012FF7C
Constructor Person(int,int,int,char*) : JK was born in 1.1.1 at 0012FF7C
Destructor ~Person() : JK at 0012FF7C bye-bye...
Destructor ~Date() : 1.1.1 at 0012FF7C
*/

// Kai Date neturi KPN 
/*
Cannot find default constructor to initialize member 'birthdate' 
in function Person::Person(int,int,int,char *)
*/

// Tokiu atveju 'Person' privalo visuose(!) konstruktoriuose 
// tiesiogiai inicijuoti 'birthdate' bet ne konstruktoriaus kune.
// 'birthdate' kaip poobjektis turi buti sukurtas pries viso
// objekto ('Person') sukurima - o tam tinka tik inicijavimo sarasas.

// Taigi, konstantas ir nuorodas papildo poobjekciai, neturintys KPN.
// Netgi, jei 'Date' turetu KPN, bet 'birthdate' butu const
// taip pat neapseitume be konstruktoriaus inicijavimo saraso.

	Person p2;	// Kai Person turi KPN
	Person p3(1964,9,19,"VG");
/*
Constructor Date(int,int,int) : 1.1.1 at 0012FF7C
Constructor Person(int,int,int,char*) : JK was born in 1.1.1 at 0012FF7C
Constructor Date(int,int,int) : 1964.9.19 at 0012FF6C
Constructor Person(int,int,int,char*) : VG was born in 1964.9.19 at 0012FF6C
Destructor ~Person() : VG at 0012FF6C bye-bye...
Destructor ~Date() : 1964.9.19 at 0012FF6C
Destructor ~Person() : JK at 0012FF7C bye-bye...
Destructor ~Date() : 1.1.1 at 0012FF7C
*/

// Tiesa, esant const 'birthdate', datu set'eriai yra klaidingi.
//	p2.birthdate.set_year(2000);
/*
'Person::birthdate' is not accessible in function main()
*/
	p2.set_birthyear(2000);
	return 0;
/*
Destructor ~Person() : JK at 0012FF7C bye-bye...
Destructor ~Date() : 2000.1.1 at 0012FF7C
*/
}
