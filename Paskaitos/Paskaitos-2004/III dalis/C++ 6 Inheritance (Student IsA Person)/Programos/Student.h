// Student.h
#include "Person.h"

class Student: public Person {
//protected:
private:
	Date enterdate;
	char* college;
//	Student (const Student&);
	Student& operator=(const Student&);
public:
	Student (int=1900,int=1,int=1,char* = "JK",
                 int=1918,int=1,int=1,char* = "VU");
	Student (Date,char*,Date,char*);
	~Student ();
	Student (const Student&);
/*
	Student& set_college(char*);
	Student& set_enteryear (int);
	Student& set_entermonth (int);
	Student& set_enterday (int);
	int get_entermonth () const;
	int get_enterday () const;
	char* get_college() const;
*/
//	int get_enteryear () const {return enterdate.y;}
// Error : 'Date::y' is not accessible in function Student::get_enteryear() const
	int get_enteryear () const {return enterdate.get_year();}


//!!! Perrasomas (ne perkraunamas) 'Person' metodas - sutampa signaturos
//    angl. override, redefine, bet ne overload
	void print_info(char*) const;
//    Taip pat perrasomas metodas
	void print_this() const; 

	void print_test() const;
};

/*
Specialus metodai: konstruktorius, kopijos konstruktorius,
kopijos priskyrimo operacija, destruktorius - nepaveldimi!
*/