# Polimorfizmas

---

## 1 DALIS: static binding, virtual, vtable

!!! abstract "Šios dalies tikslas"
    P07 pabaigoje matėme: `Shape*` rodyklė į `Circle` objektą — bet `area()`
    grąžina `0.0`. Kodėl? Ir kaip tai ištaisyti?

    - Suprasime **static binding** — kodėl kompiliatorius parenka neteisingą metodą
    - Atrasime `virtual` — vienas raktažodis, keičiantis viską
    - Pamatysime **vtable** mechanizmą — kaip dynamic binding veikia po gaubtu

---

### Problema: "fake" metodai

U5 žingsnyje 5 ir U6 žingsnyje 1 matėme šį kodą:

```cpp linenums="1"
std::vector<Shape*> shapes;
shapes.push_back(new Circle(0.0, 0.0, 3.0, "red"));
shapes.push_back(new Rectangle(2.0, 2.0, 4.0, 2.0, "green"));

for (Shape* s : shapes) {
    std::cout << s->area() << "\n";  // ← visada 0.0!
}
```

`Circle` ir `Rectangle` turi savo `area()` — bet per `Shape*` kviečiama `Shape::area()`. Kodėl?

---

### Static binding

Kompiliatorius parenka metodą **kompiliavimo metu** pagal **rodyklės tipą**:

```cpp linenums="1"
Shape* s = new Circle(0, 0, 5.0);

s->area();
// Kompiliatorius mato: s yra Shape*
// Todėl: Shape::area()  ← visada, nepriklausomai nuo tikrojo tipo
```

Tai **static binding** (statinis susiejimas) — metodo pasirinkimas užfiksuotas kompiliavimo metu.

```
Kompiliavimo metu:   s yra Shape*  →  Shape::area()  ✓ (teisingas tipas)
Vykdymo metu:        s rodo į Circle  →  norėtume Circle::area()  ✗ (negauname)
```

Be `virtual` — kompiliatorius net nežiūri į tikrąjį objekto tipą.

---

### "Fake" metodai

`Circle::area()` egzistuoja ir veikia teisingai — bet tik kai kviečiama **tiesiogiai**:

```cpp linenums="1"
Circle c(0, 0, 5.0);
c.area();          // ✅ Circle::area() — teisingai, 78.54

Shape* s = &c;
s->area();         // ❌ Shape::area() — 0.0, nors objektas yra Circle!
```

`Circle::area()` šiuo atveju yra "fake" metodas — jis lyg ir perdengė `Shape::area()`, bet per `Shape*` vis tiek kviečiamas bazinis. Tai ne klaida — tai **laukiama** static binding elgsena. Bet ne tai, ko norime.

---

### Sprendimas: `virtual`

Vienas raktažodis `Shape.h` faile:

```cpp linenums="1"
class Shape {
public:
    virtual double area() const;       // ← virtual!
    virtual void printInfo() const;    // ← virtual!
    virtual ~Shape();                  // ← virtual destruktorius (apie tai — vėliau)
};
```

Dabar kompiliatorius **nebefiksuoja** metodo kompiliavimo metu. Sprendimas atidedamas į **vykdymo laiką**:

```cpp linenums="1"
Shape* s = new Circle(0, 0, 5.0);

s->area();
// Vykdymo metu: s rodo į Circle  →  Circle::area()  ✅ 78.54!
```

Tai **dynamic binding** (dinaminis susiejimas) — metodas parenkamas vykdymo metu pagal **tikrąjį objekto tipą**.

=== "Be virtual (static binding)"

    ```cpp linenums="1"
    // Shape.h:
    double area() const;   // ← ne virtual

    // Rezultatas:
    Shape* s = new Circle(0, 0, 5.0);
    s->area();  // Shape::area() → 0.0
    ```

=== "Su virtual (dynamic binding)"

    ```cpp linenums="1"
    // Shape.h:
    virtual double area() const;   // ← virtual

    // Rezultatas:
    Shape* s = new Circle(0, 0, 5.0);
    s->area();  // Circle::area() → 78.54 ✅
    ```

=== "main.cpp nepakinta!"

    ```cpp linenums="1"
    // Šis kodas NEPAKINTA — tik Shape.h pasikeitė:
    for (Shape* s : shapes) {
        std::cout << s->area() << "\n";
    }
    // Dabar: Circle → 28.27, Rectangle → 8, Triangle → 6
    ```

!!! note "Polimorfizmo esmė"
    `virtual` leidžia **vienam kodui** veikti skirtingai su skirtingais objektais.
    `for (Shape* s : shapes) { s->area(); }` — viena eilutė, keli skirtingi rezultatai.

---

### vtable ir vptr — po gaubtu

Kaip dynamic binding veikia fiziškai? Kompiliatorius sukuria **vtable** (virtual function table).

**Kiekvienai klasei su `virtual` metodais** — atskira vtable:

```
Shape vtable:
┌─────────────────────────┐
│ Shape::area()    → 0x.. │
│ Shape::printInfo → 0x.. │
│ Shape::~Shape()  → 0x.. │
└─────────────────────────┘

Circle vtable:
┌─────────────────────────┐
│ Circle::area()   → 0x.. │  ← perašyta!
│ Circle::printInfo→ 0x.. │  ← perašyta!
│ Circle::~Circle()→ 0x.. │  ← perašyta!
└─────────────────────────┘

Rectangle vtable:
┌─────────────────────────┐
│ Rectangle::area()→ 0x.. │  ← perašyta!
│ Rectangle::printInfo→0x.│  ← perašyta!
│ Rectangle::~Rect()→ 0x..│  ← perašyta!
└─────────────────────────┘
```

**Kiekvienas objektas** su `virtual` metodais turi **vptr** — rodyklę į savo klasės vtable:

```
Circle objektas atmintyje:
┌──────────────────┐
│ vptr ────────────┼──→ Circle vtable
│ center.x         │
│ center.y         │
│ color            │
│ radius           │
└──────────────────┘
```

**Kaip veikia `s->area()`:**

```
1. s yra Shape* → rodo į Circle objektą
2. Iš Circle objekto paimamas vptr
3. vptr rodo į Circle vtable
4. Vtable[area] rodo į Circle::area()
5. Kviečiama Circle::area() ✅
```

!!! note "sizeof padidėja"
    `virtual` prideda `vptr` — papildomas 8 baitų rodyklė kiekviename objekte.
    Patikrinkite: `sizeof(Shape)` su `virtual` > `sizeof(Shape)` be `virtual`.

!!! note "static binding greičiau"
    `virtual` turi nedidelę kainą — papildomas atminties iškvietimas per vptr/vtable.
    Praktikoje tai nereikšminga, bet verta žinoti kodėl C++ nepadaro **visų** metodų `virtual` pagal nutylėjimą (kaip Java).

---

!!! tip "Užduotis U6"
    1 dalies teoriją patikrinsite **U6 žingsniuose 1–2**:

    - **Žingsnis 1:** static binding problema — U5 kodas dar kartą
    - **Žingsnis 2:** `virtual` — tas pats `main.cpp`, skirtingas rezultatas

---
---

## 2 DALIS: override, virtual destructor, pure virtual

!!! abstract "Šios dalies tikslas"
    `virtual` išsprendė metodų pasirinkimo problemą. Dabar — papildomi įrankiai:

    - `override` — saugiklis nuo tylių klaidų
    - Virtual destructor — kodėl būtinas su `delete` per bazinę klasę
    - Pure virtual → abstrakčios klasės — kai bazinė klasė neturi prasmės "viena"

---

### `override` — būtinas saugiklis

`Circle::area()` jau perdengė `Shape::area()` — net be jokio papildomo raktažodžio. Tai veikia. Bet C++11 įvedė `override`:

```cpp linenums="1"
// Be override — veikia, bet rizikinga:
class Circle : public Shape {
    double area() const;        // ← ar tikrai perdengia? niekas netikrina
};

// Su override — kompiliatorius garantuoja:
class Circle : public Shape {
    double area() const override;  // ← jei neatitinka bazinio — [NC]!
};
```

**Kodėl override atsirado?** Klasikinė tyli klaida:

```cpp linenums="1"
class Shape {
    virtual double area() const;
};

class Circle : public Shape {
    double area();        // ← KLAIDA: pamiršta const!
    //                       Tai NE override — tai naujas metodas!
    //                       Kompiliatorius nepraneša.
    //                       Per Shape* vis tiek kviečiamas Shape::area()
};
```

```cpp linenums="1"
// Su override — kompiliatorius sugauna:
class Circle : public Shape {
    double area() override;   // ← [NC]! "area() does not override"
    //                           Studentas iš karto mato klaidą
};
```

**`override` istorija:** C++98/03 neturėjo `override`. Daugelis klaidų liko nepastebėtos. C++11 pridėjo kaip **neprivalomą**, bet **labai rekomenduojamą** raktažodį.

---

### `override` vs `overload`

Dažna painiava — du skirtingi dalykai:

| | `override` | `overload` |
|---|---|---|
| **Kas** | Perdengia bazinės klasės `virtual` metodą | Kitas metodas **tuo pačiu vardu** |
| **Kur** | Paveldėtoje klasėje | Bet kur (net toje pačioje klasėje) |
| **Parašas** | **Identiškas** baziniam | **Skirtingas** (kiti parametrai) |
| **`virtual`** | Būtinas bazinėje | Nereikalingas |
| **Tikrinamas** | Su `override` — kompiliatoriaus | Niekada |

```cpp linenums="1"
class Shape {
    virtual double area() const;       // ← bazinis virtual
};

class Circle : public Shape {
    double area() const override;      // ← override ✅ (identiškas parašas)
    double area(int precision) const;  // ← overload ✅ (skirtingas parašas)
    // double area() override;         // ← [NC] pamiršta const — ne override!
};
```

---

### Virtual destructor

Problema — kai `delete` kviečiamas per bazinės klasės rodyklę:

```cpp linenums="1"
Shape* s = new Circle(0, 0, 5.0);

delete s;   // ← kokį destruktorių kviesti?
```

=== "Be virtual destruktoriaus"

    ```cpp linenums="1"
    class Shape {
    public:
        ~Shape() {   // ← ne virtual!
            std::cout << "[Shape DTOR]\n";
        }
    };

    Shape* s = new Circle(0, 0, 5.0);
    delete s;
    // Išvestis:
    // [Shape DTOR]       ← tik Shape destruktorius!
    // [Circle DTOR] — NEKVIEČIAMAS → memory leak, undefined behavior!
    ```

=== "Su virtual destruktoriumi"

    ```cpp linenums="1"
    class Shape {
    public:
        virtual ~Shape() {   // ← virtual!
            std::cout << "[Shape DTOR]\n";
        }
    };

    Shape* s = new Circle(0, 0, 5.0);
    delete s;
    // Išvestis:
    // [Circle DTOR]  ← pirma Circle (per vtable)
    // [Shape DTOR]   ← tada Shape ✅
    ```

!!! danger "Taisyklė"
    Jei klasė turi bent vieną `virtual` metodą — destruktorius **visada** turi būti `virtual`.
    Be jo `delete` per bazinės klasės rodyklę → **undefined behavior**.

!!! note "Plačiau"
    Virtual destruktorius — tai ne tik destruktorių tvarkos klausimas,
    o **ownership semantikos** klausimas. Specialieji metodai, RAII ir
    Rule of Three/Five/Zero hierarchijoje —
    žr. [P08 papildoma medžiaga](08_Paskaita_v1_ext.md).

---

### Pure virtual → abstrakčios klasės

`Shape::area()` grąžina `0.0` — tai nelogiška. Jokia "abstrakti figūra" neturi ploto. Galime **priversti** kiekvieną paveldėtoją implementuoti `area()`:

```cpp linenums="1"
class Shape {
public:
    virtual double area() const = 0;      // ← pure virtual
    virtual void printInfo() const = 0;   // ← pure virtual
    virtual ~Shape() {}
};
```

`= 0` reiškia — **nėra implementacijos bazinėje klasėje**. Kiekvienas paveldėtojas **privalo** implementuoti.

**Pasekmė:** `Shape` tampa **abstrakčia klase** — jos objektų kurti neįmanoma:

```cpp linenums="1"
Shape s(0, 0);          // ❌ [NC] — abstrakti klasė!
Shape* s = new Shape(); // ❌ [NC] — abstrakti klasė!

Shape* s = new Circle(0, 0, 5.0);  // ✅ Circle implementuoja area()
```

**Abstrakti klasė vis tiek gali turėti:**
- Konstruktorių (kviečiamas per paveldėtojų grandinę)
- Duomenų narius (`center`, `color`)
- Nevirtualiius metodus (`getColor()`, `getCenter()`)

```cpp linenums="1"
// Shape abstrakti, bet konstruktorius egzistuoja:
class Circle : public Shape {
public:
    Circle(double x, double y, double r)
        : Shape(x, y),   // ← Shape konstruktorius kviečiamas!
          radius(r) {}
};
```

---

### Abstrakčios klasės nauda

```cpp linenums="1"
// Be pure virtual — galima "pamiršti" implementuoti:
class Triangle : public Shape {
    // Pamiršome area()! Kompiliatorius neklausia.
    // Per Shape* kviečiamas Shape::area() → 0.0
};

// Su pure virtual — kompiliatorius priverstas:
class Triangle : public Shape {
    // Pamiršome area()! Kompiliatorius:
    // error: cannot instantiate abstract class 'Triangle'
    // note: 'area' is a pure virtual function
};
```

!!! note "Polimorfizmo triumfas"
    Su `virtual` + pure virtual + abstrakčia klase turime pilną polimorfizmą:

    ```cpp
    std::vector<Shape*> shapes = {
        new Circle(0, 0, 3.0),
        new Rectangle(1, 1, 4, 2),
        new Triangle(2, 2, 3, 4, 5)
    };

    for (Shape* s : shapes) {
        s->printInfo();   // ← kiekvienas spausdina savo informaciją
        s->area();        // ← kiekvienas skaičiuoja savo plotą
    }
    // Shape nežino apie Circle, Rectangle, Triangle —
    // bet kodas veikia teisingai su visais!
    ```

---

### Apibendrinimas: pointer/reference būtinybė

Polimorfizmas veikia **tik per rodykles arba nuorodas**:

```cpp linenums="1"
// ✅ Per rodyklę — polimorfizmas veikia:
Shape* s = new Circle(0, 0, 5.0);
s->area();   // Circle::area() ✅

// ✅ Per nuorodą — polimorfizmas veikia:
Circle c(0, 0, 5.0);
Shape& ref = c;
ref.area();  // Circle::area() ✅

// ❌ Per reikšmę — slicing + static binding:
Shape s = Circle(0, 0, 5.0);  // slicing!
s.area();    // Shape::area() ❌
```

**Kodėl?** Per reikšmę — **kopijuojama** tik `Shape` dalis (slicing). `vptr` taip pat nukopijuojamas iš `Shape` — tad rodo į `Shape vtable`, ne `Circle vtable`.

```
Rodyklė/nuoroda:          Reikšmė (slicing):
┌──────────┐              ┌──────────┐
│ vptr ────┼──→ Circle    │ vptr ────┼──→ Shape vtable ❌
│ center   │    vtable ✅ │ center   │
│ color    │              │ color    │
│ radius   │              └──────────┘  (radius dingo!)
└──────────┘
```

---

!!! tip "Užduotis U6"
    2 dalies teoriją patikrinsite **U6 žingsniuose 3–5**:

    - **Žingsnis 3:** `override` — saugiklis ir tyčinės klaidos eksperimentas
    - **Žingsnis 4:** `Triangle` — naujas paveldėtojas savarankiškai
    - **Žingsnis 5:** Pure virtual + Virtual destructor

---

*[NC]: Not Compiling — Nesikompiliuoja
*[vtable]: Virtual function table
*[vptr]: Virtual pointer — rodyklė į vtable
