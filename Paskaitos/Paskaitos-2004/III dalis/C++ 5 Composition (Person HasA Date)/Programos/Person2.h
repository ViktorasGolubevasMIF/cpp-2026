// Person.h
#include "Date.h"

class Person {
	Date birthdate;
	char* name;
	Person (const Person&);
	Person& operator=(const Person&);
public:
	Person (Date,char* = "JK");
	~Person ();
/*
	void set_name(char*);
	char* get_name();
	void set_birthdate (int,int,int);
	void set_birthyear (int);
	void set_birthmonth (int);
	void set_birthday (int);
	void get_birthdate (int&,int&,int&) const;
	int get_birthyear () const;
	int get_birthmonth () const;
	int get_birthday () const;
*/
};

