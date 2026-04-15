# Paveldėjimas

---

## 1 DALIS: is-a ryšys, sintaksė, konstruktorių grandinė

!!! abstract "Šios dalies tikslas"
    Iki šiol kūrėme klases, kurios **turi** kitas klases (has-a, kompozicija).
    Dabar — naujas ryšys: klasė, kuri **yra** kita klasė (is-a, paveldėjimas).

    - Suprasime **is-a** ryšį ir jo skirtumą nuo **has-a**
    - Išmoksime paveldėjimo sintaksę ir `protected` prieigą
    - Atrasime **konstruktorių grandinę** — kaip bazinė klasė inicializuojama
    - Pamatysime paveldėjimą **fiziniu** lygmeniu: kiek baitų užima objektas

---

### has-a vs is-a: du skirtingi ryšiai

Iki šiol visi mūsų ryšiai tarp klasių buvo **has-a** (kompozicija):

```cpp linenums="1"
class Room {
    vector<Window> windows;  // Room HAS-A Window
    Door* door;              // Room HAS-A Door
};
```

Dabar — **is-a** (paveldėjimas):

```
Circle    IS-A Shape   ✅  (kiekvienas apskritimas yra figūra)
Rectangle IS-A Shape   ✅  (kiekvienas stačiakampis yra figūra)
Shape     IS-A Circle  ❌  (ne kiekviena figūra yra apskritimas!)
```

!!! note "Svarbu"
    `is-a` — tai **loginis** ryšys, ne tik sintaksė.
    Prieš rašant `: public Base`, paklauskite: ar tikrai kiekvienas `Derived` objektas **yra** `Base`?

**Kuo paveldėjimas naudingas?**

```cpp linenums="1"
// Be paveldėjimo — kodas dubliuojasi:
class Circle {
    double x, y;       // centras
    string color;      // spalva
    double radius;
};

class Rectangle {
    double x, y;       // centras — TAS PATS!
    string color;      // spalva — TAS PATS!
    double width, height;
};
```

```cpp linenums="1"
// Su paveldėjimu — bendra dalis vieną kartą:
class Shape {
    double x, y;
    string color;
};

class Circle : public Shape {   // ← Circle IS-A Shape
    double radius;              // tik tai, kas nauja
};

class Rectangle : public Shape { // ← Rectangle IS-A Shape
    double width, height;         // tik tai, kas nauja
};
```

---

### Pagalbinė klasė: Point

Prieš kuriant `Shape` — maža pagalbinė `struct Point`. Atkreipkite dėmesį: `Shape` **turi** `Point` (has-a kompozicija), bet `Circle` **yra** `Shape` (is-a paveldėjimas). Tai du **skirtingi** ryšiai tarp **skirtingų** klasių — viename pavyzdyje abu kartu.

```cpp linenums="1"
// Point.h
#pragma once
#include <iostream>

struct Point {
    double x;
    double y;

    Point(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    void print() const {
        std::cout << "(" << x << ", " << y << ")";
    }
};
```

`struct` vietoj `class` — čia s1moningai: `Point` yra paprastas duomenų "konteineris be logikos", visi laukai `public` pagal nutylėjimą. Tai įprasta C++ praktika.

---

### Bazinė klasė: Shape

=== "Shape.h"

    ```cpp linenums="1"
    #pragma once
    #include "Point.h"
    #include <string>

    class Shape {
    protected:                          // ← ne private! kodėl — žr. žemiau
        Point center;                   // Shape HAS-A Point
        std::string color;

    public:
        Shape(double x, double y, const std::string& color = "black");
        ~Shape();

        Point getCenter() const;
        std::string getColor() const;
        void setColor(const std::string& color);

        double area() const;
        void printInfo() const;
    };
    ```

=== "Shape.cpp"

    ```cpp linenums="1"
    #include "Shape.h"
    #include <iostream>

    Shape::Shape(double x, double y, const std::string& color)
        : center(x, y), color(color)
    {
        std::cout << "[Shape CTOR] center=";
        center.print();
        std::cout << " color=" << color << "\n";
    }

    Shape::~Shape() {
        std::cout << "[Shape DTOR] color=" << color << "\n";
    }

    Point Shape::getCenter() const { return center; }
    std::string Shape::getColor() const { return color; }
    void Shape::setColor(const std::string& c) { color = c; }

    double Shape::area() const { return 0.0; }

    void Shape::printInfo() const {
        std::cout << "Shape [color=" << color << ", center=";
        center.print();
        std::cout << ", area=" << area() << "]\n";
    }
    ```

---

### `protected` — trečias prieigos lygmuo

Iki šiol žinojome du:

| Raktažodis | Matoma klasėje | Matoma paveldėtojuose | Matoma išorėje |
|---|---|---|---|
| `private` | ✅ | ❌ | ❌ |
| `public` | ✅ | ✅ | ✅ |
| **`protected`** | ✅ | ✅ | ❌ |

`protected` — tai `private`, bet **matoma paveldėtose klasėse**:

```cpp linenums="1"
class Circle : public Shape {
    void someMethod() {
        center.x = 1.0;    // ✅ protected — Circle mato
        color = "blue";    // ✅ protected — Circle mato
    }
};

int main() {
    Circle c(0, 0, 5.0);
    // c.center;           // ❌ protected — išorėje nematoma
    c.getCenter();         // ✅ public getter — matomas
}
```

!!! warning "Dažna klaida"
    Jei `center` ir `color` būtų `private` — `Circle` negalėtų jų pasiekti net metoduose. Kompiliavimo klaida.

---

### Paveldėjimo sintaksė

```cpp linenums="1"
class Circle : public Shape {
//             ^^^^^^ ←  paveldėjimo būdas (apie tai — 2 dalyje)
//                    ^^^^^ ← bazinė klasė
private:
    double radius;      // tik Circle specifinis laukas

public:
    Circle(double x, double y, double r,
           const std::string& color = "black");
    ~Circle();

    double getRadius() const;
    double area() const;
    void printInfo() const;
};
```

`Circle` **automatiškai gauna** visus `Shape` `public` ir `protected` narius:
- `getCenter()`, `getColor()`, `setColor()` — nereikia rašyti iš naujo
- `center`, `color` — matomi `Circle` metoduose (nes `protected`)

---

### Konstruktorių grandinė

**Svarbiausia taisyklė:** kai kuriamas `Circle` objektas — **pirmiausia** iškviečiamas `Shape` konstruktorius.

```cpp linenums="1"
Circle::Circle(double x, double y, double r, const std::string& color)
    : Shape(x, y, color),    // ← 1. Bazinės klasės konstruktorius
      radius(r)              // ← 2. Circle savo laukai
{
    std::cout << "[Circle CTOR] r=" << radius << "\n";
    // ← 3. Circle konstruktoriaus kūnas
}
```

**Kas atsitiktų be `: Shape(x, y, color)`?**

```cpp linenums="1"
// [NC] — nesikompiliuoja!
Circle::Circle(double x, double y, double r)
    : radius(r)
{
    // Kompiliatorius bandytų kviesti Shape() — be parametrų
    // Shape() neegzistuoja → kompiliavimo klaida!
}
```

```
error: no matching function for call to 'Shape::Shape()'
note: candidate: 'Shape::Shape(double, double, const string&)'
```

!!! note "Taisyklė"
    Jei bazinė klasė **neturi default konstruktoriaus** — paveldėtojo konstruktorius **privalo** kviesti bazinės klasės konstruktorių inicializavimo sąraše.

---

### Circle ir Rectangle — pilnas pavyzdys

=== "Circle.h"

    ```cpp linenums="1"
    #pragma once
    #include "Shape.h"
    #include <cmath>

    class Circle : public Shape {
    private:
        double radius;

    public:
        Circle(double x, double y, double r,
               const std::string& color = "black");
        ~Circle();

        double getRadius() const;
        double area() const;
        void printInfo() const;
    };
    ```

=== "Circle.cpp"

    ```cpp linenums="1"
    #include "Circle.h"
    #include <iostream>

    Circle::Circle(double x, double y, double r, const std::string& color)
        : Shape(x, y, color), radius(r)
    {
        std::cout << "[Circle CTOR] r=" << radius << "\n";
    }

    Circle::~Circle() {
        std::cout << "[Circle DTOR] r=" << radius << "\n";
    }

    double Circle::getRadius() const { return radius; }

    double Circle::area() const {
        return M_PI * radius * radius;
    }

    void Circle::printInfo() const {
        std::cout << "Circle [color=" << color
                  << ", center=";
        center.print();
        std::cout << ", r=" << radius
                  << ", area=" << area() << "]\n";
    }
    ```

=== "Rectangle.h"

    ```cpp linenums="1"
    #pragma once
    #include "Shape.h"

    class Rectangle : public Shape {
    private:
        double width;
        double height;

    public:
        Rectangle(double x, double y, double w, double h,
                  const std::string& color = "black");
        ~Rectangle();

        double getWidth() const;
        double getHeight() const;
        double area() const;
        double perimeter() const;
        void printInfo() const;
    };
    ```

=== "Rectangle.cpp"

    ```cpp linenums="1"
    #include "Rectangle.h"
    #include <iostream>

    Rectangle::Rectangle(double x, double y,
                         double w, double h,
                         const std::string& color)
        : Shape(x, y, color), width(w), height(h)
    {
        std::cout << "[Rectangle CTOR] w=" << width
                  << " h=" << height << "\n";
    }

    Rectangle::~Rectangle() {
        std::cout << "[Rectangle DTOR] w=" << width
                  << " h=" << height << "\n";
    }

    double Rectangle::getWidth()  const { return width; }
    double Rectangle::getHeight() const { return height; }
    double Rectangle::area()      const { return width * height; }
    double Rectangle::perimeter() const { return 2 * (width + height); }

    void Rectangle::printInfo() const {
        std::cout << "Rectangle [color=" << color
                  << ", center=";
        center.print();
        std::cout << ", w=" << width
                  << ", h=" << height
                  << ", area=" << area() << "]\n";
    }
    ```

=== "main.cpp"

    ```cpp linenums="1"
    #include <iostream>
    #include "Circle.h"
    #include "Rectangle.h"

    int main() {
        std::cout << "--- Kuriame figūras ---\n";
        Circle c1(0.0, 0.0, 5.0, "red");
        Rectangle r1(1.0, 1.0, 4.0, 3.0, "blue");

        std::cout << "\n--- printInfo ---\n";
        c1.printInfo();
        r1.printInfo();

        std::cout << "\n--- Paveldėti metodai ---\n";
        c1.setColor("green");          // ← Shape metodas!
        std::cout << c1.getColor() << "\n";

        std::cout << "\n--- Programa baigiasi ---\n";
        return 0;
    }
    ```

=== "⌨️➡️🖥️"

    ```
    --- Kuriame figūras ---
    [Shape CTOR] center=(0, 0) color=red
    [Circle CTOR] r=5
    [Shape CTOR] center=(1, 1) color=blue
    [Rectangle CTOR] w=4 h=3

    --- printInfo ---
    Circle [color=red, center=(0, 0), r=5, area=78.5398]
    Rectangle [color=blue, center=(1, 1), w=4, h=3, area=12]

    --- Paveldėti metodai ---
    green

    --- Programa baigiasi ---
    [Rectangle DTOR] w=4 h=3
    [Shape DTOR] color=green
    [Circle DTOR] r=5
    [Shape DTOR] color=red
    ```

---

### Destruktorių tvarka

Atkreipkite dėmesį į išvestį — destruktoriai kviečiami **atvirkščia** konstruktorių tvarka:

```
Konstruktoriai:          Destruktoriai:
[Shape CTOR]      →      [Circle DTOR]   (pirma Circle)
[Circle CTOR]     →      [Shape DTOR]    (tada Shape)
```

Logika paprasta: `Circle` dalis buvo sukurta **po** `Shape` dalies — vadinasi naikinama **prieš** ją.

---

### Fizinis aspektas: `sizeof`

Paveldėjimas — tai ne tik loginė abstrakcija. `Circle` objektas atmintyje **fiziškai apima/įtraukia/turi** `Shape` dalį:

```
Circle objektas atmintyje:
┌──────────────────────────┐
│  Shape dalis:            │
│    center.x  (8 baitų)   │
│    center.y  (8 baitų)   │
│    color     (~32 baitų) │  ← std::string dydis priklauso nuo realizacijos
│  ────────────────────    │
│  Circle dalis:           │
│    radius    (8 baitų)   │
└──────────────────────────┘
```

```cpp linenums="1"
std::cout << "sizeof(Shape):     " << sizeof(Shape)     << "\n";
std::cout << "sizeof(Circle):    " << sizeof(Circle)    << "\n";
std::cout << "sizeof(Rectangle): " << sizeof(Rectangle) << "\n";
```

```
sizeof(Shape):     48
sizeof(Circle):    56    ← +8 (double radius)
sizeof(Rectangle): 64    ← +16 (double width + double height)
```

!!! note "Tikslūs skaičiai"
    Priklausys nuo sistemos ir kompiliatoriaus. Svarbu **skirtumas** — jis visada atitiks papildomų laukų dydį.

---

!!! tip "Užduotis U5"
    1 dalies teoriją patikrinsite **U5 žingsniuose 1–4**:

    - **Žingsnis 1:** `Shape` bazinė klasė (pilnas pavyzdys)
    - **Žingsnis 2:** `Circle : public Shape` — konstruktorių grandinė
    - **Žingsnis 3:** `Rectangle : public Shape` — savarankiškai
    - **Žingsnis 4:** `protected` vs `private` eksperimentas

---
---

## 2 DALIS: Paveldėjimo būdai, upcasting, object slicing

!!! abstract "Šios dalies tikslas"
    Pirmoje dalyje išmokome sukurti paveldėjimo hierarchiją.
    Dabar — ką su ja galime daryti:

    - **Paveldėjimo būdai**: `public`, `protected`, `private` — kuo skiriasi?
    - **Upcasting**: kaip naudoti `Shape*` rodyklę su `Circle` objektu
    - **Rodyklių masyvas**: `Shape*` masyvas — C stiliaus ir `vector`
    - **Object slicing**: dažna klaida, kurios kompiliatorius neraportuoja

---

### Paveldėjimo būdai

Iki šiol naudojome tik `public` paveldėjimą. Yra trys būdai:

```cpp linenums="1"
class Circle    : public    Shape { ... };  // dažniausias
class CircleB   : protected Shape { ... };  // retai
class CircleC   : private   Shape { ... };  // "implemented in terms of"
```

Kuo jie skiriasi? Paveldėti nariai **keičia** savo prieigos lygmenį:

| Bazinėje klasėje | `public` paveldėjimas | `protected` paveldėjimas | `private` paveldėjimas |
|---|---|---|---|
| `public` narys | `public` | `protected` | `private` |
| `protected` narys | `protected` | `protected` | `private` |
| `private` narys | nepasiekiamas | nepasiekiamas | nepasiekiamas |

**Praktiškai:**

=== "public paveldėjimas"

    ```cpp linenums="1"
    class Circle : public Shape {
        // ...
    };

    int main() {
        Circle c(0, 0, 5.0);
        c.getColor();    // ✅ Shape::getColor() išlieka public
        c.setColor("red"); // ✅
    }
    ```

    **Naudojimas:** tikras **is-a** ryšys. `Circle` yra `Shape` — visas `Shape` interfeisas matomas išorėje.

=== "private paveldėjimas"

    ```cpp linenums="1"
    class CirclePrivate : private Shape {
        // ...
    };

    int main() {
        CirclePrivate c(0, 0, 5.0);
        // c.getColor();  // ❌ Shape nariai tapo private!
    }
    ```

    **Naudojimas:** "**implemented in terms of**" — `CirclePrivate` naudoja `Shape` **implementaciją**, bet neatskleidžia jos interfeiso. Ne is-a, o implementacijos detalė.

!!! note "Praktikoje"
    `public` paveldėjimas — 95% atvejų. `private` — specializuotas panaudojimas. `protected` — labai retai.

---

### Upcasting

`Circle` yra `Shape` — tad `Shape*` rodyklė gali rodyti į `Circle` objektą. Tai **upcasting** (kilimas aukštyn hierarchijoje):

```cpp linenums="1"
Circle c(0.0, 0.0, 5.0, "red");

Shape* p = &c;          // ✅ upcasting — visada saugus
                        // Circle IS-A Shape → Shape* gali rodyti į Circle

p->getColor();          // ✅ Shape metodas — veikia
p->setColor("blue");    // ✅ Shape metodas — veikia
// p->getRadius();      // ❌ Shape nežino apie radius — kompiliavimo klaida
```

Per `Shape*` matome tik `Shape` interfeisą — net jei objektas yra `Circle`.

**Ir per nuorodą:**

```cpp linenums="1"
Shape& ref = c;         // ✅ upcasting per nuorodą
ref.getColor();         // ✅
```

---

### Rodyklių masyvas: C stiliaus

Upcasting leidžia talpinti **skirtingų tipų** figūras viename masyve — per `Shape*`:

=== "C stiliaus masyvas"

    ```cpp linenums="1"
    #include <iostream>
    #include "Circle.h"
    #include "Rectangle.h"

    int main() {
        const int N = 4;
        Shape* shapes[N];    // ← masyvas rodyklių į Shape

        // Kuriame objektus dinaminėje atmintyje (heap'e)
        shapes[0] = new Circle(0.0, 0.0, 5.0, "red");
        shapes[1] = new Circle(1.0, 1.0, 3.0, "blue");
        shapes[2] = new Rectangle(2.0, 2.0, 4.0, 3.0, "green");
        shapes[3] = new Rectangle(3.0, 3.0, 6.0, 2.0, "black");

        std::cout << "\n--- Figūros ---\n";
        for (int i = 0; i < N; i++) {
            std::cout << "color=" << shapes[i]->getColor()
                      << ", center=";
            shapes[i]->getCenter().print();
            std::cout << "\n";
        }

        // SVARBU: atlaisvinti atmintį!
        for (int i = 0; i < N; i++) {
            delete shapes[i];
        }

        return 0;
    }
    ```

=== "⌨️➡️🖥️"

    ```
    [Shape CTOR] center=(0, 0) color=red
    [Circle CTOR] r=5
    [Shape CTOR] center=(1, 1) color=blue
    [Circle CTOR] r=3
    [Shape CTOR] center=(2, 2) color=green
    [Rectangle CTOR] w=4 h=3
    [Shape CTOR] center=(3, 3) color=black
    [Rectangle CTOR] w=6 h=2

    --- Figūros ---
    color=red,   center=(0, 0)
    color=blue,  center=(1, 1)
    color=green, center=(2, 2)
    color=black, center=(3, 3)

    [Circle DTOR] r=5
    [Shape DTOR] color=red
    [Circle DTOR] r=3
    [Shape DTOR] color=blue
    [Rectangle DTOR] w=4 h=3
    [Shape DTOR] color=green
    [Rectangle DTOR] w=6 h=2
    [Shape DTOR] color=black
    ```

**Atkreipkite dėmesį:**

- `shapes[i]` tipas — `Shape*`, bet objektas gali būti `Circle` arba `Rectangle`
- Prieiga tik per `Shape` interfeisą (`getColor`, `getCenter`) — `getRadius` nematomas
- `delete shapes[i]` — **privaloma**, nes objektai sukurti su `new`

---

### Rodyklių masyvas: vector<Shape*>

C stiliaus masyvas reikalauja žinoti dydį iš anksto. Modernesnė alternatyva — `vector<Shape*>`:

=== "vector<Shape*>"

    ```cpp linenums="1"
    #include <iostream>
    #include <vector>
    #include "Circle.h"
    #include "Rectangle.h"

    int main() {
        std::vector<Shape*> shapes;   // ← dinaminis dydis

        shapes.push_back(new Circle(0.0, 0.0, 5.0, "red"));
        shapes.push_back(new Circle(1.0, 1.0, 3.0, "blue"));
        shapes.push_back(new Rectangle(2.0, 2.0, 4.0, 3.0, "green"));
        shapes.push_back(new Rectangle(3.0, 3.0, 6.0, 2.0, "black"));

        std::cout << "\n--- Figūros ---\n";
        for (Shape* s : shapes) {
            std::cout << "color=" << s->getColor()
                      << ", center=";
            s->getCenter().print();
            std::cout << "\n";
        }

        // SVARBU: atlaisvinti kiekvieną objektą!
        for (Shape* s : shapes) {
            delete s;
        }

        return 0;
    }
    ```

=== "Palyginimas"

    | | `Shape* shapes[N]` | `vector<Shape*>` |
    |---|---|---|
    | Dydis | Fiksuotas (`const int N`) | Dinaminis (`push_back`) |
    | Sintaksė | C stiliaus | Modernesnė |
    | `delete` | Privaloma abu atveju | Privaloma abu atveju |
    | Iteravimas | `for (int i...)` | Range-based for |

!!! warning "Dažna klaida"
    `vector<Shape*>` valdo rodyklių **sąrašą**, bet ne objektus, į kuriuos jos rodo.
    `vector` destruktorius sunaikina rodykles — bet ne `Circle`/`Rectangle` objektus heap'e.
    **`delete` privalomas abiem atvejais!**

!!! note "Po gaubtu"
    `for (Shape* s : shapes)` atrodo moderniai, bet po gaubtu — ta pati **rodyklių aritmetika**:

    ```cpp
    // Range-based for iš esmės yra:
    for (int i = 0; i < shapes.size(); i++) {
        Shape* s = shapes[i];
        // arba dar giliau
        // Shape* s = *(shapes.data() + i);  // ← rodyklių aritmetika
        // ...
    }
    ```

    Modernios C++ priemonės (`vector`, range-based for) neišnaikina rodyklių aritmetikos — jos ją **paslepia**.

Natūralus klausimas — kodėl ne tiesiog `vector<Shape>` be rodyklių?

```cpp linenums="1"
vector<Shape> shapes;
shapes.push_back(Circle(0, 0, 5.0));      // ← kas čia vyksta?
shapes.push_back(Rectangle(1, 1, 4, 3));  // ← ?
```

Atsakymas — **object slicing**.

---

### Object slicing

**Object slicing** — kai paveldėtojo objektas priskiriamas bazinės klasės kintamajam **per reikšmę**. Paveldėtojo dalis „nupjaunama":

```cpp linenums="1"
Circle c(0.0, 0.0, 5.0, "red");

Shape s = c;    // ← SLICING! Kopijuojama tik Shape dalis
                //   radius dingo!
```

```
Prieš (c):                  Po (s = c):
┌────────────────┐           ┌────────────────┐
│ center=(0,0)   │   →       │ center=(0,0)   │  ✅
│ color="red"    │   →       │ color="red"    │  ✅
│ ─────────────  │           └────────────────┘
│ radius=5.0     │   →       (dingo!)            ❌
└────────────────┘
```

**Trys slicing scenarijai:**

=== "Priskyrimas kintamajam"

    ```cpp linenums="1"
    Circle c(0, 0, 5.0, "red");

    Shape s = c;          // ← slicing
    s.printInfo();        // Shape [color=red, center=(0,0), area=0]
                          //                                 ^^^^^^^^
                          //                    ne 78.54 — radius dingo!
    c.printInfo();        // Circle [color=red, center=(0,0), r=5, area=78.54]
    ```

=== "Perdavimas į funkciją per reikšmę"

    ```cpp linenums="1"
    void print(Shape s) {    // ← per reikšmę — SLICING!
        s.printInfo();
    }

    void printRef(const Shape& s) {  // ← per nuorodą — saugiau
        s.printInfo();
    }

    int main() {
        Circle c(0, 0, 5.0, "red");

        print(c);       // Shape [color=red, ..., area=0]   ← slicing
        printRef(c);    // Shape [color=red, ..., area=0]   ← taip pat 0!
                        // (apie tai — P08: virtual)
    }
    ```

=== "vector<Shape>"

    ```cpp linenums="1"
    vector<Shape> shapes;
    shapes.push_back(Circle(0, 0, 5.0, "red"));
    //               ↑ Circle sukuriamas, bet į vector įdedama tik Shape kopija
    //                 radius nupjaunamas!

    for (const Shape& s : shapes) {
        s.printInfo();  // Shape [area=0] — ne Circle!
    }
    ```

**Kodėl slicing nepavojingas su rodyklėmis?**

```cpp linenums="1"
vector<Shape*> shapes;
shapes.push_back(new Circle(0, 0, 5.0, "red"));
// ↑ Circle sukuriamas heap'e — visas, su radius
//   į vector įdedama tik RODYKLĖ (8 baitų) — nieko nenupjaunama!
```

!!! warning "Slicing — klastingas"
    Object slicing **nekelia kompiliavimo klaidos**. Programa kompiliuojasi ir veikia — tik **neteisingai**. Kompiliatorius nieko nepraneša.

    Todėl polimorfizmui (skirtingi objektai per bendrą interfeisą) naudojame **rodykles arba nuorodas**, ne reikšmes.

---

### Apibendrinimas: kada ką naudoti

```
Tikslas                              Sprendimas
─────────────────────────────────────────────────────
Vienas Circle objektas               Circle c(0, 0, 5.0);
Vienas, bet per Shape interfeisą     Shape* p = new Circle(0, 0, 5.0);
Kolekcija, fiksuotas dydis           Shape* shapes[N]; + new/delete
Kolekcija, dinaminis dydis           vector<Shape*>;   + new/delete
```

```
Vengti:
─────────────────────────────────────────────────────
Shape s = circle;          ← slicing (prarandama informacija)
vector<Shape>              ← slicing kiekvienam push_back
void f(Shape s)            ← slicing perdavime
```

---

!!! tip "Užduotis U5"
    2 dalies teoriją patikrinsite **U5 žingsnyje 5** ir **BONUS**:

    - **Žingsnis 5:** upcasting — C stiliaus masyvas ir `vector<Shape*>`
    - **BONUS:** Object slicing eksperimentas

---

*[NC]: Not Compiling — Nesikompiliuoja
*[RAII]: Resource Acquisition Is Initialization
