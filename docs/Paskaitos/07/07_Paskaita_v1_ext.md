# Paveldėjimas — papildoma medžiaga

!!! abstract "Apie šį dokumentą"
    Tai **papildoma medžiaga** prie [P07: Paveldėjimas](07_Paskaita_v1.md).
    
    Čia rasite:
    
    - **NC/RT eksperimentus** — klaidos, kurias verta išbandyti patiems
    - **Upcast ir downcast** — terminologija, `static_cast`, `dynamic_cast`
    - **Dinaminį rodyklių masyvą** — `Shape**` su pilnu `new`/`delete` valdymu
    - **RAII ekskursą** — kodėl modernūs C++ mechanizmai egzistuoja
    - **`unique_ptr` užuominą** — kur link eina "teisingas" C++

---

## 1 DALIS: NC/RT eksperimentai

### Eksperimentas 1: `protected` nepasiekiamas iš išorės

```cpp linenums="1"
// [NC] — nesikompiliuoja!
int main() {
    Circle c(0.0, 0.0, 5.0, "red");

    c.center.x = 10.0;   // ❌ protected — nematoma išorėje
    c.color = "blue";    // ❌ protected — nematoma išorėje

    return 0;
}
```

=== "gcc klaida"

    ```
    error: 'Point Shape::center' is protected within this context
    note: declared protected here
    error: 'std::string Shape::color' is protected within this context
    note: declared protected here
    ```

=== "clang klaida"

    ```
    error: 'center' is a protected member of 'Shape'
    note: declared protected here
    error: 'color' is a protected member of 'Shape'
    note: declared protected here
    ```

**Išvada:** `protected` — tai `private` iš išorės perspektyvos. Skirtumas matosi tik **paveldėtose klasėse**.

---

### Eksperimentas 2: `private` nariai nepasiekiami paveldėtojuose

```cpp linenums="1"
// Pakeiskime Shape.h: protected → private
class Shape {
private:          // ← PAKEISTA
    Point center;
    std::string color;
    // ...
};
```

```cpp linenums="1"
// Circle.cpp — [NC]
void Circle::printInfo() const {
    std::cout << color;    // ❌ private — Circle nemato!
    center.print();        // ❌ private — Circle nemato!
}
```

=== "gcc klaida"

    ```
    error: 'std::string Shape::color' is private within this context
    error: 'Point Shape::center' is private within this context
    ```

=== "Sprendimas"

    ```cpp linenums="1"
    // Su private nariais — naudojame getterius:
    void Circle::printInfo() const {
        std::cout << getColor();      // ✅ public getter
        getCenter().print();          // ✅ public getter
    }
    ```

    Arba paliekame `protected` — tada paveldėtojai mato tiesiogiai.

!!! note "Projektavimo klausimas"
    Kada rinktis `protected`, kada `private` su getteriais?

    - `protected` — paprasčiau, bet **atskleidžia** implementacijos detales paveldėtojams
    - `private` + getteriai — griežtesnė inkapsuliacija, bet daugiau kodo
    
    Nėra vienareikšmio atsakymo — priklauso nuo situacijos.

---

### Eksperimentas 3: konstruktorių grandinė be bazinės klasės

```cpp linenums="1"
// [NC] — Shape neturi default konstruktoriaus
class Circle : public Shape {
public:
    Circle(double r) : radius(r) {  // ← pamiršta : Shape(...)
        // Kompiliatorius bando kviesti Shape() — jo nėra!
    }
private:
    double radius;
};
```

=== "gcc klaida"

    ```
    error: no matching function for call to 'Shape::Shape()'
    note: candidate: 'Shape::Shape(double, double, const string&)'
    note: candidate expects 3 arguments, 0 provided
    ```

=== "clang klaida"

    ```
    error: constructor for 'Circle' must explicitly initialize
           the base class 'Shape' which does not have a default constructor
    ```

**Sprendimas:** visada iškviesti bazinės klasės konstruktorių:

```cpp linenums="1"
Circle::Circle(double x, double y, double r)
    : Shape(x, y),   // ← būtina!
      radius(r)
{}
```

---

### Eksperimentas 4: `private` paveldėjimas užblokuoja interfeisą

```cpp linenums="1"
class CirclePrivate : private Shape {
public:
    CirclePrivate(double x, double y, double r)
        : Shape(x, y), radius(r) {}
private:
    double radius;
};
```

```cpp linenums="1"
// [NC]
int main() {
    CirclePrivate c(0, 0, 5.0);

    c.getColor();    // ❌ Shape::getColor() tapo private!
    c.getCenter();   // ❌ Shape::getCenter() tapo private!

    Shape* p = &c;   // ❌ upcasting neveikia su private paveldėjimu!
}
```

=== "gcc klaida"

    ```
    error: 'Shape' is an inaccessible base of 'CirclePrivate'
    error: 'std::string Shape::getColor() const' is inaccessible
    ```

**Išvada:** `private` paveldėjimas — tai **ne** is-a ryšys. `CirclePrivate` negali būti naudojamas vietoj `Shape`. Tai tik implementacijos detalė.

---

### Eksperimentas 5: object slicing — tylus [RT]

Šis eksperimentas ypatingas — **jokios kompiliavimo klaidos**, bet rezultatas neteisingas:

```cpp linenums="1"
void spausdinti(Shape s) {    // ← per reikšmę
    s.printInfo();
}

int main() {
    Circle c(0.0, 0.0, 5.0, "red");
    
    std::cout << "Originalas:\n";
    c.printInfo();            // Circle [r=5, area=78.54]

    std::cout << "\nPer reikšmę (slicing):\n";
    spausdinti(c);            // Shape [area=0] ← radius dingo!

    std::cout << "\nPriskyrimas (slicing):\n";
    Shape s = c;
    s.printInfo();            // Shape [area=0] ← radius dingo!
    
    return 0;
}
```

```
Originalas:
Circle [color=red, center=(0, 0), r=5, area=78.5398]

Per reikšmę (slicing):
[Shape CTOR] center=(0, 0) color=red   ← kopijos konstruktorius!
Shape [color=red, center=(0, 0), area=0]
[Shape DTOR] color=red

Priskyrimas (slicing):
Shape [color=red, center=(0, 0), area=0]
```

!!! danger "Slicing — klastingiausias"
    Kompiliatorius **nieko nepraneša**. Programa veikia. Tik rezultatas neteisingas.
    Būtent dėl to slicing laikomas viena pavojingiausių C++ klaidų pradedantiesiems.

---

## 2 DALIS: Upcast ir Downcast

### Terminologija: kur yra "viršus"?

Paveldėjimo hierarchija vizualizuojama kaip medis — **bazinė klasė viršuje**:

```
      Shape          ← "viršus" (bazinė / tėvinė klasė)
     /     \
 Circle  Rectangle   ← "apačia" (paveldėtojos / vaikinės klasės)
```

Iš čia kryptys:

- **Upcast** — kėlimas **aukštyn** hierarchijoje: `Circle*` → `Shape*`
- **Downcast** — leidimasis **žemyn** hierarchijoje: `Shape*` → `Circle*`

!!! note "Dažna painiava"
    Intuityviai gali atrodyti, kad `Circle` yra "aukštesnio lygio" nei `Shape` — nes ji **išplečia** `Shape`, turi daugiau laukų, yra "konkretesnė".
    Bet hierarchijoje **viršuje** yra abstraktesnė, bendresnė klasė — `Shape`.
    `Circle` yra **žemiau**, nes ji specializuoja `Shape`.

---

### Upcast — kėlimas aukštyn

```cpp linenums="1"
Circle c(0.0, 0.0, 5.0, "red");

// Upcast — implicit (netiesioginis), visada saugus:
Shape* p  = &c;          // ✅ Circle* → Shape*  (rodyklė)
Shape& r  = c;           // ✅ Circle& → Shape&  (nuoroda)
Shape  s  = c;           // ⚠️ taip pat "upcast", bet — SLICING!
```

**Kodėl visada saugus?**

`Circle IS-A Shape` — tai garantuoja, kad kiekvienas `Circle` objektas **turi** visus `Shape` narius. `Shape*` rodyklė į `Circle` objektą visada ras tai, ko tikisi.

```cpp linenums="1"
Shape* p = &c;
p->getColor();     // ✅ Shape::getColor() — tikrai egzistuoja Circle objekte
p->getCenter();    // ✅ Shape::getCenter() — tikrai egzistuoja
```

Kompiliatorius leidžia upcast **be jokio explicit cast** — nes jis **visada saugus**.

---

### Downcast — leidimasis žemyn

O jei turime `Shape*` ir norime grįžti prie `Circle*`?

```cpp linenums="1"
Shape* p = new Circle(0.0, 0.0, 5.0, "red");  // upcast

// Downcast — explicit (tiesioginis), pavojingas:
Circle* cp = static_cast<Circle*>(p);   // ← reikia explicit cast
cp->getRadius();                         // ✅ veikia — jei p TIŠ TIKRŲJŲ rodo į Circle
```

**Kodėl pavojingas?**

```cpp linenums="1"
Shape* p = new Rectangle(1.0, 1.0, 4.0, 3.0);  // Rectangle!

Circle* cp = static_cast<Circle*>(p);    // ← kompiliatorius leidžia!
cp->getRadius();                          // ❌ UNDEFINED BEHAVIOR — p rodo į Rectangle,
                                          //    ne į Circle! radius lauko ten nėra.
```

`static_cast` — kompiliatorius **patiki** jums. Jei klystate — undefined behavior.

---

### Saugus downcast: `dynamic_cast`

```cpp linenums="1"
#include <iostream>
#include "Circle.h"
#include "Rectangle.h"

void processShape(Shape* p) {
    // Bandome downcast į Circle:
    Circle* cp = dynamic_cast<Circle*>(p);

    if (cp != nullptr) {
        // p IŠ TIKRŲJŲ rodo į Circle
        std::cout << "Circle, r=" << cp->getRadius() << "\n";
    } else {
        // p rodo į kažką kitą (Rectangle ar pan.)
        std::cout << "Ne Circle\n";
    }
}

int main() {
    Shape* s1 = new Circle(0, 0, 5.0, "red");
    Shape* s2 = new Rectangle(1, 1, 4, 3, "blue");

    processShape(s1);   // Circle, r=5
    processShape(s2);   // Ne Circle

    delete s1;
    delete s2;
    return 0;
}
```

```
Circle, r=5
Ne Circle
```

`dynamic_cast` grąžina `nullptr` jei tipas neatitinka — **saugus** tikrinimas vykdymo metu.

!!! warning "`dynamic_cast` reikalauja `virtual`"
    `dynamic_cast` veikia tik kai bazinė klasė turi bent vieną `virtual` metodą.
    Be `virtual` — kompiliavimo klaida.
    
    Tai natūrali jungtis su **P08: Polimorfizmas** — ten `virtual` bus pagrindinė tema.

---

### Palyginimas: upcast vs downcast

| | **Upcast** | **Downcast** |
|---|---|---|
| Kryptis | `Derived*` → `Base*` | `Base*` → `Derived*` |
| Saugumas | Visada saugus | Pavojingas — gali būti klaidingas |
| Sintaksė | Implicit (be cast) | Explicit (`static_cast` arba `dynamic_cast`) |
| Kompiliatorius | Leidžia be klausimų | `static_cast` — patiki jums; `dynamic_cast` — tikrina vykdymo metu |
| Kada naudoti | Dažnai — kolekcijos, funkcijos | Retai — tik kai tikrai žinote tipą |

---

### Kada downcast naudojamas (ir kada išvengti)

**Dažniausiai downcast — blogas projektavimo ženklas:**

```cpp linenums="1"
// ❌ Blogas stilius — kam tada paveldėjimas?
for (Shape* s : shapes) {
    Circle* c = dynamic_cast<Circle*>(s);
    if (c) { /* Circle logika */ }

    Rectangle* r = dynamic_cast<Rectangle*>(s);
    if (r) { /* Rectangle logika */ }
}
```

Jei kiekvieną kartą downcast'inate ir tikrinate tipą — tai ženklas, kad geriau naudoti **virtualias funkcijas** (P08). Jos išsprendžia šią problemą elegantiškai.

**Teisėtas downcast** — kai iš konteksto **garantuotai** žinote tipą:

```cpp linenums="1"
// ✅ Teisėtas — jei tikrai žinome, kad shapes[0] yra Circle
Circle* c = static_cast<Circle*>(shapes[0]);
```

!!! note "Istorija iš praktikos"
    Komandos, kurios naudoja `dynamic_cast` kiekviename žingsnyje, dažnai turi **blogai suprojektuotą** hierarchiją — bazinė klasė nepakankamai abstrakti, arba `virtual` metodai neteisingai apibrėžti.
    Downcast yra įrankis — bet jo dažnas naudojimas rodo, kad kažkas ne taip su architektūra.

---

!!! tip "Žvilgsnis į priekį: P08"
    `dynamic_cast` veikia tik su `virtual`. Kodėl?
    Nes `virtual` sukuria **vykdymo laiko tipo informaciją** (RTTI — Runtime Type Information) — tik tada C++ žino, koks tikrasis objekto tipas per bazinės klasės rodyklę.
    
    P08: `virtual` metodai → polimorfizmas → ir `dynamic_cast` taps aiškesnis.

---

## 3 DALIS: Dinaminis rodyklių masyvas

### Problema: N nežinomas kompiliavimo metu

C stiliaus masyvas `Shape* shapes[N]` reikalauja **konstantos** `N`:

```cpp linenums="1"
const int N = 4;
Shape* shapes[N];   // ✅ N žinomas kompiliavimo metu
```

Bet ką daryti, jei dydis žinomas tik vykdymo metu?

```cpp linenums="1"
int n;
std::cout << "Kiek figūrų? ";
std::cin >> n;

Shape* shapes[n];   // ❌ VLA — nestandartinis C++!
```

Sprendimas — **dinaminis rodyklių masyvas**:

```cpp linenums="1"
Shape** shapes = new Shape*[n];   // ← rodyklė į rodyklių masyvą
```

---

### Dviguba rodyklė: `Shape**`

```
Shape** shapes
    │
    └──► [ Shape* | Shape* | Shape* | Shape* ]   ← heap: rodyklių masyvas
              │         │         │         │
              ▼         ▼         ▼         ▼
           Circle   Circle  Rectangle Rectangle   ← heap: objektai
```

Dvi atminties alokacijos:

1. `new Shape*[n]` — masyvas **rodyklių** (kiekviena po 8 baitus)
2. `new Circle(...)` — kiekvienas **objektas** atskirai

Ir dvi atlaisvinimo operacijos:

1. `delete shapes[i]` — kiekvienas **objektas**
2. `delete[] shapes` — **rodyklių masyvas**

---

### Pilnas pavyzdys

=== "Sukūrimas ir naudojimas"

    ```cpp linenums="1"
    #include <iostream>
    #include "Circle.h"
    #include "Rectangle.h"

    int main() {
        int n;
        std::cout << "Kiek figūrų? ";
        std::cin >> n;

        // 1. Alokuojame rodyklių masyvą
        Shape** shapes = new Shape*[n];

        // 2. Alokuojame kiekvieną objektą
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) {
                shapes[i] = new Circle(i, i, i + 1.0, "red");
            } else {
                shapes[i] = new Rectangle(i, i, i + 1.0, i + 2.0, "blue");
            }
        }

        // 3. Naudojame
        std::cout << "\n--- Figūros ---\n";
        for (int i = 0; i < n; i++) {
            shapes[i]->printInfo();
        }

        // 4. Atlaisviname: PIRMA objektai, TADA masyvas
        for (int i = 0; i < n; i++) {
            delete shapes[i];     // ← kiekvienas objektas
        }
        delete[] shapes;          // ← rodyklių masyvas
        shapes = nullptr;         // ← gera praktika

        return 0;
    }
    ```

=== "⌨️➡️🖥️ (n=3)"

    ```
    Kiek figūrų? 3
    [Shape CTOR] center=(0, 0) color=red
    [Circle CTOR] r=1
    [Shape CTOR] center=(1, 1) color=blue
    [Rectangle CTOR] w=2 h=3
    [Shape CTOR] center=(2, 2) color=red
    [Circle CTOR] r=3

    --- Figūros ---
    Circle [color=red, center=(0, 0), r=1, area=3.14159]
    Rectangle [color=blue, center=(1, 1), w=2, h=3, area=6]
    Circle [color=red, center=(2, 2), r=3, area=28.2743]

    [Circle DTOR] r=1
    [Shape DTOR] color=red
    [Rectangle DTOR] w=2 h=3
    [Shape DTOR] color=blue
    [Circle DTOR] r=3
    [Shape DTOR] color=red
    ```

=== "Atminties schema"

    ```
    Stack:                Heap:
    ┌──────────┐
    │ shapes ──┼──────► [ *  |  *  |  *  ]   ← new Shape*[3]
    │ n = 3   │           │     │     │
    └──────────┘           ▼     ▼     ▼
                        Circle  Rect  Circle  ← new Circle/Rectangle
    
    delete shapes[i] — naikina kiekvieną objektą
    delete[] shapes  — naikina rodyklių masyvą
    ```

---

### Klaidingos atlaisvinimo tvarkos pavojus

```cpp linenums="1"
// [RT] — memory leak arba undefined behavior!

// Klaida 1: pamiršta delete[] shapes
for (int i = 0; i < n; i++) {
    delete shapes[i];
}
// delete[] shapes;  ← PAMIRŠTA → rodyklių masyvas "nuteka"

// Klaida 2: pamiršta delete shapes[i]
// for (...) delete shapes[i];  ← PAMIRŠTA → visi objektai "nuteka"
delete[] shapes;

// Klaida 3: atvirkščia tvarka (undefined behavior)
delete[] shapes;          // ← pirma masyvas — rodyklės jau netinkamos!
for (int i = 0; i < n; i++) {
    delete shapes[i];     // ← undefined behavior
}
```

!!! danger "Taisyklė"
    Visada: **pirma** `delete shapes[i]` kiekvienam elementui, **tada** `delete[] shapes`.
    Atvirkščia tvarka — undefined behavior.

---

## 4 DALIS: RAII ekskursas

### Problema: rankinis valdymas yra pavojingas

Pažiūrėkime į tipišką scenarijų su dinaminiu masyvu:

```cpp linenums="1"
void processShapes(int n) {
    Shape** shapes = new Shape*[n];

    for (int i = 0; i < n; i++) {
        shapes[i] = new Circle(i, i, i + 1.0);
    }

    // ... daug kodo ...

    if (someCondition) {
        return;           // ← MEMORY LEAK! delete[] pamiršta
    }

    // ... dar kodo ...

    for (int i = 0; i < n; i++) delete shapes[i];
    delete[] shapes;      // ← pasiekiama tik "laimingame" kelyje
}
```

**Problemos:**

- Kiekvienas `return`, `throw`, `break` — potencialus memory leak
- Kuo ilgesnė funkcija — tuo didesnė tikimybė pamiršti
- Išimtys (`exceptions`) — `delete` gali būti visiškai praleistas

---

### RAII principas (priminimas)

**RAII** — *Resource Acquisition Is Initialization*.

Principas: resursas **įsigyjamas konstruktoriuje**, **atlaisvinamas destruktoriuje**. Kadangi destruktorius kviečiamas **automatiškai** — resursas visada atlaisvinamas, nepriklausomai nuo to, kaip baigiasi funkcija.

```cpp linenums="1"
// Matėme P05: std::string, std::vector — RAII pavyzdžiai
{
    std::vector<int> v = {1, 2, 3};  // konstruktorius: new int[...]
    // ... naudojame ...
}   // ← destruktorius: delete[] automatiškai — visada!
```

**Tas pats principas — mūsų problema:**

```
Problema:          Shape** shapes = new Shape*[n];  → rankinė
                   delete shapes[i]; delete[] shapes; → gali būti pamiršta

RAII sprendimas:   vector<Shape*> shapes;            → automatinis
                   delete shapes[i];                 → objektai vis dar rankiniai!
```

---

### Trys lygiai: nuo C iki modernaus C++

=== "Lygis 1: C stiliaus (viskas rankinė)"

    ```cpp linenums="1"
    // Pilna kontrolė, pilna atsakomybė
    Shape** shapes = new Shape*[n];

    shapes[0] = new Circle(0, 0, 5.0);
    shapes[1] = new Rectangle(1, 1, 4, 3);

    // naudojame...

    delete shapes[0];       // ← rankinė
    delete shapes[1];       // ← rankinė
    delete[] shapes;        // ← rankinė
    ```

    ✅ Pilna kontrolė  
    ❌ Lengva pamiršti  
    ❌ Neveikia su išimtimis  

=== "Lygis 2: vector<Shape*> (masyvas — automatinis)"

    ```cpp linenums="1"
    // vector valdo masyvą, objektai — vis dar rankiniai
    std::vector<Shape*> shapes;

    shapes.push_back(new Circle(0, 0, 5.0));
    shapes.push_back(new Rectangle(1, 1, 4, 3));

    // naudojame...

    for (Shape* s : shapes) delete s;  // ← objektai vis dar rankiniai
    // delete[] — nereikia, vector destruktorius pasirūpina
    ```

    ✅ Masyvo dydis automatinis  
    ✅ Masyvo atmintis automatinė  
    ❌ Objektų atmintis vis dar rankinė  

=== "Lygis 3: vector<unique_ptr<Shape>> (viskas automatinis)"

    ```cpp linenums="1"
    #include <memory>

    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Circle>(0, 0, 5.0));
    shapes.push_back(std::make_unique<Rectangle>(1, 1, 4, 3));

    // naudojame...

    // delete — nereikia nieko! unique_ptr destruktorius
    // pasirūpina kiekvienu objektu automatiškai
    ```

    ✅ Viskas automatinis  
    ✅ Veikia su išimtimis  
    ✅ Neįmanoma pamiršti `delete`  

---

### Palyginimas

```
                  Rodyklių masyvas   vector<Shape*>   vector<unique_ptr>
                  ────────────────   ──────────────   ──────────────────
Masyvo dydis      fiksuotas arba     dinaminis        dinaminis
                  new Shape*[n]

Masyvo atmintis   delete[] shapes    automatinis      automatinis
                  rankinė

Objektų atmintis  delete shapes[i]   delete shapes[i] automatinis
                  rankinė            rankinė

Saugumas          žemas              vidutinis        aukštas

Kada naudoti      C kodas,           dažnas           modernūs
                  edukacija          C++ kodas        projektai
```

---

??? tip "Žvilgsnis į priekį: `unique_ptr`"
    `std::unique_ptr<T>` — tai RAII apvalkalas aplink `T*` rodyklę.
    Veikia kaip rodyklė, bet **automatiškai** iškviečia `delete` destruktoriuje.

    ```cpp linenums="1"
    #include <memory>

    // Vietoj:
    Shape* p = new Circle(0, 0, 5.0);
    // ... naudojame p ...
    delete p;   // ← privaloma

    // Su unique_ptr:
    auto p = std::make_unique<Circle>(0, 0, 5.0);
    // ... naudojame p kaip rodyklę ...
    // delete — nereikia! automatinis
    ```

    **Apribojimai:**
    - `unique_ptr` **negali būti kopijuojamas** — jis yra unikalus savininkas
    - Perduoti į funkciją — per nuorodą arba `std::move`
    - Kelių savininkų scenarijams — `std::shared_ptr`

    ```cpp linenums="1"
    // vector su unique_ptr:
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(0, 0, 5.0));
    shapes.push_back(std::make_unique<Rectangle>(1, 1, 4, 3));

    for (const auto& s : shapes) {
        s->printInfo();   // naudojame kaip rodyklę
    }
    // Programa baigiasi — viskas automatiškai išvaloma
    ```

    Šiandien — tik užuomina. `unique_ptr` ir RAII pilnai — vėlesnėse paskaitose.

---

### Kodėl visa tai svarbu?

```
C (viskas rankinė)
    │
    │  new/delete rankiniai → lengva pamiršti → memory leaks
    │
    ▼
C++ su RAII konteineriais (vector, string)
    │
    │  konteineriai automatiniai → objektai vis dar rankiniai
    │
    ▼
C++ su smart pointers (unique_ptr, shared_ptr)
    │
    │  viskas automatinis → "Rule of Zero"
    │
    ▼
Modernūs C++ projektai: delete rašomas retai arba niekada
```

!!! note "Šiandieninė situacija"
    Šiame kurse dirbame su `new`/`delete` tiesiogiai — **ne todėl, kad tai geriausia praktika**, o todėl, kad reikia suprasti **kas vyksta po gaubtu**. Kai suprasite mechanizmą — `unique_ptr` atrodys natūraliai, ne kaip magija.

    Tai tas pats principas, kaip mokėmės `char*` prieš `std::string`, arba C masyvus prieš `std::vector`.

---

*[NC]: Not Compiling — Nesikompiliuoja
*[RT]: Runtime Error — Vykdymo klaida
*[RAII]: Resource Acquisition Is Initialization
*[VLA]: Variable Length Array
