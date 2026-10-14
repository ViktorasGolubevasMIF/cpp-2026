// StudentDriver4Upcast.cpp (and Slice) 

#include <iostream.h>
#include "Student.h"
void o_print(Person p) {
 p.print_info("fo - Person: ");cout << endl;}
void p_print(Person* p) {
 p->print_info("fp - Person or  Person's child?: ");cout << endl;}
void r_print(Person& p) {
 p.print_info("fr - Person or Person's child?: ");cout << endl;}

int main() {
 Student s4(1980,7,15,"FX",1999,9,1,"VU");
 Person o_Person=s4;
 Person* p_Person=&s4;
 Person& r_Person=s4;
 s4.print_info("ao - Student: ");cout << endl;
 o_Person.print_info("ao - Person: ");cout << endl;
 p_Person->print_info("ap - Person or  Person's child?: ");cout << endl;
 r_Person.print_info("ar - Person or  Person's child?: ");cout << endl;
 o_print(s4);
 p_print(&s4);
 r_print(s4);
 // 'upcating'as galioja ir grazinamom funkciju reiksmem.

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
        1980.7.15       : 0012FF5C copy of 0012FF6C
 Person(const Person&)
        1980.7.15 FX    : 0012FF5C copy of 0012FF6C
ao - Student: 1980.7.15 FX 1999.9.1 VU
ao - Person: 1980.7.15 FX
ap - Person or  Person's child?: 1980.7.15 FX
ar - Person or  Person's child?: 1980.7.15 FX
 Date(const Date&)
        1980.7.15       : 0012FF1C copy of 0012FF6C
 Person(const Person&)
        1980.7.15 FX    : 0012FF1C copy of 0012FF6C
fo - Person: 1980.7.15 FX
~Person()
        1980.7.15 FX    : 0012FF1C
~Date()
        1980.7.15       : 0012FF1C
fp - Person or  Person's child?: 1980.7.15 FX
fr - Person or Person's child?: 1980.7.15 FX
~Person()
        1980.7.15 FX    : 0012FF5C
~Date()
        1980.7.15       : 0012FF5C
~Student()
        1980.7.15 FX 1999.9.1 VU        : 0012FF6C
~Date()
        1999.9.1        : 0012FF7C
~Person()
        1980.7.15 FX    : 0012FF6C
~Date()
        1980.7.15       : 0012FF6C
*/

// Isitikinome, kad upcasting'as veikia ir tiesiog objektams, o ne
// vien rodyklems ar nuorodoms. Taciau svarbu pastebeti, kad skirtingai
// nuo rodykliu, kur priskiriami adresai, 'Student' perduotas pagal
// reiksme kaip 'Person' arb atiesio priskirtas 'Person' yra 
// "apkarpomas" (slice) iki 'Person'!

// Ir vis viena islieka problema. Po 'upcasting'o panaudojimo
// 'Student' neturetu nustoti buti studentu ir pasizymeti tik tuo kuo
// pasizymi "nuoga" 'Person'. Juk visi panudoti "spausdinimai" - tai
// kreipiniai i Person::print_info(), o ne i Student::print_info().
// Reikalingas mechanizmas(priemones) kartu su objektu nurodyti ir jo
// priklausomybe tipui - virtualios funkcijos - polimorfizmo C++
// nesejos.
}
