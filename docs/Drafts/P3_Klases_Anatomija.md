# 3️⃣ Paskaita — Klasės anatomija: paprasti objektai

!!! abstract "Šios paskaitos tikslas"
    P2 pabaigoje pamatėme **šuolį į C++**: `class` išsprendė `Stack` gyvavimo ciklo
    problemą automatiškai. Dabar — detaliai išnagrinėsime klasės sandarą
    su paprastesniais, „žemiškesniais" objektais.

    - **`struct` vs `class`** — kuo skiriasi
    - **Konstruktoriai** — inicializavimo sąrašas, keli variantai
    - **Destruktorius** — logging kaip pedagoginis įrankis
    - **`this` rodyklė** — kas tai ir kada reikia
    - **Prieigos kontrolė** — `public`/`private`, getters/setters
    - **`static` nariai** — bendri visiems objektams

    > Du objektai — `Studentas` (realus) ir `Point` (matematinis, paprastas).

---

## 1️⃣ `struct` vs `class` — vienas skirtumas

C++ `struct` ir `class` yra **beveik identiški**. Tik vienas skirtumas:

```cpp
struct Foo {
    int x;      // numatyta: public
    void f();   // numatyta: public
};

class Bar {
    int x;      // numatyta: private
    void f();   // numatyta: private
};
```

!!! tip "Konvencija"
    - `struct` — kai visi nariai `public` (duomenų konteineris, be logikos)
    - `class` — kai yra `private` nariai ir metodai (tikras OOP objektas)

---

## 2️⃣ Klasės anatomija — `Studentas`

### Minimalus variantas

```cpp linenums="1"
class Studentas {
    // private (numatyta):
    std::string vardas;
    int amzius;

public:
    // konstruktorius
    Studentas(const std::string& v, int a) : vardas(v), amzius(a) {}

    // metodai
    void spausdinti() const {
        std::cout << vardas << ", " << amzius << " m.\n";
    }
};
```

Išskaidykime kiekvieną dalį.

---

### Inicializavimo sąrašas

```cpp linenums="1" hl_lines="2 3"
Studentas(const std::string& v, int a)
    : vardas(v),    // ← inicializavimo sąrašas
      amzius(a)     //    nariai inicializuojami čia
{
    // kūnas — papildomos operacijos jei reikia
}
```

**Kodėl ne tiesiog `vardas = v` kūne?**

=== "Su inicializavimo sąrašu ✅"

    ```cpp
    Studentas(const std::string& v, int a)
        : vardas(v), amzius(a) {}
    // vardas sukonstruojamas TIK VIENĄ kartą — su v reikšme
    ```

=== "Be inicializavimo sąrašo ⚠️"

    ```cpp
    Studentas(const std::string& v, int a) {
        vardas = v;   // pirma — default konstruktorius (tuščias string)
        amzius = a;   // paskui — priskyrimas
    }                 // du žingsniai vietoj vieno — neefektyvu
    ```

!!! warning "Kai inicializavimo sąrašas **būtinas**"
    - `const` nariai — juos galima tik inicializuoti, ne priskirti
    - Nuorodos (`int&`) — privalo būti inicializuotos
    - Nariai be numatytojo konstruktoriaus

    ```cpp
    class Foo {
        const int id;    // const — tik inicializavimo sąraše!
        int& ref;        // nuoroda — tik inicializavimo sąraše!
    public:
        Foo(int i, int& r) : id(i), ref(r) {}  // būtina
    };
    ```

---

### Destruktorius su logging

Kol klasė nevaldo dinaminės atminties — destruktorius gali būti tuščias.
Bet **logging** padeda suprasti gyvavimo ciklą:

```cpp linenums="1"
class Studentas {
    std::string vardas;
    int amzius;
public:
    Studentas(const std::string& v, int a) : vardas(v), amzius(a) {
        std::cout << "[CTOR] Studentas('" << vardas << "')\n";
    }
    ~Studentas() {
        std::cout << "[DTOR] Studentas('" << vardas << "')\n";
    }
    void spausdinti() const {
        std::cout << vardas << ", " << amzius << " m.\n";
    }
};

int main() {
    std::cout << "--- pradžia ---\n";
    {
        Studentas s1("Jonas", 20);
        Studentas s2("Marta", 19);
        s1.spausdinti();
        s2.spausdinti();
    }   // ← destruktoriai čia
    std::cout << "--- pabaiga ---\n";
}
```

=== "⌨️➡️🖥️"

    ```
    --- pradžia ---
    [CTOR] Studentas('Jonas')
    [CTOR] Studentas('Marta')
    Jonas, 20 m.
    Marta, 19 m.
    [DTOR] Studentas('Marta')
    [DTOR] Studentas('Jonas')
    --- pabaiga ---
    ```

??? note "Kodėl atvirkštine tvarka?"
    Objektai sunaikinami **atvirkštine** kūrimo tvarka — kaip kortų kaladė.
    `s2` sukurtas po `s1`, todėl naikinamas pirmiau (_LIFO — Last In, First Out_).
    Stack'o principas — kurį jau žinome iš P1–P2!

---

## 3️⃣ `this` rodyklė

`this` — rodyklė į **patį objektą**, kuriam metodas buvo iškviestas.

### Kada reikia `this`?

```cpp linenums="1"
class Studentas {
    std::string vardas;
    int amzius;
public:
    // 1. Kai parametras vadinasi taip pat kaip narys:
    void setAmzius(int amzius) {
        this->amzius = amzius;  // this->amzius = narys
        //   amzius   = parametras
    }

    // 2. Kai grąžiname patį objektą (method chaining):
    Studentas& setVardas(const std::string& v) {
        vardas = v;
        return *this;   // ← grąžiname objektą
    }
};

int main() {
    Studentas s("", 0);
    s.setVardas("Jonas").setAmzius(20);  // method chaining
}
```

!!! tip "Praktikoje"
    Dažniausiai `this` nereikia — kompiliatorius pats randa narius.
    Naudojamas tik kai vardo konfliktas arba grąžiname `*this`.

---

## 4️⃣ Prieigos kontrolė ir inkapsuliacija

### `public` / `private`

```cpp linenums="1"
class Studentas {
    // ── private (numatyta) ──────────────────────────
    std::string vardas;
    int amzius;
    double pazymys;

public:
    // ── public ──────────────────────────────────────
    Studentas(const std::string& v, int a, double p)
        : vardas(v), amzius(a), pazymys(p) {}

    // Getters — skaityti (const!)
    std::string getVardas()  const { return vardas; }
    int         getAmzius()  const { return amzius; }
    double      getPazymys() const { return pazymys; }

    // Setters — rašyti (su validacija!)
    void setAmzius(int a) {
        if (a > 0 && a < 150)
            amzius = a;
        // jei neteisinga — tyliai ignoruojame (arba throw P6+)
    }

    void setPazymys(double p) {
        if (p >= 0.0 && p <= 10.0)
            pazymys = p;
    }

    // Veiklos metodai
    bool arPilnametis() const { return amzius >= 18; }

    void spausdinti() const {
        std::cout << vardas << ", " << amzius << " m."
                  << ", pažymys: " << pazymys;
        if (arPilnametis()) std::cout << " (pilnametis)";
        std::cout << "\n";
    }
};
```

### Kodėl `private`?

```cpp linenums="1"
// Be inkapsuliavimo — bet kas gali sulaužyti:
struct Studentas { int amzius; };
Studentas s;
s.amzius = -5;   // ← absurdas, bet kompiliuojasi

// Su inkapsuliavimas — kompiliatorius saugo:
class Studentas { int amzius; public: void setAmzius(int a) { if(a>0) amzius=a; } };
Studentas s;
s.amzius = -5;   // ❌ NC — private!
s.setAmzius(-5); // ✅ kompiliuojasi, bet setter ignoruoja
```

---

## 5️⃣ Keli konstruktoriai

```cpp linenums="1"
class Studentas {
    std::string vardas;
    int amzius;
    double pazymys;
public:
    // 1. Numatytasis (default) — be parametrų
    Studentas()
        : vardas("Nežinomas"), amzius(0), pazymys(0.0) {
        std::cout << "[CTOR] default\n";
    }

    // 2. Pilnas — visi parametrai
    Studentas(const std::string& v, int a, double p)
        : vardas(v), amzius(a), pazymys(p) {
        std::cout << "[CTOR] '" << vardas << "'\n";
    }

    // 3. Dalinis — vardas ir amžius (pažymys = 0)
    Studentas(const std::string& v, int a)
        : vardas(v), amzius(a), pazymys(0.0) {
        std::cout << "[CTOR] '" << vardas << "' (be pažymio)\n";
    }
};

int main() {
    Studentas s1;                       // default
    Studentas s2("Jonas", 20, 8.5);     // pilnas
    Studentas s3("Marta", 19);          // dalinis
}
```

=== "⌨️➡️🖥️"

    ```
    [CTOR] default
    [CTOR] 'Jonas'
    [CTOR] 'Marta' (be pažymio)
    ```

!!! warning "Dėmesio: kai NRA numatytojo konstruktoriaus"
    ```cpp
    class Foo {
        int x;
    public:
        Foo(int v) : x(v) {}   // tik su parametru
    };

    Foo a;      // ❌ NC — nėra default konstruktoriaus!
    Foo b(5);   // ✅
    Foo arr[3]; // ❌ NC — masyvui reikia default konstruktoriaus
    ```

---

## 6️⃣ `static` nariai

`static` nariai priklauso **klasei**, ne konkrečiam objektui.
Bendri **visiems** egzemplioriams.

### `liveCount` — gyvų objektų skaitliukas

```cpp linenums="1" hl_lines="3 4 11 12 16 17"
class Studentas {
    std::string vardas;
    static int liveCount;   // bendras visiems
    static int nextId;      // id generatorius
    int id;

public:
    Studentas(const std::string& v) : vardas(v), id(nextId++) {
        ++liveCount;
        std::cout << "[CTOR] '" << vardas
                  << "' id=" << id
                  << " (gyvi: " << liveCount << ")\n";
    }
    ~Studentas() {
        --liveCount;
        std::cout << "[DTOR] '" << vardas
                  << "' (gyvi: " << liveCount << ")\n";
    }

    static int getLiveCount() { return liveCount; }
    int getId() const { return id; }
};

// Inicializacija — už klasės ribų, .cpp faile:
int Studentas::liveCount = 0;
int Studentas::nextId    = 1;

int main() {
    std::cout << "Gyvi: " << Studentas::getLiveCount() << "\n"; // 0
    {
        Studentas s1("Jonas");
        Studentas s2("Marta");
        std::cout << "Gyvi: " << Studentas::getLiveCount() << "\n"; // 2
    }
    std::cout << "Gyvi: " << Studentas::getLiveCount() << "\n"; // 0
}
```

=== "⌨️➡️🖥️"

    ```
    Gyvi: 0
    [CTOR] 'Jonas' id=1 (gyvi: 1)
    [CTOR] 'Marta' id=2 (gyvi: 2)
    Gyvi: 2
    [DTOR] 'Marta' (gyvi: 1)
    [DTOR] 'Jonas' (gyvi: 0)
    Gyvi: 0
    ```

!!! note "`static` nario inicializacija"
    `static` narys **neįeina** į objekto atmintį — jis egzistuoja atskirai.
    Todėl inicializuojamas **už klasės ribų** (`.cpp` faile):
    ```cpp
    int Studentas::liveCount = 0;  // būtina!
    ```
    Antraštės faile — tik deklaracija.

---

## 7️⃣ Antras objektas — `Point`

`Point` — matematinis taškas plokštumoje. Paprastesnis nei `Studentas`,
bet puikiai demonstruoja operatorius ir `const` metodus:

```cpp linenums="1"
#include <cmath>

class Point {
    double x, y;
public:
    // Konstruktoriai
    Point() : x(0.0), y(0.0) {}
    Point(double x, double y) : x(x), y(y) {}

    // Getters
    double getX() const { return x; }
    double getY() const { return y; }

    // Veiklos metodai
    double atstumas(const Point& kitas) const {
        double dx = x - kitas.x;
        double dy = y - kitas.y;
        return std::sqrt(dx*dx + dy*dy);
    }

    double atstumasNuoOrigines() const {
        return std::sqrt(x*x + y*y);
    }

    void spausdinti() const {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

int main() {
    Point p1(0.0, 0.0);
    Point p2(3.0, 4.0);

    p1.spausdinti();                               // (0, 0)
    p2.spausdinti();                               // (3, 4)
    std::cout << p1.atstumas(p2) << "\n";          // 5
    std::cout << p2.atstumasNuoOrigines() << "\n"; // 5
}
```

=== "⌨️➡️🖥️"

    ```
    (0, 0)
    (3, 4)
    5
    5
    ```

!!! tip "Kodėl `Point` naudingas?"
    - Visi metodai `const` — geras `const correctness` pavyzdys
    - `atstumas(const Point&)` — `const T&` parametras praktikoje (P4 ryšys)
    - Vėliau taps `Rectangle` sudėtine dalimi (P9 kompozicija)

---

## 8️⃣ Viskas kartu — pilnas `Studentas`

=== "Studentas.h"

    ```cpp linenums="1"
    #pragma once
    #include <string>
    #include <iostream>

    class Studentas {
        std::string vardas;
        int         amzius;
        double      pazymys;
        int         id;

        static int liveCount;
        static int nextId;

    public:
        Studentas(const std::string& v, int a, double p);
        Studentas(const std::string& v, int a);
        Studentas();
        ~Studentas();

        std::string getVardas()  const;
        int         getAmzius()  const;
        double      getPazymys() const;
        int         getId()      const;

        void setPazymys(double p);
        void setAmzius(int a);

        bool arPilnametis() const;
        void spausdinti()   const;

        static int getLiveCount();
    };
    ```

=== "Studentas.cpp"

    ```cpp linenums="1"
    #include "Studentas.h"

    int Studentas::liveCount = 0;
    int Studentas::nextId    = 1;

    Studentas::Studentas(const std::string& v, int a, double p)
        : vardas(v), amzius(a), pazymys(p), id(nextId++) {
        ++liveCount;
        std::cout << "[CTOR] '" << vardas << "' id=" << id
                  << " (gyvi: " << liveCount << ")\n";
    }

    Studentas::Studentas(const std::string& v, int a)
        : vardas(v), amzius(a), pazymys(0.0), id(nextId++) {
        ++liveCount;
    }

    Studentas::Studentas()
        : vardas("Nežinomas"), amzius(0), pazymys(0.0), id(nextId++) {
        ++liveCount;
    }

    Studentas::~Studentas() {
        --liveCount;
        std::cout << "[DTOR] '" << vardas
                  << "' (gyvi: " << liveCount << ")\n";
    }

    std::string Studentas::getVardas()  const { return vardas; }
    int         Studentas::getAmzius()  const { return amzius; }
    double      Studentas::getPazymys() const { return pazymys; }
    int         Studentas::getId()      const { return id; }

    void Studentas::setPazymys(double p) {
        if (p >= 0.0 && p <= 10.0) pazymys = p;
    }

    void Studentas::setAmzius(int a) {
        if (a > 0 && a < 150) amzius = a;
    }

    bool Studentas::arPilnametis() const { return amzius >= 18; }

    void Studentas::spausdinti() const {
        std::cout << "[" << id << "] " << vardas
                  << ", " << amzius << " m."
                  << ", pažymys: " << pazymys;
        if (arPilnametis()) std::cout << " (pilnametis)";
        std::cout << "\n";
    }

    int Studentas::getLiveCount() { return liveCount; }
    ```

=== "main.cpp"

    ```cpp linenums="1"
    #include "Studentas.h"
    #include <iostream>

    int main() {
        std::cout << "Gyvi: " << Studentas::getLiveCount() << "\n";

        Studentas s1("Jonas", 20, 8.5);
        Studentas s2("Marta", 17, 9.0);

        s1.spausdinti();
        s2.spausdinti();

        s2.setAmzius(18);
        std::cout << "Marta pilnametė? "
                  << (s2.arPilnametis() ? "taip" : "ne") << "\n";

        std::cout << "Gyvi: " << Studentas::getLiveCount() << "\n";
        return 0;
    }
    ```

=== "⌨️➡️🖥️"

    ```
    Gyvi: 0
    [CTOR] 'Jonas' id=1 (gyvi: 1)
    [CTOR] 'Marta' id=2 (gyvi: 2)
    [1] Jonas, 20 m., pažymys: 8.5 (pilnametis)
    [2] Marta, 17 m., pažymys: 9
    Marta pilnametė? ne
    Marta pilnametė? taip
    Gyvi: 2
    [DTOR] 'Marta' (gyvi: 1)
    [DTOR] 'Jonas' (gyvi: 0)
    ```

---

## 🔗 Ryšys su tolesnėmis paskaitomis

```
P3 (ši paskaita):          Kur naudojama toliau:

Studentas klasė ────────►  P4: const T& parametrai
                            P5: std::vector<Studentas>
                            P9: Kursas turi Studentų

const metodai ──────────►  P4: const correctness tema

inicializavimo sąrašas ──►  P6: RAII konstruktoriuje
                            P8: Rule of Three

liveCount / id ─────────►  U1 užduotis
                            Kompozicija_pimpl: kur dėti su pimpl?

Point ──────────────────►  P9: Rectangle has-a Point (kompozicija)
```

---

*[NC]: Not Compiling — Nesikompiliuoja
*[LIFO]: Last In, First Out
