#include <iostream.h>

class CPolygon {
  protected:
    int width, height;	
  public:
    CPolygon (int=1, int=1);
//  Virtualiu funkciju problematika zr. zemiau
    virtual int area (void) { return 1; }
    void set_values(int a, int b) { width = a; height = b;}
    void get_values(int& a, int& b) const { a = width; b = height;}
    //virtual ~CPolygon();
};
CPolygon::CPolygon (int a, int b) { width = a; height = b;}

class CRectangle : public CPolygon {
  public:
    CRectangle (int=1, int=1);	
    int area (void) {return (width*height); }
    //virtual ~CRectangle();
};
CRectangle::CRectangle (int a, int b): CPolygon(a,b) {}

class CTriangle : public CPolygon {
  public:
    CTriangle (int a=1, int b=1) : CPolygon(a,b) {}; // inline konstruktorius
    int area (void) {return (width*height/2); }
    //virtual ~CTriangle();
};

int main () {

  CPolygon *appoly[2];
  appoly[0]  = new CRectangle(3,4) ;
  appoly[1]  = new CTriangle(5,6) ;
  
  for (int i=0; i<2; i++)
    cout << i+1 << " polygon area is " << appoly[i]->area() << endl;

  delete [] appoly;
}
