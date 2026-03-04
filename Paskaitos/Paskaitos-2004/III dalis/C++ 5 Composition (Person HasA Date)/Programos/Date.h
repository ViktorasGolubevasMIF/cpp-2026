// Date.h

class Date {
	int y,m,d;
public:
	Date (int=1,int=1,int=1);
//	Date (int,int,int);
	Date (const Date&);
	Date& operator=(const Date&);
	~Date ();
	void set_date (int,int,int);
	void set_year (int);
	void set_month (int);
	void set_day (int);
	void get_date (int&,int&,int&) const;
	int get_year () const;
	int get_month () const;
	int get_day () const;
	Date& add_year1 (int);
	Date add_year2 (int);
};

