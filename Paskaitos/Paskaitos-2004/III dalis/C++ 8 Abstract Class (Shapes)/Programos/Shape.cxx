//////////////////////////////////////////////////////////////////////////////
//
//  Abstract: Example program which demonstrates inheritance
//
//  This program demonstrates various aspects of inheritance and polymorphism.
//  It declares a hierarchy of graphical shapes which are then drawn by the
//  main() program.  However, no real drawing takes place, we only print some
//  confirmation.
//
//  For purposes of instruction, we assume that each graphical object that we
//  want to draw requires some form of graphics structure allocated by the
//  operating system.  This is to demonstrate the need for and the use of
//  virtual destructors, the "system graphics structures" are fictitious.
//
//  At the end of the main program, there is a section which shows how to use
//  Run-Time Type Information (RTTI) and the associated "typeinfo" class and
//  dynamic casts.  This new feature of ANSI C++ 3.x has not yet been implemented
//  by all compilers and is therefore currently disabled.  To enable it, change
//  the constant of the "#if" directive from a 0 to a 1.
//
//
//  Version  Date         Author            Purpose
//  1.2.1    17-Mar-1998  Thomas Wieland    Fixed problem with uninit'd array
//  1.2      10-Mar-1998  Thomas Wieland    Added code using RTTI functionality;
//                                          tested on Visual Studio 97/VC++ 5.0
//  1.1      08-Mar-1998  Thomas Wieland    Added more printouts and examples to
//                                          main(); debugged Line::get_length()
//  1.0      06-Mar-1998  Thomas Wieland    Created for class use
//
//////////////////////////////////////////////////////////////////////////////


#include <iostream.h>
#include <math.h>
#ifndef	M_PI	// not in MS VC++ math.h!!
#define M_PI    3.14159265358979323846
#endif


//
// A simple point in 2D space (used by Shape's descendants);
// also demonstrates definition of the copy constructor and
// overloaded assignment and output operators.
//

class Point
{
public:

  Point()  { xc = yc = 0.0; }
  Point(float x, float y)  { xc = x; yc = y; }

  // copy constructor (not really necessary here)
  Point(const Point& p)  { xc = p.xc; yc = p.yc; }

  // assignment operator (not really necessary here)
  Point& operator=(const Point& p);
  // { if (this == &p) return *this; else { xc = p.xc; yc = p.yc; } }

  // this is how we print a Point
  friend ostream& operator<<(ostream& os, const Point& p);

  float get_x()  { return xc; }
  float get_y()  { return yc; }
  void  set_x(float x)  { xc = x; }
  void  set_y(float y)  { yc = y; }

private:

  float xc, yc;  // x and y coordinates

  // ....
};


Point& Point::operator=(const Point& p)
{
  if (this != &p)
  { xc = p.xc; yc = p.yc; }

  return *this;
}

ostream& operator<<(ostream& os, const Point& p)
{
  os << "(" << p.xc << "," << p.yc << ")";

  return os;
}


//
// Dummy declaration for "system" graphics datatypes; these
// would normally be allocated through a operating system call
//
typedef int SysGraphData;
typedef int SysGraphLine;
typedef int SysGraph2dData;
typedef int SysGraphCircle;


//
// A shape - any shape
//

class Shape
{
public:

  Shape() { drawdata = new SysGraphData; }  // system dependent
  virtual ~Shape() { delete drawdata; }     // clean up

  virtual void draw() =0;
  virtual void erase() =0;

private:

  SysGraphData* drawdata;

  // ....
};


//
// A line - a 1-dimensional object
//

class Line : public Shape
{
public:

  Line()  // default: return line of unit length along x-axis
  { start = Point(0.0, 0.0);  end = Point(1.0, 0.0);
    linedata = new SysGraphLine; }  // for line drawing

  Line(Point& s, Point& e)
  { start = s;  end = e; linedata = new SysGraphLine; }

  virtual ~Line()  { delete linedata; }  // clean up

  virtual void draw()  { cout << "Drawing Line: " << start << "-" << end << endl; }
  virtual void erase() { cout << "Erasing Line: " << start << "-" << end << endl; }

  float get_length();

private:

  Point  start, end;  // start & end points of the line

  SysGraphLine* linedata;

  // ....
};

float Line::get_length()
{
  // Calculate differences in double to prevent round-off errors
  double tmp_x = (double)end.get_x()-(double)start.get_x();
  double tmp_y = (double)end.get_y()-(double)start.get_y();

  return((float)(sqrt(tmp_x*tmp_x+tmp_y*tmp_y)));
}


//
// A 2D shape - any 2D shape
//

class Shape2D : public Shape
{
public:

  Shape2D()  { drawdata2d = new SysGraph2dData; } // system dependent
  virtual ~Shape2D() { delete drawdata2d; }       // clean up

  virtual void draw() =0;
  virtual void erase() =0;
  virtual float get_area() =0;    // return area of 2D shape
  virtual float get_circum() =0;  // return circumference of 2D shape

  const Point& get_center()  { return center; }
  void  set_center(float cx, float cy)  { center = Point(cx, cy); }

protected:

  Point center;  // center point of shape

private:

  SysGraph2dData* drawdata2d;

  // ....
};


//
// A circle
//

class Circle : public Shape2D
{
public:

  Circle()  // default: circle around origin, r = 1.0
  { center = Point(0.0, 0.0);  radius = 1.0;
    circledata = new SysGraphCircle; }  // system dependent

  Circle(const Point& c, float r = 1.0)
  { center = c; radius = r;
    circledata = new SysGraphCircle; }

  virtual ~Circle()  { delete circledata; }       // clean up

  virtual void draw()  { cout << "Drawing Circle: " << center << ", r=" << radius << endl; }
  virtual void erase() { cout << "Erasing Circle: " << center << ", r=" << radius << endl; }

  virtual float get_area()   { return (float)(M_PI * radius * radius); }
  virtual float get_circum() { return (float)(2 * M_PI * radius); }

  float get_radius()         { return radius; }
  void  set_radius(float r)  { radius = r; }

private:

  float radius;  // radius of circle

  SysGraphCircle* circledata;

  // ....
};


//
// Program to exercise shapes
//

// Needed for RTTI only (see below)
#if 0
#include <typeinfo.h>
#endif

const int MaxShapes = 10;

void main(void)
{
  // these are ILLEGAL: cannot instantiate abstract classes
  // Shape s;
  // Shape2D s2;

  Point p1(1.0, 2.0);
  Point p2 = Point(3.0, 4.0);

  Line  l1;
  Line  l2(Point(1.0,0.0), Point(0.0,1.0));
  Line* l_ptr;

  Circle  c1 = Circle(Point(2.0,2.0), 2.0);
  Circle* c_ptr;

  // POINTERS to objects of an abstract class are OK
  Shape* s_ptr;
  Shape* my_shapes[MaxShapes] = { 0,0,0,0,0,0,0,0,0,0 };

  // populate the array with some (pointers to concrete) shapes
  my_shapes[0] = &l1;
  my_shapes[1] = &c1;
  
  my_shapes[2] = new Line;
  my_shapes[3] = new Circle;

  my_shapes[4] = new Line(p1, p2);
  my_shapes[5] = new Circle(p1);
  my_shapes[6] = new Circle(p2, 2.5);

  // draw all the shapes in the array
  for (int i=0; i < MaxShapes; i++)
  {
      if (my_shapes[i])
		  my_shapes[i] -> draw();
  }

  // always safe (normal member function invocation)
  cout << "line length l2 = " << l2.get_length() << endl;
  cout << "circle area c1 = " << c1.get_area() << endl;

  // Illegal: class Shape does not define member function get_length()
  // s_ptr = my_shapes[0];
  // s_ptr->get_length();

  // WARNING: UNSAFE!  Must be sure it's a Line!!!
  l_ptr = (Line*) my_shapes[0];
  cout << "line length = " << l_ptr->get_length() << endl;

  // DANGEROUS & UNSAFE: results undefined (may crash program)
  // c_ptr = (Circle*) my_shapes[0];
  // cout << "circle area = " << c_ptr->get_area() << endl;

#if 0
  //
  // Run-Time Type Information (RTTI)
  //
  // Note: This will only work on compilers that support RTTI as defined
  //       in the ANSI C++ 3.x standard.  Both Microsoft Visual C++ 5.0
  //       and Borland C++ 5.0 support this, many (most?) Unix C++ compilers,
  //       with the possible exception of GNU g++, do not (yet).
  //
  // Visual C++ 5.0 compiler option: /GR (Enable RTTI)
  //   
  // Borland C++ 5.0 compiler option: -RT (Enable RTTI)
  //


  // dynamic cast: succeeds if pointer/reference is compatible, otherwise
  //               NULL (for pointer) or bad_typeid exception (for reference)
  if (l_ptr = dynamic_cast<Line*>(my_shapes[0]))
  {
     cout << "dynamic_cast succeeded: we have a Line!\n";
     cout << "line length = " << l_ptr->get_length() << endl;
  }
  else
     cout << "dynamic_cast FAILED: not a Line!!!\n";

  // typeid() returns a reference to class information
  // type_info class holds information on the class of an object
  const type_info& shape_info = typeid(s_ptr);
  const type_info& line_info  = typeid(l_ptr);

  // type_info allows to print out the name of a class
  cout << "s_ptr is of type: " << shape_info.name() << endl;
  cout << "l_ptr is of type: " << line_info.name() << endl;

  // type_info also defines equality (==) and inequality (!=)
  if (shape_info == line_info)
     cout << "s_ptr and l_ptr are of the same class\n";
  else
     cout << "s_ptr and l_ptr are of different classes\n";

#endif
}

/******** Output  *********

Drawing Line: (0,0)-(1,0)
Drawing Circle: (2,2), r=2
Drawing Line: (0,0)-(1,0)
Drawing Circle: (0,0), r=1
Drawing Line: (1,2)-(3,4)
Drawing Circle: (1,2), r=1
Drawing Circle: (3,4), r=2.5
line length = 1
line length l2 = 1.41421
circle area c1 = 12.5664

// with RTTI enabled only:
dynamic_cast succeeded: we have a Line!
line length = 1
s_ptr is of type: class Shape *
l_ptr is of type: class Line *
s_ptr and l_ptr are of different classes

**************************/

