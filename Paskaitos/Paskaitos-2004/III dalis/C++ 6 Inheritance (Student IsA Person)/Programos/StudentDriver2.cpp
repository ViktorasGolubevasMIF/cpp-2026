// StudentDriver2.cpp 

#include <iostream.h>
#include "Student.h"

int main() {

	Student s2(Date(1964,9,19),"VG",Date(1982,9,1),"VU");
}
/*
 Date(int,int,int)
        1982.9.1        : 0012FF38
 Date(int,int,int)
        1964.9.19       : 0012FF28
 Date(const Date&)
        1964.9.19       : 0012FEE0 copy of 0012FF28
 Date(const Date&)
        1964.9.19       : 0012FF6C copy of 0012FEE0
 Person(Date,char*)
        1964.9.19 VG    : 0012FF6C
~Date()
        1964.9.19       : 0012FEE0
 Date(const Date&)
        1982.9.1        : 0012FF7C copy of 0012FF38
 Student(Date,char*,Date,char*)
        1964.9.19 VG 1982.9.1 VU        : 0012FF6C
~Date()
        1964.9.19       : 0012FF28
~Date()
        1982.9.1        : 0012FF38
~Student()
        1964.9.19 VG 1982.9.1 VU        : 0012FF6C
~Date()
        1982.9.1        : 0012FF7C
~Person()
        1964.9.19 VG    : 0012FF6C
~Date()
        1964.9.19       : 0012FF6C
*/