# 4️⃣ Paskaita — Geresnis C

!!! abstract "Šios paskaitos tikslas"
    C++ nėra tik „C su klasėmis" — jis įveda **švaresnę sintaksę** ir **saugesnes priemones**
    net ir be OOP. Ši paskaita — tiltas tarp C ir klasių pasaulio.

    - **Nuorodos** (`&`) — pseudonimai be rodyklių keblumų
    - **`const` taisyklingumas** — kompiliatoriaus garantuota apsauga
    - **Modernios sintaksės priemonės** — `auto`, range-loops, `nullptr`, `{}`
    - **Grąžinimas per nuorodą** — galimybė ir pavojus

    > Šios priemonės naudojamos **kiekvienoje** tolesnėje paskaitoje —
    > verta suprasti, ne tik įsiminti sintaksę.

---

## 1️⃣ Parametrų perdavimas — evoliucija

### Trys mechanizmai

C++ paveldėjo du C mechanizmus ir pridėjo trečią — tikrąjį perdavimą pagal nuorodą.

=== "1. By Value (kopija)"

    ```cpp linenums="1"
    void swap_val(int x, int y) {  // gauna KOPIJAS
        int temp = x;
        x = y;
        y = temp;
    }  // kopijos sunaikinamos — originalai nepakito

    int main() {
        int a = 1, b = 2;
        swap_val(a, b);
        // a = 1, b = 2  ← nepakito!
    }
    ```

    [👉 Python Tutor — pamatykite kaip kopijos gyvena atskirame stack frame](https://pythontutor.com/visualize.html#code=%23include%20%3Cstdio.h%3E%0Avoid%20swap_val%28int%20x,%20int%20y%29%20%7B%20//%20Gauna%20KOPIJAS%20%28x%3D1,%20y%3D2%29%0A%20%20%20%20int%20temp%20%3D%20x%3B%0A%20%20%20%20x%20%3D%20y%3B%0A%20%20%20%20y%20%3D%20temp%3B%0A%7D%0Aint%20main%28%29%20%7B%0A%20%20int%20a%3D1,%20b%3D2%3B%0A%20%20swap_val%28a,b%29%3B%0A%20%20/*%20a%3D1,b%3D2%20*/%0A%7D&cumulative=false&curInstr=9&heapPrimitives=nevernest&mode=display&origin=opt-frontend.js&py=c_gcc9.3.0&rawInputLstJSON=%5B%5D&textReferences=false)

=== "2. Pointer (C stilius)"

    ```cpp linenums="1"
    void swap_ptr(int* x, int* y) {  // gauna ADRESUS
        int temp = *x;   // išadresavimas: paimk reikšmę
        *x = *y;
        *y = temp;
    }

    int main() {
        int a = 1, b = 2;
        swap_ptr(&a, &b);  // ← siunčiame adresus
        // a = 2, b = 1  ✓
    }
    ```

    [👉 Python Tutor — pamatykite rodykles ir adresus](https://pythontutor.com/visualize.html#code=%23include%20%3Cstdio.h%3E%0Avoid%20swap_ptr%28int*%20x,%20int*%20y%29%20%7B%20//%20Gauna%20ADRESUS%0A%20%20%20%20int%20temp%20%3D%20*x%3B%0A%20%20%20%20*x%20%3D%20*y%3B%0A%20%20%20%20*y%20%3D%20temp%3B%0A%7D%0A%0Aint%20main%28%29%20%7B%0A%20%20int%20a%3D1,%20b%3D2%3B%0A%20%20swap_ptr%28%26a,%20%26b%29%3B%0A%20%20/*%20a%3D2,b%3D1%20*/%0A%7D&cumulative=false&curInstr=9&heapPrimitives=nevernest&mode=display&origin=opt-frontend.js&py=c_gcc9.3.0&rawInputLstJSON=%5B%5D&textReferences=false)

=== "3. Reference (C++ stilius)"

    ```cpp linenums="1"
    void swap_cpp(int& x, int& y) {  // x yra 'a' pseudonimas
        int temp = x;
        x = y;    // keičia patį originalą
        y = temp;
    }

    int main() {
        int a = 1, b = 2;
        swap_cpp(a, b);  // ← atrodo kaip by value, veikia kaip pointer
        // a = 2, b = 1  ✓
    }
    ```

    [👉 Python Tutor — pamatykite kad nuoroda nėra atskiras kintamasis](https://pythontutor.com/visualize.html#code=void%20swap_cpp%28int%26%20x,%20int%26%20y%29%20%7B%0Aint%20temp%20%3D%20x%3B%0A%20%20%20%20x%20%3D%20y%3B%0A%20%20%20%20y%20%3D%20temp%3B%0A%7D%0A%0Aint%20main%28%29%20%7B%0A%20%20int%20a%3D1,%20b%3D2%3B%0A%20%20swap_cpp%28a,%20b%29%3B%0A%7D&cumulative=false&curInstr=10&heapPrimitives=nevernest&mode=display&origin=opt-frontend.js&py=cpp_g%2B%2B9.3.0&rawInputLstJSON=%5B%5D&textReferences=false)

### Palyginimas

| Savybė | By Value | Pointer (C) | Reference (C++) |
|---|---|---|---|
| **Deklaracija** | `void f(int x)` | `void f(int* x)` | `void f(int& x)` |
| **Kvietimas** | `f(a)` | `f(&a)` | `f(a)` |
| **Veiksmas** | Dirba su kopija | Dirba su adresu | Dirba su originalu |
| **Gali būti `null`?** | — | ✅ pavojinga | ❌ ne |
| **Verdiktas** | Saugus, lokalus | Galingas, pavojingas | **Modernus standartas** |

---

## 2️⃣ Nuorodos — kas tai iš tikrųjų

### Pseudonimas, ne rodyklė

Nuoroda (`&`) — tai **kitas vardas** tam pačiam kintamajam.
Ne kopija. Ne rodyklė. Tas pats atminties objektas, kitas pavadinimas.

```cpp linenums="1"
int a = 42;
int& r = a;    // r yra 'a' pseudonimas

r = 100;       // keičiame per r...
std::cout << a;  // ...bet matosi per a: 100

std::cout << (&a == &r);  // true — tas pats adresas!
```

### Privalomasis inicijavimas

Nuoroda **privalo** būti inicializuota iš karto — vėliau jos pakeisti negalima:

```cpp linenums="1"
int& r;        // ❌ NC: nuoroda be inicijavimo

int a = 1, b = 2;
int& r = a;    // ✅
r = b;         // tai ne „r dabar rodo į b" — tai „a = b"!
               // r vis dar yra 'a' pseudonimas
```

!!! note "Nuoroda vs rodyklė"
    | | Rodyklė `int*` | Nuoroda `int&` |
    |---|---|---|
    | Gali būti `null` | ✅ | ❌ |
    | Privalo būti inicializuota | ❌ | ✅ |
    | Galima pakeisti kur rodo | ✅ | ❌ |
    | Sintaksė naudojant | `*p`, `p->` | tiesiog `r` |

### Kodėl tai svarbu klasėms?

Nuorodos yra **būtinos** klasių konstruktoriuose — tai pamatysime P6–P8:

```cpp
// Inicializavimo sąraše nuorodos ir const laukai
// PRIVALO būti inicializuoti — vėliau nebegalima:
class Foo {
    const int id;      // const — tik inicializavimo sąraše
    int& ref;          // nuoroda — tik inicializavimo sąraše
public:
    Foo(int i, int& r) : id(i), ref(r) {}
    //                   ^^^^^^^^^^^^^ būtina!
};
```

---

## 3️⃣ `const` taisyklingumas parametruose

### Problema be `const`

```cpp linenums="1"
// Norime tik spausdinti — bet niekas netrukdo atsitiktinai pakeisti
void spausdinti(Studentas& s) {
    s.vardas = "SUGADINTA";  // ← kompiliatorius leidžia!
    std::cout << s.vardas;
}
```

### `const T&` — efektyvu ir saugu

```cpp linenums="1"
void spausdinti(const Studentas& s) {
    //           ^^^^^
    s.vardas = "SUGADINTA";  // ❌ NC — kompiliatorius draudžia
    std::cout << s.vardas;   // ✅ skaityti galima
}
```

`const T&` — **universalus C++ sprendimas** perduodant objektus į funkcijas:

- **`&`** — nekopijuojama (efektyvu dideliems objektams)
- **`const`** — garantija kad originalas nepakeis

```cpp linenums="1"
// Kada ką naudoti:
void f1(int x)              // ✅ primityvai — by value
void f2(const Studentas& s) // ✅ objektai — const ref (skaityti)
void f3(Studentas& s)       // ✅ objektai — ref (keisti)
void f4(Studentas s)        // ⚠️ objektai — kopija (brangu!)
```

### `const` metoduose

`const` po skliausto reiškia kad metodas **nekeičia** objekto:

```cpp linenums="1"
class Studentas {
    std::string vardas;
    int amzius;
public:
    // const metodai — tik skaityti
    std::string getVardas() const { return vardas; }
    void spausdinti()       const { std::cout << vardas; }

    // ne-const metodai — gali keisti
    void setVardas(const std::string& v) { vardas = v; }
};
```

!!! tip "Taisyklė"
    Jei metodas **nekeičia** objekto — rašykite `const` po skliausto.
    Kompiliatorius tai patikrina — tai dokumentacija ir apsauga vienu metu.

---

## 4️⃣ Grąžinimas per nuorodą

### Galimybė — modifikuoti per getter

```cpp linenums="1"
class Masyvas {
    int data[10]{};
public:
    // Grąžiname nuorodą — galima keisti elementą tiesiogiai
    int& operator[](int i) { return data[i]; }
};

int main() {
    Masyvas m;
    m[3] = 42;   // veikia nes operator[] grąžina nuorodą
}
```

### ⚠️ Pavojus — lokalios nuorodos

```cpp linenums="1" hl_lines="3 4"
int& pavojinga() {
    int x = 42;
    return x;    // ← KLAIDA: grąžiname nuorodą į lokalų kintamąjį!
}                // x sunaikinamas čia — nuoroda rodo į negaliojančią atmintį

int main() {
    int& r = pavojinga();
    std::cout << r;   // undefined behavior — gali spausdinti šiukšles
}                     // arba crash
```

!!! danger "Lokalios nuorodos taisyklė"
    **Niekada** negrąžinkite nuorodos į lokalų kintamąjį.
    Lokalus kintamasis sunaikinamas funkcijos pabaigoje —
    nuoroda tampa „kabančia" (_dangling reference_).

    Kompilatoriaus įspėjimas: `-Wall` su `g++` įspės apie tai.

=== "❌ Bloga"

    ```cpp
    int& bloga() {
        int x = 5;
        return x;   // x mirs — nuoroda kabos
    }
    ```

=== "✅ Gerai — static"

    ```cpp
    int& gerai_static() {
        static int x = 5;  // static — gyvena visą programą
        return x;
    }
    ```

=== "✅ Gerai — klasės narys"

    ```cpp
    class Foo {
        int x = 5;
    public:
        int& getX() { return x; }  // narys gyvena kartu su objektu
    };
    ```

---

## 5️⃣ Modernios sintaksės priemonės

### `auto` — tipo išvedimas

Kompiliatorius pats nusprendžia tipą iš dešinės pusės:

```cpp linenums="1"
auto x = 42;           // int
auto y = 3.14;         // double
auto s = std::string("Labas");  // std::string

// Ypač naudinga su ilgais tipais:
std::vector<Studentas> studentai;

// Sena:
std::vector<Studentas>::iterator it = studentai.begin();

// Su auto:
auto it = studentai.begin();   // ← tas pats, bet skaitomiau
```

!!! tip "Kada naudoti `auto`?"
    - ✅ Kai tipas akivaizdus iš dešinės (`auto x = 42`)
    - ✅ Kai tipas ilgas ir sudėtingas (iteratoriai)
    - ✅ Range-based for cikluose
    - ⚠️ Vengti kai tipas svarbus skaitomumui (`auto x = f()` — kas grąžina `f`?)

### Range-based `for` — ciklas su diapazonu

```cpp linenums="1"
std::vector<int> skaiciai = {1, 2, 3, 4, 5};

// Sena (C stilius):
for (int i = 0; i < skaiciai.size(); ++i)
    std::cout << skaiciai[i] << " ";

// Su iteratoriumi:
for (auto it = skaiciai.begin(); it != skaiciai.end(); ++it)
    std::cout << *it << " ";

// Range-based (modernus):
for (auto x : skaiciai)          // kopija — saugu, bet gali būti brangu
    std::cout << x << " ";

for (const auto& x : skaiciai)   // const nuoroda — efektyvu ir saugu
    std::cout << x << " ";

for (auto& x : skaiciai)         // nuoroda — galima keisti
    x *= 2;
```

!!! tip "Praktinė taisyklė"
    - Primityvai (`int`, `double`): `for (auto x : ...)` — kopija OK
    - Objektai (`Studentas`, `std::string`): `for (const auto& x : ...)` — nuoroda

### `nullptr` — vietoj `NULL`

```cpp linenums="1"
// C stilius:
int* p = NULL;    // NULL = 0 — sukelia dviprasmybes

// C++ stilius:
int* p = nullptr; // tiksliai rodo „tuščia rodyklė"

// Kodėl geriau:
void f(int x)  {}
void f(int* p) {}

f(NULL);     // ⚠️ kuri versija? NULL = 0 = int!
f(nullptr);  // ✅ aiškiai rodyklės versija
```

### Vieninga inicializacija `{}`

```cpp linenums="1"
// Sena C++ (daug variantų):
int a = 5;
int b(5);
int arr[3] = {1, 2, 3};

// Moderni — vienas sintaksės variantas visiems atvejams:
int a{5};
int arr[3]{1, 2, 3};
std::vector<int> v{1, 2, 3};
Studentas s{"Jonas", 20};    // konstruktorius

// Papildomas privalumas — draudžia siaurinančias konversijas:
int x = 3.14;   // ⚠️ kompiliuojasi (prarandame .14)
int x{3.14};    // ❌ NC — kompiliatorius draudžia
```

---

## 🔗 Ryšys su tolesnėmis paskaitomis

```
Šioje paskaitoje:          Kur naudojama toliau:

const T&  ──────────────►  P6-P8: copy constructor parametras
                            P9:    kompozicija (prideti metodas)

int& grąžinimas ─────────►  P8:    operator[] su IntArray

auto + range-for ────────►  P5:    std::vector iteravimas
                            P9:    Kursas::spausdinti()

nullptr ─────────────────►  P6:    pimpl (impl = nullptr sauga)

{} inicializacija ───────►  P3:    konstruktoriaus argumentai
                            P6:    new int[n]{} — nuliai
```

---

*[NC]: Not Compiling — Nesikompiliuoja
*[UB]: Undefined Behavior — Neapibrėžtas elgesys
