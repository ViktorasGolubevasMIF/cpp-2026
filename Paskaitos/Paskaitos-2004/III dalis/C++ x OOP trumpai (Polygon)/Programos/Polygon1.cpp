#include <iostream.h>

// Klasës=Vartotojo tipo (Duomenys+Funkcijos) apibrëþimas 
class CRectangle {
  private:   // privati (ne klasës nariams nepasiekiama) sekcija= realizacija
    int x, y;				   // duomenys nariai= atributai
  
  public:	   // vieðoji (pasiekiama klasës iðorëje) sekcija=interfeisas
    void set_values (int,int); // funkcijos nariai= metodai
    int area (void);		 // metodø apraðai arba apibrëþimai(þr.þemiau)
};

// Klasës metodø apibrëþimas
void CRectangle::set_values (int a, int b) {
  x = a;
  y = b;
}

int CRectangle::area (void) {
  return (x*y);
}

// Pagrindinë programa
int main () {
  CRectangle rect;		// objekto= klasës egzemplioriaus sukûrimas

  rect.set_values (3,4);			// metodø iðkvietimas=
  cout << "area: " << rect.area();	// praneðimø objektui perdavimas
}
