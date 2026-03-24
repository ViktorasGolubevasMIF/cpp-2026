# 5️⃣ Paskaita — STL įvadas: konteineriai

!!! abstract "Šios paskaitos tikslas"
    Iki šiol masyvus valdėme C stiliumi: `int arr[10]`, `new int[n]`, rankiniai indeksai.
    C++ standartinė biblioteka (STL) suteikia **saugesnius, patogesnius** konteinerius.

    - **`std::array`** — fiksuoto dydžio masyvas su C++ privalumais
    - **`std::vector`** — dinaminis masyvas, kurį jau matėme P9 (Kompozicija)
    - **Iteratoriai** — universali prieiga prie bet kurio konteinerio
    - **Range-based `for`** — elegantiškas iteravimas (P4 ryšys)
    - **Algoritmai** — `std::sort`, `std::find` ir draugai

    > Objektas: `Studentas` iš P3 — dabar dedame jį į konteinerius.

---

## 1️⃣ Problema su C masyvais

```cpp linenums="1"
// C stilius — pilnas keblumų:
int n = 5;
Studentas arr[n];          // ⚠️ VLA — nestandartinis C++

Studentas* arr2 = new Studentas[n];  // reikia delete[]
// ...
delete[] arr2;             // ← lengva pamiršti

// Dydis prarandamas:
void f(Studentas* arr) {
    // kiek elementų? nežinom!
    // sizeof(arr) = rodyklės dydis, ne masyvo
}
```

STL konteineriai sprendžia visas šias problemas.

---

## 2️⃣ `std::array` — fiksuoto dydžio

`std::array<T, N>` — C masyvo pakaitalas kai dydis žinomas kompiliavimo metu.

```cpp linenums="1"
#include <array>

std::array<int, 5> skaiciai = {10, 20, 30, 40, 50};

// Dydis visada žinomas:
std::cout << skaiciai.size() << "\n";   // 5

// Ribų tikrinimas:
skaiciai.at(2) = 99;    // ✅ tikrina ribas, meta std::out_of_range
skaiciai[2]    = 99;    // ✅ greičiau, bet be tikrinimo (kaip C)

// Range-based for:
for (const auto& x : skaiciai)
    std::cout << x << " ";
```

### Su `Studentas` objektais

```cpp linenums="1"
#include <array>
#include "Studentas.h"

int main() {
    std::array<Studentas, 3> grupe = {
        Studentas("Jonas",  20, 8.5),
        Studentas("Marta",  19, 9.0),
        Studentas("Petras", 21, 7.5)
    };

    std::cout << "Grupė (" << grupe.size() << " studentai):\n";
    for (const auto& s : grupe)
        s.spausdinti();
}
```

=== "⌨️➡️🖥️"

    ```
    [CTOR] 'Jonas' id=1 (gyvi: 1)
    [CTOR] 'Marta' id=2 (gyvi: 2)
    [CTOR] 'Petras' id=3 (gyvi: 3)
    Grupė (3 studentai):
    [1] Jonas, 20 m., pažymys: 8.5 (pilnametis)
    [2] Marta, 19 m., pažymys: 9 (pilnametis)
    [3] Petras, 21 m., pažymys: 7.5 (pilnametis)
    [DTOR] 'Petras' (gyvi: 2)
    [DTOR] 'Marta' (gyvi: 1)
    [DTOR] 'Jonas' (gyvi: 0)
    ```

??? tip "`std::array` vs C masyvas"
    | | `int arr[5]` | `std::array<int,5>` |
    |---|---|---|
    | Dydis žinomas | ❌ (prarandamas) | ✅ `.size()` |
    | Ribų tikrinimas | ❌ | ✅ `.at()` |
    | Perdavimas į funkciją | Rodyklė | Pilnas objektas |
    | `delete` reikia | ❌ | ❌ |
    | Overhead | Nėra | Nėra |

---

## 3️⃣ `std::vector` — dinaminis masyvas

`std::vector<T>` — dinamiškai augantis masyvas.
Dydis nežinomas kompiliavimo metu — keičiasi vykdymo metu.

### Pagrindinis naudojimas

```cpp linenums="1"
#include <vector>

std::vector<int> v;           // tuščias
v.push_back(10);              // pridėti gale
v.push_back(20);
v.push_back(30);

std::cout << v.size() << "\n";   // 3
std::cout << v[1]     << "\n";   // 20

v.pop_back();                 // pašalinti paskutinį
std::cout << v.size() << "\n";   // 2
```

### Su `Studentas` — dinaminis grupės valdymas

```cpp linenums="1"
#include <vector>
#include "Studentas.h"

int main() {
    std::vector<Studentas> grupe;

    // Pridedame dinamiškai:
    grupe.push_back(Studentas("Jonas",  20, 8.5));
    grupe.push_back(Studentas("Marta",  19, 9.0));
    grupe.push_back(Studentas("Petras", 21, 7.5));

    std::cout << "Grupė (" << grupe.size() << "):\n";
    for (const auto& s : grupe)
        s.spausdinti();

    // Pašalinamas paskutinis:
    grupe.pop_back();
    std::cout << "\nPo pop_back (" << grupe.size() << "):\n";
    for (const auto& s : grupe)
        s.spausdinti();
}
```

=== "⌨️➡️🖥️"

    ```
    [CTOR] 'Jonas' id=1 (gyvi: 1)
    [CTOR] 'Marta' id=2 (gyvi: 2)
    [CTOR] 'Petras' id=3 (gyvi: 3)
    Grupė (3):
    [1] Jonas, 20 m., pažymys: 8.5 (pilnametis)
    [2] Marta, 19 m., pažymys: 9 (pilnametis)
    [3] Petras, 21 m., pažymys: 7.5 (pilnametis)

    Po pop_back (2):
    [1] Jonas, 20 m., pažymys: 8.5 (pilnametis)
    [2] Marta, 19 m., pažymys: 9 (pilnametis)
    [DTOR] 'Petras' (gyvi: 2)
    [DTOR] 'Marta' (gyvi: 1)
    [DTOR] 'Jonas' (gyvi: 0)
    ```

### Dažniausiai naudojami metodai

| Metodas | Aprašas |
|---|---|
| `push_back(x)` | Pridėti elementą gale |
| `pop_back()` | Pašalinti paskutinį |
| `size()` | Elementų skaičius |
| `empty()` | Ar tuščias? |
| `at(i)` | Elementas su ribų tikrinimu |
| `[i]` | Elementas be ribų tikrinimo |
| `front()` | Pirmas elementas |
| `back()` | Paskutinis elementas |
| `clear()` | Išvalyti viską |
| `reserve(n)` | Rezervuoti atmintį iš anksto |

---

## 4️⃣ Iteratoriai

Iteratorius — objektas kuris „rodo" į konteinerio elementą.
Universali sąsaja: tas pats kodas veikia su `vector`, `array`, `list`...

```cpp linenums="1"
std::vector<int> v = {10, 20, 30, 40, 50};

// begin() — rodo į pirmą elementą
// end()   — rodo UŽ paskutinio (sentinel)

for (auto it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << " ";   // * — išadresavimas kaip rodyklė
}
```

### Iteratoriaus tipai

```cpp linenums="1"
std::vector<Studentas> grupe = { /*...*/ };

// Įprastas iteratorius — galima keisti:
for (auto it = grupe.begin(); it != grupe.end(); ++it) {
    it->setPazymys(10.0);    // -> metodai per iteratorių
}

// Const iteratorius — tik skaityti:
for (auto it = grupe.cbegin(); it != grupe.cend(); ++it) {
    it->spausdinti();
    // it->setPazymys(10.0);  ❌ NC — const iteratorius
}
```

### Range-based `for` — iteratoriaus „cukrus"

```cpp linenums="1"
// Šie du ciklai yra ekvivalentūs:

// Iteratoriumi:
for (auto it = grupe.begin(); it != grupe.end(); ++it)
    it->spausdinti();

// Range-based (kompiliatorius išskleidžia į viršų):
for (const auto& s : grupe)
    s.spausdinti();
```

!!! tip "Praktikoje"
    Range-based `for` — **beveik visada** geresnis pasirinkimas.
    Tiesioginį iteratorių naudokite kai reikia pozicijos
    (pvz. `erase`, `insert`) arba iteruoti atgal.

---

## 5️⃣ Algoritmai

`#include <algorithm>` — daugybė paruoštų algoritmų.

### `std::sort`

```cpp linenums="1"
#include <algorithm>
#include <vector>

std::vector<int> v = {5, 2, 8, 1, 9, 3};
std::sort(v.begin(), v.end());   // didėjančia tvarka
// v = {1, 2, 3, 5, 8, 9}

// Mažėjančia:
std::sort(v.begin(), v.end(), std::greater<int>());
```

### Rikiavimas su `Studentas` — lambda

```cpp linenums="1"
std::vector<Studentas> grupe = {
    Studentas("Jonas",  20, 8.5),
    Studentas("Marta",  19, 9.0),
    Studentas("Petras", 21, 7.5)
};

// Rikiuojame pagal pažymį (mažėjančia):
std::sort(grupe.begin(), grupe.end(),
    [](const Studentas& a, const Studentas& b) {
        return a.getPazymys() > b.getPazymys();
    }
);

for (const auto& s : grupe)
    s.spausdinti();
```

=== "⌨️➡️🖥️"

    ```
    [2] Marta, 19 m., pažymys: 9 (pilnametis)
    [1] Jonas, 20 m., pažymys: 8.5 (pilnametis)
    [3] Petras, 21 m., pažymys: 7.5 (pilnametis)
    ```

### `std::find_if`

```cpp linenums="1"
// Ieškome pirmo pilnamečio studento:
auto it = std::find_if(grupe.begin(), grupe.end(),
    [](const Studentas& s) { return s.arPilnametis(); }
);

if (it != grupe.end())
    it->spausdinti();
else
    std::cout << "Nerasta\n";
```

??? note "Lambda išraiška — trumpai"
    ```cpp
    [](const Studentas& s) { return s.arPilnametis(); }
    //^   ^^^^^^^^^^^^^^       ^^^^^^^^^^^^^^^^^^^^^^^^
    //užfiksavimas  parametras        kūnas
    ```
    Lambda = anoniminė funkcija vietoje. Išsamiau — P15 (šablonai ir algoritmai).

---

## 6️⃣ `std::array` vs `std::vector` — kada ką?

| | `std::array<T,N>` | `std::vector<T>` |
|---|---|---|
| **Dydis** | Fiksuotas (kompiliavimo metu) | Dinaminis (vykdymo metu) |
| **Atmintis** | Stack | Heap |
| **`push_back`** | ❌ | ✅ |
| **Greitis** | Šiek tiek greitesnis | Labai greitas |
| **Naudoti kai** | Dydis žinomas ir nekinta | Dydis nežinomas arba kinta |
| **Pavyzdys** | Savaitės dienos (7) | Studentų grupė |

```cpp linenums="1"
// ✅ array — dydis visada 7:
std::array<std::string, 7> dienos = {
    "Pirmadienis", "Antradienis", /*...*/ "Sekmadienis"
};

// ✅ vector — nežinome kiek studentų:
std::vector<Studentas> grupe;
grupe.push_back(Studentas("Jonas", 20, 8.5));
// ...kiek pridėsime — nežinom iš anksto
```

---

## 7️⃣ `vector` ir copy constructor — svarbi detalė

!!! warning "Reikalavimas elementų tipui"
    `std::vector` viduje kopijuoja elementus (pvz. per `push_back`,
    per realokaciją). Todėl elementų tipas turi turėti
    **copy constructor**.

    `Studentas` iš P3 — kompiliatorius sugeneruoja jį automatiškai
    (nes neturime dinaminių resursų). **Kol kas tai veikia.**

    Kai P6–P8 klasės turės dinaminę atmintį (`new`) —
    reikės copy constructor rašyti patiems. Tai ir bus
    **Rule of Three** tema.

```cpp linenums="1"
// Šiame kode push_back kopijuoja Studentas:
grupe.push_back(Studentas("Jonas", 20, 8.5));
//              ^^^^^^^^^^^^^^^^^^^^^^^^^
//              laikinas objektas — nukopijuojamas į vector,
//              paskui sunaikinamas

// Galima matyti su [COPY] logging — žr. P6-P8 bonus skyrių
```

---

## 🔗 Ryšys su kitomis paskaitomis

```
P5 (ši paskaita):          Ryšys:

std::vector<Studentas> ──►  P9: Kursas turi vector<Studentas>
                             (kompozicija — vector VALDO studentus)

push_back + copy ────────►  P6-P8: kodėl reikia copy constructor
                             su dinaminės atminties klasėmis

range-based for ─────────►  P4: const auto& sintaksė (jau žinome)

std::sort + lambda ──────►  P15: algoritmai ir šablonai išsamiau

std::array ──────────────►  P1-P2: C masyvo C++ pakaitalas
```

---

*[STL]: Standard Template Library
*[NC]: Not Compiling — Nesikompiliuoja
*[VLA]: Variable Length Array — kintamo ilgio masyvas
