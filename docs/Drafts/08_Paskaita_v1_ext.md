# Polimorfizmas — papildoma medžiaga

!!! abstract "Apie šį dokumentą"
    Tai **papildoma medžiaga** prie [P08: Polimorfizmas](08_Paskaita_v1.md).

    Čia rasite koncentruotą atsakymą į klausimą, kuris išplaukia iš trijų
    per kursą sutiktų temų sankirtos:

    - **Objekto gyvavimo ciklas** — konstruktoriai, destruktorius
    - **Ownership semantika** — kas valdo resursą ir kas jį atlaisvina
    - **Polimorfinis elgesys** — kaip hierarchija veikia su specialiaisiais metodais

    > Konstruktoriai, destruktorius ir kopijos/perkėlimo operatoriai —
    > tai klasės **gyvavimo ciklo valdymo aparatas**.
    > Paveldėjimas nesunaikina šio aparato — jis jį **pratęsia per grandinę**.
    > RAII principas išlieka, bet atsakomybės paskirstomos hierarchijos lygiais.

---

## 1. Specialieji metodai — kas jie ir kiek jų

C++ klasėje yra **7 specialieji metodai** (kartais vadinami "didžioji septyneta"):

```cpp
class Shape {
public:
    // 1. Default konstruktorius
    Shape();

    // 2. Kopijos konstruktorius
    Shape(const Shape& other);

    // 3. Kopijos priskyrimo operatorius
    Shape& operator=(const Shape& other);

    // 4. Perkėlimo konstruktorius      (C++11)
    Shape(Shape&& other);

    // 5. Perkėlimo priskyrimo operatorius (C++11)
    Shape& operator=(Shape&& other);

    // 6. Destruktorius
    ~Shape();

    // + Parametriniai konstruktoriai (ne "specialieji" griežtąja prasme,
    //   bet svarbūs hierarchijoje)
    Shape(double x, double y, const std::string& color = "black");
};
```

### Du automatiškumo aspektai

**Aspektas 1: Kompiliatorius juos generuoja** jei nerašai pats.

Bet ne visada teisingai — jei klasė valdo dinaminę atmintį, kompiliatoriaus
sugeneruoti metodai daro **paviršinę kopiją** (shallow copy). Tai U4 tema:
Rule of Three / Rule of Five.

**Aspektas 2: Jie kviečiami automatiškai** — ne tiesiogiai programuotojo,
o C++ "sistemos":

```cpp
Circle c1(0, 0, 5.0);      // konstruktorius
Circle c2 = c1;             // kopijos konstruktorius (automatiškai)
c2 = c1;                    // kopijos priskyrimo op. (automatiškai)
{
    Circle c3(1, 1, 3.0);
}                           // destruktorius (automatiškai, scope pabaigoje)

std::vector<Circle> v;
v.push_back(c1);            // kopijos konstruktorius (automatiškai, vector viduje)
```

---

## 2. Kas paveldima, ko ne

```
Paveldima:                              Nepaveldima:
─────────────────────────────────────   ──────────────────────────────────────
✅ public/protected duomenų nariai      ❌ Konstruktoriai
✅ public/protected metodai             ❌ Destruktorius
✅ virtual metodai (per vtable)         ❌ Kopijos konstruktorius
✅ static nariai (bendri hierarchijai)  ❌ Kopijos priskyrimo operatorius
                                        ❌ Perkėlimo konstruktorius
                                        ❌ Perkėlimo priskyrimo operatorius
                                        ❌ friend deklaracijos
```

### Kodėl konstruktoriai nepaveldimi?

`Circle` žino kaip inicializuoti `radius` — `Shape` to nežino. Ir atvirkščiai —
`Shape` žino kaip inicializuoti `center` ir `color` — `Circle` to neturėtų kartoti.

**Sprendimas — grandinė:**

```cpp linenums="1"
Circle::Circle(double x, double y, double r, const std::string& color)
    : Shape(x, y, color),   // ← Shape dalis — Shape atsakomybė
      radius(r)              // ← Circle dalis — Circle atsakomybė
{}
```

Kiekvienas lygis inicializuoja **savo** dalį. Tai ne apribojimas — tai
**aiškus atsakomybių paskirstymas**.

### Kodėl destruktorius nepaveldimas?

Analogiškai — kiekvienas lygis valo **savo** resursus:

```cpp linenums="1"
Circle::~Circle() {
    // Valo Circle resursus (jei yra)
    std::cout << "[Circle DTOR]\n";
}   // ← čia automatiškai kviečiamas Shape::~Shape()

Shape::~Shape() {
    // Valo Shape resursus (jei yra)
    std::cout << "[Shape DTOR]\n";
}
```

Grandinė veikia **atvirkščia** konstruktorių tvarka — tai garantuoja,
kad "vaikas" visada sunaikinamas prieš "tėvą".

### Kodėl kopijos operatoriai nepaveldimi?

```cpp linenums="1"
// Kompiliatorius sugeneruoja Circle kopijos ctor:
Circle::Circle(const Circle& other)
    : Shape(other),          // ← automatiškai kviečia Shape kopijos ctor!
      radius(other.radius)   // ← Circle narius kopijuoja pats
{}
```

Kompiliatorius **automatiškai** įtraukia bazinės klasės kopijos ctor
į sugeneruotą paveldėtojo kopijos ctor. Tai veikia — **jei** Shape kopijos
ctor teisingas (t.y. ne shallow copy problema).

---

## 3. RAII hierarchijoje — ownership semantika

**RAII** (Resource Acquisition Is Initialization) — resurso valdymas
susietas su objekto gyvavimo ciklu.

### Trys ownership scenarijai

=== "Stack objektas"

    ```cpp linenums="1"
    {
        Circle c(0, 0, 5.0);
        // Shape dalis: sukurta Shape ctor
        // Circle dalis: sukurta Circle ctor

    }   // scope pabaiga:
        // Circle::~Circle() → Shape::~Shape()
        // automatiškai, teisingai, visada
    ```

    **Savininkas:** stack frame. Nereikia nieko daryti — RAII veikia tobulai.

=== "Heap per savo tipą"

    ```cpp linenums="1"
    Circle* c = new Circle(0, 0, 5.0);
    // ...
    delete c;
    // Circle::~Circle() → Shape::~Shape() ✅
    ```

    **Savininkas:** programuotojas. `delete` kviečia teisingą destruktorių,
    nes `c` tipas yra `Circle*`.

=== "Heap per bazinę klasę"

    ```cpp linenums="1"
    Shape* s = new Circle(0, 0, 5.0);  // ← upcasting
    // ...
    delete s;
    // Be virtual ~Shape(): tik Shape::~Shape() ← PROBLEMA!
    // Su virtual ~Shape(): Circle::~Circle() → Shape::~Shape() ✅
    ```

    **Savininkas:** programuotojas per `Shape*`. Čia RAII gali suirti
    be `virtual ~Shape()`.

### Ownership per kompoziciją vs paveldėjimą

```
Kompozicija (has-a):
    Room owns Window objects
    → Room destruktorius TURI valdyti Window gyvavimą
    → delete door; (jei pointer semantics)

Paveldėjimas (is-a):
    Circle IS-A Shape
    → Shape dalis yra Circle objekto DALIS
    → Ji niekada neegzistuoja atskirai
    → Grandinė automatiškai valdo gyvavimą
```

Esminis skirtumas: kompozicijoje savininkas **aktyviai valdo** komponento
gyvavimą. Paveldėjime — grandinė tai daro **automatiškai**.

---

## 4. Rule of Three/Five/Zero hierarchijoje

### Scenarijus: Shape su dinamine atmintimi

```cpp linenums="1"
class Shape {
protected:
    Point center;
    std::string color;
    char* name;          // ← dinaminė atmintis!

public:
    Shape(double x, double y, const std::string& color,
          const char* name);
    virtual ~Shape();                            // Rule of Three #1

    Shape(const Shape& other);                   // Rule of Three #2
    Shape& operator=(const Shape& other);        // Rule of Three #3
};
```

### Ar Circle reikia rašyti kopijos ctor?

```cpp linenums="1"
class Circle : public Shape {
    double radius;   // ← tik primityvus tipas, ne dinaminė atmintis
};
```

**Atsakymas:** Ne — jei `Shape` kopijos ctor teisingas, kompiliatorius
sugeneruos teisingą `Circle` kopijos ctor:

```cpp
// Kompiliatorius sugeneruoja:
Circle::Circle(const Circle& other)
    : Shape(other),          // ← kviečia Shape kopijos ctor (gilus kopijavimas!)
      radius(other.radius)   // ← primityvus tipas, paprasta kopija
{}
```

### Kada Circle reikia rašyti kopijos ctor?

Tik jei `Circle` **pats** valdo dinaminę atmintį:

```cpp linenums="1"
class Circle : public Shape {
    double* points;    // ← Circle valdo dinaminę atmintį
    int pointCount;

public:
    Circle(const Circle& other)
        : Shape(other),                          // ← Shape dalis
          pointCount(other.pointCount),
          points(new double[other.pointCount])   // ← Circle dalis: gilusis kopijavimas
    {
        std::copy(other.points, other.points + pointCount, points);
    }
};
```

### Rule of Zero — modernaus C++ tikslas

Geriausias kodas — kai **nereikia rašyti jokio** specialiojo metodo:

```cpp linenums="1"
class Shape {
protected:
    Point center;          // value type — RAII automatiškai
    std::string color;     // std::string — RAII automatiškai

public:
    // Konstruktoriai — taip, reikia (inicializacija)
    // Destruktorius — NE (nėra dinaminės atminties)
    // Kopijos ctor — NE (kompiliatorius sugeneruoja teisingai)
    // Kopijos op. — NE (kompiliatorius sugeneruoja teisingai)
};
```

**Taisyklė:** naudok `std::string`, `std::vector`, `std::unique_ptr`
vietoj `char*`, `T[]`, `T*` — ir Rule of Zero veikia savaime.

---

## 5. Virtual metodai ir specialieji metodai — sąveika

### Ar konstruktorius gali būti virtual?

**Ne** — ir tai logiška:

```cpp linenums="1"
// [NC] — konstruktorius negali būti virtual:
virtual Shape(double x, double y);  // ← klaida!
```

Kodėl? `virtual` reikalauja `vptr` — bet `vptr` nustatomas **konstruktoriaus metu**.
Konstruktoriaus metu objektas dar "nėra" `Circle` — jis tik tampa `Circle`.
Todėl `virtual` konstruktorius būtų prieštaravimas pats sau.

### Ar destruktorius TURI būti virtual?

**Taisyklė:** jei klasė turi bent vieną `virtual` metodą —
destruktorius **visada** turi būti `virtual`.

```cpp linenums="1"
class Shape {
public:
    virtual double area() const = 0;  // ← virtual metodas
    virtual ~Shape() {}               // ← todėl destruktorius virtual!
};
```

### Ar kopijos ctor/operatorius gali būti virtual?

**Ne** — ir tai taip pat logiška:

```cpp linenums="1"
// Kopijos ctor negali būti virtual:
virtual Shape(const Shape& other);   // ← [NC]
```

Kopijos ctor kuria **naują** objektą — bet `virtual` mechanizmas
reikalauja **egzistuojančio** objekto `vptr`. Prieštaravimas.

**Alternatyva** — `virtual clone()` metodas (Factory pattern):

```cpp linenums="1"
class Shape {
public:
    virtual Shape* clone() const = 0;  // ← "virtual kopijos ctor"
};

class Circle : public Shape {
public:
    Circle* clone() const override {
        return new Circle(*this);   // ← kviečia Circle kopijos ctor
    }
};

// Naudojimas:
Shape* s = new Circle(0, 0, 5.0);
Shape* copy = s->clone();   // ← sukuria Circle kopiją per Shape*!
```

---

## 6. Apibendrinimas: trys kertiniai dalykai kartu

```
OBJEKTO GYVAVIMO CIKLAS:
    Konstruktorių grandinė    →  kiekvienas lygis inicializuoja savo dalį
    Destruktorių grandinė     →  kiekvienas lygis valo savo dalį (atvirkščiai)
    Virtual destruktorius     →  garantuoja teisingą grandinę per Shape*

OWNERSHIP SEMANTIKA:
    Stack objektas            →  RAII automatiškas, nereikia nieko
    Heap per savo tipą        →  delete veikia teisingai
    Heap per bazinę klasę     →  BŪTINAS virtual destruktorius
    Kompozicija               →  savininkas aktyviai valdo
    Paveldėjimas              →  grandinė valdo automatiškai

POLIMORFINIS ELGESYS:
    virtual metodai           →  dynamic binding per vtable
    Konstruktoriai             →  NE virtual (objektas dar kuriamas)
    Destruktorius             →  virtual (objektas naikinamas per bazinę)
    Kopijos operatoriai       →  NE virtual (clone() kaip alternatyva)
    Rule of Three/Five        →  taikomas kiekviename lygyje atskirai
    Rule of Zero              →  tikslas — nereikia rašyti nieko
```

---

## 7. Abstrakčios klasės kaip interfeisai

C++ abstrakti klasė su vien tik `pure virtual` metodais ir be duomenų narių —
tai **interfeisas** (interface):

```cpp linenums="1"
// "Gryna" abstrakti klasė = interfeisas
class IDrawable {
public:
    virtual void draw() const = 0;
    virtual ~IDrawable() = default;  // ← = default: kompiliatorius sugeneruoja
};

class ISerializable {
public:
    virtual std::string serialize() const = 0;
    virtual ~ISerializable() = default;
};
```

`Shape` gali paveldėti kelis interfeisus:

```cpp linenums="1"
class Shape : public IDrawable, public ISerializable {
    // ...
    void draw() const override { printInfo(); }
    std::string serialize() const override { /* ... */ }
};
```

### Palyginimas su kitomis kalbomis

| | C++ | Java | C# |
|---|---|---|---|
| Interfeisas | Abstrakti klasė (be duomenų) | `interface` raktažodis | `interface` raktažodis |
| Duomenys | Negalima (konvencija) | Negalima | Negalima |
| Kelių paveldėjimas | ✅ (`class A : public I1, public I2`) | ✅ (`implements I1, I2`) | ✅ (`: I1, I2`) |
| Abstrakti klasė | `= 0` metodai | `abstract class` | `abstract class` |
| Pavadinimų konvencija | `IDrawable` (tradicija) | `Drawable` | `IDrawable` |

**Pastaba:** Java ir C# atskyrė `interface` nuo `abstract class` — tai paprasčiau
pradedantiesiems. C++ leidžia abu modeliuoti per tą pačią klasę, bet reikia
disciplinos (konvencijų).

!!! note "Kodėl C++ neturi `interface` raktažodžio?"
    Stroustrup sprendimas: C++ turi kelių paveldėjimą ir abstrakčias klases —
    `interface` būtų tik sintaksinis cukrus. Kitos kalbos atsisakė kelių
    paveldėjimo (per daug sudėtinga), tad `interface` tapo būtinas kompromisas.

---

## 8. `final` raktažodis

C++11 įvedė `final` — draudžia tolesnį paveldėjimą arba perdenginėjimą:

```cpp linenums="1"
// Klasės lygmeniu — negalima paveldėti:
class Circle final : public Shape {
    // ...
};

class SpecialCircle : public Circle { };  // ← [NC]! Circle yra final

// Metodo lygmeniu — negalima toliau perdenginėti:
class Shape {
    virtual double area() const;
};

class Circle : public Shape {
    double area() const override final;  // ← override ir final kartu
};

class SpecialCircle : public Circle {
    double area() const override;  // ← [NC]! area() yra final
};
```

**Kada naudoti `final`?**

- Kai klasė sąmoningai suprojektuota **nebūti paveldima**
- Kai metodas turi specifinę semantiką, kurios **negalima keisti**
- Kompiliatorius gali optimizuoti `final` metodus (žino tikslų tipą)

**Praktikoje:** `final` naudojamas retai — dažniau tai dizaino sprendimas,
ne techninis apribojimas.

---

## 9. Pamąstymas: RegularPolygon hierarchijoje

??? tip "Gilesniam mąstymui"

    Įsivaizduokime išplėstą hierarchiją:

    ```
    Shape (abstrakti)
      ├── Circle
      ├── Rectangle
      └── RegularPolygon (abstrakti? ar konkreti?)
            ├── Triangle (3 kraštinės)
            ├── Square (4 kraštinės)
            └── Pentagon (5 kraštinės)
    ```

    `RegularPolygon` galėtų turėti **bendrą** `area()` formulę:

    ```cpp
    // Taisyklingo n-kampio plotas:
    // area = (n * s² * cot(π/n)) / 4
    // kur n = kraštinių skaičius, s = kraštinės ilgis

    class RegularPolygon : public Shape {
    protected:
        int sides;
        double sideLength;
    public:
        double area() const override {
            double angle = M_PI / sides;
            return (sides * sideLength * sideLength) / (4.0 * tan(angle));
        }
        // printInfo() vis tiek pure virtual — kiekvienas pats spausdina
        virtual void printInfo() const = 0;
    };
    ```

    **Įdomus klausimas:** ar `RegularPolygon` turėtų būti abstrakti?

    - `area()` turi implementaciją → ne pure virtual
    - `printInfo()` neturi → pure virtual → klasė abstrakti

    Taigi `RegularPolygon` yra **iš dalies abstrakti** — turi vieną
    konkretų ir vieną pure virtual metodą. Tai visiškai legalu C++.

    **Ir štai kur iškyla `Square` dilema:**

    ```
    RegularPolygon::Square (4 lygios kraštinės)
    vs
    Rectangle (plotis ≠ aukštis)
    ```

    `Square IS-A RegularPolygon` ✅  
    `Square IS-A Rectangle`? — LSP pažeidimas! (žr. P07 kritika)

    Tai puikus pavyzdys kodėl hierarchijos projektavimas yra svarbus
    **prieš** rašant kodą.

---

*[RAII]: Resource Acquisition Is Initialization
*[NC]: Not Compiling — Nesikompiliuoja
*[vtable]: Virtual function table
*[vptr]: Virtual pointer — rodyklė į vtable
*[LSP]: Liskov Substitution Principle
