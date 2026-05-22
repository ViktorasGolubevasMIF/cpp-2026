# Šablonai: bendrinis programavimas C++

---

## Įžanga: bendrinis programavimas

C++ dažnai pristatoma kaip objektinio programavimo kalba. Bet tai tik dalis tiesos.

C++ palaiko kelias programavimo paradigmas:

- **Procedūrinę** — funkcijos, C stilius
- **Objektinę** (_OOP_) — klasės, paveldimumas, polimorfizmas
- **Bendrinio programavimo** (_generic programming_) — **šablonai**

**Bendrinis programavimas** (_generic programming_) — paradigma, kurioje algoritmai ir duomenų struktūros aprašomos nepriklausomai nuo konkretaus tipo. Tipas tampa parametru, kurį nurodo naudotojas.

C++ šablonai (_templates_) — pagrindinis šios paradigmos įrankis. Standartinė biblioteka (STL) yra bendrinio programavimo rezultatas: `vector<T>`, `sort<T>`, `map<K,V>` — visa tai šablonai.

---

### Ta pati problema, kitas kampas

Kurso pradžioje matėme `swap_cpp` — elegantišką C++ sprendimą:

```cpp linenums="1"
void swap_cpp(int& x, int& y) {
    int temp = x;
    x = y;
    y = temp;
}
```

Veikia su `int`. Bet kas, jei reikia sukeisti `double`? `std::string`? `Point`?

```cpp linenums="1"
void swapInt(int& x, int& y)         { int temp = x; x = y; y = temp; }
void swapDouble(double& x, double& y){ double temp = x; x = y; y = temp; }
void swapString(std::string& x, std::string& y){ std::string temp = x; x = y; y = temp; }
```

Logika **identiška** — tik tipas skiriasi. Tris kartus parašėme tą patį kodą.

Tai klasikinė **kodo dubliavimo** problema. Ir tai tiksliai ta problema, kurią sprendžia **šablonai** (_templates_).

---

## 1 DALIS: Funkcijų šablonai

!!! abstract "Šios dalies tikslas"
    `template<typename T>` sintaksė. Tipo išvedimas — kompiliatorius pats supranta tipą.
    Keli praktiniai pavyzdžiai.

---

### `swap<T>` — pirmasis šablonas

```cpp linenums="1"
template<typename T>
void swap(T& x, T& y) {
    T temp = x;
    x = y;
    y = temp;
}
```

`template<typename T>` — tai **šablono deklaracija**. `T` yra **tipo parametras** — rezervuota vieta tipui, kurią kompiliatorius užpildys.

!!! note "`template<typename T>` — už įprastos sintaksės ribų"
    Prisiminkite right-left "shuttle" algoritmą deklaracijų skaitymui — `int* p`, `const T&`, `void(*f)(int)`. Šablono deklaracija `template<typename T>` į šią harmoniją neįsipaišo — tai **antstatas** virš įprastos C++ deklaracijos sintaksės. Geriausia ją skaityti atskirai: "tai šablonas su tipo parametru T", o toliau — įprasta funkcijos ar klasės deklaracija.

Naudojimas:

```cpp linenums="1"
int a = 1, b = 2;
swap(a, b);          // T = int  — kompiliatorius supranta pats
// a=2, b=1

double x = 1.5, y = 2.5;
swap(x, y);          // T = double
// x=2.5, y=1.5

std::string s1 = "hello", s2 = "world";
swap(s1, s2);        // T = std::string
// s1="world", s2="hello"
```

Viena funkcija — visi tipai. Kompiliatorius **generuoja** reikiamas versijas kompiliavimo metu.

---

### Kaip tai veikia "po kapotu"

Kompiliatorius pamato `swap(a, b)` kur `a` ir `b` yra `int`. Jis **sugeneruoja** konkrečią funkciją:

```cpp linenums="1"
// Tai kompiliatorius sugeneruoja automatiškai:
void swap_generated_int(int& x, int& y) {
    int temp = x;
    x = y;
    y = temp;
}
```

Tai vadinamas **šablono instanciavimas** (_template instantiation_). Kiekvienam naudojamam tipui — atskira versija. Bet rašome tik vieną kartą.

!!! note "Kompiliavimo metas, ne vykdymo metas"
    Šablonai — **kompiliavimo meto** mechanizmas. Tai skiriasi nuo polimorfizmo (`virtual`), kuris veikia vykdymo metu. Šablonų kaina vykdymo metu — nulinė. Kompiliatorius viską išsprendžia iš anksto.

---

### Tipo išvedimas (_type deduction_)

Daugeliu atvejų nereikia nurodyti tipo — kompiliatorius jį **išveda** iš argumentų:

```cpp linenums="1"
swap(a, b);          // ← kompiliatorius: "a ir b yra int, vadinasi T = int"
swap<int>(a, b);     // ← galima ir taip — tiesiogiai, bet perteklinė
swap<double>(a, b);  // ← klaida: a ir b yra int, ne double
```

Tiesioginis nurodymas reikalingas tik kai kompiliatorius negali išvesti pats — tai retesni atvejai.

---

### `min<T>` ir `max<T>`

```cpp linenums="1"
template<typename T>
T min(const T& a, const T& b) {
    return (a < b) ? a : b;
}

template<typename T>
T max(const T& a, const T& b) {
    return (a > b) ? a : b;
}
```

```cpp linenums="1"
std::cout << min(3, 7);           // 3    (T = int)
std::cout << min(3.14, 2.71);     // 2.71 (T = double)
std::cout << max('a', 'z');       // z    (T = char)
```

!!! note "STL turi `std::swap`, `std::min`, `std::max`"
    `#include <algorithm>` — ir visi šie šablonai jau prieinami. Mes juos parašėme patys, kad suprastume mechanizmą. U8/1 žingsnyje — taip pat.

---

### Keli tipo parametrai

Šablonas gali turėti skirtingų tipų parametrus:

```cpp linenums="1"
template<typename T, typename U>
void printPair(const T& first, const U& second) {
    std::cout << first << " ir " << second << "\n";
}

printPair(42, 3.14);          // T=int, U=double
printPair("hello", 'x');      // T=const char*, U=char
```

Tai retesnis atvejis — dažniausiai vieno `T` pakanka.

---

## 2 DALIS: Klasių šablonai

!!! abstract "Šios dalies tikslas"
    `template<typename T>` prie klasės. `Pair<T>` kaip paprasčiausias pavyzdys.
    Sintaksės skirtumai nuo funkcijų šablonų.

---

### Problema su klasėmis

Funkcijų šablonai sprendžia dubliavimą funkcijoms. Bet tas pats taikytina ir klasėms:

```cpp linenums="1"
class PairInt {
    int first, second;
public:
    PairInt(int f, int s) : first(f), second(s) {}
    int getFirst() const { return first; }
    int getSecond() const { return second; }
};

class PairDouble {
    double first, second;
    // ... tas pats kodas, kitas tipas
};
```

Sprendimas — klasės šablonas.

---

### `Pair<T>`

```cpp linenums="1"
template<typename T>
class Pair {
    T first;
    T second;
public:
    Pair(const T& f, const T& s) : first(f), second(s) {}

    const T& getFirst()  const { return first; }
    const T& getSecond() const { return second; }

    void print() const {
        std::cout << "(" << first << ", " << second << ")\n";
    }
};
```

Naudojimas:

```cpp linenums="1"
Pair<int> p1(3, 7);
p1.print();                    // (3, 7)

Pair<double> p2(1.5, 2.5);
p2.print();                    // (1.5, 2.5)

Pair<std::string> p3("hello", "world");
p3.print();                    // (hello, world)
```

!!! note "Klasių šablonams tipas — privalomas"
    `Pair<int> p1(3, 7)` — tipas nurodomas tiesiogiai (_eksplicitiškai_). Kompiliatorius **negali** išvesti tipo iš konstruktoriaus argumentų (iki C++17). `Pair p1(3, 7)` — neleistina (iki C++17).

---

### Metodo aprašas už klasės ribų

Jei metodą rašome už klasės ribų (`.cpp` faile) — šablono deklaracija kartojama:

```cpp linenums="1"
template<typename T>
void Pair<T>::print() const {
    std::cout << "(" << first << ", " << second << ")\n";
}
```

Dėl to klasių šablonai paprastai rašomi **tik `.h` faile** — kompiliatorius turi matyti visą šablono kodą kur jis naudojamas.

!!! note "Kodėl ne `.cpp`?"
    Šablonas nėra kodas — tai **instrukcija** kompiliatoriui kaip generuoti kodą. Kompiliatorius generuoja konkrečią versiją kur `Pair<int>` naudojamas — tam reikia matyti visą šablono tekstą. Jei šablonas `.cpp` faile — kitas `.cpp` failo kompiliavimo vienetas jo nemato. Tai dažna klaida pradedant dirbti su šablonais.

!!! note "`.h` arba `.hpp`?"
    Klasių šablonai praktikoje dažnai talpinami `.hpp` failuose (_header plus plus_) — kad vizualiai atskirtų nuo C stiliaus `.h`. Reikšmė identiška, tik konvencija. Abu variantai priimtini — svarbu nuoseklumas projekte.

---

## 3 DALIS: STL ryšys

!!! abstract "Šios dalies tikslas"
    `vector<T>`, `stack<T>`, `pair<T>` — tai šablonai. Visą laiką naudojome šablonus.

---

### Atpažįstami vardai

```cpp linenums="1"
std::vector<int>         v1;   // vector<T>, T = int
std::vector<std::string> v2;   // vector<T>, T = std::string
std::vector<Shape*>      v3;   // vector<T>, T = Shape*
std::vector<std::unique_ptr<Shape>> v4;  // vector<T>, T = unique_ptr<Shape>

std::pair<int, std::string> p(42, "hello");   // pair<T,U>
std::cout << p.first << " " << p.second;      // 42 hello

std::stack<int> s;   // stack<T>
s.push(1);
s.push(2);
```

Nuo pat pirmųjų paskaitų ir pratybų naudojame `vector<T>`. Nuo P10 — `vector<unique_ptr<Shape>>`. Visa tai — klasių šablonai, tokios pačios struktūros kaip `Pair<T>` kurį ką tik parašėme.

---

### `vector<T>` — supaprastinta vidinė logika

`vector<T>` viduje — dinaminis masyvas su šablonu:

```cpp linenums="1"
template<typename T>
class vector {
    T* data;          // dinaminis T tipo masyvas
    size_t size;
    size_t capacity;
public:
    void push_back(const T& value) { /* ... */ }
    T& operator[](size_t i)        { return data[i]; }
    // ...
    ~vector() { delete[] data; }   // ← RAII
};
```

Pastebėkime, kad `MyString` kurso pradžioje sukūrėme su logika panašia į  `vector<char>` — dinaminis `char` masyvas su rankine atminties valdymu (RAII). `vector<T>` implementuoja tą patį principą, tai — generalizuotas šablonu.

---

## 4 DALIS: `Stack<T>` — kurso ciklo užbaigimas

### Nuo C iki C++

Kurso pradžioje turėjome C `Stack` — dinaminis `char` masyvas su "rankine" atminties valdymu:

```c linenums="1"
// C laikais:
struct Stack { char* data; int top; int capacity; };
void push(Stack* s, char c) { /* ... */ }
char pop(Stack* s)          { /* ... */ }
void destroy(Stack* s)      { free(s->data); }
```

Tada — C++ `Stack` su RAII:

```cpp linenums="1"
// C++ su RAII:
class Stack {
    char* data;
    int top, capacity;
public:
    Stack(int cap) : data(new char[cap]), top(0), capacity(cap) {}
    ~Stack()       { delete[] data; }
    void push(char c) { data[top++] = c; }
    char pop()        { return data[--top]; }
};
```

Bet `Stack` dirba tik su `char`. `StackInt`, `StackDouble` — vėl dubliavimas.

### `Stack<T>`

```cpp linenums="1"
template<typename T>
class Stack {
    T* data;
    int top;
    int capacity;
public:
    Stack(int cap = 16)
        : data(new T[cap]), top(0), capacity(cap) {}

    ~Stack() { delete[] data; }

    // Kopijos draudžiamos (Rule of Three — arba unique_ptr viduje)
    Stack(const Stack&)            = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const T& value) {
        if (top < capacity) data[top++] = value;
    }

    T pop() {
        return data[--top];
    }

    bool isEmpty() const { return top == 0; }
    int  size()    const { return top; }
};
```

Naudojimas:

```cpp linenums="1"
Stack<int> si(10);
si.push(1); si.push(2); si.push(3);
std::cout << si.pop();   // 3
std::cout << si.pop();   // 2

Stack<std::string> ss(10);
ss.push("hello"); ss.push("world");
std::cout << ss.pop();   // world

Stack<double> sd(10);
sd.push(3.14); sd.push(2.71);
```

### Lankas užbaigtas

```
C: struct Stack + rankinis valdymas
    ↓
C++: class Stack + RAII (destruktorius)
    ↓
C++: Stack<T> + šablonas
    ↓
STL: std::stack<T> — tai ką naudosime toliau
```

`std::stack<T>` yra lygiai toks pats principas — tik su pilnu STL funkcionalumu. Tai, ką parašėme, yra supaprastinta jo versija.


---

## Apibendrinimas

| | Funkcijų šablonas | Klasės šablonas |
|---|---|---|
| **Sintaksė** | `template<typename T> void f(T x)` | `template<typename T> class C { ... }` |
| **Tipo nurodymas** | Automatinis (išvedamas) | Tiesioginis: `C<int>` |
| **Kodo vieta** | `.h` arba `.cpp` | Tik `.h` |
| **STL pavyzdys** | `std::swap`, `std::min` | `std::vector`, `std::stack` |

Šablonai — **kompiliavimo meto polimorfizmas**. `virtual` — vykdymo meto. Abu sprendžia "vienas kodas, daug tipų" problemą, bet skirtingais būdais ir skirtingose situacijose.

---

!!! tip "Užduotis U8"
    Šablonų mechaniką išbandysite **U8** užduotyje:
    `swap<T>` → `min<T>`/`max<T>` → `Pair<T>` — nuo funkcijų iki klasių.

---

*[STL]: Standard Template Library
