#include <iostream.h>

class CRectangle {
    int width, height;					
  public:
    CRectangle ();					// Konstruktorius pagal nutylejima (KPN)
    CRectangle (int,int);				// Perkrautas konstruktorius (overload)
    int area (void) {return (width*height);} 
};

CRectangle::CRectangle () {
  width = 1;
  height = 1;
}

CRectangle::CRectangle (int a, int b) {
  width = a;
  height = b;
}

int main () {
  CRectangle default_rect;	// sukurimas butu neimanomas, neturint KPN
  CRectangle other_rect(5,6);
  cout << "default_rect area: " << default_rect.area() << endl;
  cout << "other_rect area: " << other_rect.area() << endl;
}
