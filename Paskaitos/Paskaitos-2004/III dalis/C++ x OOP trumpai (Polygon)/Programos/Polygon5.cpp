#include <iostream.h>

class CPolygon {
  protected:
    int width, height;	
//  Galima prideti kitas objekto savybes, pvz. Color color;	
  public:
    CPolygon (int=1, int=1);	  // parametrizuotas KPN
    int area (void) { return 1; } // konkretizuoti neimanoma
    void set_values(int a, int b) { width = a; height = b;}
    void get_values(int& a, int& b) const { a = width; b = height;}
};
CPolygon::CPolygon (int a, int b) { width = a; height = b;}

class CRectangle : public CPolygon {
  public:
    // parametrizuotas konstruktorius pagal nutylejima
    // Svarbu - spec.metodai (K, D, PO) ir draugai nepaveldimi
    CRectangle (int=1, int=1);	
    // metodo perrasymas (override) isvestineje klaseje
    int area (void) {return (width*height); }
};
// Isvestines klases objektas poobjekciu turi tevines klases objekta,
// o poobjekciu konstravimas vyksta visu pirma - reikalingas mechanizmas -
// tai konstruktoriaus inicijavimo sarasas
CRectangle::CRectangle (int a, int b): CPolygon(a,b) {}

class CTriangle : public CPolygon {
  public:
    CTriangle (int a=1, int b=1) : CPolygon(a,b) {}; // inline K
    int area (void) {return (width*height/2); }
};

int main () {
// Visi daugiakampiai sudaro is CPolygon paveldeta objektu (klasiu) seima,
// todel noretusi juos valdyti kazkokiu bendru mechanizmu.
// Tas mechanizmas - bazines klases rodykle, nes C++ leidzia isvestiniu
// klasiu rodykles (ir ne tik) priskirti tevines klases rodyklems (upcasting)

  CPolygon *ppoly1 = new CPolygon;
  CPolygon *ppoly2 = new CRectangle(3,4);
  
  cout << "Polygon area is " << ppoly1->area() << endl;
  cout << "Polygon(Rectagle) area is " << ppoly2->area() << endl;

  CRectangle *prect = new CRectangle(5,6);
 
  cout << "Rectagle area is " << prect->area() << endl;

}
