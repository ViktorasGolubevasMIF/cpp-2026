#include <iostream.h>

class CRectangle {
    int width, height;					
  public:
    CRectangle (int=1,int=1); 	// Konstruktorius pagal nutylejima (KPN)
    				// (visi parametrai pagal nutylejima)
    int area (void) {return (width*height);} 
};

CRectangle::CRectangle (int a, int b) {
  width = a;
  height = b;
}

int main () {
  CRectangle default_rect;	// vienas 'universalus' kostruktorius tinka abieju 
  CRectangle other_rect(3,4);	// objektu (nutylimo ir parametrizuoto) sukurimui

  CRectangle *ptr1 = new CRectangle;		// objektai dinamineje atmintyje
  CRectangle *ptr2 = new CRectangle(5,6);	// ju adresais inicijuojamos klases (tipo) rodykles

  ptr2->set_values(1,1);

  cout << "default_rect area: " << ptr1->area() << endl;
  cout << "other_rect area: " << ptr2->area() << endl;

  delete ptr1;
  delete ptr2;
}
