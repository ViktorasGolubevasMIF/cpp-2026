// Date1.cpp 

class Date {
	int y,m,d;
//	Date (); // K1b
public:
	Date (){}// K1a
//	Date (int yy, int mm, int dd) { y=yy;m=mm;d=dd;} // K2
//	Date (int yy) { y=yy;} // K3a
//	explicit Date (int yy) { y=yy;} // K3b
//	Date (int yy=1, int mm=1, int dd=1); // K4
	~Date (){}
};

/*
Date::Date (int yy, int mm, int dd) {
  y=yy;m=mm;d=dd;
}
*/
//--------------------------------------------------------------------

int main() {

// NAK
//Date d0;

// Viesas "tuscias" (aprasytas, bet neapibreztas) K1a veikia kaip NAK
// ir yra KPN, todel veiks
//Date d11;

// Privatus K1b uzdraudzia isoriniame (negiminingame ir nedraugiskame)
// kontekste kurti sios klases objektus  
//Date d12;
//'Date::Date()' is not accessible in function main()

// K. su parametrais K2 (inline realizacija)
//Date d21(1999,3,31);
//Date d22=Date(1999,3,31);  //"blogas" stilius

// Jei K2 vienintelis K, tai nebeturime KPN, todel neveiks
//Date d23;
// Could not find a match for 'Date::Date()' in function main()

// K. perkrovimas, pvz. K1a, K2, K3a
//Date d31(2000);
//Date d32=Date(2000);

// ! K3a veikia kaip konvertavimo konstruktorius
//Date d33=2000;
// K3b - explicit direktyva uzdraudzia K naudoti netiesioginiam,
// konvertavmui, bet butu naudojamas d31 ir d32 sukurimui
//Cannot convert 'int' to 'Date' in function main()

// "Universalus" KPN (su visais parametrais pagal nutylejima)
Date d41;
//Date d42(2000);
//Date d43(2000,3,29);

return 0;}
