#include <iostream.h>

class CRectangle {
    int width, height;					// pagal nutylëjimà private
  public:
    CRectangle (int,int);				// KONSTRUKTORIUS
    int area (void) {return (width*height);} // áterptinë funkcija
};

CRectangle::CRectangle (int a, int b) {
  width = a;
  height = b;
}

int main () {
  CRectangle rect (3,4);		// (Parametrizuotas) Objektu konstravimas
  CRectangle rectb (5,6);	// Kas sukonstravo objekta pirmame pavyzdyje?
  cout << "rect area: " << rect.area() << endl;	// cout << vietoj printf()
  cout << "rectb area: " << rectb.area() << endl;	// cin  >> vietoj scanf()
}
