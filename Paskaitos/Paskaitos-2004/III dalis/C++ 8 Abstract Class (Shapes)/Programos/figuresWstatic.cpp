#include <iostream.h>
const int MAX=3;
const double PI=3.14;

class Shape {
protected:
	int x0,y0;  // class Point
	static int nObj;
public:
	Shape(int x=0, int y=0): x0(x),y0(y)
	{
	nObj++;
	cout << "Shape (int=,int=)" << endl;
	}
	virtual ~Shape() //	~Shape()
	{cout << "~Shape ()" << endl;}
	static int get_number(){return nObj;}
	//	void set_x0(double x) {x0=x;}
	// get_x0(double& x) const {x=x0;}
	//	double get_x0() const {return center;}

	virtual double area()=0; // {return 0;}
	//virtual void draw() const = 0;
};
int Shape::nObj = 0;

class Circle: public Shape {
private:
	double radius;
public:
	Circle(int x=0, int y=0, double r=1): Shape(x,y),radius(r)
	{
	cout << "Circle (int, int, double)" << endl;
	}
	~Circle()
	{
	cout << "~Circle ()" << endl;
	}
	double area() {return (PI*radius*radius);}
};

class Rectangle: public Shape {
private:
	double height,weight;
public:
	Rectangle(int x=0, int y=0, double h=1, double w=1):
	Shape(x,y),height(h),weight(w)
	{
	cout << "Rectangle (double,double,double,double)" << endl;
	}
	~Rectangle()
	{
	cout << "~Rectangle ()" << endl;
	}
	double area() {return (height*weight);}
};

int main() {
Shape *pSh[MAX];
int count=0;
//pSh[count++]= new Shape();
pSh[count++]= new Circle();
pSh[count++]= new Rectangle();
pSh[count++]= new Rectangle(1,1,2,2);

double tot_area=0;
for ( int i=0; i<Shape::get_number() ; i++)
	tot_area += pSh[i] -> area();

cout << tot_area << endl;

for (i=0; i<Shape::get_number(); i++) 	delete pSh[i];


return 0;
}
