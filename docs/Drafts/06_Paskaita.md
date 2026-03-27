# Operatorių perkrovimas ir klaidų valdymas

---

## 1 DALIS: Operatorių perkrovimas

!!! abstract "Šios dalies tikslas"
    C++ leidžia apibrėžti, kaip standartiniai operatoriai (`+`, `==`, `<<`, `[]`) veikia su mūsų klasėmis.
    Tai daro kodą **intuityvų** ir **natūralų**:
    
    ```cpp
    MyString s3 = s1 + s2;       // Vietoj: s3 = s1.concat(s2)
    cout << s1;                   // Vietoj: s1.print()
    ```
    
    - Suprasime **kada** ir **kodėl** perkrauti operatorius
    - Išmoksime **member** vs **friend** skirtumą
    - Atrasime **return type** svarbą (value vs reference)

---

### Kodėl perkrauti operatorius?

#### **Problema: Neintuityvi sintaksė**

```cpp linenums="1"
// Be operator overloading:
class MyString {
public:
    MyString concat(const MyString& other) const;
    bool equals(const MyString& other) const;
    void print(std::ostream& os) const;
};

MyString s1("Hello"), s2("World");
MyString s3 = s1.concat(s2);      // ❌ Nenatūralu
if (s1.equals(s2)) { /* ... */ }  // ❌ Nenatūralu
s1.print(std::cout);              // ❌ Nenatūralu
```

**Kas blogai?**

- Reikia prisiminti metodų vardus (`concat`, `equals`, `print`)
- Sintaksė skiriasi nuo įprastų tipų (`int`, `string`)
- Kodas tampa sunkiau skaitomas

---

#### **Sprendimas: Operator overloading**

```cpp linenums="1"
// Su operator overloading:
class MyString {
public:
    MyString operator+(const MyString& other) const;
    bool operator==(const MyString& other) const;
    friend std::ostream& operator<<(std::ostream& os, const MyString& s);
};

MyString s1("Hello"), s2("World");
MyString s3 = s1 + s2;            // ✅ Natūralu!
if (s1 == s2) { /* ... */ }       // ✅ Natūralu!
std::cout << s1;                  // ✅ Natūralu!
```

**Privalumai:**

- ✅ Intuityvi sintaksė (kaip įprasti tipai)
- ✅ Kodas skaitomas natūraliai
- ✅ STL algoritmai veikia automatiškai (pvz., `std::sort`)

**Išvada:** Operatorių perkrovimas = **geresnė sintaksė** tai pačiai funkcijai.

---

### Operatorių kategorijos

| Kategorija | Operatoriai | Pavyzdys | Paskirtis |
|------------|-------------|----------|-----------|
| **Aritmetiniai** | `+`, `-`, `*`, `/`, `%` | `s1 + s2` | Matematinės operacijos |
| **Palyginimo** | `==`, `!=`, `<`, `>`, `<=`, `>=` | `s1 == s2` | Palyginimas |
| **Priskyrimo** | `=`, `+=`, `-=`, `*=`, `/=` | `s1 = s2` | Priskyrimas (Rule of Three!) |
| **Indeksavimo** | `[]` | `s[0]` | Prieiga prie elemento |
| **Srautų** | `<<`, `>>` | `cout << s` | Spausdinimas/nuskaitymas |
| **Kiti** | `++`, `--`, `()`, `->`, `*` | (vėliau) | Inkrementas, funkcijos kvietimas |

**Pastaba:** Ne visi operatoriai perkraunami (pvz., `.`, `::`, `?:`).

---

### Member function vs Friend function

Pagrindinis skirtumas: **kairysis operandas**.

---

#### **1. Member function operatoriai**

**Taisyklė:** Jei **kairysis operandas = mūsų klasė** → member function! ✅

=== "MyString.h"

    ```cpp linenums="1"
    #pragma once
    
    class MyString {
        char* data;
        size_t length;
    public:
        // Member function: kairysis operandas = this
        MyString operator+(const MyString& other) const;
        bool operator==(const MyString& other) const;
        char operator[](size_t index) const;
    };
    ```

=== "MyString.cpp"

    ```cpp linenums="1"
    #include "MyString.h"
    
    MyString MyString::operator+(const MyString& other) const {
        // this = kairysis (s1)
        // other = dešinysis (s2)
        // Implementacija: sujungti s1 + s2
    }
    
    bool MyString::operator==(const MyString& other) const {
        // this == other
        // Implementacija: palyginti
    }
    
    char MyString::operator[](size_t index) const {
        // this[index]
        // Implementacija: grąžinti elementą
    }
    ```

=== "main.cpp (naudojimas)"

    ```cpp linenums="1"
    #include "MyString.h"
    
    int main() {
        MyString s1("Hello"), s2("World");
        
        s1 + s2;      // s1.operator+(s2)
        s1 == s2;     // s1.operator==(s2)
        s1[0];        // s1.operator[](0)
        
        return 0;
    }
    ```

**Kaip veikia?**

```cpp
s1 + s2;  
// Kompiliatorius "matai":
s1.operator+(s2);
// Kairysis (s1) = this
// Dešinysis (s2) = parametras
```

---

#### **2. Friend function operatoriai**

**Kada reikia friend?**

- Kai **kairysis operandas ≠ mūsų klasė**
- Pavyzdys: `std::cout << s` → kairėje `ostream`, ne `MyString`!

**Kodėl `friend`?**

- Norime prieiti prie `private` laukų (`data`, `length`)
- Alternatyva: `public` getter'iai, bet `friend` patogiau

---

=== "MyString.h"

    ```cpp linenums="1"
    #pragma once
    #include <iostream>
    
    class MyString {
        char* data;      // private!
        size_t length;   // private!
    public:
        // ... konstruktoriai, Rule of Three ...
        
        // Friend operatorius — deklaracija klasėje
        friend std::ostream& operator<<(std::ostream& os, const MyString& s);
    };
    ```

=== "MyString.cpp"

    ```cpp linenums="1"
    #include "MyString.h"
    
    // Friend funkcija — implementacija IŠORĖJE klasės
    std::ostream& operator<<(std::ostream& os, const MyString& s) {
        os << s.data;  // ← Prieiga prie private data!
        return os;     // ← Grąžina os (chaining!)
    }
    ```

=== "main.cpp"

    ```cpp linenums="1"
    #include <iostream>
    #include "MyString.h"
    
    int main() {
        MyString s1("Hello");
        MyString s2("World");
        
        std::cout << s1;              // ✅ Veikia!
        std::cout << s1 << " " << s2; // ✅ Chaining!
        
        return 0;
    }
    ```

**Kaip veikia?**

```cpp
std::cout << s1;
// Kompiliatorius "matai":
operator<<(std::cout, s1);
// Kairysis = std::cout (ostream&)
// Dešinysis = s1 (MyString&)
```

---

### operator<< — "Natūralus spausdinimas"

#### **Studentų klausimas:**

!!! question "Kodėl negaliu tiesiog `print()` ir tikėtis `cout << s` veiks?"
    ```cpp
    class MyString {
    public:
        void print() const {
            std::cout << data;
        }
    };
    
    MyString s("Hello");
    std::cout << s;  // ❌ Nekompiliuojasi!
    ```

**Atsakymas:**

- `std::cout << s` → kompiliatorius ieško `operator<<(std::cout, s)`
- `print()` — tai **metodas**, ne **operatorius**!
- Reikia perkrauti `operator<<`

---

#### **Teisingas būdas:**

=== "MyString.h"

    ```cpp linenums="1"
    #pragma once
    #include <iostream>
    
    class MyString {
        char* data;
        size_t length;
    public:
        // ... konstruktoriai, Rule of Three ...
        
        // Friend deklaracija
        friend std::ostream& operator<<(std::ostream& os, const MyString& s);
    };
    ```

=== "MyString.cpp"

    ```cpp linenums="1"
    #include "MyString.h"
    
    // Friend funkcija — ne member!
    std::ostream& operator<<(std::ostream& os, const MyString& s) {
        os << s.data;  // Prieiga prie private!
        return os;     // Grąžina os → chaining
    }
    ```

=== "main.cpp"

    ```cpp linenums="1"
    #include <iostream>
    #include "MyString.h"
    
    int main() {
        MyString s1("Hello");
        MyString s2("World");
        
        std::cout << s1 << std::endl;         // "Hello"
        std::cout << s1 << " " << s2 << "!";  // "Hello World!"
        
        return 0;
    }
    ```

---

#### **Kodėl grąžina `ostream&`?**

```cpp
std::ostream& operator<<(std::ostream& os, const MyString& s) {
    os << s.data;
    return os;  // ← Grąžina os (ne kopiją!)
}
```

**Chaining:**

```cpp
std::cout << s1 << " " << s2;

// Ekvivalentas:
operator<<(operator<<(std::cout, s1), " ");
//         └─────────┬─────────┘
//              grąžina std::cout& → galima naudoti toliau!

// Toliau:
operator<<(std::cout, " ") << s2;
```

**Be `return os`:**

```cpp
void operator<<(std::ostream& os, const MyString& s) {
    os << s.data;
    // Negrąžina nieko!
}

std::cout << s1 << s2;  // ❌ Klaida! void << s2
```

---

### Return type gairės

| Operatorius | Return type | Priežastis | Pavyzdys |
|-------------|-------------|------------|----------|
| `operator+` | `MyString` (value) | Naujas objektas (temporary) | `s1 + s2` |
| `operator=` | `MyString&` (reference) | Modifikuoja esamą (`*this`) | `s1 = s2` |
| `operator[]` (read) | `char` (value) | Const, tik skaitymas | `s[0]` |
| `operator[]` (write) | `char&` (reference) | Modifikuoti elementą | `s[0] = 'x'` |
| `operator<<` | `ostream&` (reference) | Chaining | `cout << s` |
| `operator==` | `bool` (value) | True/false rezultatas | `s1 == s2` |

**Esmė:**

- **Value** → naujas objektas arba rezultatas
- **Reference** → modifikuojame esamą arba chaining

---

### operator+ vs operator= — Return type skirtumas

**Iš Paskaitos 05 (Rule of Three):**

```cpp
// operator= grąžina reference:
MyString& MyString::operator=(const MyString& other) {
    // ... modifikuoja *this ...
    return *this;  // ← Grąžina reference į esamą objektą
}

// operator+ grąžina value:
MyString MyString::operator+(const MyString& other) const {
    MyString result;
    // ... sukuria naują objektą ...
    return result;  // ← Grąžina VALUE (ne reference!)
}
```

**Kodėl skirtingi?**

- `operator=` — modifikuoja **esamą** objektą (`*this`)
- `operator+` — sukuria **naują** objektą (temporary)

!!! tip "Plačiau"
    Detalus paaiškinimas: **Paskaita 05** → "Advanced: operator+ vs operator= — Subtilus skirtumas"

---

### operator[] — const vs non-const

#### **Problema:**

```cpp
const MyString s1("Hello");
s1[0];          // ← Skaitymas (const objektas)
// s1[0] = 'h'; // ❌ Klaida! (const)

MyString s2("World");
s2[0] = 'w';    // ← Rašymas (non-const objektas)
```

**Kaip palaikyti abu?**

---

#### **Sprendimas: 2 versijos**

=== "MyString.h"

    ```cpp linenums="1"
    class MyString {
    public:
        // Const versija — tik skaitymas
        char operator[](size_t index) const;
        
        // Non-const versija — skaitymas IR rašymas
        char& operator[](size_t index);
    };
    ```

=== "MyString.cpp"

    ```cpp linenums="1"
    #include "MyString.h"
    #include <stdexcept>
    
    // Const versija — grąžina VALUE
    char MyString::operator[](size_t index) const {
        if (index >= length) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];  // Kopija (value)
    }
    
    // Non-const versija — grąžina REFERENCE
    char& MyString::operator[](size_t index) {
        if (index >= length) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];  // Nuoroda (reference)
    }
    ```

=== "main.cpp"

    ```cpp linenums="1"
    #include <iostream>
    #include "MyString.h"
    
    int main() {
        const MyString s1("Hello");
        char ch = s1[0];  // ← Const versija
        // s1[0] = 'h';   // ❌ Kompiliavimo klaida
        
        MyString s2("World");
        s2[0] = 'w';      // ← Non-const versija
        std::cout << s2;  // "world"
        
        return 0;
    }
    ```

**Kompiliatorius pasirenka:**

- Const objektas → `char operator[](size_t) const`
- Non-const objektas → `char& operator[](size_t)`

---

### Operatorių perkrovimo taisyklės

#### **✅ Galima perkrauti:**

```cpp
// Aritmetiniai:
+  -  *  /  %

// Palyginimo:
==  !=  <  >  <=  >=

// Priskyrimo:
=  +=  -=  *=  /=  %=

// Indeksavimo/prieigos:
[]  ()  ->  ->*

// Inkrementas/dekrementas:
++  --

// Srautų:
<<  >>

// Kiti:
&  |  ^  ~  !  &&  ||
```

---

#### **❌ NEGALIMA perkrauti:**

```cpp
.     // Member access
::    // Scope resolution
?:    // Ternary conditional
sizeof
typeid
```

**Kodėl?**

- Šie operatoriai turi specialią C++ semantiką
- Kompiliatorius turi juos valdyti tiesiogiai

---

#### **⚠️ Rekomenduojama NEPERKRAUTI:**

```cpp
&&  ||   // Logical AND/OR
```

**Kodėl?**

- Prarandi **short-circuit evaluation**:

```cpp
// Įprastai:
if (ptr != nullptr && ptr->isValid()) { }
//  └──┬──┘            └──┬──┘
//     ✓ tikrina tik jei ptr != nullptr

// Su perkrautu &&:
if (a && b) { }
// Abu išreiškimai įvertinami VISADA!
```

---

### Pilnas MyString pavyzdys

=== "MyString.h"

    ```cpp linenums="1"
    #pragma once
    #include <iostream>
    
    class MyString {
        char* data;
        size_t length;
    public:
        // Rule of Three
        MyString(const char* s = "");
        ~MyString();
        MyString(const MyString& other);
        MyString& operator=(const MyString& other);
        
        // Operatoriai
        MyString operator+(const MyString& other) const;
        bool operator==(const MyString& other) const;
        char operator[](size_t index) const;
        char& operator[](size_t index);
        
        friend std::ostream& operator<<(std::ostream& os, const MyString& s);
        
        // Helper metodai
        size_t size() const { return length; }
        const char* c_str() const { return data; }
    };
    ```

=== "MyString.cpp (operatoriai)"

    ```cpp linenums="1"
    #include "MyString.h"
    #include <cstring>
    #include <stdexcept>
    
    // operator+ (concatenation)
    MyString MyString::operator+(const MyString& other) const {
        size_t newLen = length + other.length;
        char* newData = new char[newLen + 1];
        
        std::strcpy(newData, data);
        std::strcat(newData, other.data);
        
        MyString result(newData);
        delete[] newData;
        return result;
    }
    
    // operator==
    bool MyString::operator==(const MyString& other) const {
        if (length != other.length) {
            return false;
        }
        return std::strcmp(data, other.data) == 0;
    }
    
    // operator[] const
    char MyString::operator[](size_t index) const {
        if (index >= length) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }
    
    // operator[] non-const
    char& MyString::operator[](size_t index) {
        if (index >= length) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }
    
    // operator<< (friend)
    std::ostream& operator<<(std::ostream& os, const MyString& s) {
        os << s.data;
        return os;
    }
    ```

---

!!! tip "Užduotis U4"
    **1 DALIS** teoriją praktikuosite **U4 Žingsniuose 5-6**:
    
    - **Žingsnis 5:** MyString operator+ (concatenation)
    - **Žingsnis 6 (BONUS):** operator[], ==, <<, <
    
    → [U4: Trijų Taisyklė](../Pratybos/U4.md)

---

## 2 DALIS: Klaidų valdymas (Error Handling įvadas)

!!! abstract "Šios dalies tikslas"
    Iki šiol klaidų netvarkėme sistemingai (arba `cerr`, arba tiesiog crash).
    Šiandien — **įvadas** į C++ išimčių (_exceptions_) sistemą:
    
    - Kodėl `cerr` neužtenka?
    - Kas yra išimtys (_exceptions_)?
    - Kaip `throw`, `try`, `catch` veikia?
    - Kaip RAII + exceptions veikia kartu?
    
    **Pastaba:** Pilnas _exception safety_ (_strong guarantee_, RAII _tricks_) → vėlesnės paskaitos.
    Šiandien — **pagrindai**.

---

### Problema: Kaip pranešti apie klaidą?

#### **Būdas 1: cerr (C++ stream)**

```cpp linenums="1"
char MyString::operator[](size_t index) const {
    if (index >= length) {
        std::cerr << "Index out of bounds!" << std::endl;
        // Bet ką grąžinti?!
        return '\0';  // ← "Magic value"
    }
    return data[index];
}
```

**Naudojimas:**

```cpp
char ch = s[100];  // ← Klaida, bet programa tęsiasi
std::cout << ch;   // '\0' — ar tai klaida ar tuščias simbolis?
```

**Problemos:**

- ❌ Kaip grąžinti klaidą? ("Magic values": `'\0'`, `-1`, `nullptr`)
- ❌ Programos tęsimas su neteisinga būsena
- ❌ Nėra kontrolės kviečiančiame kode
- ❌ Sunku atskirti klaidą nuo valid reikšmės

---

#### **Būdas 2: Grąžinti klaidos kodą**

```cpp linenums="1"
bool MyString::get(size_t index, char& result) const {
    if (index >= length) {
        return false;  // Klaida
    }
    result = data[index];
    return true;  // Sėkmė
}
```

**Naudojimas:**

```cpp
char ch;
if (s.get(100, ch)) {
    std::cout << ch;
} else {
    std::cerr << "Klaida!" << std::endl;
}
```

**Problemos:**

- ❌ Kebli sintaksė (ne `s[100]`)
- ❌ Lengva užmiršti patikrinti
- ❌ Sunku "propagate" klaidą aukštyn
- ❌ Konstruktoriai negali grąžinti error code!

---

#### **Būdas 3: Exceptions (išimtys)**

```cpp linenums="1"
char MyString::operator[](size_t index) const {
    if (index >= length) {
        throw std::out_of_range("Index out of bounds");  // ← Išmeta išimtį!
    }
    return data[index];
}
```

**Naudojimas:**

```cpp
try {
    char ch = s[100];  // ← Klaida!
    std::cout << ch;   // Nepasiekiamas
} catch (const std::out_of_range& e) {
    std::cerr << "Klaida: " << e.what() << std::endl;
    // Tvarkyti klaidą
}
```

**Privalumai:**

- ✅ Natūrali sintaksė (`s[100]`)
- ✅ Automatinis "propagation" aukštyn
- ✅ RAII valo resursus automatiškai!
- ✅ Konstruktoriai gali mesti išimtis

---

### Exception sintaksė

```cpp
// Mesti išimtį:
throw Exception_type(message);

// Gaudyti išimtį:
try {
    // Kodas, kuris gali mesti
} catch (const Exception_type& e) {
    // Tvarkyti klaidą
}
```

---

### Standartinės išimtys (C++ Standard Library)

```cpp
#include <stdexcept>
```

**Hierarchija:**

```
std::exception
├─ std::logic_error           // Programavimo klaidos
│  ├─ std::invalid_argument   // Neteisingas argumentas
│  ├─ std::out_of_range       // Indeksas už ribų
│  └─ std::length_error       // Per didelis dydis
│
└─ std::runtime_error         // Vykdymo klaidos
   ├─ std::overflow_error     // Aritmetinis perpildymas
   └─ std::underflow_error    // Aritmetinis nepildymas
```

---

**Naudojimas:**

```cpp linenums="1"
#include <stdexcept>

void MyString::set(size_t index, char value) {
    if (index >= length) {
        throw std::out_of_range("Index out of bounds");
    }
    data[index] = value;
}

void MyString::reserve(size_t newCapacity) {
    if (newCapacity > MAX_SIZE) {
        throw std::length_error("Capacity too large");
    }
    // ...
}
```

---

### RAII + Exceptions = ❤️

**Problema be RAII:**

```cpp linenums="1"
void func() {
    int* data = new int[100];
    
    // Kodas, kuris gali mesti išimtį
    if (error) {
        throw std::runtime_error("Error!");
        // ❌ data niekada neišvalyta! Memory leak!
    }
    
    delete[] data;  // ← Nepasiekiama po throw!
}
```

**Kas nutinka:**

1. `throw` išmeta išimtį
2. Programa ieško `catch` bloko
3. `delete[] data` **praleidžiama** → memory leak! ❌

---

**Sprendimas su RAII:**

```cpp linenums="1"
void func() {
    IntArray arr(100);  // ← RAII objektas
    
    // Kodas, kuris gali mesti išimtį
    if (error) {
        throw std::runtime_error("Error!");
        // ✅ arr destruktorius AUTOMATIŠKAI iškviestas!
        // ✅ delete[] data vykdomas destruktoriuje!
    }
    
    // arr destruktorius čia
}
```

**Stack unwinding:**

```
1. throw išimtis
2. Ieško catch bloko
3. Kol ieško → destruktoriai visiems stack objektams ↓
4. RAII → resuršai atlaisvinami automatiškai! ✅
```

---

### Try/Catch pavyzdys

=== "main.cpp"

    ```cpp linenums="1"
    #include <iostream>
    #include <stdexcept>
    #include "MyString.h"
    
    int main() {
        try {
            MyString s("Hello");
            
            std::cout << s[0] << std::endl;   // ✅ 'H'
            std::cout << s[100] << std::endl; // ❌ throw!
            
            // Nepasiekiamas kodas
            std::cout << "After error" << std::endl;
            
        } catch (const std::out_of_range& e) {
            std::cerr << "Klaida: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Kita klaida: " << e.what() << std::endl;
        }
        
        std::cout << "Programa tęsiasi" << std::endl;
        return 0;
    }
    ```

=== "Išvestis"

    ```
    H
    Klaida: Index out of bounds
    Programa tęsiasi
    ```

---

**Kaip veikia:**

1. `s[0]` → veikia, spausdina `'H'`
2. `s[100]` → `throw std::out_of_range(...)`
3. Stack unwinding → `s` destruktorius
4. Randa `catch (const std::out_of_range& e)`
5. Vykdo catch bloką
6. Programa tęsiasi po `try/catch`

---

### Kelios catch sekos

```cpp linenums="1"
try {
    // Kodas
} catch (const std::out_of_range& e) {
    // Specifinė klaida
    std::cerr << "Index klaida: " << e.what() << std::endl;
} catch (const std::logic_error& e) {
    // Platesnė kategorija
    std::cerr << "Logikos klaida: " << e.what() << std::endl;
} catch (const std::exception& e) {
    // Bet kokia std::exception
    std::cerr << "Bendrinė klaida: " << e.what() << std::endl;
} catch (...) {
    // Bet kokia kita (net ne std::exception)
    std::cerr << "Nežinoma klaida!" << std::endl;
}
```

**Tvarka svarbi:**

- Specifinės → platesnės
- Pirmas matching catch sugauna

---

### Kada naudoti exceptions?

#### **✅ Naudoti exceptions:**

- Klaidos, kurių **negalima ignoruoti**
- Konstruktoriai (negali grąžinti error code!)
- Operatoriai (netinka error codes)
- Klaidos, kurias reikia "propagate" per daug lygių
- RAII objektai (automatic cleanup)

#### **❌ NENAUDOTI exceptions:**

- Normalus program flow (`if/else` pakanka)
- Performance-critical kodas (exceptions lėtas!)
- C bibliotekos (jos nežino apie exceptions)
- Realtime systems (netikrumas laiko atžvilgiu)

#### **⚖️ Alternatyvos:**

- `std::optional<T>` (C++17) — gali būti arba ne
- `std::expected<T, E>` (C++23) — value arba error
- Error codes (C style)

---

### Exception safety levels (preview)

**Šiandien tik įvadas, detaliau — vėliau:**

1. **No-throw guarantee** — niekada nemeta (destruktoriai!)
2. **Strong guarantee** — arba sėkmė, arba būsena nepakito
3. **Basic guarantee** — po klaidos objektas valid, bet būsena gali kisti
4. **No guarantee** — po klaidos objektas invalid (blogai!)

**Dabar:** Tik **Basic guarantee** su RAII.

**Vėliau:** Strong guarantee su _copy-and-swap idiom_.

---

### Destruktoriai ir exceptions

!!! danger "Destruktorius niekada nemeta!"
    ```cpp
    // ❌ BLOGAI:
    ~MyString() {
        if (data == nullptr) {
            throw std::logic_error("Null data!");  // NIEKADA!
        }
        delete[] data;
    }
    
    // ✅ GERAI:
    ~MyString() {
        delete[] data;  // delete[] nullptr — safe
    }
    ```

**Kodėl?**

- Destruktorius vyksta stack unwinding metu
- Jei destruktorius meta → **2 išimtys vienu metu** → `std::terminate()` → crash!

**Taisyklė:** Destruktorius = `noexcept` (neturi mesti)

---

### Paprastas pavyzdys su RAII

=== "IntArray.h"

    ```cpp linenums="1"
    #pragma once
    #include <stdexcept>
    
    class IntArray {
        int* data;
        int size;
    public:
        IntArray(int s);
        ~IntArray();
        IntArray(const IntArray& other);
        IntArray& operator=(const IntArray& other);
        
        int get(int i) const;
        void set(int i, int v);
    };
    ```

=== "IntArray.cpp"

    ```cpp linenums="1"
    #include "IntArray.h"
    
    IntArray::IntArray(int s) : size(s) {
        if (s < 0) {
            throw std::invalid_argument("Negative size");
        }
        data = new int[s];
        for (int i = 0; i < s; ++i) {
            data[i] = 0;
        }
    }
    
    IntArray::~IntArray() {
        delete[] data;  // RAII cleanup
    }
    
    int IntArray::get(int i) const {
        if (i < 0 || i >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[i];
    }
    
    void IntArray::set(int i, int v) {
        if (i < 0 || i >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        data[i] = v;
    }
    ```

=== "main.cpp"

    ```cpp linenums="1"
    #include <iostream>
    #include "IntArray.h"
    
    int main() {
        try {
            IntArray arr(5);
            arr.set(0, 10);
            std::cout << arr.get(0) << std::endl;  // 10
            
            arr.get(100);  // ← throw!
            
        } catch (const std::exception& e) {
            std::cerr << "Klaida: " << e.what() << std::endl;
            // arr destruktorius automatiškai iškviestas! ✅
        }
        
        return 0;
    }
    ```

---

!!! tip "Užduotis U4"
    **2 DALIS** teoriją taikysite **U4 operator[] bounds checking**:
    
    Pasirinkite error handling būdą:
    
    - **Variantas A:** `throw std::out_of_range("Index out of bounds")`
    - **Variantas B:** `std::cerr << "Error!" << std::endl; return '\0';`
    
    → [U4: Trijų Taisyklė](../Pratybos/U4.md)

---

### Tiltas į priekį

!!! note "Ką matysime toliau"
    **Exception safety** — kaip rašyti exception-safe kodą:
    
    - Strong guarantee su _copy-and-swap idiom_
    - RAII _wrappers_ (smart pointers: `unique_ptr`, `shared_ptr`)
    - RAII file handles, locks, connections
    - Exception safety STL konteineriuose

---

## 📝 Santrauka

### **1 DALIS: Operatorių perkrovimas**

| Koncepcija | Raktiniai žodžiai |
|------------|-------------------|
| Member vs Friend | `this` / `friend` |
| Return types | Value / Reference |
| operator<< | `friend ostream&` |
| operator[] | Const / Non-const |
| Kada perkrauti | Intuityvi sintaksė |

**Išmokome:**

- ✅ Member function — kairysis = mūsų klasė
- ✅ Friend function — kairysis ≠ mūsų klasė
- ✅ Return value vs reference
- ✅ Const correctness (`operator[]`)

---

### **2 DALIS: Klaidų valdymas**

| Koncepcija | Raktiniai žodžiai |
|------------|-------------------|
| Exceptions | `throw` / `try` / `catch` |
| RAII + Exceptions | Stack unwinding |
| Standard exceptions | `std::out_of_range` |
| Kada naudoti | Konstruktoriai, operatoriai |
| Destruktoriai | NIEKADA nemeta! |

**Išmokome:**

- ✅ `throw`/`try`/`catch` sintaksė
- ✅ RAII automatiškai valo su exceptions
- ✅ Standartinės išimtys (`<stdexcept>`)
- ✅ Destruktoriai = `noexcept`

---

*[RAII]: Resource Acquisition Is Initialization
*[UDT]: User-Defined Type
