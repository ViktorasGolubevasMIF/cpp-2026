# Išmaniosios rodyklės ir nuosavybė

---

## Tęsinys nuo P09

P09 pabaigoje paliko atvirą klausimą:

```cpp linenums="1"
Shape* raw = new Circle(0, 0, 5.0, "red");
throw std::runtime_error("kažkas negerai");
delete raw;   // ← NEPASIEKIAMA — atminties nutekėjimas
```

`throw` "peršoka" per `delete`. Stack unwinding sunaikina stack objektus — bet `heap` objektai nėra stack'e. Juos reikia sunaikinti rankiniu būdu. Ir jei `throw` įvyksta prieš `delete` — niekas to nepadarys.

Ar galima turėti heap objektą, kuris elgtųsi kaip stack objektas — sunaikintų save automatiškai?

Galima. Tai `std::unique_ptr`. U7/6 užduotyje šis mechanizmas taikomas praktiškai — šioje paskaitoje — kodėl jis veikia.

---

## 1 DALIS: `unique_ptr` — RAII heap objektams

!!! abstract "Šios dalies tikslas"
    `unique_ptr` kaip RAII taikymas heap atminčiai.
    `make_unique` sintaksė. Kodėl `delete` nebereikia.

---

### Problema su `new`/`delete`

Žinome RAII nuo P05: resursas įgyjamas konstruktoriuje, atlaisvinamas destruktoriuje. `MyString`, `Stack` — abu taip veikia.

Bet kol kas `new`/`delete` naudojome tiesiogiai:

```cpp linenums="1"
Shape* s = new Circle(0, 0, 3.0, "red");
s->printInfo();
delete s;   // ← privalome prisiminti
```

Trys problemos:

1. **Pamirštame `delete`** — atminties nutekėjimas (_memory leak_)
2. **`throw` prieš `delete`** — atminties nutekėjimas net kai "nepamirštame"
3. **`delete` du kartus** — neapibrėžtas elgesys (_undefined behavior_)

Visas tris problemas sukelia vienas dalykas: `Shape*` yra **tik adresas** — jis neturi jokios atsakomybės. Niekas "nevaldo" to objekto.

---

### Sprendimas: rodyklė su atsakomybe

`std::unique_ptr<T>` — tai stack objektas, kuris:

- **laiko** heap objekto adresą
- **valdo** jo gyvavimo ciklą
- **automatiškai** kviečia `delete` savo destruktoriuje

```cpp linenums="1"
#include <memory>   // ← būtinas

// Senasis būdas:
Shape* raw = new Circle(0, 0, 3.0, "red");
raw->printInfo();
delete raw;   // ← rankinis

// Naujasis būdas:
auto smart = std::make_unique<Circle>(0, 0, 3.0, "red");
//   ↑ stack    ↑ sukuria heap objektą ir "įpakuoja"
smart->printInfo();   // ← naudojamas lygiai kaip rodyklė
// delete nereikia — smart destruktorius pasirūpina
```

Rodyklės sintaksė (`->`) lieka ta pati. Tik `new` ir `delete` išnyksta.

---

### `make_unique` — kodėl ne `new`?

`std::make_unique<T>(args...)` — standartas nuo C++14. Tai **vienintelis rekomenduojamas** būdas sukurti `unique_ptr`.

```cpp linenums="1"
// Taip — visada:
auto c = std::make_unique<Circle>(0, 0, 3.0, "red");

// Ne taip:
std::unique_ptr<Circle> c(new Circle(0, 0, 3.0, "red"));
//                         ↑ veikia, bet nereikalinga
```

`make_unique` argumentai perduodami tiesiai į `Circle` konstruktorių — lygiai kaip `new Circle(...)`.

---

### Stack unwinding veikia automatiškai

```cpp linenums="1"
void demonstracija() {
    auto smart = std::make_unique<Circle>(0, 0, 5.0, "blue");
    //   ↑ stack objektas

    throw std::runtime_error("kažkas negerai");
    // throw → stack unwinding → smart destruktorius → delete
    // Atminties nutekėjimo nėra.
}
```

```
[Shape CTOR] color=blue
[Circle CTOR] r=5
[Circle DTOR] r=5        ← automatiškai, stack unwinding
[Shape DTOR] color=blue  ← automatiškai
```

`unique_ptr` yra stack objektas. Stack unwinding sunaikina visus stack objektus. Todėl `unique_ptr` destruktorius **visada** iškviečiamas — net su išimtimis.

---

## 2 DALIS: Nuosavybė (_ownership_)

!!! abstract "Šios dalies tikslas"
    `unique_ptr` pavadinimas — ne atsitiktinis. Nuosavybė kaip koncepcija.
    Kodėl negalima kopijuoti. `std::move` — nuosavybės perdavimas.

---

### Kas yra nuosavybė?

Kurso pradžioje kalbėjome apie `has-a` santykį kompozicijoje — `Shape` **turi** `Point`. Bet `Shape` ir `Point` egzistuoja kartu: sunaikinus `Shape`, sunaikinama ir `Point`.

`unique_ptr` išreiškia kitokį santykį: **nuosavybę** (_ownership_). `unique_ptr` **valdo** heap objektą — ir atsakingas už jo sunaikinimą.

"Unique" reiškia: **vienas savininkas**. Vienu metu tik vienas `unique_ptr` gali valdyti konkretų objektą.

```cpp linenums="1"
auto a = std::make_unique<Circle>(0, 0, 3.0, "red");
auto b = a;   // ← KLAIDA: negalima kopijuoti
//               du savininkai = du delete = UB
```

Kompiliatorius tai draudžia. Ne klaida dizaine — **saugumo garantija**.

---

### Nuosavybės perdavimas su `std::move`

Nuosavybę galima **perduoti** — bet ne kopijuoti:

```cpp linenums="1"
auto a = std::make_unique<Circle>(0, 0, 3.0, "red");
auto b = std::move(a);   // ← nuosavybė pereina iš a į b
//                           a tampa "tuščias" (nullptr)

if (a == nullptr) {
    std::cout << "a nebeturi objekto\n";   // ← taip
}
b->printInfo();   // ← veikia, b dabar savininkas
```

`std::move` — ne kopija, o **perdavimas**. Po `move` senasis savininkas netenka objekto.

!!! note "Move semantika — plačiau"
    `std::move` yra platesnės **move semantikos** dalis — tai C++11 koncepcija, leidžianti efektyviai "perkelti" resursus be kopijavimo. `unique_ptr` yra klasikinis pavyzdys kur tai natūraliai taikoma. Move semantika kaip atskira tema — už šio kurso ribų, bet terminą girdėsite dažnai.

---

### Į funkciją ir atgal

```cpp linenums="1"
// Perduodame nuosavybę į funkciją:
void printAndDiscard(std::unique_ptr<Shape> s) {
    s->printInfo();
}   // ← s sunaikinama čia — objektas ištrinamas

// Kviečiame:
auto c = std::make_unique<Circle>(0, 0, 3.0, "red");
printAndDiscard(std::move(c));   // ← perduodame nuosavybę
// c dabar nullptr

// Grąžiname nuosavybę iš funkcijos:
std::unique_ptr<Shape> create(double r) {
    return std::make_unique<Circle>(0, 0, r, "red");
    // ← nuosavybė perduodama skambintojui
}

auto s = create(3.0);   // ← s dabar savininkas
```

Grąžinant iš funkcijos — `std::move` nereikia. Kompiliatorius tai atlieka automatiškai.

---

## 3 DALIS: `vector<unique_ptr<Shape>>` — kurso kulminacija

!!! abstract "Šios dalies tikslas"
    `Shape*[]` → `vector<Shape*>` → `vector<unique_ptr<Shape>>` — evoliucija.
    Polimorfizmas per `unique_ptr`. RAII + ownership + polimorfizmas kartu.

---

### Evoliucija

Prisiminkite kaip su `Shape` kolekcijomis dirbome iki šiol:

```cpp linenums="1"
// P07: C stiliaus masyvas — rankinis valdymas
Shape* shapes[3];
shapes[0] = new Circle(0, 0, 3.0, "red");
shapes[1] = new Rectangle(0, 0, 4.0, 2.0, "blue");
shapes[2] = new Circle(1, 1, 1.5, "green");
// ...
for (int i = 0; i < 3; i++) delete shapes[i];   // ← nepamiršti

// P08: vector<Shape*> — dinamiškas dydis, bet vis dar rankinis valdymas
std::vector<Shape*> shapes;
shapes.push_back(new Circle(0, 0, 3.0, "red"));
// ...
for (auto* s : shapes) delete s;   // ← nepamiršti
```

Abu variantai reikalauja rankinio `delete`. Abu pavojingi su išimtimis.

```cpp linenums="1"
// P10: vector<unique_ptr<Shape>> — automatinis valdymas
std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Circle>(0, 0, 3.0, "red"));
shapes.push_back(std::make_unique<Rectangle>(0, 0, 4.0, 2.0, "blue"));
shapes.push_back(std::make_unique<Circle>(1, 1, 1.5, "green"));

for (const auto& s : shapes) {
    s->printInfo();   // ← polimorfizmas veikia lygiai taip pat
}
// vector sunaikinamas → kiekvienas unique_ptr sunaikinamas →
// kiekvienas Shape objektas sunaikinamas automatiškai
// delete niekur nerašome
```

---

### Polimorfizmas per `unique_ptr`

`unique_ptr<Shape>` viduje laiko `Shape*`. Virtualių funkcijų mechanizmas (`vptr`) yra **objekto viduje** — ne rodyklėje, ne `unique_ptr`. Todėl polimorfizmas veikia lygiai taip pat:

```cpp linenums="1"
std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Circle>(0, 0, 3.0, "red"));
shapes.push_back(std::make_unique<Rectangle>(0, 0, 4.0, 2.0, "blue"));

for (const auto& s : shapes) {
    s->printInfo();   // ← Circle::printInfo() arba Rectangle::printInfo()
    //                     sprendžiama vykdymo metu — kaip visada su virtual
}
```

```
[Shape CTOR] color=red
[Circle CTOR] r=3
[Shape CTOR] color=blue
[Rectangle CTOR] w=4 h=2

Circle    [color=red,  center=(0,0), r=3,   area=28.2743]
Rectangle [color=blue, center=(0,0), w=4, h=2, area=8]

[Rectangle DTOR] w=4 h=2   ← automatiškai (atvirkštine tvarka)
[Shape DTOR] color=blue
[Circle DTOR] r=3
[Shape DTOR] color=red
```

---

### `push_back` su `unique_ptr`

Kadangi `unique_ptr` negalima kopijuoti — į `vector` dedama per `move`:

```cpp linenums="1"
// Variantas A — tiesiogiai (rekomenduojama):
shapes.push_back(std::make_unique<Circle>(0, 0, 3.0, "red"));

// Variantas B — per kintamąjį (reikia move):
auto c = std::make_unique<Circle>(0, 0, 3.0, "red");
shapes.push_back(std::move(c));   // ← move, ne kopija
// c dabar nullptr
```

Variantas A paprastesnis — `make_unique` grąžina laikiną objektą, kuris automatiškai "perkeliamas".

---

### Bendra struktūra: RAII + ownership + polimorfizmas

```cpp linenums="1"
#include <iostream>
#include <memory>
#include <vector>
#include "Circle.h"
#include "Rectangle.h"

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Circle>(0, 0, 3.0, "red"));
    shapes.push_back(std::make_unique<Rectangle>(1, 1, 4.0, 2.0, "blue"));
    shapes.push_back(std::make_unique<Circle>(2, 2, 1.5, "green"));

    double totalArea = 0.0;
    for (const auto& s : shapes) {
        s->printInfo();
        totalArea += s->area();
    }

    std::cout << "\nBendras plotas: " << totalArea << "\n";

    // Viskas sunaikinama automatiškai — ne vieno delete
    return 0;
}
```

Tai **modernaus C++ OOP** šablonas: kolekcija valdo gyvavimo ciklą, polimorfizmas valdo elgseną. Rankinis `new`/`delete` — nebereikalingas.

---

## 4 DALIS: `shared_ptr` — trumpai

!!! abstract "Šios dalies tikslas"
    `unique_ptr` tinka daugumai atvejų. Kada jo nepakanka — `shared_ptr` egzistencija.

---

### Kai vieno savininko nepakanka

`unique_ptr` — vienas savininkas. Bet kartais objektas turi priklausyti keliems:

```cpp linenums="1"
// Pavyzdys: keli grafikos komponentai naudoja tą patį tekstūros objektą
// Tekstūra turi egzistuoti kol bent vienas komponentas ją naudoja
// Kas turėtų būti savininkas?
```

Tam yra `std::shared_ptr<T>` — **dalijama nuosavybė**. Viduje skaičiuoja kiek savininkų — kai paskutinis sunaikinamas, sunaikinamas ir objektas.

```cpp linenums="1"
auto a = std::make_shared<Circle>(0, 0, 3.0, "red");
auto b = a;   // ← kopija leidžiama — du savininkai
// objektas sunaikinamas kai ir a, ir b išeina iš scope
```

!!! note "Šiame kurse"
    `shared_ptr` daugumai užduočių nereikės — `unique_ptr` pakanka. Svarbu žinoti kad egzistuoja ir kada ieškoti. Detalės — `_ext`.

---

## Apibendrinimas

```
new/delete (rankinis)
    ↓ problema: pamirštame, throw aplenkia
RAII + klasė (MyString, Stack)
    ↓ veikia, bet kiekvienai klasei rašome patys
unique_ptr (automatinis RAII heap objektams)
    ↓ ownership aiškus, delete niekada
vector<unique_ptr<Shape>> (kolektyvinė nuosavybė + polimorfizmas)
    ↓
Modernaus C++ OOP pagrindas
```

| | `Shape*` | `unique_ptr<Shape>` |
|---|---|---|
| `delete` | rankinis | automatinis |
| išimtys | pavojinga | saugu |
| nuosavybė | neapibrėžta | aiški |
| polimorfizmas | ✅ | ✅ |
| kopija | ✅ | ❌ (tyčia) |

---

!!! tip "Užduotis U7"
    U7 žingsnyje 6 ir BONUS — `unique_ptr` su `Shape` hierarchija.
    Viskas ką matėme šioje paskaitoje — ten praktiškai.

---

*[RAII]: Resource Acquisition Is Initialization
*[UB]: Undefined Behavior — neapibrėžtas elgesys
