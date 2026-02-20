#include <iostream.h>

enum Color {Yellow,Green,Red,Blue};

class CPolygon {
  protected:	// prieinami vaikams (public), bet nematomi isoreje (private)
    int width, height;	
  public:
    void set_values(int a, int b) { width = a; height = b;}
    void get_values(int& a, int& b) const { a = width; b = height;}
};

class CRectangle : public CPolygon { // paveldejimas (isvestine : bazine)
  protected:
    Color color;	
  public:

    int area (void) {return (width*height); }
    void  set_color(Color c) {color = c;}
    Color get_color() const  {return color;}
};

int main () {
  int x,y;
  CPolygon poly;	// Objektai sukuriami naudojant KPN
  CRectangle flag;	// toks sunaus sukurimas imanomas jei tevas turi KPN 
  
  poly.get_values(x,y);
  flag.set_values(9,6); 
  flag.set_color(Blue); 
  cout << "poly width is " << x << " and height is " << y << endl;
  cout << "flag area is " << flag.area() << endl;
  cout << "flag color is " << flag.get_color() << endl;
}
