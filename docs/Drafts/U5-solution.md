# U5: Paveldėjimas — Sprendimas

> **Tik dėstytojui** — nedalinti studentams prieš atsiskaitymus.

---

## 📁 Failų struktūra (bendra)

```
U5/
├── 01/   Shape bazinė klasė
├── 02/   Circle : public Shape
├── 03/   Rectangle : public Shape
├── 04/   Prieigos lygiai
├── 05/   Upcasting — vector<Shape*>
└── 06/   Object Slicing
```

Kiekviename žingsnyje: `Point.h`, `Shape.h`, `Shape.cpp` — identiški 01/ versijai (kopijavimas).

---

## Bendri failai (visi žingsniai)

### Point.h

```cpp
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

### Shape.h

```cpp
#pragma once
#include "Point.h"
#include <string>

class Shape {
protected:
    Point center;
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

### Shape.cpp

```cpp
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

## DALIS 1: Shape hierarchija

---

## 1 žingsnis: Shape bazinė klasė

Pilnas pavyzdys — studentas kopijuoja ir paleidžia.

### main.cpp

```cpp
#include <iostream>
#include "Shape.h"

int main() {
    std::cout << "=== 1 žingsnis: Shape bazinė klasė ===\n\n";

    Shape s1(0.0, 0.0);
    Shape s2(3.0, 4.0, "red");

    s1.printInfo();
    s2.printInfo();

    std::cout << "\nCentras: ";
    s2.getCenter().print();
    std::cout << "\n";

    std::cout << "\n--- Programa baigiasi ---\n";
    return 0;
}
```

### Laukiama išvestis

```
=== 1 žingsnis: Shape bazinė klasė ===

[Shape CTOR] center=(0, 0) color=black
[Shape CTOR] center=(3, 4) color=red
Shape [color=black, center=(0, 0), area=0]
Shape [color=red, center=(3, 4), area=0]

Centras: (3, 4)

--- Programa baigiasi ---
[Shape DTOR] color=red
[Shape DTOR] color=black
```

### Pedagoginės įžvalgos

- `area()` grąžina `0.0` — tyčia. Studentas turėtų paklausti "kodėl?". Atsakymas: `Shape` nežino savo formos. Tai motyvacija virtualiosioms funkcijoms (U6).
- `protected` šiame žingsnyje dar "neatidirbo" — skatinti studentą pakeisti į `private` ir įsitikinti, kad 1 žingsnyje viskas veikia taip pat. Skirtumas paaiškės 2 žingsnyje.
- `s2.getCenter().print()` — kompozicinis iškvietimas. Geras klausimas egzaminui: "kodėl `.` ne `->`?"

### Makefile

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall

all: programa

programa: main.o Shape.o
	$(CXX) $(CXXFLAGS) -o programa main.o Shape.o

main.o: main.cpp Shape.h Point.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Shape.o: Shape.cpp Shape.h Point.h
	$(CXX) $(CXXFLAGS) -c Shape.cpp

clean:
	rm -f *.o programa
```

---

## 2 žingsnis: Circle paveldi Shape

### Circle.h

```cpp
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

### Circle.cpp

```cpp
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
    std::cout << "Circle [color=" << color << ", center=";
    center.print();
    std::cout << ", r=" << radius
              << ", area=" << area() << "]\n";
}
```

### Makefile

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall

all: programa

programa: main.o Shape.o Circle.o
	$(CXX) $(CXXFLAGS) -o programa main.o Shape.o Circle.o

main.o: main.cpp Circle.h Shape.h Point.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Shape.o: Shape.cpp Shape.h Point.h
	$(CXX) $(CXXFLAGS) -c Shape.cpp

Circle.o: Circle.cpp Circle.h Shape.h
	$(CXX) $(CXXFLAGS) -c Circle.cpp

clean:
	rm -f *.o programa
```

### main.cpp

```cpp
#include <iostream>
#include "Circle.h"

int main() {
    std::cout << "=== 2 žingsnis: Circle paveldi Shape ===\n\n";

    std::cout << "--- Kuriame c1 ---\n";
    Circle c1(0.0, 0.0, 5.0);

    std::cout << "\n--- Kuriame c2 ---\n";
    Circle c2(3.0, 4.0, 2.5, "red");

    std::cout << "\n--- printInfo ---\n";
    c1.printInfo();
    c2.printInfo();

    std::cout << "\n--- sizeof ---\n";
    std::cout << "sizeof(Shape):  " << sizeof(Shape)  << " baitų\n";
    std::cout << "sizeof(Circle): " << sizeof(Circle) << " baitų\n";
    std::cout << "Skirtumas:      " << sizeof(Circle) - sizeof(Shape)
              << " baitų (double radius)\n";

    std::cout << "\n--- Programa baigiasi ---\n";
    return 0;
}
```

### Laukiama išvestis

```
=== 2 žingsnis: Circle paveldi Shape ===

--- Kuriame c1 ---
[Shape CTOR] center=(0, 0) color=black
[Circle CTOR] r=5

--- Kuriame c2 ---
[Shape CTOR] center=(3, 4) color=red
[Circle CTOR] r=2.5

--- printInfo ---
Circle [color=black, center=(0, 0), r=5, area=78.5398]
Circle [color=red, center=(3, 4), r=2.5, area=19.635]

--- sizeof ---
sizeof(Shape):  48 baitų
sizeof(Circle): 56 baitų
Skirtumas:      8 baitų (double radius)

--- Programa baigiasi ---
[Circle DTOR] r=2.5
[Shape DTOR] color=red
[Circle DTOR] r=5
[Shape DTOR] color=black
```

### Pedagoginės įžvalgos

- **Konstruktorių grandinė** — dažniausia klaida: pamiršti `: Shape(x, y, color)`. Kompiliavimo klaida aiški, bet studentai kartais bando "kitus kelius". Verta parodyti klaidą gyvai.
- **`color` ir `center` `printInfo()` viduje** — studentas turi pamatyti, kad `protected` nariai pasiekiami tiesiogiai be getter'ių. Jei buvo pakeista į `private` 1 žingsnyje — dabar kompiliavimo klaida. Puikus "aha!" momentas.
- **`sizeof` skirtumas** — verta sustoti ir aptarti: `Circle` = `Shape` + `double`. Fizinis paveldėjimo aspektas.
- **Destruktorių tvarka** — atvirkščia konstruktorių tvarkai. Geras egzamino klausimas.

---

## 3 žingsnis: Rectangle paveldi Shape

### Rectangle.h

```cpp
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

    double getWidth()  const;
    double getHeight() const;
    double area()      const;
    double perimeter() const;
    void printInfo()   const;
};
```

### Rectangle.cpp

```cpp
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

double Rectangle::getWidth()    const { return width; }
double Rectangle::getHeight()   const { return height; }
double Rectangle::area()        const { return width * height; }
double Rectangle::perimeter()   const { return 2 * (width + height); }

void Rectangle::printInfo() const {
    std::cout << "Rectangle [color=" << color << ", center=";
    center.print();
    std::cout << ", w=" << width
              << ", h=" << height
              << ", area=" << area() << "]\n";
}
```

### Makefile

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall

all: programa

programa: main.o Shape.o Circle.o Rectangle.o
	$(CXX) $(CXXFLAGS) -o programa main.o Shape.o Circle.o Rectangle.o

main.o: main.cpp Circle.h Rectangle.h Shape.h Point.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Shape.o: Shape.cpp Shape.h Point.h
	$(CXX) $(CXXFLAGS) -c Shape.cpp

Circle.o: Circle.cpp Circle.h Shape.h
	$(CXX) $(CXXFLAGS) -c Circle.cpp

Rectangle.o: Rectangle.cpp Rectangle.h Shape.h
	$(CXX) $(CXXFLAGS) -c Rectangle.cpp

clean:
	rm -f *.o programa
```

### main.cpp

```cpp
#include <iostream>
#include "Circle.h"
#include "Rectangle.h"

int main() {
    std::cout << "=== 3 žingsnis: Rectangle paveldi Shape ===\n\n";

    Circle c1(0.0, 0.0, 5.0, "blue");
    Rectangle r1(1.0, 1.0, 4.0, 3.0, "green");

    std::cout << "\n--- Figūros ---\n";
    c1.printInfo();
    r1.printInfo();

    std::cout << "\n--- Palyginimas ---\n";
    std::cout << "Circle plotas:    " << c1.area() << "\n";
    std::cout << "Rectangle plotas: " << r1.area() << "\n";
    std::cout << "Perimetras:       " << r1.perimeter() << "\n";

    std::cout << "\n--- sizeof ---\n";
    std::cout << "sizeof(Shape):     " << sizeof(Shape)     << "\n";
    std::cout << "sizeof(Circle):    " << sizeof(Circle)    << "\n";
    std::cout << "sizeof(Rectangle): " << sizeof(Rectangle) << "\n";

    std::cout << "\n--- Programa baigiasi ---\n";
    return 0;
}
```

### Laukiama išvestis

```
[Shape CTOR] center=(0, 0) color=blue
[Circle CTOR] r=5
[Shape CTOR] center=(1, 1) color=green
[Rectangle CTOR] w=4 h=3

=== 3 žingsnis: Rectangle paveldi Shape ===

--- Figūros ---
Circle [color=blue, center=(0, 0), r=5, area=78.5398]
Rectangle [color=green, center=(1, 1), w=4, h=3, area=12]

--- Palyginimas ---
Circle plotas:    78.5398
Rectangle plotas: 12
Perimetras:       14

--- sizeof ---
sizeof(Shape):     48
sizeof(Circle):    56
sizeof(Rectangle): 64

--- Programa baigiasi ---
[Rectangle DTOR] w=4 h=3
[Shape DTOR] color=green
[Circle DTOR] r=5
[Shape DTOR] color=blue
```

### Pedagoginės įžvalgos

- Šis žingsnis — **savarankiškas**. Studentas, kuris suprato 2 žingsnį, turėtų susitvarkyti be pagalbos.
- **Dažna klaida:** `perimeter()` — studentai kartais pamiršta `2 *` arba sudeda tik `width + height`.
- **`sizeof(Rectangle)` = `sizeof(Shape)` + 16** — du `double` laukai. Verta palyginti su `Circle` (+8).

---

## DALIS 2: Paveldėjimo būdai, Upcasting, Slicing

---

## 4 žingsnis: Prieigos lygiai ir paveldėjimo būdai

### Dalis A — laukiama kompiliavimo klaida

Pakeitus `protected` į `private` `Shape.h`:

```
// gcc:
Circle.cpp: error: 'std::string Shape::color' is private within this context
Circle.cpp: error: 'Point Shape::center' is private within this context

// clang:
Circle.cpp: error: 'color' is a private member of 'Shape'
Circle.cpp: error: 'center' is a private member of 'Shape'
```

**Studentas turi įrašyti į komentarą:**
```cpp
// KLAIDA (gcc): 'std::string Shape::color' is private within this context
// IŠVADA: private nariai paveldėtose klasėse nepasiekiami net iš jų pačių metodų
```

### Dalis B — paveldėjimo būdai

```cpp
// main.cpp
PublicCircle    pc(0, 0, 1.0);
ProtectedCircle tc(0, 0, 1.0);
PrivateCircle   vc(0, 0, 1.0);

pc.getColor();   // ✅ veikia — public paveldėjimas išsaugo public
// tc.getColor();  // ❌ [NC] — protected paveldėjimas: getColor() tapo protected
// vc.getColor();  // ❌ [NC] — private paveldėjimas: getColor() tapo private
```

### main.cpp (Dalis A — po private eksperimento, grąžinus protected)

```cpp
#include <iostream>
#include "Circle.h"
#include "Rectangle.h"

// Dalis B — mini eksperimentas paveldėjimo būdais
class PublicCircle : public Shape {
public:
    PublicCircle(double x, double y, double r) : Shape(x, y), radius(r) {}
private:
    double radius;
};

class ProtectedCircle : protected Shape {
public:
    ProtectedCircle(double x, double y, double r) : Shape(x, y), radius(r) {}
private:
    double radius;
};

class PrivateCircle : private Shape {
public:
    PrivateCircle(double x, double y, double r) : Shape(x, y), radius(r) {}
private:
    double radius;
};

int main() {
    std::cout << "=== 4 žingsnis: Prieigos lygiai ===\n\n";

    // Dalis A: rezultatas po eksperimento
    // KLAIDA (gcc): 'std::string Shape::color' is private within this context
    // IŠVADA: private nariai paveldėtose klasėse nepasiekiami net iš jų pačių metodų

    // Dalis B:
    PublicCircle    pc(0, 0, 1.0);
    ProtectedCircle tc(0, 0, 1.0);
    PrivateCircle   vc(0, 0, 1.0);

    pc.getColor();     // ✅ veikia — public išlieka public
    // tc.getColor();  // ❌ [NC] — protected: getColor() tapo protected
    // vc.getColor();  // ❌ [NC] — private: getColor() tapo private

    // IŠVADA:
    // public    paveldėjimas: public nariai išlieka public  (is-a ryšys)
    // protected paveldėjimas: public nariai tampa protected (retai naudojama)
    // private   paveldėjimas: public nariai tampa private   ("implemented in terms of")

    return 0;
}
```

### Laukiama išvestis

```
[Shape CTOR] center=(0, 0) color=black
[Shape CTOR] center=(0, 0) color=black
[Shape CTOR] center=(0, 0) color=black
```

*(Trys Shape objektai sukuriami — po vieną kiekvienai klasei. `getColor()` kvietimas nekelia išvesties.)*

### Pedagoginės įžvalgos

- **Dalis A** — studentas *pats* sukelia klaidą ir *pats* ją dokumentuoja. Tai efektyviau nei skaityti aprašymą.
- **Dalis B** — trys klasės viename faile atrodo keistai, bet tai tyčia — kompaktiškas eksperimentas. Verta paaiškinti: tai tik demonstracija, ne tipinis kodo stilius.
- `private` paveldėjimas — "implemented in terms of" koncepcija. Studentams pakanka suprasti, kad tai **ne** is-a ryšys.

---

## 5 žingsnis: Upcasting — vector\<Shape*\>

### main.cpp

```cpp
#include <iostream>
#include <vector>
#include "Circle.h"
#include "Rectangle.h"

int main() {
    std::cout << "=== 5 žingsnis: Upcasting ===\n\n";

    // --- A būdas: C stiliaus rodyklių masyvas ---
    const int N = 4;
    Shape* arr[N];

    arr[0] = new Circle(0.0, 0.0, 3.0, "red");
    arr[1] = new Circle(1.0, 1.0, 5.0, "blue");
    arr[2] = new Rectangle(2.0, 2.0, 4.0, 2.0, "green");
    arr[3] = new Rectangle(3.0, 3.0, 6.0, 3.0, "black");

    std::cout << "--- A būdas: Shape* arr[] ---\n";
    for (int i = 0; i < N; i++) {
        std::cout << "color=" << arr[i]->getColor()
                  << ", center=";
        arr[i]->getCenter().print();
        std::cout << ", area=" << arr[i]->area() << "\n";
        // area() visada 0.0 — kviečiama Shape::area()
        // kompiliatorius pasirenka pagal rodyklės tipą (Shape*), ne objekto tipą
    }

    for (int i = 0; i < N; i++) {
        delete arr[i];
    }

    // --- B būdas: vector<Shape*> ---
    std::vector<Shape*> shapes;
    shapes.push_back(new Circle(0.0, 0.0, 3.0, "red"));
    shapes.push_back(new Circle(1.0, 1.0, 5.0, "blue"));
    shapes.push_back(new Rectangle(2.0, 2.0, 4.0, 2.0, "green"));
    shapes.push_back(new Rectangle(3.0, 3.0, 6.0, 3.0, "black"));

    std::cout << "\n--- B būdas: vector<Shape*> ---\n";
    for (Shape* s : shapes) {
        std::cout << "color=" << s->getColor()
                  << ", center=";
        s->getCenter().print();
        std::cout << ", area=" << s->area() << "\n";
    }

    for (Shape* s : shapes) {
        delete s;
    }

    return 0;
}
```

### Laukiama išvestis

```
=== 5 žingsnis: Upcasting ===

[Shape CTOR] center=(0, 0) color=red
[Circle CTOR] r=3
[Shape CTOR] center=(1, 1) color=blue
[Circle CTOR] r=5
[Shape CTOR] center=(2, 2) color=green
[Rectangle CTOR] w=4 h=2
[Shape CTOR] center=(3, 3) color=black
[Rectangle CTOR] w=6 h=3

--- A būdas: Shape* arr[] ---
color=red,   center=(0, 0), area=0
color=blue,  center=(1, 1), area=0
color=green, center=(2, 2), area=0
color=black, center=(3, 3), area=0
[Circle DTOR] r=3
[Shape DTOR] color=red
...

--- B būdas: vector<Shape*> ---
color=red,   center=(0, 0), area=0
color=blue,  center=(1, 1), area=0
color=green, center=(2, 2), area=0
color=black, center=(3, 3), area=0
[Circle DTOR] r=3
[Shape DTOR] color=red
...
```

### Pedagoginės įžvalgos

- **`area() = 0.0`** — tai šio žingsnio esminis "skausmas". Studentas sukuria `Circle`, bet per `Shape*` nemato jo ploto. Tai stipriausia motyvacija virtualiosioms funkcijoms.
- **`delete` tvarka** — studentai kartais pamiršta. Verta patikrinti ar visi `delete` yra.
- **Destruktorių logging** — parodo, kad `Circle`/`Rectangle` destruktoriai kviečiami teisingai net per `Shape*`. Tai svarbu — be `virtual` destruktoriaus būtų problema (U6 tema).

---

---

## BONUS: Object Slicing

### Shape.h (su default konstruktoriumi — pataisyta)

```cpp
#pragma once
#include "Point.h"
#include <string>

class Shape {
protected:
    Point center;
    std::string color;

public:
    // Default konstruktorius — būtinas Shape arr[N]
    Shape(double x = 0.0, double y = 0.0,
          const std::string& color = "black");
    ~Shape();

    Point getCenter() const;
    std::string getColor() const;
    void setColor(const std::string& color);

    double area() const;
    void printInfo() const;
};
```

Pastaba: visi ankstesni žingsniai veikia nepakitę — default argumentai atgaliniai suderinami.

### main.cpp

```cpp
#include <iostream>
#include <vector>
#include "Circle.h"
#include "Rectangle.h"

void printShapeByValue(Shape s) {
    s.printInfo();
}

void printShapeByRef(const Shape& s) {
    s.printInfo();
}

void printShapeByPtr(const Shape* s) {
    s->printInfo();
}

int main() {
    std::cout << "=== BONUS: Object Slicing ===\n\n";

    Circle c(0.0, 0.0, 5.0, "red");

    // 1 dalis — priskyrimas ir perdavimas
    std::cout << "--- Shape s = c ---\n";
    c.printInfo();
    Shape s = c;
    s.printInfo();

    std::cout << "\n--- perdavimas per reikšmę ---\n";
    printShapeByValue(c);

    std::cout << "\n--- perdavimas per nuorodą ---\n";
    printShapeByRef(c);

    std::cout << "\n--- perdavimas per rodyklę ---\n";
    printShapeByPtr(&c);

    // 2 dalis — masyvas ir sizeof
    std::cout << "\n--- Shape arr[] ---\n";
    Shape arr[2];                            // ← veikia po default ctor
    arr[0] = c;
    arr[1] = Rectangle(1, 1, 4, 3, "blue");
    for (int i = 0; i < 2; i++) {
        arr[i].printInfo();
    }

    std::cout << "\n--- sizeof palyginimas ---\n";
    std::cout << "sizeof(arr[0])   : " << sizeof(arr[0])   << "\n";
    std::cout << "sizeof(Shape)    : " << sizeof(Shape)     << "\n";
    std::cout << "sizeof(Circle)   : " << sizeof(Circle)    << "\n";
    std::cout << "sizeof(Rectangle): " << sizeof(Rectangle) << "\n";

    // 3 dalis — vector palyginimas
    std::cout << "\n--- vector<Shape> (slicing!) ---\n";
    std::vector<Shape> shapesVal;
    shapesVal.push_back(c);
    shapesVal.push_back(Rectangle(1, 1, 4, 3, "blue"));
    for (const Shape& sh : shapesVal) {
        sh.printInfo();
    }

    std::cout << "\n--- vector<Shape*> (be slicing) ---\n";
    std::vector<Shape*> shapesPtr;
    shapesPtr.push_back(new Circle(0.0, 0.0, 5.0, "red"));
    shapesPtr.push_back(new Rectangle(1, 1, 4, 3, "blue"));
    for (Shape* sp : shapesPtr) {
        sp->printInfo();
    }
    for (Shape* sp : shapesPtr) {
        delete sp;
    }

    return 0;
}
```

### Laukiama išvestis

```
=== BONUS: Object Slicing ===

[Shape CTOR] center=(0, 0) color=red
[Circle CTOR] r=5

--- Shape s = c ---
Circle [color=red, center=(0, 0), r=5, area=78.5398]
Shape  [color=red, center=(0, 0), area=0]

--- perdavimas per reikšmę ---
Shape [color=red, center=(0, 0), area=0]

--- perdavimas per nuorodą ---
Shape [color=red, center=(0, 0), area=0]

--- perdavimas per rodyklę ---
Shape [color=red, center=(0, 0), area=0]

--- Shape arr[] ---
Shape [color=red,  center=(0, 0), area=0]
Shape [color=blue, center=(1, 1), area=0]

--- sizeof palyginimas ---
sizeof(arr[0])   : 48
sizeof(Shape)    : 48
sizeof(Circle)   : 56
sizeof(Rectangle): 64

--- vector<Shape> (slicing!) ---
Shape [color=red,  center=(0, 0), area=0]
Shape [color=blue, center=(1, 1), area=0]

--- vector<Shape*> (be slicing) ---
Circle    [color=red,  center=(0, 0), r=5, area=78.5398]
Rectangle [color=blue, center=(1, 1), w=4, h=3, area=12]
```

### Pedagoginės įžvalgos

- **Default konstruktorius** — studentas turi pats suprasti iš klaidos pranešimo. Tai geras savarankiškas sprendimas.
- **Trys perdavimo būdai** — tik `ByValue` sukelia slicing'ą. `ByRef` ir `ByPtr` — interfeisas ribotas, bet informacija neprarandama. Su `virtual` (U6) skirtumas bus esminis.
- **`sizeof` — fizinis įrodymas** — `arr[0]` = 48 baitų, `Circle` = 56 baitų. Studentas mato kodėl netelpa.
- **`vector<Shape>` vs `vector<Shape*>`** — puikus egzamino klausimas.
- **Logging** — `[Shape CTOR]` per `printShapeByValue` rodo kopiją. Geras vizualinis įrodymas.

---

## Dažniausios studentų klaidos

| Klaida | Žingsnis | Simptomai |
|--------|----------|-----------|
| Pamiršta `: Shape(x, y, color)` konstruktoriuje | 2, 3 | Kompiliavimo klaida: "no matching function for call to Shape::Shape()" |
| `private` vietoj `protected` | 2 | Kompiliavimo klaida Circle.cpp: `color` is private |
| Pamiršta `delete` `vector<Shape*>` | 5 | Memory leak (nematomas, bet valgrind rodo) |
| `vector<Shape>` vietoj `vector<Shape*>` | 5, 6 | Slicing — area() visada 0 |
| `->` vietoj `.` per `Shape&` nuorodą | 5, 6 | Kompiliavimo klaida |
| Pamiršta `#include <cmath>` | 2 | `M_PI` undefined |

---

## Egzamino klausimai (pasiūlymai)

1. Kas yra konstruktorių grandinė? Kodėl `Circle` konstruktorius privalo iškviesti `Shape` konstruktorių?
2. Kuo skiriasi `private`, `protected`, `public` prieigos modifikatoriai paveldėjimo kontekste?
3. Kas yra upcasting? Kodėl jis visada saugus?
4. Kas yra object slicing? Kodėl `vector<Shape>` yra problema, o `vector<Shape*>` — ne?
5. Kodėl `area()` grąžina `0.0` per `Shape*` rodyklę į `Circle` objektą?
6. Kokia `sizeof(Circle)` ir `sizeof(Shape)` skirtumas? Iš kur jis atsiranda?
7. Kokia destruktorių kvietimo tvarka naikinant `Circle` objektą?
