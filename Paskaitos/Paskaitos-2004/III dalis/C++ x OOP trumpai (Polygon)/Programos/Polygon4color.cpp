#include <iostream.h>

enum Color {Yellow,Green,Red,Blue};
class CPolygon {
  protected:	// prieinami vaikams (kaip public), bet nematomi isoreje (kaip private)
    int width, height;	
    Color backcolor;	
  public:
    CPolygon (int=1,int=1,Color=Blue);
    int area (void) {return 1; }
    void  set_backcolor(Color c) {backcolor = c;}
    Color get_backcolor() const  {return backcolor;}
};

CPolygon::CPolygon (int a, int b, Color c) {
  width = a;
  height = b;
  backcolor = c;
}



class CRectangle : public CPolygon {
    Color starcolor;	
  public:
    CRectangle (int=1, int=1, Color=Blue, Color=Yellow);
    int area (void) {return (width*height); }
    void  set_starcolor(Color c) {starcolor = c;}
    Color get_starcolor() const  {return starcolor;}
};

CRectangle::CRectangle (int a, int b, Color c, Color s):
    CPolygon(a,b,c), starcolor(s) {}

int main () {
  CPolygon poly;
  CRectangle flag;
 
  cout << "poly area is " << poly.area() << endl;
  cout << "flag color is " << flag.get_backcolor() << " with " << flag.get_starcolor() << " stars" << endl;

}
