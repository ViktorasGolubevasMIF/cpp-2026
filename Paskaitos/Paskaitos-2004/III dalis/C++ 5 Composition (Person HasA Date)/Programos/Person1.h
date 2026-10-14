// Person1.h
#include "Date.h"

class Person {
//	Date birthdate;
	const Date birthdate;
	char* name;
	Person (const Person&);
	Person& operator=(const Person&);
public:
	Person (int=1,int=1,int=1,char* = "JK");
//	Person (int,int,int,char*);
	~Person ();

	void set_name(char*);
	char* get_name() const;

	void set_birthdate (int,int,int);
	void get_birthdate (int&,int&,int&) const;

	void set_birthyear (int);
	void set_birthmonth (int);
	void set_birthday (int);
	int get_birthyear () const;
	int get_birthmonth () const;
	int get_birthday () const;
};

