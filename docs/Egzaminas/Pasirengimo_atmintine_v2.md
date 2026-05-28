\---

# OOP C++ egzaminas — pasirengimo atmintinė

Šis sąrašas apibendrina svarbiausius kurso akcentus: dažniausias klaidas,
subtilias taisykles ir dėsningumus, kurie ne visada akivaizdūs iš pirmo žvilgsnio.

\---

## 1. Klasės anatomija ir moduliavimas (`.h` + `.cpp`)

- `::` (srities nustatymo operatorius, _scope resolution_) **būtinas** apibrėžiant metodus už klasės ribų — be jo kompiliatorius laiko tai įprasta globalia funkcija
- Deklaracija/aprašas (`.h`) ir apibrėžimas/definicija (`.cpp`) yra skirtingi dalykai; deklaracijų gali būti kelios, apibrėžimas — tik vienas
- `static const int` narys inicializuojamas tiesiai klasėje; `static int` — **tik už klasės ribų** `.cpp` faile: `int Klasė::narys = 0;`
- `const` metodas negali kviesti ne-`const` metodų — net jei jų poveikis atrodo nekenksmingai
- Inicializavimo sąrašas (`: member(value)`) **inicializuoja** narius kūrimo/konstravimo metu, tai - ne reikšmių priskyrimas sukurtam objektui: `const` nariai ir nuorodos (_references_) **privalo** būti inicializuoti būtent čia!
- Narių inicializavimo tvarka atitinka jų **deklaracijos eilę klasėje**, o ne inicializavimo sąrašo
- Jei klasė turi parametrinį konstruktorių — kompiliatorius **nebegeneruoja** numatytojo; tada `Klasė obj;` nebeveikia
- `static` metodas neturi `this` rodyklės — negali pasiekti nestatinių klasės narių

\---

## 2. Konstruktoriai ir destruktoriai

- Destruktorius iškviečiamas **automatiškai**, kai objektas išeina iš galiojimo srities (scope) — tai RAII principo pagrindas
- Objektai naikinami **atvirkštine** sukūrimo tvarka (LIFO - last In First Out - steko/dėtuvės veikimo principas)
- `throw` konstruktoriuje — vienintelis būdas pranešti apie klaidos sąlygą - juk konstruktorius neturi grąžinamosios reikšmės (kuri dažnai naudojama klasikiniame klaidų apdorojime, _error handling_)
- Kai `throw` metamas/iškyla konstruktoriuje — jau sukurtos bazinės klasės (poobjekčių) ir narių dalys **automatiškai** sunaikinamos (_stack unwinding_)
- Validacija turi įvykti **prieš** šalutinius efektus (skaitiklio didinimą, logingą/žurnalizavimą) — kitaip objekto dar nėra, o skaitiklis jau padidintas
- Konstruktoriai kviečiami nuo **bazinės** iki **paveldėtosios** klasės; destruktoriai — **atvirkštine** tvarka
- Jei bazinė klasė neturi numatytojo (_default_) konstruktoriaus — paveldėtosios klasės konstruktorius **privalo** kviesti bazinį (paveldimos klasės konstruktorių) inicializavimo sąraše
- Konstruktorius **negali** būti `virtual` — objektas dar tik konstruojamas, `vptr` dar nenustatytas

\---

## 3. RAII ir dinaminis atminties valdymas

- Jei klasė tiesiogiai naudoja `new` — destruktorius **privalo** kviesti `delete`; kitu atveju atmintis nuteka
- `delete[]` ir `delete` **nėra sukeičiami** — netinkamo varianto naudojimas sukelia neapibrėžtą elgseną (_undefined behavior_)
- `std::string` ir `std::vector` patys laikosi RAII principo — jų destruktoriai atlaisvina atmintį automatiškai
- RAII (_Resource Acquisition Is Initialization_) esmė: resursas įgyjamas konstruktoriuje **turi būti** atlaisvinamas destruktoriuje — taip pat ir išimčių atveju
- _Stack unwinding_ garantuoja destruktorių iškvietimą — tai priežastis, kodėl RAII apsaugo nuo atminties nutekėjimo net metant išimtis
- Naudojant "įprastus" `new` ir `delete`: jei tarp jų iškyla išimtis, `delete` nebeiškviečiamas — išmanioji rodyklė (_smart pointer_)`std::unique_ptr` šią problemą pašalina

\---

## 4. Kopijavimas ir Trejeto taisyklė (_Rule of Three_)

- Kompiliatorius generuoja kopijos konstruktorių automatiškai — tačiau jis atlieka tik paviršinę kopiją (_shallow copy_): kopijuoja rodyklę (adresą), o ne turinį to, į ką ji rodo
- Paviršinė kopija kartu su dinamiškai rezervuota atmintimi lemia dvigubą atlaisvinimą (_double delete_) — programa griūna (_crash_)
- Rule of Three: jei klasei reikia **destruktoriaus** su `delete` — jai (greičiausiai) taip pat reikia **kopijos konstruktoriaus** ir **kopijos priskyrimo operatoriaus**
- Priskyrimo operatoriuje visada pirmiausia patikriname savęs pri(si)skyrimą: `if (this == &other) return *this;`
- `Type obj2 = obj1;` iškviečia **kopijos konstruktorių**; `obj2 = obj1;` (kai `obj2` jau egzistuoja) — **kopijos priskyrimo operatorių**
- `vector<Shape>` — kiekvienas `push_back` kopijuoja objektą (ir gali sukelti objekto "apkarpimą", _slicing_); `vector<Shape*>` — kopijuojama tik rodyklė (vėlgi — adresas — "paprasta" reikšmė)
- Jei bazinė klasė teisingai įgyvendina _Rule of Three_, paveldėtosios klasės kopijos konstruktorių kompiliatorius sugeneruos teisingai

\---

## 5. Kompozicija ir paveldėjimas

- `has-a` (**kompozicija**): narys saugomas **per reikšmę** (`Engine engine`) — savininkas valdo jo gyvavimo ciklą
- **Agregacija**: narys saugomas **per rodyklę** (`Engine* ptr`) — klasė tik žino apie objektą, bet jo gyvavimo ciklo nekontroliuoja
- `is-a` (**paveldėjimas**): `Circle` yra `Shape` (bet ne atvirkščiai), todėl `Circle` objektas, rodyklė `Circle*` ar nuoroda `Circle&` gali būti perduodamas ar naudojamas ten, kur tikimasi `Shape`, `Shape*` ar `Shape&`. Toks konvertavimas iš paveldėtosios klasės į bazinę vadinamas _**Upcasting**_ (tačiau žr. _**Object slicing**_)
- **`protected`** nariai pasiekiami paveldėtose klasėse, bet nepasiekiami išoriniame kode
- Sub-objektą tikslinga inicializuoti inicializavimo sąraše: `: engine(hp, type)` — tai efektyviau nei priskyrimas konstruktoriaus kūne
- **`public`** paveldėjimas išsaugo prieigos lygius; **`private`** paveldėjimas — visus narius paverčia privačiais (retai naudojama)
- _Upcasting_ (`Circle*` → `Shape*`) — saugus ir automatinis; _downcast_ reikalauja tiesioginio (_explicit_) konvertavimo (_casting_) ir yra rizikingas, tačiau dažnai naudojamas

\---

## 6. Polimorfizmas ir `virtual`

- Vieno `virtual` metodo bazinėje klasėje — pakankama, kad paveldėtojai automatiškai paveldėtų **virtualumo** (polimorfinės elgsenos, dinaminio susiejimo) savybę
- `override` nėra funkcionalumo pakeitimas — tai "saugiklis" **kompiliatoriui patikrinti**, ar signatūra tiksliai atitinka bazinės klasės metodą (įskaitant `const` buvimą, t.y. metodo konstantinę versiją)
- Pamirštas `const` → metodas tampa perkrautu (_**overloaded**_), o ne perdengtu (_**overridden**_); su `override` raktažodžiu kompiliatorius tai patikrina ir aptinka
- Jei `delete` vykdomas per bazinės klasės rodyklę `Base*` — destruktorius turi būti `virtual`; kitaip elgsena neapibrėžta (_undefined behavior_): dažniausiai iškviečiamas tik bazinės klasės destruktorius, o paveldėtosios klasės destruktorius praleidžiamas.
- `gcc` apie trūkstamą `virtual` destruktorių dažnai tik perspėja, ne klaida — programa "veikia", bet neteisingai
- Statinis susiejimas (_static binding_): metodas parenkamas pagal **rodyklės tipą** kompiliavimo metu
- Dinaminis susiejimas (_dynamic binding_): metodas parenkamas pagal **tikrąjį objekto tipą** vykdymo metu (per `vtable`)
- Kiekvienas objektas, kurio klasė turi `virtual`, gauna/patalpina savyje `vptr` (dažniausiai – 8 baitai, 64-bit) — tai galima stebėti su `sizeof` logingu.

\---

## 7. _Object slicing_

- Objekto „apkarpymas“ (_object slicing_) įvyksta, kai, kai paveldėtosios klasės objektas perduodamas arba priskiriamas **per reikšmę** kaip bazinės klasės objektas — papildomos dalys prarandamos
- Slicing **nesukelia kompiliavimo klaidos** — programa veikia, tačiau neteisingai
- Trys tipiški scenarijai: priskyrimas kintamajam, perdavimas į funkciją per reikšmę, `vector<Shape>`
- `void f(Shape s)` — sukelia slicing; `void f(const Shape& s)` — saugus perdavimas; `void f(Shape* s)` — pilnas polimorfizmas
- `vector<Shape*>` vietoj `vector<Shape>` — pagrindinis praktinis sprendimas

\---

## 8. Išimtys

- `throw` konstruktoriuje — jei išimtis pagaunama (`catch`) — programa gali tęsti darbą, nors objektas ir nebuvo sukurtas
- Jei `throw` iškyla **prieš** priskyrimą — objektas lieka nepakeistas (stipri išimčių sauga, _strong exception safety_)
- Validacijos logika pagrįstai kartojasi konstruktoriuje ir setter'iuose — abi vietos yra "vartai" į netinkamą objekto būseną
- `throw` iš bazinės klasės konstruktoriaus automatiškai sklinda (_propagates_) per paveldėtosios klasės konstruktorių — papildomai to tvarkyti nereikia
- `catch (const std::exception& e)` pagauna visas standartines išimtis, paveldinčias iš `std::exception`
- Konkretesnės/specifinės `catch` šakos (`invalid_argument`, `runtime_error`) turi eiti **prieš** bendrąją — kitaip nebus pasiektos
- `try/catch` blokas turėtų apgaubti tik konkrečią operaciją, ne visą `main()` — tai leidžia programai tęsti darbą po nesėkmingos operacijos

\---

## 9. `vector<T>` su polimorfizmu

- `vector<Shape*>` su `push_back(new Circle(...))` — vektoriuje saugoma rodyklė, o `Circle` objektas — heap'o atmintyje
- Iteracija `for (Shape* s : shapes) s->printInfo()` veikia polimorfiškai, jei `printInfo()` yra `virtual`
- Kiekvienas elementas turi būti atlaisvintas atskirai: `for (Shape* s : shapes) delete s;` — `shapes.clear()` to neatlieka
- `shapes.clear()` pašalina rodykles iš vektoriaus, tačiau heap'o atminties **neatlaisvina**
- `delete` per `Shape*` be `virtual` destruktoriaus → paveldėtosios klasės destruktorius nekviestas → atminties nutekėjimas

\---

## 10. Šablonai (_Templates_)

- `template<typename T>` leidžia rašyti bendrinį (_generic_) kodą, kuris veikia su skirtingais tipais (`int`, `double`, `std::string`, ...)
- **Funkcijų šablonai**: kompiliatorius pats išveda tipą (_type deduction_): `swap(a, b)` → `T = int`
- **Klasių šablonai** kuriami su tipo parametru: `vector<int>`, `stack<string>`, `pair<double, int>` — tai skirtingos tos pačios klasės versijos
- Šablonai **generuojami kompiliavimo metu** — kompiliatorius kiekvienam naudojamam tipui sugeneruoja atskirą funkcijos ar klasės versiją (realizaciją)
- Šablonų funkcijų ir klasių apibrėžimai dažniausiai laikomi `.h` faile (arba `.hpp`) — kompiliatorius turi matyti pilną kodą generavimo metu
- Jei `Stack<T>` valdo dinaminę atmintį — būtina teisingai realizuoti kopijavimą (_Rule of Three_) arba jį tiesiog uždrausti (`= delete`)

## 11. Kompiliatorių elgsena

- `gcc` dažnai leidžia klaidingą kodą su perspėjimu — programa "veikia", tačiau elgiasi neteisingai (pvz., be `virtual` destruktoriaus)
- `clang` griežtesnis — daugiau klaidų vietoj perspėjimų; naudingas tikrinimui
- Kompiliavimo klaida ≠ susiejimo/linkerio (_linker_) klaida — `static` nario neapibrėžimas kompiliuojasi, tačiau linkeris jo neranda (`undefined reference`)
- Programa griūna arba duoda netikėtus rezultatus? Pirma ieškoti: _shallow copy_, _double delete_, _object slicing_, trūkstamo `virtual`

\---
