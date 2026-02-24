# Stack ADT Evoliucija C++ kalboje
## II dalis: Nuo C `struct` iki C++ `class` – Klasės Anatomija

---

> **Tęsinys po:** I dalies 9 etapų (C ADT su opaque pointer ir factory pattern)  
> **Tikslas:** Atrasti C++ klasę kaip *naują kartą* `struct` – per tikras kompiliavimo klaidas  
> **Metodologija:** Tas pats principas – klaidos = mokymo įrankis!

---

## 🔁 Prisiminkime: Kur sustojome?

!!! success "I dalis: Pasiekėme pilną C ADT (09 etapas)"
    ```c
    struct Stack *s = create();   // rankinis create()
    push(s, 'a');                 // C sintaksė: funkcija(objektas, ...)
    char c = pop(s);
    destroy(s);                   // ← REIKIA PRISIMINTI!
    ```
    
    **Pasiekėme:**
    - ✅ Information hiding (opaque pointer)
    - ✅ Encapsulation (duomenys + funkcijos modulyje)
    - ✅ Multiple instances (`*pst1`, `*pst2`, ...)
    - ✅ Lifecycle management (`create()` → `destroy()`)

??? warning "Bet C kalboje liko neišspręstų problemų..."
    - **Manual lifecycle:** programuotojas *privalo* prisiminti `destroy()` – užmirštus memory leak
    - **Sintaksė:** `push(pst, 'a')` – objektas kaip parametras, ne kvietėjas
    - **No operator overloading:** negalima natūralaus `pst1 = pst2;`
    - **Opaque pointer biurokratija:** `create()`/`destroy()` – rankinis darbas, kurį galėtų automatizuoti kompiliatorius

---

## 🚀 Šuolis: C ADT → C++ Class

```c
// C (09 etapas) – rankinis valdymas
struct Stack *s = create();
push(s, 'a');
char c = pop(s);
destroy(s);            // ← reikia prisiminti
```

```cpp
// C++ – automatinis valdymas
Stack s;               // konstruktorius = automatinis create()
s.push('a');           // metodo sintaksė: objektas.funkcija(...)
char c = s.pop();
                       // destruktorius = automatinis destroy() 🎉
```

!!! tip "Paradigminis pokytis"
    **C:** funkcija valdo objektą → `push(pst, 'a')`  
    **C++:** objektas turi funkcijas → `s.push('a')`  
    
    Tai ne tik sintaksės skirtumas – tai kitoks mąstymo būdas apie kodą.

---

## 🗺️ II dalies kelionė

| Nr | Etapas | Koncepcija | Esmė |
|----|--------|------------|------|
| **11** | [Discovering_CPP_Struct](#11) | Funkcijos *į* `struct {}` vidų – C stiliaus kvietimas | NC: `main()` nebemato funkcijų globaliai |
| **12** | [Anatomy_of_CPP_Struct](#12) | Funkcijos viduje + `this` + parametrų išmetimas | OK: C++ `struct` semantika |
| **13** | [Discovering_CLASS](#13) | `struct` → `class` + `public:`/`private:` | Paradigminis switch: kompiliatorius saugo! |

---

<a name="11"></a>
## 1️⃣1️⃣ Etapas: `11_Discovering_CPP_Struct`

> [`11_Discovering_CPP_Struct`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/11_Discovering_CPP_Struct/)

!!! note "🎯 Tikslas"
    Suprasti, kas nutinka kai C funkcijų apibrėžimus **perkeliame į `struct {}` bloką** – ir `main()` jas vis dar kviečia C stiliumi.

!!! info "🔍 Ką darysime"
    - Paimsime `05_Defining_USER_TYPE/03_OK/usestack.c` kodą
    - Funkcijų apibrėžimus **fiziškai perkeliame į** `struct Stack {}` vidų (su visais `struct Stack *pst` parametrais)
    - `main()` lieka nepakeistas – kviečia C stiliumi: `init(&st1)`
    - Skaitome klaidas

---

### 1 žingsnis: Funkcijos į `struct` vidų, `main()` nepakeistas → `01_NC_Naive`

> Failas: `11_Discovering_CPP_Struct/01_NC_Naive/usestack.cpp`

!!! quote "Mintis / ketinimas"
    „C++ `struct` gali turėti funkcijas viduje – įdėsiu jas ten. `main()` vis tiek ras jas, kaip ir anksčiau."

???+ "📄 `usestack.cpp`"
    ```cpp
    #include <stdio.h>
    // ===> stack dalis
    #define SIZE 5
    struct Stack {
        char stack[SIZE];
        int top;
        static void reset(struct Stack *pst) { pst->top = 0; }  // ← viduje!
        void init(struct Stack *pst)         { pst->top = 0; }
        int isEmpty(struct Stack *pst)       { return 0 == pst->top; }
        int isFull(struct Stack *pst)        { return SIZE == pst->top; }
        void push(struct Stack *pst, char c) {
            if (!isFull(pst)) pst->stack[pst->top++] = c;
        }
        char pop(struct Stack *pst) {
            if (!isEmpty(pst)) return pst->stack[--pst->top];
            return '\0';
        }
    };
    // ===> user dalis
    int main(void) {
        char c;
        struct Stack st1, st2;
        init(&st1);                                              // ← C stilius
        while ((!isFull(&st1)) && ('\n' != (c = getchar()))) push(&st1, c);
        while (!isEmpty(&st1)) putchar(pop(&st1));
        putchar('\n');
        init(&st2);
        while ((!isFull(&st2)) && ('\n' != (c = getchar()))) push(&st2, c);
        while (!isEmpty(&st2)) putchar(pop(&st2));
        putchar('\n');
        return 0;
    }
    ```

=== "🔨 Kompiliavimas"
    ```bash
    clang usestack.cpp -o app
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
        Pastebėsite, kad `static void reset(...)` viduje `struct` kompiliatorius priima be skundo.  
        Tai ne atsitiktinumas – `static` klasės viduje reiškia visai **ką kitą** nei C kalboje. Tai „klasės lygio" narys, ne egzemplioriaus. Prie šio `static` grįšime vėliau – kol kas tiesiog atkreipkite dėmesį.

---

### 2 žingsnis: `main()` pakeičiamas į objekto sintaksę → `02_OK`

> Failas: `11_Discovering_CPP_Struct/02_OK/usestack.cpp`

!!! quote "Mintis / ketinimas"
    „Jei funkcijos priklauso `Stack` – kviesiu jas per objektą: `st1.init(...)`."

???+ "📄 `usestack.cpp` – tik `main()` pakeistas"
    ```cpp
    #include <stdio.h>
    #define SIZE 5
    struct Stack {
        char stack[SIZE];
        int top;
        static void reset(struct Stack *pst) { pst->top = 0; }
        void init(struct Stack *pst)         { pst->top = 0; }
        int isEmpty(struct Stack *pst)       { return 0 == pst->top; }
        int isFull(struct Stack *pst)        { return SIZE == pst->top; }
        void push(struct Stack *pst, char c) {
            if (!isFull(pst)) pst->stack[pst->top++] = c;
        }
        char pop(struct Stack *pst) {
            if (!isEmpty(pst)) return pst->stack[--pst->top];
            return '\0';
        }
    };
    int main(void) {
        char c;
        struct Stack st1, st2;
        st1.init(&st1);                                          // ← objekto sintaksė
        while ((!st1.isFull(&st1)) && ('\n' != (c = getchar()))) st1.push(&st1, c);
        while (!st1.isEmpty(&st1)) putchar(st1.pop(&st1));
        putchar('\n');
        st2.init(&st2);
        while ((!st2.isFull(&st2)) && ('\n' != (c = getchar()))) st2.push(&st2, c);
        while (!st2.isEmpty(&st2)) putchar(st2.pop(&st2));
        putchar('\n');
        return 0;
    }
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang usestack.cpp -o app   # ✅
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
## 1️⃣2️⃣ Etapas: `12_Anatomy_of_CPP_Struct`

> [`12_Anatomy_of_CPP_Struct`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/12_Anatomy_of_CPP_Struct/)

!!! note "🎯 Tikslas"
    Suprasti, kodėl `struct Stack *pst` parametras nebereikalingas – ir kas jį pakeičia.

!!! info "🔍 Ką darysime"
    - Iš 11/02_OK turime `st1.init(&st1)` – objektas perduodamas **du kartus**
    - Išmetame `struct Stack *pst` parametrą iš visų funkcijų
    - Pakeičiame `pst->top` į tiesiog `top`, `pst->stack` į `stack`
    - Suprantame, kas yra `this`

---

### 1 žingsnis: Išmetame `pst` parametrą → `01_OK`

!!! quote "Mintis / ketinimas"
    "`st1.init(&st1)` – absurdas. Objektas jau žino, kas jis yra. Išmeskime `&st1` kaip parametrą!"

???+ "📄 `usestack.cpp` – `pst` parametro nebėra"
    ```cpp
    #include <stdio.h>
    #define SIZE 5

    struct Stack {
        char stack[SIZE];
        int top;

        void init()       { top = 0; }            // ← pst išmestas, top tiesiogiai
        int isEmpty()     { return 0 == top; }
        int isFull()      { return SIZE == top; }
        void push(char c) {
            if (!isFull()) stack[top++] = c;       // ← stack, top – tiesiogiai
        }
        char pop() {
            if (!isEmpty()) return stack[--top];
            return '\0';
        }
    };

    int main(void) {
        char c;
        struct Stack st1, st2;
        st1.init();                                // ← švaru!
        while ((!st1.isFull()) && ('\n' != (c = getchar()))) st1.push(c);
        while (!st1.isEmpty()) putchar(st1.pop());
        putchar('\n');
        st2.init();
        while ((!st2.isFull()) && ('\n' != (c = getchar()))) st2.push(c);
        while (!st2.isEmpty()) putchar(st2.pop());
        putchar('\n');
        return 0;
    }
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang usestack.cpp -o app   # ✅
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

!!! tip "Klasės anatomija – kas nutiko su `pst`?"
    **Prieš (11/02_OK – dar su `pst`):**
    ```cpp
    void init(struct Stack *pst) { pst->top = 0; }
    // kvietimas:
    st1.init(&st1);   // ← absurdas: objektas du kartus
    ```
    
    **Po (12/01_OK – be `pst`):**
    ```cpp
    void init() { top = 0; }
    // kvietimas:
    st1.init();        // ← švaru
    ```
    
    Kompiliatorius *automatiškai* perduoda nuorodą į kviečiantį objektą. Tas paslėptas parametras vadinamas **`this`**.

??? info "Kas yra `this`?"
    Kiekvienoje ne-`static` klasės funkcijoje kompiliatorius *netiesiogiai* prideda parametrą:
    ```cpp
    // Tai ką mes rašome:
    void init() { top = 0; }
    
    // Tai ką kompiliatorius „mato" po gaubtu:
    void init(Stack* const this) { this->top = 0; }
    ```
    
    Kai rašome `st1.init()` – kompiliatorius išverčia į `Stack::init(&st1)`.  
    `this` visada rodo į **tą objektą**, kuris kvietė metodą – todėl `st2.init()` operuoja su `st2`, ne `st1`.

??? question "Refleksija: kas liko neišspręsta?"
    - Visi nariai vis dar **vieši** – bet kuris `main()` kodas gali daryti `st1.top = 999;`
    - Vis dar rašome `struct Stack st1` – C++ leidžia tiesiog `Stack st1`
    - `init()` vis dar reikia kviesti rankiniu būdu – ar galėtų tai daryti automatiškai?

---

<a name="13"></a>
## 1️⃣3️⃣ Etapas: `13_Discovering_CLASS`

> [`13_Discovering_CLASS`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/13_Discovering_CLASS/)

!!! note "🎯 Tikslas"
    Suprasti, kuo `class` skiriasi nuo `struct` – ir kodėl tai yra **paradigminis pokytis**.

!!! info "🔍 Ką darysime"
    - Pakeisti `struct` → `class`
    - Pridėti `public:` / `private:`
    - Pamatyti, kaip kompiliatorius *automatiškai* saugo duomenis

---

### 1 žingsnis: `struct` → `class` + prieigos specifikatoriai → `01_OK`

!!! quote "Mintis / ketinimas"
    „C++ turi specialų raktažodį objektams – `class`. Pabandykime!"

???+ "📄 `usestack.cpp` – `class` su `public:`/`private:`"
    ```cpp
    #include <stdio.h>
    #define SIZE 5
    
    class Stack {
    private:
        char stack[SIZE];   // ← tik klasės vidus gali liesti
        int top;
    
    public:
        void init()       { top = 0; }
        int isEmpty()     { return 0 == top; }
        int isFull()      { return SIZE == top; }
        void push(char c) { if (!isFull()) stack[top++] = c; }
        char pop()        { if (!isEmpty()) return stack[--top]; return '\0'; }
    };
    
    int main(void) {
        char c;
        Stack st1, st2;              // ← jau be "struct"!
        st1.init();
        while ((!st1.isFull()) && ('\n' != (c = getchar()))) st1.push(c);
        while (!st1.isEmpty()) putchar(st1.pop());
        putchar('\n');
        st2.init();
        while ((!st2.isFull()) && ('\n' != (c = getchar()))) st2.push(c);
        while (!st2.isEmpty()) putchar(st2.pop());
        putchar('\n');
        return 0;
    }
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang usestack.cpp -o app   # ✅
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

### 2 žingsnis: Pabandome „ataką" → `02_NC_Attack`

!!! quote "Mintis / ketinimas"
    „I dalyje su C moduliu irgi veikė – bet tada `05_OK_Attack` parodė, kad duomenys vieši. Patikriname!"

???+ "📄 `attack.cpp` – bandymas tiesiogiai pasiekti privačius narius"
    ```cpp
    #include <stdio.h>
    #define SIZE 5
    // ... (tas pats class Stack kaip aukščiau)
    
    int main(void) {
        Stack st1;
        st1.top = 0;              // ← bandome tiesiogiai!
        st1.stack[0] = 'X';      // ← bandome tiesiogiai!
        return 0;
    }
    ```

=== "🔨 Kompiliavimas"
    ```bash
    clang attack.cpp -o app
    ```

=== "💥 Klaidos"
    ```
    attack.cpp:XX:X: error: 'top' is a private member of 'Stack'
    attack.cpp:XX:X: error: 'stack' is a private member of 'Stack'
    2 errors generated.
    ```

!!! success "✅ Ataka nepavyko! Kompiliatorius saugo!"

!!! tip "Paradigminis pokytis – lyginimas su C"
    | | C (07-09 etapai) | C++ (13 etapas) |
    |--|------------------|-----------------|
    | **Information hiding mechanizmas** | Opaque pointer + atskiras `.c` failas | `private:` raktažodis |
    | **Kas saugo?** | Programuotojas (taisyklė: niekada neliesti vidaus) | **Kompiliatorius** (klaida kompiliavimo metu) |
    | **Kiek failų reikia?** | `stack.h` + `stack.c` + `user.c` | Galima viename faile! |
    | **Atakos galimybė?** | Teoriškai – taip (per kitą `.c` failą) | Ne – kompiliatorius blokuoja |

??? question "Refleksija: kas dar liko?"
    Turime `class` su `private:`/`public:` – tai didžiulis žingsnis!  
    Bet `init()` vis dar kviečiamas rankiniu būdu...  
    
    **Kitas klausimas:** ar galėtų objektas *pats* pasirūpinti savo inicializacija?  
    → **14 etapas:** Konstruktorius – automatinis `init()`! 🚀

---

## 🎯 II dalies santrauka (11–13 etapai)

| Nr | Etapas | Esmė | Atradimas |
|----|--------|------|-----------|
| **11** | Discovering_CPP_Struct | NC: funkcijos ne viduje | C++ `struct` ≠ C `struct` |
| **12** | Anatomy_of_CPP_Struct | Funkcijos viduje, `this` | Metodai + implicitinis `this` |
| **13** | Discovering_CLASS | `private:`/`public:` | Kompiliatorius saugo duomenis |

!!! abstract "Klasės anatomija – apibendrinimas"
    ```cpp
    class Stack {
    // ──────────────────────────────────
    private:             // ← prieigos specifikatorius
        char stack[5];   // ← duomenų nariai (data members)
        int top;         //    (paslėpti nuo išorės)
    // ──────────────────────────────────
    public:              // ← prieigos specifikatorius
        void init();     // ← funkcijų nariai (member functions / metodai)
        void push(char c);
        char pop();
        int isEmpty();
        int isFull();
    // ──────────────────────────────────
    };   // ← kabliataškis! (skirtingai nuo funkcijų)
    ```
    
    **`struct` vs `class` C++ kalboje:**  
    Vienintelis formalus skirtumas – numatytoji prieiga:  
    `struct` → nariai **public** pagal nutylėjimą  
    `class` → nariai **private** pagal nutylėjimą

**Kitas žingsnis:** Konstruktorius ir destruktorius – automatinis lifecycle! 🚀
