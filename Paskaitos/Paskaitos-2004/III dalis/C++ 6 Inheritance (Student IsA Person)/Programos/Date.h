// Date.h

class Date {
	int y,m,d;
public:
	Date (int=1900,int=1,int=1);
//	Date (int,int,int);
	Date (const Date&);
	~Date ();
	Date& set_year (int);
	Date& set_month (int);
	Date& set_day (int);
	int get_year () const;
	int get_month () const;
	int get_day () const;
	void print_info(char*) const; 
	void print_this() const; 
};

