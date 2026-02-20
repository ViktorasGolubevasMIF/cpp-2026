#include <iostream.h>

class CRectangle {
    int width, height;					
  public:
    CRectangle (int=1,int=1);
    				
    void set_values(int a, int b) {  width = a; height = b;}
    int area (void) {return (width*height);} 
    ~CRectangle (); // DESTRUKTORIUS – specialus klases metodas
};

CRectangle::CRectangle (int a, int b) {
  width = a;
  height = b;
  cout << "CRectangle object " << width << "," << height << " is constructed at " << this << endl;
}

CRectangle::~CRectangle () {
  cout << "CRectangle object at " << this << " is destructed" << endl;
}

int main () {
  CRectangle default_rect;
  CRectangle other_rect(3,4);

  CRectangle *ptr1 = new CRectangle;
  CRectangle *ptr2 = new CRectangle(5,6);

  ptr2->set_values(2,2);

  cout << "CRectangle object at " << ptr1 << " area is " << ptr1->area() << endl;
  cout << "CRectangle object at " << ptr2 << " area is " << ptr2->area() << endl;

  delete ptr1;
  delete ptr2;
}
