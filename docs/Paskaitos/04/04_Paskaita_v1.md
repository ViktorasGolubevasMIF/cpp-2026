# Paskaita 4: Apžvalga - Klasės, Objektai, Kompozicija

**Tikslas:** Susisteminti U1-U3 ir Paskaitų 1-3 medžiagą prieš pereinant prie Rule of Three.

**Trukmė:** ~90 min

**Struktūra:**

- **Dalis 1:** Klasė kaip UDT (User Defined Type)
- **Dalis 2:** Objekto gyvavimo ciklas
- **Dalis 3:** Kompozicija (has-a ryšys)

---

## Skaidrė 1: Ko išmokome per 3 modulius? 🎯

=== "Modulis 1: Klasė kaip UDT"

    **Tema:** Klasės "anatomija", enkapsuliacija
    
    **Išmokome:**
    
    - Klasės struktūra (`private`/`public`)
    - Static nariai (klasės kintamieji)
    - Const static konstantos
    - Fizinis moduliavimas (.h + .cpp)
    
    **Nuorodos:**
    
    - U2 Žingsnis 1
    - Paskaita Stack 01-09

=== "Modulis 2: Objektų gyvavimas"

    **Tema:** Konstruktoriai, destruktoriai, objekto ciklas
    
    **Išmokome:**
    
    - Parametriniai konstruktoriai
    - Default konstruktorius (kada būtinas?)
    - Destruktoriai (kada reikia?)
    - `this` rodyklė
    - `const` metodai ir parametrai
    
    **Nuorodos:**
    
    - U2 Žingsniai 2-5
    - Paskaita Stack 11-16

=== "Modulis 3: Kompozicija"

    **Tema:** Has-a ryšys, sudėtingi objektai
    
    **Išmokome:**
    
    - Value semantics (`vector<Type>`)
    - Pointer semantics (`Type*`)
    - Kompozicija vs agregacija
    - Globalios funkcijos vs metodai
    
    **Nuorodos:**
    
    - U3 Visi žingsniai

---

<div style="page-break-after: always;"></div>

---

## 📚 1 DALIS : Klasė kaip UDT

---

## Skaidrė 2: Klasės "anatomija" 🏗️

**Klasė** — tai **naujas tipas** (User Defined Type), kurį mes patys apibrėžiame.

```cpp
class Student {
private:                    // ← DUOMENYS (slėpti)
    string name;
    int age;
    static int studentCount; // ← KLASĖS kintamasis
    
public:                     // ← INTERFEISAS (matomas)
    Student(string n, int a); // ← Konstruktorius
    ~Student();               // ← Destruktorius
    
    string getName() const;   // ← Getter
    void setAge(int a);       // ← Setter
    
    static int getStudentCount(); // ← Klasės metodas
};
```

??? tip "Terminologija"

    - **Duomenys-nariai** (data members / member variables) — `name`, `age`
    - **Funkcijos-nariai** (member functions / methods) — `getName()`, `setAge()`
    - **Static nariai** — priklauso **klasei**, ne objektui

**Ref:** U2 Žingsnis 1

---

## Skaidrė 3: Enkapsuliacija vs Informacijos slėpimas 🔒

**DU skirtingi dalykai!**

=== "Enkapsuliacija (Encapsulation)"

    **Apibrėžimas:** Duomenų ir metodų **sujungimas** į vieną vienetą (klasę)
    
    ```cpp
    class BankAccount {
        // Duomenys IR metodai kartu!
        double balance;
        void deposit(double amount) { balance += amount; }
    };
    ```
    
    ✅ **Privalumas:** Logiškai susijęs kodas vienoje vietoje

=== "Informacijos slėpimas (Information Hiding)"

    **Apibrėžimas:** `private`/`public` mechanizmas — **paslepia** implementaciją
    
    ```cpp
    class BankAccount {
    private:           // ← SLĖPIMAS!
        double balance;
        
    public:            // ← INTERFEISAS
        void deposit(double amount);
        double getBalance() const;
    };
    ```
    
    ✅ **Privalumas:** Apsaugo duomenis, leidžia validaciją

??? example "U3 pavyzdys: Private access klaida"

    ```cpp
    Window w(1.5, 1.0);
    cout << w.height;  // ❌ KLAIDA: 'height' is private!
    
    // Privaloma naudoti interfeisą:
    cout << w.getHeight();  // ✅ Veikia
    ```
    
    **Ref:** U3 Žingsnis 2

---

## Skaidrė 4: Fizinis moduliavimas (.h + .cpp) — 1 📁

**C++ paveldas iš C:** Atskirti **deklaracijas** nuo **implementacijų**.

**Problema:** Visos funkcijos `main.cpp` → netvarkinga, sunku testuoti.

**Sprendimas:** Sukurti atskirą **modulį** (failų porą).

=== "Student.h (Header)"

    **Deklaracijos** — "ką" klasė daro
    
    ```cpp
    #ifndef STUDENT_H  // ← Header guard (apsauga nuo dvigubo įtraukimo)
    #define STUDENT_H
    
    #include <string>
    using namespace std;
    
    class Student {
    private:
        string name;
        int age;
        
    public:
        // TIKTAI deklaracijos (prototypes)
        Student(string n, int a);
        string getName() const;
        void setAge(int a);
    };
    
    #endif  // STUDENT_H
    ```

=== "Student.cpp (Implementation)"

    **Implementacijos** — "kaip" klasė veikia
    
    ```cpp
    #include "Student.h"
    #include <iostream>
    
    // Scope resolution operator ::
    Student::Student(string n, int a) : name(n), age(a) {
        cout << "Student created: " << name << endl;
    }
    
    string Student::getName() const {
        return name;
    }
    
    void Student::setAge(int a) {
        if (a > 0) {
            age = a;
        }
    }
    ```

=== "main.cpp (Usage)"

    **Naudojimas** — clean ir paprastas
    
    ```cpp
    #include "Student.h"
    
    int main() {
        Student s("Jonas", 20);
        cout << s.getName() << endl;
        
        s.setAge(21);
        
        return 0;
    }
    ```

??? info "Header guard — kas tai?"

    **Problema be guard:**
    ```cpp
    // main.cpp
    #include "Student.h"  // 1. Įtraukia Student klasę
    #include "Course.h"   // 2. Course.h irgi daro #include "Student.h"
    // ❌ Student klasė apibrėžta DU kartus → klaida!
    ```
    
    **Sprendimas:**
    ```cpp
    #ifndef STUDENT_H  // Jei STUDENT_H dar neapibrėžtas
    #define STUDENT_H  // Apibrėžk jį
    
    // ... klasės kodas ...
    
    #endif  // Uždaryk
    // ✅ Antras #include nieko nedaro (STUDENT_H jau apibrėžtas)
    ```

**Ref:** U2 Žingsnis 5 (scope resolution operator `::`)

---

## Skaidrė 5: Fizinis moduliavimas — 2 (kompiliacija) ⚙️

**Kaip sukompiliuoti kelių failų projektą?**

**Projekto struktūra:**

```
project/
├── main.cpp
├── Student.h
├── Student.cpp
├── Course.h
├── Course.cpp
└── Makefile
```

=== "Kompiliacija rankiniu būdu"

    **Žingsnis po žingsnio:**
    
    ```bash
    # 1. Kompiliuoti kiekvieną .cpp į .o (object file)
    g++ -c Student.cpp   # → Student.o
    g++ -c Course.cpp    # → Course.o
    g++ -c main.cpp      # → main.o
    
    # 2. Susieti (link) visus .o į vykdomąjį failą
    g++ Student.o Course.o main.o -o app
    
    # 3. Paleisti
    ./app
    ```
    
    **Kas vyksta?**
    
    - `-c` — compile only (nesijungia)
    - Kiekvienas `.cpp` → `.o` failas
    - Linker sujungia visus `.o` → `app`

=== "Su Makefile (rekomenduojama)"

    **Makefile:**
    
    ```makefile
    CXX = g++
    CXXFLAGS = -std=c++11 -Wall
    
    # Target: reikalingi failai
    app: main.o Student.o Course.o
    	$(CXX) main.o Student.o Course.o -o app
    
    main.o: main.cpp Student.h Course.h
    	$(CXX) $(CXXFLAGS) -c main.cpp
    
    Student.o: Student.cpp Student.h
    	$(CXX) $(CXXFLAGS) -c Student.cpp
    
    Course.o: Course.cpp Course.h Student.h
    	$(CXX) $(CXXFLAGS) -c Course.cpp
    
    clean:
    	rm -f *.o app
    ```
    
    **Naudojimas:**
    ```bash
    make        # Kompiliuoja
    make clean  # Išvalo
    ```

??? tip "Kodėl atskirti .h ir .cpp?"

    **Privalumai:**
    
    1. **Greičiau kompiliuoja** — keičiant `.cpp`, tik jis perkompiliuojamas
    2. **Apsauga** — vartotojas mato tik `.h` (interfeisą), ne implementaciją
    3. **Organizacija** — aiškus skirstymas: kas / kaip
    4. **Komandinis darbas** — skirtingi žmonės dirba su skirtingais failais

??? warning "Dažnos klaidos"

    **1. Pamiršti `#include "Student.h"` Student.cpp:**
    ```cpp
    // Student.cpp
    // #include "Student.h"  ← PAMIRŠTA!
    
    Student::Student(string n, int a) { ... }
    // ❌ error: 'Student' has not been declared
    ```
    
    **2. Naudoti `#include <Student.h>` vietoj `#include "Student.h"`:**
    ```cpp
    #include <Student.h>  // ❌ Ieško sisteminiuose directorijose
    #include "Student.h"  // ✅ Ieško projekte
    ```
    
    **3. Dvigubas apibėrimas (be header guard):** Rezultatas - kompiliavimo klaida.

**Ref:** U1 Žingsnis 3, U2 Žingsnis 5

---

## Skaidrė 6: Static nariai (klasės kintamieji) 📊

**Static narys** — bendras **visiems** objektams (vienas egzempliorius).

```cpp
class Window {
private:
    float height, width;
    static int windowCount;  // ← Vienas visiems!
    
public:
    Window(float h, float w) {
        height = h;
        width = w;
        windowCount++;       // ← Visi objektai mato
    }
    
    static int getWindowCount() {  // ← Static metodas
        return windowCount;
    }
};

// Inicializacija UŽ KLASĖS (privaloma!)
int Window::windowCount = 0;  // ← .cpp faile
```

??? warning "Dažna klaida #1"

    **Pamiršti inicializuoti už klasės:**
    ```cpp
    // Window.cpp
    // int Window::windowCount = 0;  ← PAMIRŠTA!
    
    // Kompiliavimo/linkerio klaida:
    // undefined reference to `Window::windowCount'
    ```

??? tip "Naudojimas"

    ```cpp
    // Be objekto:
    cout << Window::getWindowCount();  // ← Klasės metodas
    
    // Su objektu (irgi veikia):
    Window w(1.5, 1.0);
    cout << w.getWindowCount();
    ```

**Ref:** U3 Žingsnis 1

---

## Skaidrė 7: Const static konstantos 🔢

**Const static** — klasės **konstanta** (nepriklausoma nuo objekto).

```cpp
class Stack {
private:
    static const int MAX_SIZE = 100;  // ← Inline inicializacija!
    int data[MAX_SIZE];
    int top;
    
public:
    Stack() : top(-1) { }
    
    bool isFull() const {
        return top >= MAX_SIZE - 1;  // ← Naudojam konstantą
    }
};

// Nereikia inicializuoti už klasės! (const static ypatybė)
```

??? info "Skirtumas nuo static int"

    | Tipas | Inicializacija | Keičiamas? |
    |-------|---------------|-----------|
    | `static int` | Už klasės (out-of-line) | ✅ Taip |
    | `static const int` | Klasėje (inline) | ❌ Ne |
    
    ```cpp
    class Example {
        static int counter;           // Out-of-line init
        static const int MAX = 100;   // Inline init
    };
    
    int Example::counter = 0;  // ← Reikia
    // int Example::MAX = 100; // ← Nereikia!
    ```

??? tip "Kodėl const gali inline?"

    Kompiliatorius žino, kad reikšmė **niekada** nepasikeis → gali optimizuoti.

**Ref:** U2 Žingsnis 5 pastaba

---

## Skaidrė 8: 1 Dalies Santrauka ✅

**Išmokome:**

| Tema | Sintaksė | Kada naudoti |
|------|----------|-------------|
| **Klasės struktūra** | `class Name { private: ... public: ... };` | Visada |
| **Fizinis moduliavimas** | `.h` + `.cpp` | Multi-failiniai projektai |
| **Header guards** | `#ifndef`, `#define`, `#endif` | Visi `.h` failai |
| **Static nariai** | `static int count;` + init už klasės | Bendri duomenys |
| **Const static** | `static const int MAX = 100;` | Klasės konstantos |
| **Scope resolution** | `Class::method()` | `.cpp` implementacijos |

**Dažniausios klaidos:**

- ❌ Pamiršti `int Class::staticVar = 0;`
- ❌ Naudoti `<Student.h>` vietoj `"Student.h"`
- ❌ Pamiršti header guards
- ❌ Bandyti `obj.privateVar` tiesiai

---

<div style="page-break-after: always;"></div>

---

## 📚 2 DALIS: Objekto gyvavimo ciklas

---

## Skaidrė 9: Objekto sukūrimas - Stack vs Heap 🏗️

**Du būdai sukurti objektą:**

=== "Stack (automatinis)"

    ```cpp
    void function() {
        Student s("Jonas", 20);  // ← Stack
        
        // Destruktorius automatiškai iškviestas
    }  // ← s sunaikinamas čia
    ```
    
    ✅ **Privalumai:**
    
    - Automatinis gyvavimo valdymas
    - Greitas
    - Nereikia `delete`
    
    ❌ **Trūkumai:**
    
    - Gyvuoja tik scope'e
    - Ribota atmintis

=== "Heap (dinaminis)"

    ```cpp
    void function() {
        Student* p = new Student("Jonas", 20);  // ← Heap
        
        // Naudojam per rodyklę
        cout << p->getName();
        
        delete p;  // ← PRIVALOMA!
    }
    ```
    
    ✅ **Privalumai:**
    
    - Gyvuoja kol neiškviestas `delete`
    - Didesnė atmintis
    
    ❌ **Trūkumai:**
    
    - Reikia rankiniu būdu `delete`
    - Galimi memory leaks

??? danger "Memory leak"

    ```cpp
    void leak() {
        Student* p = new Student("Jonas", 20);
        // delete p;  ← PAMIRŠTA!
    }  // ← Atmintis "nuteka"
    ```

**Ref:** U3 Žingsnis 2

---

## Skaidrė 10: Default konstruktorius - Kada būtinas? 🤔

**Default konstruktorius** — konstruktorius **be parametrų**.

```cpp
class Point {
    int x, y;
public:
    Point() : x(0), y(0) { }  // ← Default
};
```

??? question "Kada kompiliatorius GENERUOJA default konstruktorių?"

    **TAISYKLĖ:** Tik kai **NĖRA JOKIŲ** konstruktorių!
    
    ```cpp
    class Auto {
        int x;
    };
    // ✅ Kompiliatorius sugeneruoja: Auto() { }
    
    class Manual {
        int x;
    public:
        Manual(int val) : x(val) { }  // ← Parametrinis
    };
    // ❌ Kompiliatorius NEGENERUOJA default!
    ```

??? question "Kada default konstruktorius BŪTINAS?"

    **3 atvejai:**
    
    **1. Masyvai:**
    ```cpp
    Window windows[10];  // ❌ Klaida jei nėra Window()
    ```
    
    **2. Dinaminiai masyvai:**
    ```cpp
    Window* arr = new Window[10];  // ❌ Klaida jei nėra Window()
    ```
    
    **3. Konteineriai (kartais):**
    ```cpp
    vector<Window> vec(10);  // ❌ Klaida jei nėra Window()
    ```

??? example "U3 problema: new Window[10]"

    ```cpp
    class Room {
        Window* windows;
        int capacity;
    public:
        Room() {
            capacity = 10;
            windows = new Window[capacity];  // ❌ KLAIDA!
            // error: no matching function for call to 'Window::Window()'
        }
    };
    ```
    
    **Kodėl?** Window turi tik `Window(float h, float w)`, nėra `Window()`.
    
    **Sprendimas:** Naudoti `Window**` arba `vector<Window>`.
    
    **Ref:** U3 Žingsnis 4 problema

??? tip "Geroji praktika: Logiškai nebūtini default'ai"

    ```cpp
    class Window {
    public:
        Window(float h, float w);  // Tik parametrinis
        // Nėra Window() — sąmoningai!
    };
    ```
    
    **Kodėl?** Langas be matmenų — nelogiškas.
    
    **Ref:** U3 Žingsnis 1 Svarbu

---

## Skaidrė 11: Parametriniai konstruktoriai ir Overloading 🎛️

**Parametrinis konstruktorius** — priima argumentus.

```cpp
class Student {
    string name;
    int age;
    
public:
    // Konstruktorius su 2 parametrais
    Student(string n, int a) : name(n), age(a) {
        cout << "Student created: " << name << endl;
    }
};

// Naudojimas:
Student s1("Jonas", 20);
```

??? info "Constructor Overloading"

    **Galima keletas konstruktorių:**
    
    ```cpp
    class Student {
        string name;
        int age;
        
    public:
        // 1. Default
        Student() : name("Unknown"), age(0) { }
        
        // 2. Tik vardas
        Student(string n) : name(n), age(0) { }
        
        // 3. Vardas ir amžius
        Student(string n, int a) : name(n), age(a) { }
    };
    
    // Naudojimas:
    Student s1;               // ← Kviečia #1
    Student s2("Jonas");      // ← Kviečia #2
    Student s3("Jonas", 20);  // ← Kviečia #3
    ```

??? tip "Delegation (C++11)"

    ```cpp
    class Student {
    public:
        Student() : Student("Unknown", 0) { }  // ← Deleguoja
        Student(string n) : Student(n, 0) { }  // ← Deleguoja
        Student(string n, int a) : name(n), age(a) { 
            // Pagrindinis konstruktorius
        }
    };
    ```

**Ref:** U2 Žingsnis 3

---

## Skaidrė 12: Inicializacijos sąrašas (Member Initializer List) ⭐

**Sintaksė:** `Constructor() : member1(val1), member2(val2) { }`

```cpp
class Student {
    string name;
    int age;
    
public:
    // ✅ GERIAU: Inicializacijos sąrašas
    Student(string n, int a) : name(n), age(a) {
        // Body tuščias arba papildoma logika
    }
    
    // ⚠️ VEIKIA, bet ne optimaliai:
    Student(string n, int a) {
        name = n;  // Assignment, ne inicializacija
        age = a;
    }
};
```

??? question "Kada inicializacijos sąrašas PRIVALOMAS?"

    **3 atvejai:**
    
    **1. Const nariai:**
    ```cpp
    class Example {
        const int id;
    public:
        Example(int i) : id(i) { }  // ← PRIVALOMA
        // Example(int i) { id = i; }  // ❌ Negalima!
    };
    ```
    
    **2. Reference nariai:**
    ```cpp
    class Wrapper {
        int& ref;
    public:
        Wrapper(int& r) : ref(r) { }  // ← PRIVALOMA
    };
    ```
    
    **3. Objektai be default konstruktoriaus:**
    ```cpp
    class Room {
        Window window;  // Window neturi Window()
    public:
        Room() : window(1.5, 1.0) { }  // ← PRIVALOMA
    };
    ```

??? tip "Kodėl inicializacijos sąrašas geresnis?"

    ```cpp
    class Student {
        string name;
    public:
        // Su inicializacijos sąrašu:
        Student(string n) : name(n) { }
        // 1 operacija: konstruojamas name su n
        
        // Be inicializacijos sąrašo:
        Student(string n) {
            name = n;
        }
        // 2 operacijos: 
        // 1. Default konstruojamas name
        // 2. Assignment name = n
    };
    ```
    
    **Efektyviau** su inicializacijos sąrašu!

??? example "Tvarka svarbi?"

    **Inicializavimo tvarka** priklauso nuo **deklaravimo tvarkos**, ne sąrašo!
    
    ```cpp
    class Example {
        int a;
        int b;
    public:
        Example(int x) : b(x), a(b) { }  // ⚠️ PROBLEMA!
        // Inicializuojama: a (pirmas), b (antras)
        // a naudoja b, bet b dar neinicializuotas!
    };
    ```

**Ref:** Paskaita Stack 11-16

---

## Skaidrė 13: Destruktorius - Kada reikia rašyti? 🧹

**Destruktorius** — iškviestas automatiškai, kai objektas **sunaikinamas**.

```cpp
class Student {
public:
    ~Student() {  // ← Destruktorius
        cout << "Student destroyed" << endl;
    }
};
```

??? question "Kada destruktorius BŪTINAS?"

    **TAISYKLĖ:** Kai klasė **valdo dinaminę atmintį** arba **resursus**.
    
    ```cpp
    class DynamicArray {
        int* data;
        int size;
        
    public:
        DynamicArray(int s) : size(s) {
            data = new int[size];  // ← Allocated!
        }
        
        ~DynamicArray() {
            delete[] data;  // ← PRIVALOMA!
        }
    };
    ```
    
    **Jei pamiršite destruktorių → memory leak!**

??? info "Destruktorių iškvietimo tvarka"

    **Atvirkščia** nuo konstruktorių:
    
    ```cpp
    {
        Student s1("Jonas");   // 1. Konstruojamas
        Student s2("Petras");  // 2. Konstruojamas
        
    }  // 2. s2 destruktorius ← Pirmasis!
       // 1. s1 destruktorius ← Antrasis!
    ```

??? example "U3: Room destruktorius"

    ```cpp
    class Room {
        vector<Window> windows;  // ← Vector naikina automatiškai
        Door* door;              // ← Rodyklė
        
    public:
        ~Room() {
            // Vector automatiškai sunaikina Window objektus
            // Door* — nevaldome (agregacija)
        }
    };
    ```
    
    **Ref:** U3 Žingsnis 3

---

## Skaidrė 14: `this` rodyklė - Kada naudoti? 👆

**`this`** — rodyklė į **dabartinį objektą**.

```cpp
class Student {
    string name;
    int age;
    
public:
    void setName(string name) {
        this->name = name;  // ← this-> atskiria
        // be this->: name = name (parametras = parametras!)
    }
    
    Student& addAge(int years) {
        this->age += years;
        return *this;  // ← Grąžinam save (chaining)
    }
};
```

??? tip "Naudojimo atvejai"

    **1. Parametras ir member tą patį vardą:**
    ```cpp
    void setHeight(float height) {
        this->height = height;  // ← Aiškus skirtumas
    }
    ```
    
    **2. Method chaining:**
    ```cpp
    student.setName("Jonas")
           .setAge(20)
           .print();  // ← Grandinė
    ```
    
    **3. Grąžinti save:**
    ```cpp
    Student& getSelf() {
        return *this;
    }
    ```

??? example "U3: Window konstruktorius su this->"

    ```cpp
    Window::Window(float height, float width) {
        this->height = height;  // ← this-> praktika
        this->width = width;
    }
    ```
    
    **Ref:** U3 Žingsnis 2

---

## Skaidrė 15: `const` metodai - Garantuoja nekeitimą 🔒

**`const` metodas** — garantuoja, kad **nemodifikuos** objekto.

```cpp
class Student {
    string name;
    int age;
    
public:
    // ✅ const metodas (getter)
    string getName() const {
        return name;  // Tik skaito
        // age++;     // ❌ Klaida! Bandymas keisti
    }
    
    // ❌ Ne-const metodas (setter)
    void setAge(int a) {
        age = a;  // Keičia objektą
    }
};
```

??? question "Kada const PRIVALOMAS?"

    **2 atvejai:**
    
    **1. Const objektas:**
    ```cpp
    const Student s("Jonas", 20);
    
    cout << s.getName();  // ✅ const metodas
    s.setAge(21);         // ❌ ne-const metodas
    ```
    
    **2. Const reference:**
    ```cpp
    void print(const Student& s) {
        cout << s.getName();  // ✅ const metodas
        // s.setAge(21);      // ❌ ne-const metodas
    }
    ```

??? tip "Visi getteriai turėtų būti const"

    ```cpp
    class Point {
        int x, y;
    public:
        int getX() const { return x; }  // ✅ Geroji praktika
        int getY() const { return y; }
    };
    ```

**Ref:** U2, U3 getteriai

---

## Skaidrė 16: Parametrų perdavimas: `const Type&` ⚡

**Klausimas:** Kaip efektyviai perduoti objektus?

=== "❌ Blogai: Pass by value"

    ```cpp
    void process(Student s) {  // ← Kopijuoja visą objektą!
        cout << s.getName();
    }
    ```
    
    **Problema:** Jei `Student` didelis → lėta kopija

=== "✅ Gerai: Pass by const reference"

    ```cpp
    void process(const Student& s) {  // ← Tik rodyklė!
        cout << s.getName();
    }
    ```
    
    **Privalumai:**
    
    - Nekopijuoja objekto (greičiau)
    - `const` garantuoja nekeitimą

=== "⚠️ Pass by reference (be const)"

    ```cpp
    void modify(Student& s) {  // ← Gali keisti!
        s.setAge(25);
    }
    ```
    
    **Kada naudoti:** Kai funkcija **turi** keisti objektą

??? info "Sintaksės detalizacija"

    | Sintaksė | Kopijuoja? | Gali keisti? | Kada naudoti |
    |----------|-----------|-------------|-------------|
    | `Type obj` | ✅ Taip | ✅ Taip | Maži tipai (`int`, `double`) |
    | `Type& obj` | ❌ Ne | ✅ Taip | Keisti objektą |
    | `const Type& obj` | ❌ Ne | ❌ Ne | **Skaityti** objektą (dažniausiai!) |

??? example "U3 pavyzdžiai"

    ```cpp
    // addWindow - nekopijuoja, nemodifikuoja
    void Room::addWindow(const Window& w) {
        windows.push_back(w);  // vector daro kopiją
    }
    
    // calculateWallArea - nekopijuoja, nemodifikuoja
    float calculateWallArea(const Room& room) {
        return room.getLength() * room.getHeight();
    }
    ```
    
    **Ref:** U3 Žingsniai 3-4

---

## Skaidrė 17: 2 Dalies Santrauka ✅

**Išmokome:**

| Tema | Sintaksė | Kada būtina |
|------|----------|------------|
| **Default konstruktorius** | `Class() { }` | Masyvai, `vector(n)` |
| **Parametrinis** | `Class(args) : members(args) { }` | Visada geriau |
| **Init sąrašas** | `: member(val)` | `const`, `&`, objektai |
| **Destruktorius** | `~Class() { }` | Dinaminė atmintis |
| **`this`** | `this->member` | Vardų konfliktas |
| **`const` metodas** | `int get() const` | Getteriai |
| **`const Type&`** | `void f(const T& obj)` | Perduoti objektus |

**Dažniausios klaidos:**

- ❌ Pamiršti default konstruktorių masyvams
- ❌ Naudoti assignment vietoj init sąrašo
- ❌ Pamiršti `delete` destruktoriuje
- ❌ Perduoti objektus per value vietoj `const&`

---

<div style="page-break-after: always;"></div>

---

## 📚 3 DALIS: Kompozicija (has-a ryšys) 🏗️

---

## Skaidrė 18: Has-a ryšys - Sudėtingi objektai 🏗️

**Kompozicija** — objektas **talpina** kitus objektus.

```cpp
class Room {
    float length, width, height;  // ← Primityvūs tipai
    
    vector<Window> windows;       // ← Sudėtingas tipas!
    Door* door;                   // ← Sudėtingas tipas!
};
```

??? info "Has-a vs Is-a"

    | Ryšys | Pavyzdys | C++ |
    |-------|----------|-----|
    | **Has-a** | Room **turi** Windows | Kompozicija |
    | **Is-a** | Car **yra** Vehicle | Paveldėjimas (vėliau) |

??? example "Realaus pasaulio pavyzdžiai"

    ```
    Computer HAS-A:
      ├─ CPU
      ├─ RAM
      └─ Storage
    
    Car HAS-A:
      ├─ Engine
      ├─ Wheels[4]
      └─ Driver*
    ```

**Ref:** U3 intro

---

## Skaidrė 19: Value Semantics (Kompozicija) 📦

**Value semantics** — objektas **laiko kopijas** komponentų.

```cpp
class Room {
    vector<Window> windows;  // ← Laiko kopijas!
    
public:
    void addWindow(const Window& w) {
        windows.push_back(w);  // ← Kopijuoja Window
    }
    
    ~Room() {
        // Vector automatiškai sunaikina Window objektus!
    }
};
```

??? info "Kas vyksta?"

    ```cpp
    Room room;
    Window w(1.5, 1.0);  // 1. Sukuriamas Window stack'e
    
    room.addWindow(w);   // 2. Vector kopijuoja w
                         // 3. Stack'o w sunaikinamas
    
    // 4. Room destruktorius sunaikina vector
    // 5. Vector destruktorius sunaikina Window kopijas
    ```

??? tip "Strong ownership"

    Room **valdo** Window gyvavimo ciklą:
    
    - Room sukūrė → Room sunaikina
    - Window **negali** egzistuoti be Room

??? example "U3 pavyzdys"

    ```cpp
    {
        Room room(5, 4, 3);
        
        Window w1(1.5, 1.0);
        room.addWindow(w1);  // ← w1 kopijuojasi į room
        
    }  // ← room destruktorius sunaikina Window kopijas
       //    w1 sunaikinamas atskirai
    ```
    
    **Ref:** U3 Žingsnis 3

---

## Skaidrė 20: Pointer Semantics (Agregacija) 🔗

**Pointer semantics** — objektas **laiko tik rodyklę**.

```cpp
class Room {
    Door* door;  // ← Tik rodyklė!
    
public:
    Room() : door(nullptr) { }
    
    void setDoor(Door* d) {
        door = d;  // ← Tik adresas
    }
    
    ~Room() {
        // NENAIKINOME door!
        // Kas sukūrė su 'new', tas ir naikina su 'delete'
    }
};
```

??? info "Kas vyksta?"

    ```cpp
    Room room;
    Door* d = new Door(2.0, 0.9);  // 1. Sukuriam heap'e
    
    room.setDoor(d);                // 2. Room laiko adresą
    
    // 3. Room destruktorius NENAIKINA d
    delete d;                       // 4. Privaloma išorėje!
    ```

??? tip "Weak relationship"

    Room **nevaldo** Door gyvavimo ciklo:
    
    - Door gali egzistuoti nepriklausomai
    - Kas sukūrė, tas ir naikina

??? example "U3 pavyzdys"

    ```cpp
    {
        Room room(5, 4, 3);
        
        Door* door = new Door(2.0, 0.9);
        room.setDoor(door);  // ← Tik rodyklė
        
        delete door;  // ← main.cpp atsakingas!
    }  // ← room destruktorius NENAIKINA door
    ```
    
    **Ref:** U3 Žingsnis 3

---

## Skaidrė 21: Kompozicija vs Agregacija vs Asociacija 🎯

**Trys ryšių tipai:**

=== "Kompozicija (Composition)"

    **"Owns" — Strong ownership**
    
    ```cpp
    class Car {
        Engine engine;  // ← Car VALDO Engine
    };
    ```
    
    - Engine **priklauso** Car
    - Engine **negali** egzistuoti be Car
    - Car destruktorius sunaikina Engine
    
    **Pavyzdys:** `vector<Window>`

=== "Agregacija (Aggregation)"

    **"Has" — Weak relationship**
    
    ```cpp
    class Car {
        Driver* driver;  // ← Car ŽINO apie Driver
    };
    ```
    
    - Driver **gali** egzistuoti be Car
    - Car destruktorius NENAIKINA Driver
    - Išorė atsakinga už Driver
    
    **Pavyzdys:** `Door*`

=== "Asociacija (Association)"

    **"Knows about" — Uses**
    
    ```cpp
    float calculateArea(const Room& room) {
        // Funkcija NAUDOJA Room, bet nevaldo
    }
    ```
    
    - Funkcija **žino apie** Room
    - Bet jokios atsakomybės už gyvavimą
    
    **Pavyzdys:** Globalios funkcijos

??? tip "Kaip pasirinkti?"

    | Klausimas | Taip → | Ne → |
    |-----------|--------|------|
    | Komponentas priklauso **tik** šiam objektui? | Kompozicija | ↓ |
    | Komponentas **dalinamas** su kitais? | Agregacija | ↓ |
    | Tik **naudojamas**, bet nevaldomas? | Asociacija | ↓ |

**Ref:** U3 Žingsnis 3 Technikos

---

## Skaidrė 22: Vector copy problema (sneak peek Rule of 3) 🐛

**Problema:** Ką daro `vector.push_back(obj)`?

```cpp
Room room;

Window w(1.5, 1.0);
room.addWindow(w);  // ← Kas vyksta?
```

??? question "Vector kopijuoja objektą - kaip?"

    **Vector naudoja copy konstruktorių:**
    
    ```cpp
    void vector::push_back(const Window& w) {
        // Sukuriamas naujas Window:
        new (&data[size]) Window(w);  // ← Copy constructor!
    }
    ```
    
    **Jei Window neturi copy konstruktoriaus?**
    
    - Kompiliatorius **generuoja** default copy constructor
    - **Shallow copy** — tiesiog kopijuoja member'ius

??? danger "Shallow copy problema"

    ```cpp
    class DynamicArray {
        int* data;
        int size;
        
    public:
        DynamicArray(int s) : size(s) {
            data = new int[size];
        }
        
        ~DynamicArray() {
            delete[] data;
        }
        
        // Nėra copy konstruktoriaus!
        // Kompiliatorius generuoja shallow copy:
        // DynamicArray(const DynamicArray& other) 
        //     : data(other.data), size(other.size) { }
    };
    ```
    
    **Problema:**
    ```cpp
    vector<DynamicArray> vec;
    DynamicArray arr(10);
    
    vec.push_back(arr);  // ← Shallow copy!
                         // vec[0].data == arr.data (tas pats!)
    
    // Destruktoriuje:
    // 1. arr destruktorius: delete[] data
    // 2. vec[0] destruktorius: delete[] data (tas pats!)
    // ❌ CRASH: double delete!
    ```

??? tip "Sprendimas: Rule of Three (kita tema!)"

    ```cpp
    class DynamicArray {
        int* data;
        int size;
        
    public:
        // 1. Destruktorius
        ~DynamicArray() {
            delete[] data;
        }
        
        // 2. Copy konstruktorius (DEEP COPY)
        DynamicArray(const DynamicArray& other) 
            : size(other.size) 
        {
            data = new int[size];  // ← Nauja atmintis!
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        
        // 3. Copy assignment
        DynamicArray& operator=(const DynamicArray& other) {
            // ... (bus kitoje temoje)
        }
    };
    ```

**Ref:** U3 BONUS (būsimas)

---

## Skaidrė 23: 3 Dalies Santrauka ✅

**Išmokome:**

| Ryšys | Sintaksė | Valdymas | Pavyzdys |
|-------|----------|----------|----------|
| **Kompozicija** | `Type member;` | Strong (owns) | `vector<Window>` |
| **Agregacija** | `Type* member;` | Weak (knows) | `Door*` |
| **Asociacija** | `const Type&` | None (uses) | Globalios funkcijos |

**Copy semantics:**

- `vector.push_back()` → **kopijuoja** objektą
- Default copy constructor → **shallow copy**
- Jei dinaminė atmintis → **reikia deep copy** (Rule of 3)

**Dažniausios klaidos:**

- ❌ Destruktoriuje `delete` agregacijos rodyklę
- ❌ Nesuprasti shallow vs deep copy
- ❌ Nenaudoti `const&` perduodant objektus

---

## Skaidrė 24: Visos dalys - Bendra santrauka 🎓

**Klasė kaip UDT:**

- Fizinis moduliavimas (.h + .cpp)
- Header guards privalomi
- Enkapsuliacija ≠ informacijos slėpimas
- `static` nariai — klasės kintamieji
- `static const` — klasės konstantos

**Objekto gyvavimas:**

- Default konstruktorius būtinas masyvams
- Inicializacijos sąrašas > assignment
- Destruktorius būtinas dinaminei atmintį
- `this` atskiria member nuo parametro
- `const` metodai geteriams
- `const Type&` perduoti objektus

**Kompozicija:**

- Value semantics → Strong ownership
- Pointer semantics → Weak relationship
- Vector kopijuoja → shallow copy problema

---

## Skaidrė 25: Kas toliau? Rule of Three 🚀

**Problema, kurią dabar matome:**

```cpp
class DynamicArray {
    int* data;
public:
    DynamicArray(int s) { data = new int[s]; }
    ~DynamicArray() { delete[] data; }
    
    // ❌ Nėra copy konstruktoriaus!
    // ❌ Kompiliatorius generuoja shallow copy
};

vector<DynamicArray> vec;
vec.push_back(arr);  // ← CRASH vėliau!
```

**Rule of Three (kita paskaita):**

1. **Destruktorius** — atlaisvina atmintį
2. **Copy konstruktorius** — deep copy
3. **Copy assignment operator** — deep copy su cleanup

**+ Rule of Five (C++11):**

4. **Move konstruktorius** — efektyvus perkėlimas
5. **Move assignment** — efektyvus perkėlimas su cleanup

**Intro to RAII (Resource Acquisition Is Initialization):**

- Konstruktorius įgyja resursą
- Destruktorius atlaisvina resursą
- Automatinis valdymas!

---

## Skaidrė 26: Masyvai - Stack inicializacija 📊

**C++ masyvai** — fiksuoto dydžio konteineriai.

**Stack masyvai (įvairūs inicializavimo būdai):**

```cpp
// 1. Neincializuotas (šiukšlės reikšmės!)
int arr1[5];  // ⚠️ Nežinomos reikšmės

// 2. C-style inicializacija
int arr2[5] = {1, 2, 3, 4, 5};  // Visi nurodyti

// 3. Dalinė inicializacija
int arr3[5] = {1, 2};  // {1, 2, 0, 0, 0} - likusieji 0

// 4. Dydis išvedamas
int arr4[] = {1, 2, 3};  // Dydis = 3

// 5. Visi zeros
int arr5[5] = {};  // {0, 0, 0, 0, 0}

// 6. C++11 uniform initialization (be =)
int arr6[5]{1, 2, 3, 4, 5};  // ✅ Modernas stilius

// 7. C++11 zeros
int arr7[5]{};  // {0, 0, 0, 0, 0}
```

??? warning "Dažna klaida: Neincializuoti masyvai"

    ```cpp
    int arr[100];
    cout << arr[0];  // ⚠️ Šiukšlės reikšmė!
    
    // Geriau:
    int arr[100] = {};  // Visi 0
    // arba
    int arr[100]{};     // C++11
    ```

??? info "Objektų masyvai stack'e"

    ```cpp
    class Point {
        int x, y;
    public:
        Point() : x(0), y(0) { }  // ← Default būtinas!
    };
    
    Point points[3];  // Kviečia Point() 3 kartus
    ```

**Ref:** U1 Žingsnis 1 (masyvai)

---

## Skaidrė 27: Masyvai - Heap ir new[] operatorius 💾

**Dinaminiai masyvai** — dydis gali būti žinomas runtime.

=== "new[] vs new"

    **`new Type[n]`** — masyvas (daug objektų)
    
    ```cpp
    int* arr = new int[10];  // 10 int'ų masyvas
    ```
    
    **`new Type`** — vienas objektas
    
    ```cpp
    int* p = new int;  // Vienas int
    ```
    
    **Skirtumas:**
    
    - `new[]` sukuria **masyvą** objektų
    - `new` sukuria **vieną** objektą

=== "delete[] vs delete"

    **`delete[] ptr`** — naikina masyvą
    
    ```cpp
    int* arr = new int[10];
    delete[] arr;  // ← PRIVALOMA []
    ```
    
    **`delete ptr`** — naikina vieną objektą
    
    ```cpp
    int* p = new int;
    delete p;  // ← Be []
    ```
    
    **KRITINIS SKIRTUMAS:**
    
    ```cpp
    int* arr = new int[10];
    delete arr;    // ❌ BLOGAI! (undefined behavior)
    delete[] arr;  // ✅ GERAI!
    
    int* p = new int;
    delete[] p;    // ❌ BLOGAI! (undefined behavior)
    delete p;      // ✅ GERAI!
    ```

??? danger "Kas vyksta su delete vs delete[]?"

    **Su `delete[]`:**
    ```cpp
    Student* arr = new Student[3];
    // Sukurti 3 objektai:
    // Student() konstruktorius × 3
    
    delete[] arr;
    // ✅ Student destruktorius × 3
    // ✅ Atlaisvinama visa atmintis
    ```
    
    **Su `delete` (KLAIDINGAI):**
    ```cpp
    Student* arr = new Student[3];
    
    delete arr;  // ❌ BLOGAI!
    // ❌ Student destruktorius × 1 (tik pirmas!)
    // ❌ Memory leak (likusieji 2 objektai)
    // ❌ Undefined behavior
    ```

**Heap masyvų inicializacija:**

```cpp
// 1. Neincializuotas
int* arr1 = new int[5];  // ⚠️ Šiukšlės

// 2. Zero-inicializacija (C++03)
int* arr2 = new int[5]();  // {0, 0, 0, 0, 0}

// 3. C++11 inicializacija
int* arr3 = new int[5]{1, 2, 3, 4, 5};  // ✅ Nurodytos reikšmės

// 4. C++11 dalinė
int* arr4 = new int[5]{1, 2};  // {1, 2, 0, 0, 0}
```

??? warning "Objektų masyvai heap'e - Default konstruktorius BŪTINAS!"

    ```cpp
    class Window {
    public:
        Window(float h, float w);  // Tik parametrinis
    };
    
    Window* arr = new Window[3];  // ❌ KLAIDA!
    // error: no matching function for call to 'Window::Window()'
    ```
    
    **Kodėl?** `new Window[3]` kviečia `Window()` 3 kartus!

**Ref:** U3 Žingsnis 2 (new operatorius), U3 Žingsnis 4 problema

---

## Skaidrė 28: Objektų masyvai - Default konstruktorius būtinas! 🚨

**Problema:** Objektų masyvams reikia default konstruktoriaus.

```cpp
class Window {
    float height, width;
public:
    Window(float h, float w) : height(h), width(w) { }
    // Nėra Window() ← PROBLEMA!
};
```

**3 bandymai sukurti masyvą:**

```cpp
// 1. Stack masyvas
Window windows[3];  // ❌ KLAIDA!
// error: no matching function for call to 'Window::Window()'

// 2. Heap masyvas
Window* windows = new Window[3];  // ❌ KLAIDA!
// error: no matching function for call to 'Window::Window()'

// 3. Vector su dydžiu
vector<Window> vec(3);  // ❌ KLAIDA!
// error: no matching function for call to 'Window::Window()'
```

**Kodėl?** Visi 3 atvejai kviečia **default konstruktorių**!

??? question "Sprendimas 1: Pridėti default konstruktorių"

    ```cpp
    class Window {
        float height, width;
    public:
        Window() : height(0), width(0) { }  // ← Pridėti
        Window(float h, float w) : height(h), width(w) { }
    };
    
    Window windows[3];  // ✅ Veikia (3×Window())
    ```
    
    **Privalumai:** Paprastas
    
    **Trūkumai:** 0×0 langas — nelogiškas

??? question "Sprendimas 2: Rodyklių masyvas"

    ```cpp
    // Stack: masyvas rodyklių
    Window* windows[3];
    windows[0] = new Window(1.5, 1.0);
    windows[1] = new Window(2.0, 1.2);
    windows[2] = new Window(1.8, 1.1);
    
    // Naikinimas:
    for (int i = 0; i < 3; i++) {
        delete windows[i];
    }
    
    // ARBA Heap: rodyklė į rodyklių masyvą
    Window** windows = new Window*[3];
    windows[0] = new Window(1.5, 1.0);
    // ...
    
    // Naikinimas:
    for (int i = 0; i < 3; i++) {
        delete windows[i];
    }
    delete[] windows;
    ```
    
    **Privalumai:** Nereikia default konstruktoriaus
    
    **Trūkumai:** Sudėtingas valdymas, 2× delete

??? question "Sprendimas 3: Vector + push_back (REKOMENDUOJAMA! ✅)"

    ```cpp
    vector<Window> windows;  // ← Tuščias
    
    windows.push_back(Window(1.5, 1.0));
    windows.push_back(Window(2.0, 1.2));
    windows.push_back(Window(1.8, 1.1));
    
    // Destruktorius automatiškai naikina visus!
    ```
    
    **Privalumai:** 
    
    - Nereikia default konstruktoriaus
    - Automatinis atmintis valdymas
    - Dinamiškas dydis
    
    **Trūkumai:** Kopijuoja objektus (bus aptarta Rule of 3)

??? example "U3 problema: Room su Window masyvu"

    ```cpp
    class Room {
        Window* windows;
        int capacity;
    public:
        Room() {
            capacity = 10;
            windows = new Window[capacity];  // ❌ KLAIDA!
        }
    };
    ```
    
    **Kodėl nepavyko?** Window neturi `Window()`.
    
    **Kaip U3 išsprendėme?** `vector<Window>` su `push_back()`!
    
    **Ref:** U3 Žingsnis 3, U3 Žingsnis 4 (dokumentuota problema)

---

## Skaidrė 29: Klausimai pasitikrinimui 📝

**Testuokite savo žinias!**

??? question "Klausimas 1: Static counter problema"

    **Kodas:**
    ```cpp
    class Window {
        static int count;
    public:
        Window() { count++; }
        ~Window() { count--; }
        static int getCount() { return count; }
    };
    
    // int Window::count = 0;  ← Užkomentuota!
    
    int main() {
        Window w1, w2;
        cout << Window::getCount();
    }
    ```
    
    **Klausimai:**
    
    - Kompiliuosis?
    - Jei taip, kas bus išvesta?
    - Kaip pataisyti?

??? success "Atsakymas 1"

    **Kompiliuosis?** ❌ Ne! (linkerio klaida)
    
    ```
    undefined reference to `Window::count'
    ```
    
    **Kodėl?** Static narys **turi** būti inicializuotas už klasės.
    
    **Kaip pataisyti:**
    ```cpp
    // Window.cpp
    int Window::count = 0;  // ← PRIVALOMA!
    ```

??? question "Klausimas 2: Default konstruktorius"

    **Kodas:**
    ```cpp
    class Point {
        int x, y;
    public:
        Point(int a, int b) : x(a), y(b) { }
    };
    
    int main() {
        Point p1(5, 10);      // ← Veiks?
        Point p2;             // ← Veiks?
        Point arr[5];         // ← Veiks?
        vector<Point> vec(3); // ← Veiks?
    }
    ```
    
    **Atsakykite kiekvienam atvejui ir paaiškinkite.**

??? success "Atsakymas 2"

    ```cpp
    Point p1(5, 10);      // ✅ VEIKS (parametrinis konstruktorius)
    Point p2;             // ❌ NEVEIKS (nėra default)
    Point arr[5];         // ❌ NEVEIKS (reikia default)
    vector<Point> vec(3); // ❌ NEVEIKS (reikia default)
    ```
    
    **Kodėl p2 neveikia?**
    Kompiliatorius **negeneruoja** default konstruktoriaus, nes yra parametrinis.
    
    **Kodėl arr ir vec neveikia?**
    Masyvai ir `vector(n)` kviečia default konstruktorių.
    
    **Kaip pataisyti:**
    ```cpp
    Point() : x(0), y(0) { }  // Pridėti default
    ```

??? question "Klausimas 3: const Type& vs Type"

    **Duoti 3 variantai:**
    
    ```cpp
    // A
    void processA(Student s) {
        cout << s.getName();
    }
    
    // B
    void processB(Student& s) {
        cout << s.getName();
    }
    
    // C
    void processC(const Student& s) {
        cout << s.getName();
    }
    ```
    
    **Klausimai:**
    
    - Kuris efektyviausias?
    - Kuris saugiausias?
    - Kada naudoti kiekvieną?

??? success "Atsakymas 3"

    | Variantas | Kopijuoja? | Gali keisti? | Efektyvumas |
    |-----------|-----------|-------------|-------------|
    | A (`Student s`) | ✅ Taip | ✅ Taip | ❌ Lėtas |
    | B (`Student& s`) | ❌ Ne | ✅ Taip | ✅ Greitas |
    | C (`const Student& s`) | ❌ Ne | ❌ Ne | ✅ Greitas |
    
    **Efektyviausias:** B arba C (abu nekopijuoja)
    
    **Saugiausias:** C (garantuoja nekeitimą)
    
    **Kada naudoti:**
    
    - **A:** Maži tipai (`int`, `double`) arba kai reikia kopijos
    - **B:** Funkcija **turi** keisti objektą
    - **C:** Funkcija **tik skaito** objektą (dažniausias!)

??? question "Klausimas 4: Shallow copy crash"

    **Debuginkite kodą:**
    ```cpp
    class Array {
        int* data;
        int size;
    public:
        Array(int s) : size(s) {
            data = new int[size];
        }
        ~Array() {
            delete[] data;
        }
    };
    
    int main() {
        vector<Array> vec;
        Array a(10);
        vec.push_back(a);
        // ← Crash čia! Kodėl?
    }
    ```

??? success "Atsakymas 4"

    **Problema:** **Shallow copy** → double delete!
    
    **Kas vyksta:**
    ```cpp
    Array a(10);
    // a.data = 0x1000 (heap)
    
    vec.push_back(a);
    // Kompiliatorius generuoja shallow copy:
    // vec[0].data = 0x1000 (TAS PATS adresas!)
    
    // main() baigiasi:
    // 1. vec[0] destruktorius: delete[] 0x1000 ✅
    // 2. a destruktorius: delete[] 0x1000 ❌ CRASH! (double delete)
    ```
    
    **Sprendimas:** Reikia **deep copy** (Rule of Three):
    ```cpp
    // Copy konstruktorius
    Array(const Array& other) : size(other.size) {
        data = new int[size];  // ← Nauja atmintis!
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }
    ```

??? question "Klausimas 5: Fizinis moduliavimas"

    **Padalinkite į .h ir .cpp:**
    
    ```cpp
    // Viskas main.cpp:
    class Calculator {
        int result;
    public:
        Calculator() : result(0) { }
        void add(int x) { result += x; }
        int getResult() const { return result; }
    };
    
    int main() {
        Calculator calc;
        calc.add(5);
        cout << calc.getResult();
    }
    ```
    
    **Sukurkite:**
    
    - `Calculator.h`
    - `Calculator.cpp`
    - `main.cpp`
    - Header guards!

??? success "Atsakymas 5"

    **Calculator.h:**
    ```cpp
    #ifndef CALCULATOR_H
    #define CALCULATOR_H
    
    class Calculator {
        int result;
    public:
        Calculator();
        void add(int x);
        int getResult() const;
    };
    
    #endif
    ```
    
    **Calculator.cpp:**
    ```cpp
    #include "Calculator.h"
    
    Calculator::Calculator() : result(0) { }
    
    void Calculator::add(int x) {
        result += x;
    }
    
    int Calculator::getResult() const {
        return result;
    }
    ```
    
    **main.cpp:**
    ```cpp
    #include <iostream>
    #include "Calculator.h"
    using namespace std;
    
    int main() {
        Calculator calc;
        calc.add(5);
        cout << calc.getResult();
    }
    ```

??? question "Klausimas 6: new[] ir delete[]"

    **Kodas:**
    ```cpp
    int* arr = new int[10];
    delete arr;  // ← Kas blogai?
    ```

??? success "Atsakymas 6"

    **Problema:** Naudoja `delete` vietoj `delete[]`!
    
    ```cpp
    int* arr = new int[10];
    delete arr;    // ❌ UNDEFINED BEHAVIOR!
    delete[] arr;  // ✅ TEISINGAI
    ```
    
    **Taisyklė:**
    
    - `new` → `delete`
    - `new[]` → `delete[]`

??? question "Klausimas 7: Objektų masyvų problema"

    **Kodas:**
    ```cpp
    class Window {
    public:
        Window(float h, float w);  // Tik parametrinis
    };
    
    Window windows[3];  // ← Kompiliuosis?
    ```

??? success "Atsakymas 7"

    **Kompiliuosis?** ❌ Ne!
    
    ```
    error: no matching function for call to 'Window::Window()'
    ```
    
    **Kodėl?** `Window windows[3]` kviečia `Window()` 3 kartus, bet jo nėra!
    
    **3 sprendimai:**
    
    1. Pridėti `Window()` default konstruktorių
    2. Naudoti rodyklių masyvą: `Window* windows[3];`
    3. Naudoti `vector` su `push_back()` (geriausia!)

---

## Skaidrė 27: Nuorodos ir ištekliai 📚

**Paskaitos:**

- Paskaita 2: Stack Evolution C (01-09)
- Paskaita 2: Stack Evolution C++ (11-16)

**Pratybos:**

- U1: C++ Primityvai
- U2: OOP Pagrindai
- U3: Kompozicija

**Kita tema:**

- Rule of Three / Five
- Deep vs Shallow Copy
- RAII principas

---

**Klausimai? 🙋**