# Išmaniosios rodyklės: Įvadas į Modern C++

---

## Įvadas

P09 susidūrėme su problema:

```cpp linenums="1"
Shape* raw = new Circle(0, 0, 5.0, "red");
throw std::runtime_error("kažkas negerai");
delete raw;   // ← NEPASIEKIAMA — atminties nutekėjimas
```

`delete` eilutė **nepasiekiama** — `throw` perduoda valdymą `catch` blokui. Steko atmintis (_stack_) valoma automatiškai, bet dinaminė atmintis (_heap_) — ne. Jei `throw` įvyksta prieš `delete` — niekas to nebepadarys.

Ar galima turėti dinaminės atminties objektą, kuris būtų valomas automatiškai — kaip steko objektas?

Galima. Tai išmanioji rodyklė (_smart pointer_) `std::unique_ptr`. Šioje paskaitoje — kodėl ir kaip jis veikia.

U7/06 žingsnyje šį mechanizmą išbandysite praktiškai.

---

## 1 DALIS: `unique_ptr` — RAII heap objektams

!!! abstract "Šios dalies tikslas"
    `unique_ptr` kaip RAII taikymas heap atminčiai.
    `make_unique` sintaksė. Kodėl `delete` nebereikia.

---

### Problema su `new`/`delete`

RAII principą akcentuojame nuo P05: resursas įgyjamas konstruktoriuje, atlaisvinamas destruktoriuje. Jį taikėme U4 (`IntArray`, `MyString`) ir vėliau su `Shape` hierarchija.

Bet kol kas `new`/`delete` naudojome tiesiogiai:

```cpp linenums="1"
Shape* s = new Circle(0, 0, 3.0, "red");
s->printInfo();
delete s;   // ← privalome prisiminti
```

Trys pavojai su kuriais jau susidūrėme tiesiogiai ("rankomis") valdydami *heap* objektus:

1. **Pamirštame `delete`** — atminties nutekėjimas (_memory leak_)
2. **`throw` prieš `delete`** — atminties nutekėjimas net kai "nepamirštame"
3. **`delete` du kartus** — neapibrėžtas elgesys (_undefined behavior_)

Visas tris sukelia vienas dalykas: `Shape*` laiko **adresą** — skaičių, atminties koordinatę. Ne objektą, ne atsakomybę. Niekas "nesaugo" to heap'e gyvenančio `Shape`.

!!! note "Adresas ≠ objektas"
    Žinoti, kur kažkas gyvena — dar nereiškia būti atsakingam už tą namą.
    `Shape*` žino adresą. Bet kas sukuria, kas sunaikina — neapibrėžta.
    Tai **nuosavybės** (_ownership_) problema — ir būtent ją sprendžia `unique_ptr`.

---

### Išmanioji rodyklė `unique_ptr`

`std::unique_ptr<Circle>` — tai **steko objektas**, kuris:

- **laiko/saugo** dinaminės atminties objekto adresą
- **valdo** jo gyvavimo ciklą
- **automatiškai** kviečia `delete` savo destruktoriuje

Steko objektas — nes `unique_ptr` "gyvena" steke (kaip ir visi lokalūs kintamieji). Ir kaip visi steko objektai — automatiškai sunaikinamas kai išeina iš _scope_ (galiojimo srities/bloko). Būtent tą destruktorių ir panaudojame.

!!! note "`unique_ptr<T>` — šablono sintaksė"
    `<Circle>`, `<Shape>` — tai **tipo parametras**. Reiškia: "šio tipo objektą valdysiu". Šablonų mechanizmą nagrinėsime P11 — kol kas pakanka žinoti kad `<...>` nurodo su kokiu tipu dirbame.

```cpp linenums="1"
#include <memory>   // ← būtinas

// Senasis būdas:
Shape* raw = new Circle(0, 0, 3.0, "red");
raw->printInfo();
delete raw;   // ← rankinis naikinimas

// Naujasis būdas:
auto smart = std::make_unique<Circle>(0, 0, 3.0, "red");
smart->printInfo();
// delete nereikia — smart (objekto) destruktorius pasirūpina naikinimu 
```

Atkreipkite dėmesį — po "mįslingo" `auto` ir `make_unique` rezultate turime rodyklę, kuriai pritaikome kreipinio į narį operatorių `->`. Sintaksė ta pati kaip su paprastu `Shape*`.

Bet svarbiausia — nebesimato nei `delete`, nei `new`! Pagaliau atsikratėme? :)

!!! note "`auto` — istorija ir dabartis"
    C ir ankstyvajame C++ `auto` reiškė *automatinę saugojimo klasę* — t.y. steko kintamąjį. Bet tai buvo **numatytasis** elgesys, todėl raktažodį niekas nenaudojo. C++11 standartizuotojai šį "tuščią" raktažodį perėmė naujam tikslui: **tipo išvedimui** (_type deduction_). Kompiliatorius pats nustato tipą iš dešinės pusės išraiškos. Taigi istorinis `auto` tyliai "išnyko" — ir atgimė nauju pavidalu:

    ```cpp
    auto x = 42;        // int
    auto y = 3.14;      // double
    auto z = true;      // bool
    auto s = "labas";   // const char*

    ```

    `auto smart = std::make_unique<Circle>(...)` — kompiliatorius žino kad tai `std::unique_ptr<Circle>`, nors mes to nerašome.

---

### `make_unique` — kodėl ne (be) `new`?

`std::make_unique<T>(args...)` — standartas nuo C++14. Tai **vienintelis rekomenduojamas** būdas sukurti `unique_ptr`.
 
```cpp linenums="1"
// Modernus būdas (C++14+) — rekomenduojamas:
auto c = std::make_unique<Circle>(0, 0, 3.0, "red");
//       ↑ Circle paminėtas vieną kartą, new nematomas
 
// Senasis būdas — veikia, bet nebemodernus:
std::unique_ptr<Circle> c(new Circle(0, 0, 3.0, "red"));
//                         ↑ new grįžta, Circle kartojasi du kartus
```
 
`make_unique` laikosi DRY (_Don't Repeat Yourself_) principo — tipas nurodytas vieną kartą, `new` slepiamas viduje. Argumentai perduodami tiesiai į `Circle` konstruktorių — lygiai kaip `new Circle(...)`.
 
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

## 2 DALIS: Nuosavybės principas (_ownership_)

!!! abstract "Šios dalies tikslas"
    `unique_ptr` pavadinimas — ne atsitiktinis. Nuosavybės principas: vienas savininkas, aiški atsakomybė.
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

??? note "Kodėl kopijavimas draudžiamas — ar tai ne minusas?"
    Prisiminkite, kaip susidūrėme su giliuoju kopijavimu (_deep copy_) — turėjome patys rašyti kopijavimo konstruktorių, kad išvengtume rodyklių dubliavimosi ir _double delete_ klaidų.

    O dabar įsivaizduokite gilųjį kopijavimą su polimorfizmu. Kaip nukopijuoti `Shape*`, jei net nežinai, ar ten `Circle`, ar `Rectangle`? Tam reikėtų sudėtingo virtualaus `clone()` mechanizmo.

    `unique_ptr` nukerta šią problemą iš šaknų. Užuot kopijavus visą "namą", tiesiog perduodame "raktus" kitam savininkui — `std::move`.

    Uždrausdamas kopijavimą ir leisdamas tik perdavimą, `unique_ptr` apsaugo nuo giliosios kopijos kodo rašymo ir garantuoja, kad objektas visada turės vieną aiškų savininką. Tai ne apribojimas — tai dizaino sprendimas.

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

??? tip "Nuosavybės perdavimas į funkciją ir atgal"

    ```cpp linenums="1"
    // Perduodame nuosavybės teisę į funkciją:
    void printAndDiscard(std::unique_ptr<Shape> s) {
        s->printInfo();
    }   // ← s sunaikinama čia — objektas ištrinamas

    // Kviečiame:
    auto c = std::make_unique<Circle>(0, 0, 3.0, "red");
    printAndDiscard(std::move(c));   // ← perduodame nuosavybės teisę
    // c dabar nullptr

    // Grąžiname nuosavybę iš funkcijos:
    std::unique_ptr<Shape> create(double r) {
        return std::make_unique<Circle>(0, 0, r, "red");
        // ← nuosavybės teisė perduodama kviečiančiajai funkcijai
    }

    auto s = create(3.0);   // ← s dabar savininkas
    ```

    Grąžinant iš funkcijos — `std::move` nereikia. Kompiliatorius tai atlieka automatiškai.

---

## 3 DALIS: `vector<unique_ptr<Shape>>` — modernus C++

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

!!! note "Hierarchija nepakito"
    `Shape`, `Circle`, `Rectangle` — tie patys failai, tie patys `virtual` metodai,
    tas pats `virtual` destruktorius. `unique_ptr` nieko nekeičia klasių struktūroje —
    tik *kaip* objektai valdomi atmintyje. Tai ir yra jo elegancija: nulinė kaina
    esamam kodui (_zero-cost abstraction_).

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
    `shared_ptr` daugumai užduočių nereikės — `unique_ptr` pakanka. Svarbu žinoti kad egzistuoja ir kada ieškoti. Detalės — `_ext` dalyje (_tbd_).

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
| kopija | ✅ | ❌ (konceptualiai) |

---

!!! tip "Užduotis U7"
    U7 žingsnyje 6 ir BONUS — `unique_ptr` su `Shape` hierarchija.
    Viskas ką matėme šioje paskaitoje — ten praktiškai.

---

*[RAII]: Resource Acquisition Is Initialization
*[UB]: Undefined Behavior — neapibrėžtas elgesys
