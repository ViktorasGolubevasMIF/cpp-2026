# Stack ADT Evoliucija C++ kalboje
## II dalis: Nuo C `struct` iki C++ `class` – Klasės Anatomija

---

> **Tęsinys po:** I dalies 9 etapų "Pilnaverčio vartotojo apibrėžto tipo" (C UDT su _opaque pointer_ ir _factory pattern_)  
> **Tikslas:** Atrasti/suvokti C++ klasę `class` kaip *naujos kartos* `struct`  
> **Metodologija:** Tas pats principas – klaidos = mokymo(si)s įrankis!

---

## 🔁 Prisiminkime: Kur sustojome?

!!! success "I dalis: Pasiekėme pilną C ADT (09 etapas)"
    - ✅ **Information hiding** (`static` nariai, _opaque pointer_)
    - ✅ **Encapsulation** (duomenys + funkcijos **modulyje**)
    - ✅ **Multiple instances** (su rodyklėmis `*pst1`, `*pst2`, ...)
    - ✅ **Lifecycle management** (`create()` → `destroy()`)

!!! warning "Bet C kalboje liko neišspręstų problemų..."
    - **Manual lifecycle:** programuotojas **privalo** prisiminti `destroy()` – užmirštus _memory leak_
    - **No operator overloading**: negalima natūralaus priskyrimo/kopijavimo `pst1 = pst2;`
    - **Opaque pointer**/**Forward declaration** biurokratija: `create()`/`destroy()` – rankinis darbas, kurį galėtų automatizuoti kompiliatorius

---

## 🚀 Šuolis: C ADT → C++ Class

```c
// C (09 etapas) – rankinis valdymas
struct Stack *s = create(); // rankinis create()
push(s, 'a');               // C funkcijos iškvietimo sintaksė: funkcija(objektas, ...)
char c = pop(s);            // C objektas paprastai perduodamas rodykle=adresu
destroy(s);                 // ← REIKIA PRISIMINTI!
```

```cpp
// C++ – automatinis valdymas
Stack s;               // konstruktorius = automatinis create()
s.push('a');           // metodo sintaksė: objektas.funkcija(...)
char c = s.pop();
..}                    // destruktorius = automatinis destroy()
```

!!! tip "Paradigminis pokytis"
    **C:** (išorinės/nesavos/globalios) funkcijos valdo objektą → `push(s, 'a')`  
    **C++:** objektas turi (savas) funkcijas → `s.push('a')`  
    
    Tai ne tik sintaksės skirtumas – tai kitoks mąstymo būdas apie kodą.

---

## 🗺️ II dalies kelionė

| Nr | Etapas | Žingsniai | Esmė |
|----|--------|-----------|------|
| **11** | [Discovering_CPP_Struct](#11) | `00` → `01_NC_Naive` | NC: `main()` nebemato funkcijų globaliai |
| **12** | [ENCAPSULATING_in_CPP_Struct](#12) | `01_OK` → `02_OK_Attack` | `this` gimimas; `struct` dar atviras |
| **13** | [HIDING_in_CLASS](#13) | `01_NC_Naive` → `02_OK` → `03_NC_Attack` | Paradigminis switch: kompiliatorius saugo! |
| **14** | [CONSTRUCTORS_and_DESTRUCTORS](#14) | `00` → `01_OK` | Kūrimas = inicializavimas; naikinimas = automatinis |
| **15** | [OVERLOADING_CONSTRUCTORS](#15) | `00` → `01_OK` | Overloading: tas pats vardas, skirtingi parametrai |
| **16** | [Defining_METHODS_OUTSIDE](#16) | `00` → `01_NC_Naive` → `02_OK` → `03_OK` | `Stack::` + `.h`/`.cpp` moduliavimas |

---

<a name="11"></a>
## 1️⃣1️⃣ Etapas: `11_Discovering_CPP_Struct`

> [`11_Discovering_CPP_Struct`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/11_Discovering_CPP_Struct/)

!!! note "🎯 Tikslas"
    Suprasti, kas nutinka kai C funkcijų apibrėžimus **perkeliame į `struct {}` bloką** – ir `main()` jas vis dar kviečia C stiliumi.

---

### 0 žingsnis: Referentinis taškas → [`00`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/11_Discovering_CPP_Struct/00/)

> Failas: [`11_Discovering_CPP_Struct/00/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/11_Discovering_CPP_Struct/00/usestack.cpp)

!!! quote "Mintis / ketinimas"
    Pradedame nuo **to paties kodo** kaip `05/03_OK/usestack.c` – tik plėtinys `.cpp`.  
    Funkcijos yra **išorėje** `struct {}`. Tai veikia – ir C, ir C++ atveju.

???+ "📄 `usestack.cpp` – C stilius, funkcijos išorėje"
    ```cpp
    --8<-- "code/evolution/stack-2026/11_Discovering_CPP_Struct/00/usestack.cpp"
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang++ usestack.cpp -o app   # ✅
    ./app
    ```

!!! success "✅ Veikia – kaip ir C atveju. Tai mūsų atspirties taškas."

---

### 1 žingsnis: Funkcijos **į** `struct` vidų, `main()` nepakeistas → [`01_NC_Naive`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/11_Discovering_CPP_Struct/01_NC_Naive/)

> Failas: [`11_Discovering_CPP_Struct/01_NC_Naive/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/11_Discovering_CPP_Struct/01_NC_Naive/usestack.cpp)

!!! quote "Mintis / ketinimas"
    „Perkeliu funkcijų apibrėžimus į `struct Stack {}` vidų. `main()` lieka nepakeistas – turėtų vis tiek veikti, ne?"

???+ "📄 `usestack.cpp` – funkcijos viduje, `main()` C stiliumi"
    ```cpp
    --8<-- "code/evolution/stack-2026/11_Discovering_CPP_Struct/01_NC_Naive/usestack.cpp"
    ```

=== "🔨 Kompiliavimas"
    ```bash
    clang++ usestack.cpp -o app
    ```

=== "💥 Klaidos"
    ```
    usestack.cpp:22:5: error: use of undeclared identifier 'init'; did you mean 'int'?
    usestack.cpp:23:15: error: use of undeclared identifier 'isFull'
    usestack.cpp:23:60: error: use of undeclared identifier 'push'
    usestack.cpp:24:13: error: use of undeclared identifier 'isEmpty'
    usestack.cpp:24:36: error: use of undeclared identifier 'pop'
    ... (10 klaidų iš viso)
    ```

!!! failure "🤔 Funkcijos yra faile – kodėl kompiliatorius jų nemato?!"

??? question "Pamąstykite prieš skaitant toliau..."
    `init`, `push`, `pop` ir kt. yra **apibrėžtos** – tik viduje `struct Stack {}`.  
    `main()` jas kviečia kaip globalias: `init(&st1)`.  
    Kas pasikeitė, kai funkcijos „persikėlė" į vidų?

!!! tip "Atsakymas: funkcija „persikėlė" – ir išnyko iš globalios erdvės"
    Kai funkcija yra **viduje** `struct Stack {}`, ji nebėra globali funkcija.  
    Ji tapo **klasės nariu** – ir dabar priklauso `Stack` vardų erdvei.

    `main()` ieško `init` globaliai – ir neranda. Kompiliatorius pasiūlo: *„did you mean 'int'?"* – nes globaliai žino tik `int`, ne `init`.

    **Taisymas akivaizdus:** kviesti per objektą – `st1.init(...)`.

    ??? note "Beje: `static reset` nekelia klaidos..."
        `static void reset(...)` viduje `struct` kompiliatorius priima be skundo.  
        `static` klasės viduje reiškia visai **ką kitą** nei C kalboje – tai klasės lygio narys, ne egzemplioriaus. Prie šio `static` grįšime vėliau.

---

### 2 žingsnis: `main()` pakeičiamas į objekto sintaksę → [`02_OK_Wrong`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/11_Discovering_CPP_Struct/02_OK_Wrong/)

> Failas: [`11_Discovering_CPP_Struct/02_OK_Wrong/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/11_Discovering_CPP_Struct/02_OK/usestack.cpp)

!!! quote "Mintis / ketinimas"
    „Jei funkcijos priklauso `Stack` – kviesiu jas per objektą: `st1.init(...)`."

???+ "📄 `usestack.cpp` – tik `main()` pakeistas"
    ```cpp
    --8<-- "code/evolution/stack-2026/11_Discovering_CPP_Struct/02_OK_Wrong/usestack.cpp"
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang++ usestack.cpp -o app   # ✅
    ./app
    ```

=== "⌨️➔🖥️"
    ```
    hello
    olleh
    world
    dlrow
    ```

!!! success "✅ Veikia!"

!!! warning "Bet... `st1.init(&st1)` – ar tai normalu?"
    Veikia – bet atrodo keistai. Kviečiame per objektą `st1`, ir dar atskirai perduodame `&st1` kaip parametrą.  
    Objektas perduodamas **du kartus**: vieną kartą kaip kvietėjas, antrą kartą kaip argumentas.
    
    Tai yra **kitas žingsnis**: kaip išmesti `struct Stack *pst` parametrą – ir leisti C++ tai tvarkyti automatiškai.

---

<a name="12"></a>
## 1️⃣2️⃣ Etapas: `12_ENCAPSULATING_in_CPP_Struct`

> [`12_ENCAPSULATING_in_CPP_Struct`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/12_ENCAPSULATING_in_CPP_Struct/)

!!! note "🎯 Tikslas"
    Išmesti `struct Stack *pst` parametrą – suprasti `this`. Ir pamatyti, kad `struct` su metodais dar **nesuteikia** information hiding.

---

### 1 žingsnis: Išmetame `pst` parametrą → [`01_OK`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/12_ENCAPSULATING_in_CPP_Struct/01_OK/)

> Failas: [`12_ENCAPSULATING_in_CPP_Struct/01_OK/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/12_ENCAPSULATING_in_CPP_Struct/01_OK/usestack.cpp)

!!! quote "Mintis / ketinimas"
    "`st1.init(&st1)` – absurdas. Objektas jau žino, kas jis yra – išmeskime `&st1` kaip parametrą!"

???+ "📄 `usestack.cpp`"
    ```cpp
    --8<-- "code/evolution/stack-2026/12_ENCAPSULATING_in_CPP_Struct/01_OK/usestack.cpp"
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang++ usestack.cpp -o app   # ✅
    ./app
    ```

=== "⌨️➔🖥️"
    ```
    hello
    olleh
    world
    dlrow
    ```

!!! success "✅ Veikia!"

!!! tip "Kas nutiko su `pst`?"
    **Prieš (11 – dar su `pst`):**
    ```cpp
    void init(struct Stack *pst) { pst->top = 0; }
    st1.init(&st1);   // ← objektas du kartus – absurdas
    ```
    **Po (12 – be `pst`):**
    ```cpp
    void init() { top = 0; }
    st1.init();        // ← švaru
    ```
    Kompiliatorius *automatiškai* perduoda rodyklę į kviečiantį objektą. Ta paslėpta rodyklė vadinamas **`this`**.

??? info "Kas yra `this`?"
    ```cpp
    // Tai ką mes rašome:
    void init() { top = 0; }

    // Tai ką kompiliatorius „mato" po gaubtu:
    void init(Stack* const this) { this->top = 0; }
    ```
    Kai rašome `st1.init()` – kompiliatorius išverčia į `Stack::init(&st1)`.  
    `this` visada **rodo** į tą objektą, kuris kvietė metodą.

---

### 2 žingsnis: Ataka vis dar pavyksta → [`02_OK_Attack`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/12_ENCAPSULATING_in_CPP_Struct/02_OK_Attack/)

> Failas: [`12_ENCAPSULATING_in_CPP_Struct/02_OK_Attack/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/12_ENCAPSULATING_in_CPP_Struct/02_OK_Attack/usestack.cpp)

!!! quote "Mintis / ketinimas"
    „Turime `struct` su metodais – ar duomenys dabar saugūs?"

???+ "📄 Atakos kodas `main()` dalyje"
    ```cpp
    --8<-- "code/evolution/stack-2026/12_ENCAPSULATING_in_CPP_Struct/02_OK_Attack/usestack.cpp"
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang++ usestack.cpp -o app   # ✅ kompiliuoja!
    ./app
    ```

=== "⌨️➔🖥️"
    ```
    hello
    olleh
    world
    !dlrow   ← '!' įterptas – ataka pavyko!
    ```

!!! danger "✅ Kompiliuoja. ✅ Veikia. ❌ Ataka **pavyko**!"
    `struct Stack` nariai yra **vieši** – bet kuris kodas gali juos tiesiogiai keisti.  
    Metodai viduje – tai **encapsulation**. Bet duomenys atviri – **ne information hiding**.

---

## 📌 Terminų kampas: Encapsulation vs Information Hiding

!!! abstract "Dvi skirtingos sąvokos – dažnai supainiojamos"

    **Encapsulation** – duomenų ir operacijų **suvienijimas** į vieną vienetą:
    ```cpp
    struct Stack {
        char stack[SIZE];  // duomenys
        int top;
        void push(char c); // operacijos kartu!
        char pop();
    };
    ```
    ✅ Pasiekta jau **12 etape** – `struct` su metodais.

    **Information Hiding** – vidinės struktūros **paslėpimas** nuo vartotojo:
    ```cpp
    class Stack {
    private:
        char stack[SIZE];  // ← nematoma iš išorės
        int top;
    public:
        void push(char c);
        char pop();
    };
    ```
    ✅ Pasiekta **13 etape** – `class` su `private:`.

!!! warning "Encapsulation ≠ Information Hiding"
    | | Encapsulation | Information Hiding |
    |--|--------------|-------------------|
    | **C analogas** | 06 etapas (`struct` modulyje) | 07–09 etapai (opaque pointer) |
    | **C++ analogas** | 12 etapas (`struct` su metodais) | 13 etapas (`class` + `private:`) |
    | **Kas kartu?** | Duomenys + funkcijos | Taip pat + duomenys **paslėpti** |
    | **Ataka** | ✅ Pavyksta | ❌ NC – kompiliatorius blokuoja |

    Encapsulation yra **būtina sąlyga**, bet **nepakankama** information hiding pasiekti.

---

<a name="13"></a>
## 1️⃣3️⃣ Etapas: `13_HIDING_in_CLASS`

> [`13_HIDING_in_CLASS`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/13_HIDING_in_CLASS/)

!!! note "🎯 Tikslas"
    Suprasti, kuo `class` skiriasi nuo `struct` – ir kodėl tai yra **paradigminis pokytis**.

---

### 1 žingsnis: Plikas `class` be specifikatorių → [`01_NC_Naive`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/13_HIDING_in_CLASS/01_NC_Naive/)

> Failas: [`13_HIDING_in_CLASS/01_NC_Naive/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/13_HIDING_in_CLASS/01_NC_Naive/usestack.cpp)

!!! quote "Mintis / ketinimas"
    „C++ turi specialų raktažodį objektams – `class`. Pakeičiu `struct` į `class` ir viskas turėtų veikti kaip anksčiau."

???+ "📄 `usestack.cpp` – tik `struct` → `class`"
    ```cpp
    --8<-- "code/evolution/stack-2026/13_HIDING_in_CLASS/01_NC_Naive/usestack.cpp"
    ```

=== "🔨 g++ kompiliavimas"
    ```bash
    g++ usestack.cpp -o app
    ```

=== "💥 g++ klaidos"
    ```
    usestack.cpp: In function 'int main()':
    usestack.cpp:23:17: error: 'void Stack::init()' is private within this context
       23 |         st1.init();
          |         ~~~~~~~~^~
    usestack.cpp:8:14: note: declared private here
        8 |         void init()       { top = 0; }
          |              ^~~~
    usestack.cpp:24:28: error: 'int Stack::isFull()' is private within this context
    ... (10 klaidų iš viso)
    ```

=== "🔨 clang kompiliavimas"
    ```bash
    clang++ usestack.cpp -o app
    ```

=== "💥 clang klaidos"
    ```
    usestack.cpp:23:13: error: 'init' is a private member of 'Stack'
       23 |         st1.init();
          |             ^
    usestack.cpp:8:14: note: implicitly declared private here
        8 |         void init()       { top = 0; }
          |              ^
    ... (10 klaidų iš viso)
    ```

!!! failure "🤔 Pakeitėme tik vieną žodį – `struct` → `class`. Kodėl 10 klaidų?"

??? question "Pamąstykite prieš skaitant toliau..."
    `struct Stack` veikė, `class Stack` – ne. Koks formalus skirtumas tarp jų C++ kalboje?

!!! tip "Atsakymas: `class` pagal nutylėjimą slepia **viską**"
    | | `struct` | `class` |
    |--|----------|---------|
    | **Nariai pagal nutylėjimą** | `public` | **`private`** |
    
    Tai vienintelis formalus skirtumas. Bet jo pasekmė milžiniška:
    
    Plikas `class Stack { void init() {...} }` – `init` **automatiškai `private`**.  
    `main()` bando kviesti `private` metodą – kompiliatorius blokuoja.
    
    Atkreipkite dėmesį į clang žinutę: **`implicitly declared private here`** – ne mes parašėme `private:`, kompiliatorius tai padarė *automatiškai*.

---

### 2 žingsnis: Pridedame `public:` → [`02_OK`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/13_HIDING_in_CLASS/02_OK/)

> Failas: [`13_HIDING_in_CLASS/02_OK/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/13_HIDING_in_CLASS/02_OK/usestack.cpp)

!!! quote "Mintis / ketinimas"
    „Metodai turi būti pasiekiami iš išorės – dedu `public:`. Duomenys tegul lieka `private:`."

???+ "📄 `usestack.cpp` – su `public:`/`private:`"
    ```cpp
    --8<-- "code/evolution/stack-2026/13_HIDING_in_CLASS/02_OK/usestack.cpp"
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang++ usestack.cpp -o app   # ✅
    ./app
    ```

=== "⌨️➔🖥️"
    ```
    hello
    olleh
    world
    dlrow
    ```

!!! success "✅ Veikia!"

---

### 3 žingsnis: Ataka → [`03_NC_Attack`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/13_HIDING_in_CLASS/03_NC_Attack/)

> Failas: [`13_HIDING_in_CLASS/03_NC_Attack/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/13_HIDING_in_CLASS/03_NC_Attack/usestack.cpp)

!!! quote "Mintis / ketinimas"
    „I dalyje su C `struct` ataka **pavyko** (06 etapas). Patikriname ar čia tas pats..."

???+ "📄 Atakos kodas `main()` dalyje"
    ```cpp
    --8<-- "code/evolution/stack-2026/13_HIDING_in_CLASS/03_NC_Attack/usestack.cpp"
    ```

=== "🔨 Kompiliavimas"
    ```bash
    clang++ usestack.cpp -o app
    ```

=== "💥 Klaidos"
    ```
    usestack.cpp:XX:X: error: 'stack' is a private member of 'Stack'
    usestack.cpp:XX:X: error: 'top' is a private member of 'Stack'
    2 errors generated.
    ```

!!! success "✅ Ataka **nepavyko**! Kompiliatorius saugo!"

!!! tip "Paradigminis pokytis – lyginimas su C"
    | | C (06 etapas – `struct` modulyje) | C++ (13 etapas – `class`) |
    |--|-----------------------------------|--------------------------|
    | **Ataka** | ✅ Pavyko – `st.top = 999;` veikia | ❌ NC – kompiliatorius blokuoja |
    | **Kas saugo?** | Niekas – programuotojo atsakomybė | **Kompiliatorius** |
    | **C (07-09 – opaque pointer)** | ✅ Nepavyko – bet reikia atskiro `.c` failo | **Viename faile!** |
    | **Kiek failų?** | `stack.h` + `stack.c` + `user.c` | Galima viename faile |

??? question "Refleksija: kas dar liko?"
    Turime `class` su `private:`/`public:` – tai didžiulis žingsnis!  
    Bet `init()` vis dar kviečiamas **rankiniu būdu** – ar galėtų objektas pats pasirūpinti savo inicializacija?
    
    → **14 etapas:** Konstruktorius – automatinis `init()`! 🚀

---

## 🎯 II dalies santrauka (11–13 etapai)

| Nr | Etapas | Žingsniai | Atradimas |
|----|--------|-----------|-----------|
| **11** | Discovering_CPP_Struct | `00` OK → `01_NC_Naive` | Funkcija klasės viduje ≠ globali funkcija |
| **12** | ENCAPSULATING_in_CPP_Struct | `01_OK` → `02_OK_Attack` | `this` + encapsulation ≠ information hiding |
| **13** | HIDING_in_CLASS | `01_NC_Naive` → `02_OK` → `03_NC_Attack` | `class` = `private` pagal nutylėjimą; kompiliatorius saugo |

!!! abstract "Klasės anatomija – apibendrinimas"
    ```cpp
    class Stack {
    // ─────────────────────────────────────────
    private:              // ← prieigos specifikatorius
        char stack[5];    // ← duomenų nariai (data members)
        int top;          //   paslėpti nuo išorės
    // ─────────────────────────────────────────
    public:               // ← prieigos specifikatorius
        void init();      // ← funkcijų nariai (member functions / metodai)
        void push(char c);//   prieinami iš išorės
        char pop();
        int isEmpty();
        int isFull();
    // ─────────────────────────────────────────
    };   // ← kabliataškis! (skirtingai nuo funkcijų blokų)
    ```

    **`struct` vs `class` C++ kalboje – formalus skirtumas vienas:**
    
    | | Nariai pagal nutylėjimą |
    |--|------------------------|
    | `struct` | `public` |
    | `class` | **`private`** |

**Kitas žingsnis:** Konstruktorius ir destruktorius – automatinis lifecycle! 🚀

---

<a name="14"></a>
## 1️⃣4️⃣ Etapas: `14_CONSTRUCTORS_and_DESTRUCTORS`

> [`14_CONSTRUCTORS_and_DESTRUCTORS`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/14_CONSTRUCTORS_and_DESTRUCTORS/)

!!! note "🎯 Tikslas"
    Suprasti, kad **kūrimo momentas** yra garantuotas inicializavimo taškas – tai ne taisyklė kurią reikia prisiminti, o kalbos mechanizmas. Ir kad **naikinimo momentas** taip pat garantuotas – automatiškai.

!!! info "📌 Atsisveikinimas su `getchar()`/`putchar()`"
    C++ **pilnai palaiko** C biblioteką – `getchar()`/`putchar()` veikė ir veiks.  
    Bet nuo šiol naudosime `cin`/`cout` – C++ standartinės įvesties/išvesties srautus.

    Beje: `cin.get(c)`, o ne `cin >> c` – nes `>>` praleistų tarpelius.  
    Ir pastebėkite: `cin.get(c)` – **metodas ant objekto**, lygiai kaip `s.push(c)`.  
    `cin` ir `cout` – tai objektai! Tik šito mokykloje niekas nepasakė 🙂

---

### 0 žingsnis: Bazinis kodas – veikia atsitiktinai → [`00`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/14_CONSTRUCTORS_and_DESTRUCTORS/00/)

> Failas: [`14_CONSTRUCTORS_and_DESTRUCTORS/00/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/14_CONSTRUCTORS_and_DESTRUCTORS/00/usestack.cpp)

!!! quote "Mintis / ketinimas"
    Minimizuotas bazinis kodas – vienas objektas, `bool`, be `init()`. Ar veiks?

???+ "📄 `usestack.cpp`"
    ```cpp
    --8<-- "code/evolution/stack-2026/14_CONSTRUCTORS_and_DESTRUCTORS/00/usestack.cpp"
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang++ usestack.cpp -o app   # ✅ kompiliuoja!
    ./app
    ```

!!! warning "⚠️ Kompiliuoja. Kartais veikia. Kartais – ne."
    `top` nario reikšmė **nėra inicializuota** – joje yra kas tik atsitiko būti atmintyje.  
    Dažnai būna `0` (stack atmintis dažnai nulinata) – todėl „pasiseka".  
    Bet tai **atsitiktinumas**, ne teisingumas.

!!! danger "C++ garantuoja kūrimo momentą – bet ne inicializavimą (be konstruktoriaus)"
    Kompiliatorius **automatiškai generuoja** vadinamąjį *trivial default constructor* –  
    jis sukuria objektą, bet **nieko nedaro** su narių reikšmėmis.  
    
    Tai tarsi `create()` iš C 08 etapo – be `top = 0` viduje.

??? question "Pamąstykite: kas iš tikrųjų garantuoja inicializavimą?"
    Ne programuotojo disciplina (`init()` kvietimas).  
    Ne atsitiktinumas (atmintis „pasisekė" būti nulinata).  
    
    **Konstruktorius** – kūrimo ir inicializavimo momentas **sutampa**.

---

### 1 žingsnis: Default konstruktorius + destruktorius → [`01_OK`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/14_CONSTRUCTORS_and_DESTRUCTORS/01_OK/)

> Failas: [`14_CONSTRUCTORS_and_DESTRUCTORS/01_OK/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/14_CONSTRUCTORS_and_DESTRUCTORS/01_OK/usestack.cpp)

!!! quote "Mintis / ketinimas"
    „Noriu, kad objektas **pats** pasirūpintų savo inicializacija kūrimo metu – ir praneštu apie save gyvavimo pradžioje ir pabaigoje."

???+ "📄 `usestack.cpp`"
    ```cpp
    --8<-- "code/evolution/stack-2026/14_CONSTRUCTORS_and_DESTRUCTORS/01_OK/usestack.cpp"
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang++ usestack.cpp -o app   # ✅
    ./app
    ```

=== "⌨️➔🖥️"
    ```
    --- prieš Stack s ---
    [CTOR] Stack sukurtas
    hello
    olleh
    --- prieš scope pabaigą ---
    [DTOR] Stack sunaikintas
    --- po scope pabaigos ---
    ```

!!! success "✅ Konstruktorius ir destruktorius veikia automatiškai!"

!!! tip "Konstruktoriaus anatomija"
    ```cpp
    Stack() { top = 0; }
    //^^^^    ← tas pats vardas kaip klasė
    //     ← jokio grąžinamo tipo (net ne void!)
    ```
    
    **Kada kviečiamas?** Tiksliai tada, kai objektas **sukuriamas**.  
    **Kas kviečia?** Kompiliatorius – automatiškai. Programuotojas negali „užmiršti".

!!! tip "Destruktoriaus anatomija"
    ```cpp
    ~Stack() { cout << "[DTOR] Stack sunaikintas\n"; }
    //^^^^^    ← tildė + klasės vardas
    ```
    
    **Kada kviečiamas?** Tiksliai tada, kai objektas **baigia gyventi** – scope pabaigoje.  
    **Kas kviečia?** Kompiliatorius – automatiškai.

!!! abstract "C → C++ lifecycle palyginimas"
    | | C (08–09 etapai) | C++ (14 etapas) |
    |--|------------------|-----------------|
    | **Kūrimas** | `create()` – reikia kviesti | `Stack s;` – konstruktorius automatiškai |
    | **Naikinimas** | `destroy()` – reikia kviesti | `}` – destruktorius automatiškai |
    | **Užmiršus** | Memory leak | Neįmanoma užmiršti |
    | **Kas atsakingas?** | Programuotojas | **Kompiliatorius** |

??? question "Refleksija: kam reikalingas destruktorius Stack klasėje?"
    Mūsų `Stack` naudoja **statinį masyvą** – atminties atlaisvinti nereikia.  
    Destruktorius čia skirtas tik **demonstracijai** – matome kada objektas „miršta".  
    
    Kai klasė naudos **dinaminę atmintį** (`new`) – destruktorius taps **būtinas**.  
    → Tai Rule of Three/Five tema (vėliau kurse).

---

<a name="15"></a>
## 1️⃣5️⃣ Etapas: `15_OVERLOADING_CONSTRUCTORS`

> [`15_OVERLOADING_CONSTRUCTORS`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/15_OVERLOADING_CONSTRUCTORS/)

!!! note "🎯 Tikslas"
    Atrasti **overloading** – tą patį vardą, skirtingus parametrus. Pirmiausia per konstruktorius, nes čia labiausiai natūralu: skirtingi kūrimo būdai → skirtingi konstruktoriai.

---

### 0 žingsnis: Bazinis kodas – vienas konstruktorius → [`00`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/15_OVERLOADING_CONSTRUCTORS/00/)

> Failas: [`15_OVERLOADING_CONSTRUCTORS/00/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/15_OVERLOADING_CONSTRUCTORS/00/usestack.cpp)

!!! quote "Mintis / ketinimas"
    Turime default konstruktorių iš 14 etapo. Bet kas jei norime sukurti steką **jau su duomenimis**?

???+ "📄 `usestack.cpp` – dar tik default konstruktorius"
    ```cpp
    --8<-- "code/evolution/stack-2026/15_OVERLOADING_CONSTRUCTORS/00/usestack.cpp"
    ```

=== "⌨️➔🖥️"
    ```
    [CTOR] Stack sukurtas (tuščias)
    CBA
    [DTOR] Stack sunaikintas
    ```

!!! warning "Bet kas jei norime iš karto sukurti steką su pradiniais duomenimis?"
    `s1.push('A'); s1.push('B'); s1.push('C');` – tris kartus kviečiame `push()`.  
    C kalboje buvo galima perduoti duomenis į `create()` → ar C++ leidžia perduoti į konstruktorių?

---

### 1 žingsnis: Parametrinis konstruktorius → [`01_OK`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/15_OVERLOADING_CONSTRUCTORS/01_OK/)

> Failas: [`15_OVERLOADING_CONSTRUCTORS/01_OK/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/15_OVERLOADING_CONSTRUCTORS/01_OK/usestack.cpp)

!!! quote "Mintis / ketinimas"
    „Noriu turėti **du** kūrimo būdus: tuščią steką ir steką su pradiniais duomenimis. Abu konstruktoriai vadinsis `Stack` – ar tai leis kompiliatorius?"

???+ "📄 `usestack.cpp` – du konstruktoriai"
    ```cpp
    --8<-- "code/evolution/stack-2026/15_OVERLOADING_CONSTRUCTORS/01_OK/usestack.cpp"
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang++ usestack.cpp -o app   # ✅
    ./app
    ```

=== "⌨️➔🖥️"
    ```
    [CTOR] Stack sukurtas (tuščias)
    [CTOR] Stack sukurtas su duomenimis
    s1: X
    s2: EDCBA
    [DTOR] Stack sunaikintas
    [DTOR] Stack sunaikintas
    ```

!!! success "✅ Du konstruktoriai – tas pats vardas `Stack`, skirtingi parametrai!"

!!! tip "Overloading – tas pats vardas, skirtingi parametrai"
    ```cpp
    Stack s1;          // ← Stack()            – be parametrų
    Stack s2("ABCDE"); // ← Stack(const char*) – su parametru
    ```
    Kompiliatorius **automatiškai parenka** tinkamą konstruktorių pagal tai, **kaip** objektas kuriamas.

    Tai **function overloading** – ta pati sąvoka galioja ir paprastoms funkcijoms, ne tik konstruktoriams.

!!! abstract "Overloading taisyklė"
    Dvi funkcijos **gali turėti tą patį vardą**, jei skiriasi:
    - parametrų **skaičius**, arba
    - parametrų **tipai**

    Grąžinamo tipo **nepakanka** atskirti – kompiliatorius jį ignoruoja renkantis.

??? note "Destruktorius – visada vienas"
    Konstruktorių gali būti kiek nori – kūrimo būdų gali būti daug.  
    Destruktorius – **visada vienas**: sunaikinimo būdas yra tik vienas.  
    Todėl `~Stack()` negali turėti parametrų ir negali būti perkrautas (_overloaded_).

??? question "Kalibracija su U2 užduotimi"
    `Student` klasėje taip pat yra du konstruktoriai:
    ```cpp
    Student()                               // ← default: "Nezinomas", 0, 0.0
    Student(const char* v, int a, double p) // ← parametrinis: konkretus studentas
    ```
    Tas pats overloading principas – tik skirtinga klasė ir skirtingi duomenys.  
    **Konstruktorius = `create()` su parametrais iš C 08–09 etapų.**

---

<a name="16"></a>
## 1️⃣6️⃣ Etapas: `16_Defining_METHODS_OUTSIDE`

> [`16_Defining_METHODS_OUTSIDE`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/)

!!! note "🎯 Tikslas"
    Suprasti, kad metodų **apibrėžimus** galima (ir dažnai reikia) iškelti **už klasės ribų** – ir kaip tai daryti su `Stack::` scope resolution operatoriumi. Tada – pažįstamas žingsnis: skaidymas į `.h` ir `.cpp`.

---

### 0 žingsnis: Bazinis kodas – viskas klasės viduje → [`00`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/00/)

> Failas: [`16_Defining_METHODS_OUTSIDE/00/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/00/usestack.cpp)

!!! quote "Mintis / ketinimas"
    Turima versija iš 15 etapo – visi metodai **inline** klasės viduje. Veikia, bet klasė tampa didelė ir sunkiai skaitoma.

???+ "📄 `usestack.cpp` – viskas viduje (atspirties taškas)"
    ```cpp
    --8<-- "code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/00/usestack.cpp"
    ```

!!! warning "Klasė auga – metodų apibrėžimai užgožia struktūrą"
    Kol metodai trumpi – dar gerai. Bet kai `push()`, `pop()` taps sudėtingesni,  
    klasės **deklaracija** ir metodų **apibrėžimai** susipins į vieną krūvą.  
    C kalboje šią problemą sprendėme `.h` + `.c` skaidymu – C++ turi tą patį sprendimą.

---

### 1 žingsnis: Metodai išorėje be `Stack::` → [`01_NC_Naive`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/01_NC_Naive/)

> Failas: [`16_Defining_METHODS_OUTSIDE/01_NC_Naive/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/01_NC_Naive/usestack.cpp)

!!! quote "Mintis / ketinimas"
    „Ištrauksiu metodų apibrėžimus iš klasės – kaip C kalboje funkcijas iš `struct {}` bloko."

???+ "📄 `usestack.cpp` – metodai išorėje, bet be `Stack::`"
    ```cpp
    --8<-- "code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/01_NC_Naive/usestack.cpp"
    ```

=== "🔨 Kompiliavimas"
    ```bash
    clang++ usestack.cpp -o app
    ```

=== "💥 Klaidos"
    ```
    usestack.cpp:16:1: error: constructor cannot have a return type
    usestack.cpp:17:1: error: constructor cannot have a return type
    usestack.cpp:18:6: error: 'top' was not declared in this scope
    usestack.cpp:19:16: error: 'top' was not declared in this scope
    usestack.cpp:20:28: error: 'stack' was not declared in this scope
    ...
    ```

!!! failure "🤔 Metodai yra – kodėl `top` ir `stack` nebepasiekiami?"

??? question "Pamąstykite prieš skaitant toliau..."
    `top` ir `stack` yra `Stack` klasės **`private`** nariai.  
    Funkcija **už klasės ribų** jų nemato – ji nėra klasės narė.  
    Kaip pasakyti kompiliatoriui, kad ši funkcija **priklauso** `Stack`?

!!! tip "Atsakymas: `Stack::` – scope resolution operatorius"
    `::` nurodo, **kuriai klasei** priklauso metodas.  
    Be jo – tai paprastos globalios funkcijos, kurios nemato `private` narių.

---

### 2 žingsnis: Metodai išorėje su `Stack::` → [`02_OK`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/02_OK/)

> Failas: [`16_Defining_METHODS_OUTSIDE/02_OK/usestack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/02_OK/usestack.cpp)

!!! quote "Mintis / ketinimas"
    „Pridėsiu `Stack::` prieš kiekvieną metodo apibrėžimą – tada kompiliatorius žinos, kad jie priklauso klasei."

???+ "📄 `usestack.cpp` – metodai išorėje su `Stack::`"
    ```cpp
    --8<-- "code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/02_OK/usestack.cpp"
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang++ usestack.cpp -o app   # ✅
    ./app
    ```

=== "⌨️➔🖥️"
    ```
    X
    EDCBA
    ```

!!! success "✅ Veikia!"

!!! tip "`Stack::` – scope resolution operatorius `::`"
    ```cpp
    bool Stack::isEmpty() { return 0 == top; }
    //   ^^^^^             ← „ši funkcija priklauso Stack klasei"
    //         ↑ top – pasiekiamas, nes funkcija yra Stack narė
    ```
    Klasėje lieka tik **deklaracijos** (sąsaja).  
    Už klasės – **apibrėžimai** (implementacija).  
    Tai pažįstama struktūra: kaip C kalboje `stack.h` + `stack.c`.

---

### 3 žingsnis: Skaidymas į `stack.h` + `stack.cpp` → [`03_OK`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/03_OK/)

> Failai: [`stack.h`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/03_OK/stack.h) · [`stack.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/03_OK/stack.cpp) · [`user.cpp`](https://github.com/ViktorasGolubevasMIF/cpp-2026/blob/main/code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/03_OK/user.cpp)

!!! quote "Mintis / ketinimas"
    „02_OK viename faile jau atrodo kaip du moduliai – tiesiog fiziškai juos atskirkime."

=== "📄 `stack.h`"
    ```cpp
    --8<-- "code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/03_OK/stack.h"
    ```

=== "📄 `stack.cpp`"
    ```cpp
    --8<-- "code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/03_OK/stack.cpp"
    ```

=== "📄 `user.cpp`"
    ```cpp
    --8<-- "code/evolution/stack-2026/16_Defining_METHODS_OUTSIDE/03_OK/user.cpp"
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang++ stack.cpp user.cpp -o app   # ✅
    ./app
    ```

=== "⌨️➔🖥️"
    ```
    X
    EDCBA
    ```

!!! success "✅ Pažįstama struktūra – kaip C modulis, tik C++!"

!!! abstract "C modulis vs C++ klasės modulis"
    | | C (03–09 etapai) | C++ (16 etapas) |
    |--|------------------|-----------------|
    | **Sąsaja** | `stack.h` – `struct` + funkcijų deklaracijos | `stack.h` – `class` su deklaracijomis |
    | **Implementacija** | `stack.c` – funkcijų apibrėžimai | `stack.cpp` – `Stack::` metodų apibrėžimai |
    | **Naudotojas** | `user.c` + `#include "stack.h"` | `user.cpp` + `#include "stack.h"` |
    | **Kompiliavimas** | `gcc stack.c user.c -o app` | `clang++ stack.cpp user.cpp -o app` |

??? note "`#pragma once` vietoj `#ifndef` apsaugos"
    `#pragma once` – modernus, kompaktiškas būdas apsaugoti nuo dvigubo įtraukimo.  
    Funkcionaliai lygiavertis klasikiniam:
    ```cpp
    #ifndef STACK_H
    #define STACK_H
    // ...
    #endif
    ```
    Beveik visi modernūs kompiliatoriai palaiko `#pragma once`.

??? question "Refleksija: kas pasikeitė nuo C modulio?"
    Struktūra **labai panaši** – ir tai ne atsitiktinumas.  
    C++ `.h`/`.cpp` moduliavimas yra tiesioginė C tradicijos tąsa.  
    
    Skirtumas: `stack.h` dabar slepia **implementaciją** per `private:` –  
    ne per opaque pointer ir atskirą `.c` failą, o per kalbos mechanizmą.

---

## 🎯 Santrauka: 11–16 etapai

| Nr | Etapas | Žingsniai | Atradimas |
|----|--------|-----------|-----------|
| **11** | Discovering_CPP_Struct | `00` → `01_NC_Naive` | Funkcija klasės viduje ≠ globali funkcija |
| **12** | ENCAPSULATING_in_CPP_Struct | `01_OK` → `02_OK_Attack` | `this` + encapsulation ≠ information hiding |
| **13** | HIDING_in_CLASS | `01_NC_Naive` → `02_OK` → `03_NC_Attack` | `class` = `private` pagal nutylėjimą; kompiliatorius saugo |
| **14** | CONSTRUCTOR_and_DESTRUCTOR | `00` → `01_OK` | Kūrimas = inicializavimas; naikinimas = automatinis |
| **15** | OVERLOADING_CONSTRUCTORS | `00` → `01_OK` | Overloading: tas pats vardas, skirtingi parametrai |
| **16** | DEFINING_METHODS_OUTSIDE | `00` → `01_NC_Naive` → `02_OK` → `03_OK` | `Stack::` + `.h`/`.cpp` moduliavimas |

!!! success "🎉 C++ Stack evoliucijos II dalis – baigta!"
    Nuo `struct` su C stiliaus funkcijomis iki pilno C++ modulio su:
    - ✅ `class` + `private:`/`public:` – information hiding
    - ✅ Konstruktoriai (default + parametrinis) – automatinis lifecycle
    - ✅ Destruktorius – automatinis naikinimas
    - ✅ Overloading – tas pats vardas, skirtingi parametrai
    - ✅ `Stack::` + `.h`/`.cpp` – pažįstamas moduliavimas
