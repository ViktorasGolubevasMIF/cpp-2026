// DatePre3.c 
#include <iostream.h>

class Date {
	int y,m,d;
public:
	Date (int yy=1, int mm=1, int dd=1);
	~Date ();
};

Date::Date (int yy, int mm, int dd) {
  y=yy;m=mm;d=dd;
  cout <<"Object "<<y<<"."<<m<<"."<<d<<" at "<<this
       <<" is condtructed"<<endl;
}

Date::~Date () {
  cout <<"Object "<<y<<"."<<m<<"."<<d<<" at "<<this<<" is destructed"<<endl;
}

int main() {
Date d(2000,3,29);
Date d1=d;
Date d2;
d2=d1=d;
return 0;
}

/*
Object 2000.3.29 at 0012FF80 is condtructed
Object 1.1.1 at 0012FF68 is condtructed
Object 2000.3.29 at 0012FF68 is destructed
Object 2000.3.29 at 0012FF74 is destructed
Object 2000.3.29 at 0012FF80 is destructed
*/