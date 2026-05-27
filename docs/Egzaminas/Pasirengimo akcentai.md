\---

# OOP C++ egzaminas — konsultacijos akcentai

Šis sąrašas surinktas iš pratybų „Svarbu!" sekcijų ir papildytas
„nerašytomis tiesomis" — dažniausiomis klaidomis ir subtilybėmis,
kurios per paskaitas skamba, bet egzamine nustebina.

\---

## 1. Klasės anatomija ir moduliavimas

**Iš pratybų:**

- `::` (scope resolution) operatorius **būtinas** apibrėžiant metodus už klasės ribų — be jo kompiliatorius laiko tai globalia funkcija ir neranda klasės narių
- Aprašas (`.h`) ir apibrėžimas (`.cpp`) — skirtingi dalykai; aprašų gali būti daug, apibrėžimas — tik vienas
- `static const int` narys inicializuojamas **inline** klasėje; `static int` — **už klasės ribų** (`.cpp` faile): `int Klasė::narys = 0;`
- `const` metodas negali kviesti ne-`const` metodų — net jei tik "skaito"
- Inicializavimo sąrašas (`: member(val)`) **inicializuoja** narius, o ne priskiria — `const` nariai ir nuorodos **privalo** būti inicializuoti čia
- Inicializavimo tvarka — pagal **deklaracijos eilę klasėje**, ne sąrašo eilę

**Nerašytos tiesos:**

- Jei klasė turi parametrinį konstruktorių — kompiliatorius **nebegeneruoja** numatytojo; tuomet `Klasė obj;` nebeveikia
- `this` — rodyklė į patį objektą; naudinga kai parametro vardas sutampa su nariu
- `static` metodas neturi `this` — negali pasiekti nestatinių narių

\---

## 2. Konstruktoriai ir destruktoriai

**Iš pratybų:**

- Destruktorius iškviečiamas **automatiškai** kai objektas išeina iš scope — tai RAII principas
- Objektai naikinami **atvirkštine** sukūrimo tvarka (LIFO)
- `throw` konstruktoriuje — **vienintelis** būdas signalizuoti klaidą; grąžinamosios reikšmės nėra
- Kai `throw` įvyksta konstruktoriuje — jau sukurtos bazinės/narių dalys **automatiškai** naikinamos (stack unwinding)
- `throw` validacijoje turi būti **prieš** šalutinių efektų (skaitiklio didinimo, logging) — kitaip objekto nėra, bet skaitiklis padidėjo

**Nerašytos tiesos:**

- Konstruktoriai kviečiami nuo **bazinės** iki **paveldėtosios**; destruktoriai — **atvirkščiai**
- Jei `Shape` neturi numatytojo konstruktoriaus — `Circle` konstruktorius **privalo** kviesti `Shape(...)` inicializavimo sąraše
- Konstruktorius **negali** būti `virtual` — objektas dar tik kuriamas, `vptr` dar nenustatytas
- Default konstruktorius būtinas: masyvams (`Type arr[N]`), `vector<Type>(n)` ir kai kur kitur

\---

## 3. RAII ir dinaminis atminties valdymas

**Iš pratybų:**

- Jei klasė tiesiogiai kviečia `new` — ji turi turėti `delete` destruktoriuje; kitu atveju — atminties nutekėjimas
- `delete[]` ≠ `delete` — painiojimas → **undefined behavior**
- `std::string` ir `std::vector` — patys RAII objektai; jų destruktoriai atlaisvina atmintį automatiškai

**Nerašytos tiesos:**

- RAII principas: resursas įgyjamas konstruktoriuje, atlaisvinamas destruktoriuje — net išimčių atveju
- Stack unwinding garantuoja destruktorių iškvietimą — tai kodėl RAII apsaugo nuo nutekėjimo
- `raw new/delete` su išimtimis — `throw` po `new`, bet prieš `delete` → nutekėjimas; `unique_ptr` tai išsprendžia automatiškai

\---

## 4. Kopijavimas ir Rule of Three

**Iš pratybų:**

- Kompiliatorius generuoja kopijos konstruktorių automatiškai — bet jis daro **shallow copy** (kopijuoja tik rodyklę, ne turinį)
- Shallow copy + dinamika = **double delete** → crash programa pabaigoje
- Rule of Three: jei reikia destruktoriaus su `delete` — reikia ir kopijos konstruktoriaus, ir priskyrimo operatoriaus
- Kopijos priskyrimo operatoriuje: **pirma** patikrinkite savęs priskyrimą (`if (this == &other)`), tada išvalykite seną atmintį

**Nerašytos tiesos:**

- `Type obj2 = obj1;` → kopijos **konstruktorius**; `obj2 = obj1;` (kai `obj2` jau egzistuoja) → priskyrimo **operatorius**
- `vector<Shape>` — kiekvienas `push_back` daro kopiją; `vector<Shape*>` — kopijuojama tik rodyklė (8 baitai)
- Jei `Shape` Rule of Three teisingas, kompiliatorius sugeneruos teisingą `Circle` kopijos konstruktorių automatiškai

\---

## 5. Kompozicija ir paveldėjimas

**Iš pratybų:**

- `has-a` (kompozicija): narys per **reikšmę** (`Engine engine`) — savininkas valdo gyvavimo ciklą
- Agregacija: narys per **rodyklę** (`Engine* ptr`) — žino apie objektą, bet nevaldo
- `is-a` (paveldėjimas): `Circle` yra `Shape`, bet ne atvirkščiai
- `protected` nariai matomi paveldėtose klasėse, bet ne išoriniame kode

**Nerašytos tiesos:**

- Kompozicijoje init. sąraše galima inicializuoti sub-objektą: `: engine(hp, type)` — efektyviau nei priskyrimas kūne
- `public` paveldėjimas išsaugo prieigos lygius; `private` — viskas tampa `private`
- Upcasting (`Circle*` → `Shape*`) — saugus, automatinis; downcast — rizikingas, reikia explicit

\---

## 6. Polimorfizmas ir `virtual`

**Iš pratybų:**

- **Vienas** `virtual` bazinėje klasėje — pakanka; paveldėtojai automatiškai paveldi `virtual` savybę
- `override` — ne funkcionalumas, o **saugiklis**: kompiliatorius patikrina ar signatūra tiksliai atitinka
- Pamiršta `const` → metodas tampa **perkrautu** (overload), ne perdengtu (override); su `override` — kompiliavimo klaida
- `virtual` destruktorius: jei klasė turi bent vieną `virtual` — destruktorius **privalo** būti `virtual`; be jo `delete Base*` → undefined behavior
- Be `virtual` destruktoriaus — tik `Shape DTOR` iškviečiamas; `Circle DTOR` **praleistas** → resursų nutekėjimas
- `gcc` praneša kaip `warning`, ne `error` — programa "veikia", bet **neteisingai**

**Nerašytos tiesos:**

- Static binding: metodas parenkamas pagal **rodyklės tipą** kompiliavimo metu
- Dynamic binding: metodas parenkamas pagal **tikrąjį objekto tipą** vykdymo metu (per vtable)
- `sizeof` padidėja pridėjus `virtual` — kiekvienas objektas gauna `vptr` (8 baitai, 64-bit)
- Konstruktorius negali būti `virtual`; kopijos konstruktorius — taip pat ne

\---

## 7. Object slicing

**Iš pratybų:**

- Slicing — kai `Circle` perduodamas **per reikšmę** kaip `Shape`; `radius` dingsta
- Slicing **nekelia kompiliavimo klaidos** — programa veikia, bet **neteisingai**
- `vector<Shape>` — kiekvienas `push_back(circle)` daro slicing; `vector<Shape*>` — saugus

**Nerašytos tiesos:**

- Trys slicing scenarijai: priskyrimas kintamajam, perdavimas į funkciją per reikšmę, `vector<Shape>`
- Sprendimas: visada naudoti **rodykles arba nuorodas** kai reikia polimorfizmo
- `void f(Shape s)` — slicing; `void f(const Shape& s)` — saugus, bet dar ne polimorfinis be `virtual`; `void f(Shape* s)` — pilnas polimorfizmas

\---

## 8. Išimtys

**Iš pratybų:**

- `throw` konstruktoriuje — programa **tęsiasi** po `catch`; objekto nėra, gyvenimas eina toliau
- Objektas lieka galiojantis po metodo `throw` jei `throw` įvyko **prieš** priskyrimą — tai **strong exception safety**
- Validacijos logika kartojasi konstruktoriuje ir setter'iuose — tai normalu; abi vietos yra "vartai" į neteisingą būseną
- Stack unwinding: `throw` automatiškai kviečia visų sukurtų stack objektų destruktorius

**Nerašytos tiesos:**

- `catch (const std::exception& e)` gaudo visas standartines išimtis — plačiausias tinklas
- Konkretesnės `catch` šakos (**invalid_argument**, **runtime_error**) turi eiti **prieš** bendrąją
- `throw` iš bazinės klasės konstruktoriaus propagavosi per paveldėtojo konstruktorių automatiškai
- `try/catch` apima tik konkrečios operacijos — ne visą `main()`; tai leidžia programai tęstis po klaidos

\---

## 9. `vector<T>` su polimorfizmu

**Iš pratybų:**

- `vector<Shape*>` — `push_back(new Circle(...))` įdeda rodyklę; Circle pilnas heap'e
- Iteruojant `for (Shape* s : shapes) s->printInfo()` — veikia polimorfiškai su `virtual`
- **Delete** kiekvienas elementas: `for (Shape* s : shapes) delete s;` — ne tik `shapes.clear()`

**Nerašytos tiesos:**

- `shapes.clear()` pašalina rodykles iš vektoriaus, bet **neatlaisvina** heap atminties
- `delete` per `Shape*` be `virtual` destruktoriaus → `Circle` destruktorius nekviestas → nutekėjimas
- Po `delete s` — `s` tampa dangling pointer; `s = nullptr` — gera praktika

\---

## 10. Kompiliatoriaus elgsena — subtilybės

**Nerašytos tiesos:**

- `gcc` dažnai leidžia klaidingą kodą su `warning` — programa "veikia", bet neteisingai (pvz., be `virtual` destruktoriaus)
- `clang` griežtesnis — daugiau `error` vietoj `warning`; rekomenduojamas tikrinimui
- Kompiliavimo klaida ≠ linker klaida — `static` nario neapibrėžimas: kompiliuojasi, bet `linker` nerandas (`undefined reference`)
- Kai programa crash'ina ar duoda keistus rezultatus — pirma ieškoti shallow copy, double delete, slicing, missing `virtual`

\---
