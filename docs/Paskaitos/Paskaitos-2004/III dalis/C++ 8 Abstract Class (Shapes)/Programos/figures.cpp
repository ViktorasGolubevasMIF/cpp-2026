#include <iostream.h>
const double PI=3.14;

class Shape {
protected:
	double x0,y0;
public:
	Shape(double x, double y): x0(x),y0(y){cout<<"Shape"<<endl;}
	virtual ~Shape(){cout<<"~Shape"<<endl;}
//	~Shape(){cout<<"~Shape"<<endl;}
	void set_center(double x,double y) {x0=x;y0=y;}
	void get_center(double& x,double& y) const {x=x0;y=y0;}
	virtual double area() const = 0; 
};
class RegularShape: public Shape {
protected:
	double radius;
public:
	RegularShape(double x, double y, double r):
		Shape(x,y),radius(r){cout<<"RegularShape"<<endl;}
	virtual ~RegularShape(){cout<<"~RegularShape"<<endl;}
//	~RegularShape(){cout<<"~RegularShape"<<endl;}
	double get_radius() {return radius;}
	void set_radius(double r) {radius=r;}
};
class Circle: public RegularShape {
public:
	Circle(double x=0, double y=0, double r=1):
		RegularShape(x,y,r){cout<<"Circle"<<endl;}
	~Circle(){cout<<"~Circle"<<endl;}
	double area() const {return radius*radius*PI;}
};
class Square: public RegularShape {
public:
	Square(double x=0, double y=0, double r=1):
		RegularShape(x,y,r){cout<<"Square"<<endl;}
	~Square(){cout<<"~Square"<<endl;}
	double area() const {return radius*radius*2;}
};

int main() {
Shape* ap_Shape[4];
//ap_Shape[0]= new RegularShape;
/*
Error : Cannot create instance of abstract class 'RegularShape'
*/
ap_Shape[0] = new Circle;
ap_Shape[1] = new Square;
ap_Shape[2] = new Circle(0,0,2);
ap_Shape[3] = new Square(0,0,2);

for (int i=0; i<4 ; i++) cout << ap_Shape[i]->area() << endl;
for (int i=0; i<4; i++) delete ap_Shape[i];
ap_Shape[0]->set_center(1,1);
ap_Shape[0]->get_radius();
return 0;
}
