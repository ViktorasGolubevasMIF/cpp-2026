# U7: Išimčių apdorojimas — Sprendimas

!!! warning "Dėstytojo medžiaga"
    Šis failas — pilnas U7 sprendimas. Neskelbti studentams.

---

## 1 žingsnis — `BankAccount`

> Visas kodas pateiktas užduotyje kaip PAVYZDYS. Žemiau — tik Makefile.

**Makefile:**
```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = main
SRCS = main.cpp BankAccount.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $@  $<

clean:
	rm -f $(OBJS) $(TARGET)
```

---

## 2 žingsnis — validacija konstruktoriuose

**Circle.h** *(nepakeistas nuo U6/06)*:
```cpp
#pragma once
#include "Shape.h"

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double x, double y, double r, const std::string& color);
    ~Circle();

    double area() const override;
    void printInfo() const override;

    double getRadius() const;
};
```

**Circle.cpp:**
```cpp
#include "Circle.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

Circle::Circle(double x, double y, double r, const std::string& color)
    : Shape(x, y, color), radius(r)
{
    if (r <= 0) {
        throw std::invalid_argument(
            "Circle: radius turi būti teigiamas, gauta: "
            + std::to_string(r)
        );
    }
    std::cout << "[Circle CTOR] r=" << radius << "\n";
}

Circle::~Circle() {
    std::cout << "[Circle DTOR] r=" << radius << "\n";
}

double Circle::area() const {
    return M_PI * radius * radius;
}

void Circle::printInfo() const {
    std::cout << "Circle    [color=" << getColor()
              << ", center=(" << getX() << "," << getY() << ")"
              << ", r=" << radius
              << ", area=" << area() << "]\n";
}

double Circle::getRadius() const { return radius; }
```

**Rectangle.h** *(nepakeistas nuo U6/06)*:
```cpp
#pragma once
#include "Shape.h"

class Rectangle : public Shape {
private:
    double width, height;
public:
    Rectangle(double x, double y, double w, double h, const std::string& color);
    ~Rectangle();

    double area() const override;
    void printInfo() const override;

    double getWidth() const;
    double getHeight() const;
};
```

**Rectangle.cpp:**
```cpp
#include "Rectangle.h"
#include <iostream>
#include <stdexcept>

Rectangle::Rectangle(double x, double y, double w, double h,
                     const std::string& color)
    : Shape(x, y, color), width(w), height(h)
{
    if (w <= 0 || h <= 0) {
        throw std::invalid_argument(
            "Rectangle: matmenys turi būti teigiami"
        );
    }
    std::cout << "[Rectangle CTOR] w=" << width << " h=" << height << "\n";
}

Rectangle::~Rectangle() {
    std::cout << "[Rectangle DTOR] w=" << width << " h=" << height << "\n";
}

double Rectangle::area() const { return width * height; }

void Rectangle::printInfo() const {
    std::cout << "Rectangle [color=" << getColor()
              << ", center=(" << getX() << "," << getY() << ")"
              << ", w=" << width << ", h=" << height
              << ", area=" << area() << "]\n";
}

double Rectangle::getWidth() const  { return width; }
double Rectangle::getHeight() const { return height; }
```

**main.cpp:**
```cpp
#include <iostream>
#include <stdexcept>
#include "Circle.h"
#include "Rectangle.h"

int main() {
    std::cout << "=== 2 žingsnis: Validacija konstruktoriuose ===\n\n";

    // --- Teisingi objektai ---
    std::cout << "--- Teisingi objektai ---\n";
    try {
        Circle c(0, 0, 5.0, "red");
        Rectangle r(1, 1, 4.0, 3.0, "blue");
        c.printInfo();
        r.printInfo();
    }
    catch (const std::exception& e) {
        std::cout << "Klaida: " << e.what() << "\n";
    }

    // --- Blogas Circle ---
    std::cout << "\n--- Blogas Circle (r=-3) ---\n";
    try {
        Circle bad(0, 0, -3.0, "green");
        std::cout << "Ši eilutė NEPASIEKIAMA\n";
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Pagauta: " << e.what() << "\n";
    }

    // --- Blogas Rectangle ---
    std::cout << "\n--- Blogas Rectangle (w=-4) ---\n";
    try {
        Rectangle bad(0, 0, -4.0, 3.0, "black");
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Pagauta: " << e.what() << "\n";
    }

    std::cout << "\nPrograma baigta teisingai.\n";
    return 0;
}
```

**Makefile:**
```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = main
SRCS = main.cpp Shape.cpp Circle.cpp Rectangle.cpp Triangle.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
```

---

## 3 žingsnis — `try`/`catch`, programa tęsiasi

> Keičiamas tik `main.cpp`. Visi kiti failai — iš 2 žingsnio.

**main.cpp:**
```cpp
#include <iostream>
#include <vector>
#include <stdexcept>
#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"

Shape* createCircle(double x, double y, double r, const std::string& color) {
    return new Circle(x, y, r, color);
    // Jei r <= 0 — Circle konstruktorius meta išimtį automatiškai
}

int main() {
    std::cout << "=== 3 žingsnis: Programa tęsiasi po klaidų ===\n\n";

    // --- A dalis: keli objektai, vienas blogas ---
    std::cout << "--- A dalis: keli objektai ---\n";

    std::vector<Shape*> shapes;

    // Bandome sukurti 5 objektus — 2 iš jų blogi
    auto tryAdd = [&](auto createFn) {
        try {
            shapes.push_back(createFn());
        }
        catch (const std::exception& e) {
            std::cout << "Klaida kuriant objektą: " << e.what() << "\n";
        }
    };

    tryAdd([] { return new Circle(0, 0, 3.0, "red"); });
    tryAdd([] { return new Circle(1, 1, -2.0, "blue"); });
    tryAdd([] { return new Rectangle(2, 2, 4.0, 3.0, "green"); });
    tryAdd([] { return new Rectangle(3, 3, -1.0, 2.0, "black"); });
    tryAdd([] { return new Circle(4, 4, 5.0, "yellow"); });

    std::cout << "\nSėkmingai pridėta: " << shapes.size() << " objektai\n\n";

    for (Shape* s : shapes) s->printInfo();

    std::cout << "\n";
    for (Shape* s : shapes) delete s;
    shapes.clear();

    // --- B dalis: funkcija su throw ---
    std::cout << "\n--- B dalis: funkcija su throw ---\n";

    struct TestCase { double x, y, r; std::string color; };
    std::vector<TestCase> tests = {
        {0, 0, 3.0, "red"},
        {1, 1, -1.0, "blue"},
        {2, 2, 7.0, "green"}
    };

    for (const auto& t : tests) {
        try {
            Shape* s = createCircle(t.x, t.y, t.r, t.color);
            std::cout << "Sukurta: Circle r=" << t.r << "\n";
            delete s;
        }
        catch (const std::exception& e) {
            std::cout << "Klaida: " << e.what() << "\n";
        }
    }

    return 0;
}
```

---

## 4 žingsnis — `throw` metoduose

> Keičiami `Circle.h`, `Circle.cpp`, `Rectangle.h`, `Rectangle.cpp`, `main.cpp`.

**Circle.h** *(pridedamas `setRadius`)*:
```cpp
#pragma once
#include "Shape.h"

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double x, double y, double r, const std::string& color);
    ~Circle();

    double area() const override;
    void printInfo() const override;

    double getRadius() const;
    void setRadius(double r);      // ← naujas
};
```

**Circle.cpp** *(pridedamas `setRadius`)*:
```cpp
// ... (konstruktorius, destruktorius, area, printInfo, getRadius — kaip 2 žingsnyje)

void Circle::setRadius(double r) {
    if (r <= 0) {
        throw std::invalid_argument(
            "Circle: radius turi būti teigiamas, gauta: "
            + std::to_string(r)
        );
    }
    radius = r;   // ← priskiriame TIK jei validacija praėjo
}
```

**Rectangle.h** *(pridedamas `setWidth`)*:
```cpp
#pragma once
#include "Shape.h"

class Rectangle : public Shape {
private:
    double width, height;
public:
    Rectangle(double x, double y, double w, double h, const std::string& color);
    ~Rectangle();

    double area() const override;
    void printInfo() const override;

    double getWidth() const;
    double getHeight() const;
    void setWidth(double w);       // ← naujas
};
```

**Rectangle.cpp** *(pridedamas `setWidth`)*:
```cpp
// ... (konstruktorius, destruktorius, area, printInfo, getters — kaip 2 žingsnyje)

void Rectangle::setWidth(double w) {
    if (w <= 0) {
        throw std::invalid_argument(
            "Rectangle: matmenys turi būti teigiami"
        );
    }
    width = w;
}
```

**main.cpp:**
```cpp
#include <iostream>
#include <stdexcept>
#include "Circle.h"
#include "Rectangle.h"

int main() {
    std::cout << "=== 4 žingsnis: throw metoduose ===\n\n";

    Circle c(0, 0, 5.0, "red");
    c.printInfo();

    // --- Teisingas setRadius ---
    std::cout << "\n--- setRadius(8.0) ---\n";
    try {
        c.setRadius(8.0);
        c.printInfo();
    }
    catch (const std::exception& e) {
        std::cout << "Klaida: " << e.what() << "\n";
    }

    // --- Blogas setRadius ---
    std::cout << "\n--- setRadius(-3.0) ---\n";
    try {
        c.setRadius(-3.0);
        std::cout << "Ši eilutė NEPASIEKIAMA\n";
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Pagauta: " << e.what() << "\n";
    }

    std::cout << "\nPo klaidos: ";
    c.printInfo();   // radius = 8.0 — nepakito

    // --- Rectangle setWidth ---
    std::cout << "\n--- Rectangle setWidth(-2.0) ---\n";
    Rectangle r(0, 0, 4.0, 3.0, "blue");
    try {
        r.setWidth(-2.0);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Pagauta: " << e.what() << "\n";
    }

    return 0;
}
```

---

## 5 žingsnis — `ShapeException`

> Pridedami `ShapeException.h`/`.cpp`. Keičiami `Circle.cpp`, `Rectangle.cpp`, `main.cpp`.

**ShapeException.h:**
```cpp
#pragma once
#include <stdexcept>
#include <string>

class ShapeException : public std::invalid_argument {
public:
    explicit ShapeException(const std::string& msg)
        : std::invalid_argument("Shape klaida: " + msg) {}
};
```

**ShapeException.cpp:**
```cpp
#include "ShapeException.h"
// Konstruktorius inline ShapeException.h — .cpp tuščias
// (arba galima perkelti implementaciją čia)
```

**Circle.cpp** *(throw pakeistas į ShapeException)*:
```cpp
#include "Circle.h"
#include "ShapeException.h"
#include <iostream>
#include <cmath>

Circle::Circle(double x, double y, double r, const std::string& color)
    : Shape(x, y, color), radius(r)
{
    if (r <= 0) {
        throw ShapeException(
            "Circle: radius turi būti teigiamas, gauta: "
            + std::to_string(r)
        );
    }
    std::cout << "[Circle CTOR] r=" << radius << "\n";
}

// destruktorius, area, printInfo, getRadius — kaip 4 žingsnyje

void Circle::setRadius(double r) {
    if (r <= 0) {
        throw ShapeException(
            "Circle: radius turi būti teigiamas, gauta: "
            + std::to_string(r)
        );
    }
    radius = r;
}
```

**Rectangle.cpp** *(throw pakeistas į ShapeException)*:
```cpp
#include "Rectangle.h"
#include "ShapeException.h"
#include <iostream>

Rectangle::Rectangle(double x, double y, double w, double h,
                     const std::string& color)
    : Shape(x, y, color), width(w), height(h)
{
    if (w <= 0 || h <= 0) {
        throw ShapeException("Rectangle: matmenys turi būti teigiami");
    }
    std::cout << "[Rectangle CTOR] w=" << width << " h=" << height << "\n";
}

// destruktorius, area, printInfo, getters — kaip 4 žingsnyje

void Rectangle::setWidth(double w) {
    if (w <= 0) {
        throw ShapeException("Rectangle: matmenys turi būti teigiami");
    }
    width = w;
}
```

**main.cpp:**
```cpp
#include <iostream>
#include <stdexcept>
#include "Circle.h"
#include "Rectangle.h"
#include "ShapeException.h"

int main() {
    std::cout << "=== 5 žingsnis: ShapeException ===\n\n";

    // --- Pagauti pagal tipą ---
    std::cout << "--- Pagauti pagal tipą ---\n";
    try {
        Circle bad(0, 0, -3.0, "red");
    }
    catch (const ShapeException& e) {
        std::cout << "ShapeException: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "Kita klaida: " << e.what() << "\n";
    }

    // --- ShapeException yra std::exception ---
    std::cout << "\n--- ShapeException pagaunama ir per std::exception ---\n";
    try {
        Rectangle bad(0, 0, -4.0, 3.0, "blue");
    }
    catch (const std::exception& e) {
        std::cout << "std::exception pagavo: " << e.what() << "\n";
    }

    // --- Atskyrimas: Shape vs kita klaida ---
    std::cout << "\n--- Atskyrimas: Shape vs kita klaida ---\n";
    try {
        Circle bad(0, 0, -3.0, "red");        // ← ShapeException
    }
    catch (const ShapeException& e) {
        std::cout << "Shape validacijos klaida: " << e.what() << "\n";
    }
    catch (const std::runtime_error& e) {
        std::cout << "Kitos rūšies klaida: " << e.what() << "\n";
    }

    // --- Demonstruojame kad runtime_error TAIP PAT pagaunamas ---
    std::cout << "\n--- runtime_error ---\n";
    try {
        throw std::runtime_error("kažkas kita");
    }
    catch (const ShapeException& e) {
        std::cout << "Shape klaida: " << e.what() << "\n";
    }
    catch (const std::runtime_error& e) {
        std::cout << "Kitos rūšies klaida: " << e.what() << "\n";
    }

    return 0;
}
```

!!! note "ShapeException.cpp pastaba"
    Jei konstruktorius `inline` `.h` faile — `.cpp` gali būti tuščias arba visai neįtrauktas. Bet Makefile jį vis tiek galima palikti SRCS sąraše — kompiliatorius tiesiog sukurs tuščią `.o`.

---

## 6 žingsnis — `unique_ptr`

> A ir B dalys — PAVYZDYS užduotyje. C dalis — JŪSŲ KODAS. Žemiau pilnas `main.cpp`.

**main.cpp:**
```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>
#include "Circle.h"
#include "Rectangle.h"
#include "ShapeException.h"

int main() {
    std::cout << "=== 6 žingsnis: unique_ptr ===\n\n";

    // --- A dalis: unique_ptr vs new/delete ---
    std::cout << "--- A dalis: unique_ptr pagrindai ---\n";

    {
        Shape* raw = new Circle(0, 0, 3.0, "red");
        raw->printInfo();
        delete raw;
    }

    std::cout << "\n";

    {
        auto smart = std::make_unique<Circle>(0, 0, 3.0, "blue");
        smart->printInfo();
        // delete nereikia
    }

    std::cout << "\n--- A dalis: baigta ---\n\n";

    // --- B dalis: unique_ptr + throw ---
    std::cout << "--- B dalis: unique_ptr + throw ---\n";

    std::cout << "\nSu new/delete (atminties nutekėjimas):\n";
    try {
        Shape* raw = new Circle(0, 0, 5.0, "red");
        throw std::runtime_error("klaida po new");
        delete raw;   // NEPASIEKIAMA
    }
    catch (const std::exception& e) {
        std::cout << "Pagauta: " << e.what() << "\n";
        std::cout << "[Circle DTOR] NIEKADA neiškviečiamas!\n";
    }

    std::cout << "\nSu unique_ptr (saugu):\n";
    try {
        auto smart = std::make_unique<Circle>(0, 0, 5.0, "blue");
        throw std::runtime_error("klaida po make_unique");
    }
    catch (const std::exception& e) {
        std::cout << "Pagauta: " << e.what() << "\n";
    }

    // --- C dalis: vector<unique_ptr<Shape>> ---
    std::cout << "\n--- C dalis: vector<unique_ptr<Shape>> ---\n";

    std::vector<std::unique_ptr<Shape>> shapes;

    auto tryAdd = [&](std::unique_ptr<Shape> ptr) {
        shapes.push_back(std::move(ptr));
    };

    struct Item { double x, y, r; std::string color; };
    std::vector<Item> items = {
        {0, 0, 3.0, "red"},
        {1, 1, -2.0, "blue"},     // blogas
        {2, 2, 4.0, "green"},     // Rectangle — žr. žemiau
        {3, 3, 5.0, "black"}
    };

    try { tryAdd(std::make_unique<Circle>(0, 0, 3.0, "red")); }
    catch (const std::exception& e) {
        std::cout << "Klaida: " << e.what() << "\n";
    }

    try { tryAdd(std::make_unique<Circle>(1, 1, -2.0, "blue")); }
    catch (const std::exception& e) {
        std::cout << "Klaida: " << e.what() << "\n";
    }

    try { tryAdd(std::make_unique<Rectangle>(2, 2, 4.0, 3.0, "green")); }
    catch (const std::exception& e) {
        std::cout << "Klaida: " << e.what() << "\n";
    }

    try { tryAdd(std::make_unique<Circle>(3, 3, 5.0, "black")); }
    catch (const std::exception& e) {
        std::cout << "Klaida: " << e.what() << "\n";
    }

    std::cout << "\n";
    for (const auto& s : shapes) s->printInfo();

    // delete nereikia — vector destruktorius viską išvalo

    return 0;
}
```

---

## BONUS — `vector<unique_ptr<Shape>>` su polimorfizmu

> Visi failai iš 6 žingsnio. Keičiamas tik `main.cpp`.

**main.cpp:**
```cpp
#include <iostream>
#include <memory>
#include <vector>
#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "ShapeException.h"

int main() {
    std::cout << "=== BONUS: unique_ptr + polimorfizmas ===\n\n";

    std::vector<std::unique_ptr<Shape>> shapes;

    auto tryAdd = [&](std::unique_ptr<Shape> ptr) {
        shapes.push_back(std::move(ptr));
    };

    try { tryAdd(std::make_unique<Circle>(0, 0, 3.0, "red")); }
    catch (const std::exception& e) { std::cout << "Klaida: " << e.what() << "\n"; }

    try { tryAdd(std::make_unique<Rectangle>(1, 1, 4.0, 3.0, "blue")); }
    catch (const std::exception& e) { std::cout << "Klaida: " << e.what() << "\n"; }

    try { tryAdd(std::make_unique<Triangle>(2, 2, 3.0, 4.0, 5.0, "green")); }
    catch (const std::exception& e) { std::cout << "Klaida: " << e.what() << "\n"; }

    try { tryAdd(std::make_unique<Circle>(3, 3, -1.0, "black")); }  // blogas
    catch (const std::exception& e) { std::cout << "Klaida: " << e.what() << "\n"; }

    try { tryAdd(std::make_unique<Circle>(4, 4, 5.0, "yellow")); }
    catch (const std::exception& e) { std::cout << "Klaida: " << e.what() << "\n"; }

    std::cout << "\n";
    for (const auto& s : shapes) {
        s->printInfo();   // ← virtual — kiekvienas savo versija ✅
    }

    double total = 0.0;
    for (const auto& s : shapes) total += s->area();

    std::cout << "\n--- Bendra suma ---\n";
    std::cout << "Bendras plotas: " << total << "\n";

    // Visa tai — be vieno delete!
    return 0;
}
```

---

## Pastabos dėstytojui

**Shape.h/Shape.cpp** nesikeičia nuo U6/06 — tik `Circle`, `Rectangle` konstruktoriai gauna validaciją.

**Triangle** validacijos nėra (Herono formulė su `sqrt` — U6 patarimas 4 jau užsiminė). Galima pridėti jei norisi, bet užduotyje nereikalaujama.

**3 žingsnio lambda** — jei studentai dar nematė lambdų, galima paprasčiau:
```cpp
// Vietoj lambda — paprastas try/catch cikle:
std::vector<std::pair<double,double>> params = {{3.0,0},{-2.0,0},{4.0,0}};
for (auto [r, x] : params) {
    try {
        shapes.push_back(new Circle(x, x, r, "red"));
    } catch (const std::exception& e) {
        std::cout << "Klaida: " << e.what() << "\n";
    }
}
```

**`ShapeException.cpp`** — konstruktorius `inline` `.h` faile, todėl `.cpp` tuščias. Abu variantai teisingi.

**`unique_ptr` į `vector`** — dvi sintaksės, abi veikia:
```cpp
// 1. Tiesiogiai (rekomenduojama):
shapes.push_back(std::make_unique<Circle>(0, 0, 3.0, "red"));

// 2. Per move (kai unique_ptr jau turi vardą):
auto s = std::make_unique<Circle>(0, 0, 3.0, "red");
shapes.push_back(std::move(s));
// Po std::move — s yra "tuščias" (nullptr)
```
