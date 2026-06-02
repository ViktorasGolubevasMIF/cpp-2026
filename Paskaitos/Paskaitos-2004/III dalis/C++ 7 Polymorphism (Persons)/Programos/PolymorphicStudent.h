// PolymorphicStudent.h
#include "PolymorphicPerson.h"

class Student: public Person {
	Student& operator=(const Student&);
protected:
	Date enterdate;
	char* college;
public:
	Student (int=1900,int=1,int=1,char* = "JK",
                 int=1918,int=1,int=1,char* = "VU");
	Student (Date,char*,Date,char*);
	virtual ~Student ();
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
	int get_enteryear () const {return enterdate.get_year();}

	void print_info(char*) const;
	void print_this() const; 

	virtual void print_test() const;
};
