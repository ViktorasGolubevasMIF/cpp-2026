// Date1.c 

// Klase - tai vartotojo apibreziamas tipas (VAT).
// Tipai turi funkcionaluma, t.y. tipo egzempliorius,
// kuris vadinamas objektu, pasizymi funkcionalumu.
// C VAT'ai, kuriami 'struct' (strukturu) konstrukcijos pagalba.
// Si konstrukcija igalino duomenu(!) agregavima i 
// "sudetingesni" tipa, ir tiek. Tokiu nefunkcionaliu
// tipu funkcionalumas buvo apibreziamas isoriniu 
// C funkciju pagalba, perduodant "nefunkcionalius objektus"
// kaip parametrus.

// C++ kardinaliai pasuka link "teisingo" tipo suvokimo.
// Tipas (sintaksiskai kol kas struktura, o savokiskai
// klase) - tai ne tik duomenys (skaityk tipo duomenu apibrezimo
// sritis), bet ir funkcijos operuojancios tais duomenimis
// (skaityk operaciju rinkinys virs duomenu apibrezimo srities).
// Ir visa tai C++ igalina aprasyti kaip vientisa konstrukcija.
// Dabar VAT nariai ne tik duomenys bet ir funkcijos

// Taigi, 'Klase'='Duomenys nariai' + 'Funkcijos nariai'

// Iliustratyvumo sumetimais kol kas apribokime gana primityvaus
// tipo 'Data' funkcionaluma keliomis savybemis (funkcijomis).
// Taciau viena is ju yra fundamentali - objektu inicijavimo
// veiksmas. Toks veiksmas yra neisvengiamas nepriklausomai
// nuo tipo, kuriam priklauso objektas. Tiesiog, programuodami
// daznai apie ji uzmirstam. Sistema (kompiliatorius) si veiksma
// ivykdo netiesiogiai. Tiesa, yra priemones tiesioginiam 
// objekto (prisiminkime baziniu tipu kintamuosius) inicijavimui, pvz.:

// int i1;
// i1 = 1;
// int i2=2;

// Svarbu pastebeti, kad sukurimo ir inicijavimo veiksmai 
// sintaksiskai gali buti tiek kartu, tiek atskirai (nors tokiu
// atveju ivyksta netiesioginis-nutylimasis inicijavimas paprastai
// nezinoma reiksme, o antrasis veiksmas vadinamas reiksmes
// priskyrimo veiksmu).
// Bet konceptualiai sukurimas be inicijavimo neimanomas.
// Maza to, inicijavimas suprantamas, kaip samoningas, isreikstinis,
// zinomas, suvokiamas veiksmas.
// Taigi, suvokdami, kad toks veiksmas privalomas (nors ir 
// programiskai nebutinas) aprasome funkcija-nari - init_date().

struct Date {
	int y,m,d;
//	void add_year(int n);
	Date& add_year(int n);
//	void add_month(int n);
//	void add_day(int n);

	void init_date(int yy, int mm, int dd) {
		y=yy;m=mm;d=dd; }
};
// Klases aprase apibreziamos funkcijos-nariai (klases metodai)
// pagal nutylejima yra 'inline'.
// Jeigu f-n apibreziame uz klases apraso ribu, privalome nurodyti
// klases varda su konteksto operacija, nes skirtingos klases
// gali tureti funkcijas-narius vienodais vardais.

// void Date::add_year(int n) {y+=n;}

// Date::add_year() vadinamas kvalifikuotu add_year() vardu.
// Jeigu norime isoreje apibreziamas f-n'ius padaryti 'inline'
// (paprastai, efektyvumo sumetimais), rasome
//inline void Date::add_month(int n) {m+=n;}

Date& Date::add_year(int n) {
	y+=n;
	return *this;
}

//--------------------------------------------------------
#include <iostream.h>
int main() {
	
	Date today;

// Sukureme objekta iprastom kintamuju skelbimo (apibrezimo)
// priemonem.
// Taciau inicijavimas netiesioginis, t.y. mums nezinomas.
// Turime (nors neprivalome) inicijuoti sukurta objekta, 
// kreipdamiesi i tam tikslui musu apsibrezta tipo, 
// kuriam priklauso objektas, funkcija.
// Po procedurinio programavimo tai turi atrodyti neiprastai,
// nors kreipinio i strukturinio kintamojo nari konstrukcija
// pazistama is C.
	
	today.init_date(1999,9,9);

// !!! Objektui yra pasakoma ka daryti (kuria funkcija nari
// pritaikyti), o ne funkcijai (isorinei tipo atzvilgiu) 
// pasakoma su kuriuo objektu dirbti, pvz.:
// init_date(&today,1999.9,9);
// Kreipinys i funkcijos-nari (per objekta) dar vadinamas 
// pranesimo objektui perdavimas.

	Date tomorrow;
	tomorrow.init_date(1999,9,9);
	tomorrow.add_year(1);
	tomorrow.add_year(1).add_year(1);
	cout << tomorrow.y << endl;
}
// Klausimas, kuris kamuoja naujokus.
// Kad objektai today ir tomorrow turi savo nuosavus(!)
// duomenis (angliskai, dar vadinamus data memeber,
// arba object instances(?)) tai aisku, bet ar turi
// objektai nuosavas funkcijas?
// Perfrazuojant: Ar today turi 'savyje' init_date(),
// o tomorrow turi 'savyje' init_date() (daznai 'savyje'
// interpretuojamas fiziskai, t.y. po viena funkcijos egzemplioriu
// kiekvienam objektui)? 
// Sunkus klausimas - sunkus atsakymas...
// Greiciau neturi, negu turi.
// Filosofiskai, gal ir galima sakyti, kad objekto sugebejimas
// kazka daryti yra jo nuosavybe, bet konceptualiai tai, ko gero,
// netiesa. Identisku(!), t.y. tokiu pat sugebejimu pasizymi ir kitas
// to paties tipo (tos pacios rusies, klases...) objektas
// (egzempliorius, atstovas, individas). Skirtinga(!) skirtingu
// objektu elgsena (C++ - to paties pranesimo perdavimas arba
// funkcijos nario iskvietimas skirtingiem objektam) naudojantis 
// auksciau minetu identisku sugebejimu (C++ - tai klases 
// funkcija-narys, kitaip metodas) salygoja tiek isoriniai
// veiksniai (C++ - pranesimo, t.y. kvieciamo metodo
// parametru reiksmes), tiek vidines (asmenines, 'savo')
// objekto savybes (bruozai, charakteristikos) bei busena (statusas)
// (C++ - to objekto duomenu-nariu reiksmes).
// Technologiskai, kompiliatorius tikrai sukuria viena funkcijos-nario
// egzemplioriu, taciau, kaip sako Stroustrup'as, ta funkcija-narys
// visuomet "zino" kuriam objektui ji yra kvieciama ir net gali
// i ji kreiptis.
// Kiekviena (uzbegant i prieki, nestatine) f-n'ys turi paslepta 
// parametra 'this', kuris suprantamas, kaip rodykle i objekta,
// kuriam f-n'ys yra iskviestas, t.y. lyg ir kiekvieno f-n'io
// aprasas pasipildo netiesioginiu parametru 'this', pvz.:
// void add_year(Date* this,int n);

// Taigi, C++ turi bazine konstrukcija 'this', kuri naudojama
// f-n'iuose ir kuria priklausomai nuo konteksto galima interpretuoti
// "mano", "savo", "sio", "paties",...o techniskai teisingiausia butu 
// "tas/to objekto, kuris mane iskvies". Juk aprasant f-n'ius (grieztai
// tariant aprasant tipa), dar nera "gyvo" ne vieno tipo (klases)
// egzemplioriaus.
// Idomu, kad bet kuris kreipinys i (nestatini) klases nari
// netiesiogiai naudoja 'this', pvz.:
// void add_year(int n) {this->d+=n;}
// arba
// void add_year(int n) {(*this).d+=n;}
// (cia 'this'->d galima interpretuoti kaip "to objekto kuris 
// mane iskvies lauka d padidinti skaiciumi n")
// 'this' nera paprastas kintamasis: negalima suzinoti jo adreso,
// ir negalima jam priskirti (konstantine rodykle, t.y. Date* const this).
// Si konstrukcija butina, jei norima, kad funkcija-narys grazintu
// objekta su kuriuo ji dirba.
// zr. Date& Date::void add_year(int n)
