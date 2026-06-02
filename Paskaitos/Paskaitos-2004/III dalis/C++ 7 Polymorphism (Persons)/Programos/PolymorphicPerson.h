// Person.h
#include "Date.h"

class Person {
	Person& operator=(const Person&);
protected:
	Date birthdate;
	char* name;
public:
	Person (int=1900,int=1,int=1,char* = "JK");
	Person (Date,char*);
	virtual ~Person ();
	Person (const Person&);

	Person& set_name(char*);
	Person& set_birthyear (int);
	Person& set_birthmonth (int);
	Person& set_birthday (int);
	int get_birthyear () const;
	int get_birthmonth () const;
	int get_birthday () const;
	char* get_name() const;

	virtual void print_info(char*) const;
	virtual void print_this() const; 
};

