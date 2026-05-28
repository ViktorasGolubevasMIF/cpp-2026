\---

# OOP C++ egzaminas — pasirengimo atmintinė

Šis sąrašas apibendrina svarbiausius kurso akcentus: dažniausias klaidas,
subtilias taisykles ir dėsningumus, kurie ne visada akivaizdūs iš pirmo žvilgsnio.

\---

## 1. Klasės anatomija ir moduliavimas

- `::` (srities nustatymo operatorius) **būtinas** apibrėžiant metodus už klasės ribų — be jo kompiliatorius laiko tai įprasta globalia funkcija
- Deklaracija (`.h`) ir apibrėžimas (`.cpp`) yra skirtingi dalykai; deklaracijų gali būti kelios, apibrėžimas — tik vienas
- `static const int` narys inicializuojamas tiesiai klasėje; `static int` — **tik už klasės ribų** `.cpp` faile: `int Klasė::narys = 0;`
- `const` metodas negali kviesti ne-`const` metodų — net jei jų poveikis atrodo nekenksmingai
- Inicializavimo sąrašas (`: member(value)`) **inicializuoja** narius, o ne priskiria reikšmes — `const` nariai ir nuorodos **privalo** būti inicializuoti būtent čia
- Narių inicializavimo tvarka atitinka jų **deklaracijos eilę klasėje**, ne sąrašo eilę
- Jei klasė turi parametrinį konstruktorių — kompiliatorius **nebegeneruoja** numatytojo; tada `Klasė obj;` nebeveikia
- `static` metodas neturi `this` rodyklės — negali pasiekti nestatinių klasės narių

\---

## 2. Konstruktoriai ir destruktoriai

- Destruktorius iškviečiamas **automatiškai**, kai objektas išeina iš galiojimo srities (scope) — tai RAII principo pagrindas
- Objektai naikinami **atvirkštine** sukūrimo tvarka (LIFO)
- `throw` konstruktoriuje — vienintelis būdas pranešti apie klaidos sąlygą; grąžinamosios reikšmės konstruktorius neturi
- Kai `throw` iškyla konstruktoriuje — jau sukurtos bazinės klasės ir narių dalys **automatiškai** sunaikinamos (stack unwinding)
- Validacija turi įvykti **prieš** šalutinius efektus (skaitiklio didinimą, žurnalizavimą) — kitaip objekto nėra, tačiau skaitiklis jau padidintas
- Konstruktoriai kviečiami nuo **bazinės** iki **paveldėtosios** klasės; destruktoriai — **atvirkštine** tvarka
- Jei bazinė klasė neturi numatytojo konstruktoriaus — paveldėtosios klasės konstruktorius **privalo** kviesti bazinį inicializavimo sąraše
- Konstruktorius **negali** būti `virtual` — objektas dar tik konstruojamas, `vptr` dar nenustatytas

\---

## 3. RAII ir dinaminis atminties valdymas

- Jei klasė tiesiogiai naudoja `new` — destruktorius **privalo** kviesti `delete`; kitu atveju atmintis nuteka
- `delete[]` ir `delete` **nėra sukeičiami** — netinkamo varianto naudojimas sukelia neapibrėžtą elgseną (undefined behavior)
- `std::string` ir `std::vector` patys laikosi RAII principo — jų destruktoriai atlaisvina atmintį automatiškai
- RAII esmė: resursas įgyjamas konstruktoriuje ir **neišvengiamai** atlaisvinamas destruktoriuje — taip pat ir išimčių atveju
- Stack unwinding garantuoja destruktorių iškvietimą — tai priežastis, kodėl RAII apsaugo nuo atminties nutekėjimo net metant išimtis
- Naudojant įprastą `new`/`delete`: jei tarp jų iškyla išimtis, `delete` niekada neiškviestas — `std::unique_ptr` šią problemą pašalina

\---

## 4. Kopijavimas ir Rule of Three

- Kompiliatorius generuoja kopijos konstruktorių automatiškai — tačiau jis atlieka tik paviršinę kopiją (shallow copy): kopijuoja rodyklę, o ne jos turinį
- Paviršinė kopija kartu su dinamiškai rezervuota atmintimi lemia dvigubą atlaisvinimą (double delete) — programa griūna
- Rule of Three: jei klasei reikia destruktoriaus su `delete` — taip pat reikia kopijos konstruktoriaus ir kopijos priskyrimo operatoriaus
- Priskyrimo operatoriuje visada pirmiausia patikrinkite savęs priskyrimą: `if (this == &other) return *this;`
- `Type obj2 = obj1;` iškviečia kopijos **konstruktorių**; `obj2 = obj1;` (kai `obj2` jau egzistuoja) — priskyrimo **operatorių**
- `vector<Shape>` — kiekvienas `push_back` kopijuoja objektą (ir gali sukelti slicing); `vector<Shape*>` — kopijuojama tik rodyklė
- Jei bazinė klasė teisingai įgyvendina Rule of Three, paveldėtosios klasės kopijos konstruktorių kompiliatorius sugeneruos teisingai

\---

## 5. Kompozicija ir paveldėjimas

- `has-a` (kompozicija): narys saugomas **per reikšmę** (`Engine engine`) — savininkas valdo jo gyvavimo ciklą
- Agregacija: narys saugomas **per rodyklę** (`Engine* ptr`) — klasė tik žino apie objektą, bet jo negyvavimo ciklo nekontroliuoja
- `is-a` (paveldėjimas): `Circle` yra `Shape`, bet ne atvirkščiai — tai apibrėžia, kuris gali pakeisti kurį
- `protected` nariai pasiekiami paveldėtose klasėse, bet ne išoriniame kode
- Sub-objektą tikslinga inicializuoti inicializavimo sąraše: `: engine(hp, type)` — tai efektyviau nei priskyrimas konstruktoriaus kūne
- `public` paveldėjimas išsaugo prieigos lygius; `private` paveldėjimas — visus narius paverčia privačiais
- Upcasting (`Circle*` → `Shape*`) — saugus ir automatinis; downcast reikalauja explicit konversijos ir yra rizikingas

\---

## 6. Polimorfizmas ir `virtual`

- Vienas `virtual` bazinėje klasėje — pakankamas; paveldėtojai automatiškai paveldi virtualumo savybę
- `override` nėra funkcionalumo pakeitimas — tai **kompiliatoriaus tikrinimas**, ar signatūra tiksliai atitinka bazinės klasės metodą
- Pamirštas `const` → metodas tampa perkrautu (overload), o ne perdengtu (override); su `override` raktažodžiu kompiliatorius tai aptinka
- Jei klasė turi bent vieną `virtual` metodą — destruktorius **privalo** būti `virtual`; be jo `delete Base*` sukelia neapibrėžtą elgseną
- Be `virtual` destruktoriaus iškviečiamas tik bazinės klasės destruktorius — paveldėtosios klasės destruktorius praleistas, resursai nuteka
- `gcc` apie trūkstamą `virtual` destruktorių dažnai tik perspėja, ne klaida — programa "veikia", bet neteisingai
- Statinis susiejimas (static binding): metodas parenkamas pagal **rodyklės tipą** kompiliavimo metu
- Dinaminis susiejimas (dynamic binding): metodas parenkamas pagal **tikrąjį objekto tipą** vykdymo metu (per `vtable`)
- Kiekvienas objektas, kurio klasė turi `virtual`, gauna `vptr` (8 baitai, 64-bit) — tai `sizeof` padidėjimas

\---

## 7. Object slicing

- Slicing įvyksta, kai paveldėtosios klasės objektas perduodamas arba priskiriamas **per reikšmę** kaip bazinės klasės objektas — papildomos dalys prarandamos
- Slicing **nesukelia kompiliavimo klaidos** — programa veikia, tačiau neteisingai
- Trys tipiški scenarijai: priskyrimas kintamajam, perdavimas į funkciją per reikšmę, `vector<Shape>`
- `void f(Shape s)` — sukelia slicing; `void f(const Shape& s)` — saugus perdavimas; `void f(Shape* s)` — pilnas polimorfizmas
- `vector<Shape*>` vietoj `vector<Shape>` — pagrindinis praktinis sprendimas

\---

## 8. Išimtys

- `throw` konstruktoriuje — programa **tęsia darbą** po `catch` bloko; objektas nebuvo sukurtas, tačiau programa nesibaigia
- Jei `throw` iškyla **prieš** priskyrimą — objektas lieka nepakeistas (stipri išimčių sauga, strong exception safety)
- Validacijos logika pagrįstai kartojasi konstruktoriuje ir setter'iuose — abi vietos yra "vartai" į netinkamą objekto būseną
- `throw` iš bazinės klasės konstruktoriaus automatiškai sklinda per paveldėtosios klasės konstruktorių — papildomai to tvarkyti nereikia
- `catch (const std::exception& e)` pagauna visas standartines išimtis — tai plačiausias tinklas
- Konkretesnės `catch` šakos (`invalid_argument`, `runtime_error`) turi eiti **prieš** bendrąją — kitaip nebus pasiektos
- `try/catch` blokas turėtų apgaubti tik konkrečią operaciją, ne visą `main()` — tai leidžia programai tęsti darbą po nesėkmingos operacijos

\---

## 9. `vector<T>` su polimorfizmu

- `vector<Shape*>` su `push_back(new Circle(...))` — vektoriuje saugoma rodyklė, o `Circle` objektas — heap'o atmintyje
- Iteracija `for (Shape* s : shapes) s->printInfo()` veikia polimorfiškai, jei `printInfo()` yra `virtual`
- Kiekvienas elementas turi būti atlaisvintas atskirai: `for (Shape* s : shapes) delete s;` — `shapes.clear()` to neatlieka
- `shapes.clear()` pašalina rodykles iš vektoriaus, tačiau heap'o atminties **neatlaisvina**
- `delete` per `Shape*` be `virtual` destruktoriaus → paveldėtosios klasės destruktorius nekviestas → atminties nutekėjimas

\---

## 10. Kompiliatoriaus elgsena

- `gcc` dažnai leidžia klaidingą kodą su perspėjimu — programa "veikia", tačiau elgiasi neteisingai (pvz., be `virtual` destruktoriaus)
- `clang` griežtesnis — daugiau klaidų vietoj perspėjimų; naudingas tikrinimui
- Kompiliavimo klaida ≠ susiejimo (linker) klaida — `static` nario neapibrėžimas kompiliuojasi, tačiau `linker` jo neranda (`undefined reference`)
- Programa griūna arba duoda netikėtus rezultatus? Pirma ieškoti: shallow copy, double delete, slicing, trūkstamas `virtual`

\---
