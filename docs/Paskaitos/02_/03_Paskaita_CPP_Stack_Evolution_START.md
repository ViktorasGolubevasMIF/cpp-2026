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
| **12** | [Anatomy_of_CPP_Struct](#12) | `01_OK` → `02_OK_Attack` | `this` gimimas; `struct` dar atviras |
| **13** | [HIDING_in_CLASS](#13) | `01_NC_Naive` → `02_OK` → `03_NC_Attack` | Paradigminis switch: kompiliatorius saugo! |

---

<a name="11"></a>
## 1️⃣1️⃣ Etapas: `11_Discovering_CPP_Struct`

> [`11_Discovering_CPP_Struct`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/11_Discovering_CPP_Struct/)

!!! note "🎯 Tikslas"
    Suprasti, kas nutinka kai C funkcijų apibrėžimus **perkeliame į `struct {}` bloką** – ir `main()` jas vis dar kviečia C stiliumi.

---

### 0 žingsnis: Referentinis taškas → `00`

> Failas: `11_Discovering_CPP_Struct/00/usestack.cpp`

!!! quote "Mintis / ketinimas"
    Pradedame nuo **to paties kodo** kaip `05/03_OK/usestack.c` – tik plėtinys `.cpp`.  
    Funkcijos yra **išorėje** `struct {}`. Tai veikia – ir C, ir C++ atveju.

???+ "📄 `usestack.cpp` – C stilius, funkcijos išorėje"
    ```cpp
    #include <stdio.h>
    // ===> stack dalis
    #define SIZE 5
    struct Stack {
        char stack[SIZE];
        int top;
    };

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

    // ===> user dalis
    int main(void) {
        char c;
        struct Stack st1, st2;
        init(&st1);
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

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang usestack.cpp -o app   # ✅
    ./app
    ```

!!! success "✅ Veikia – kaip ir C atveju. Tai mūsų atspirties taškas."

---

### 1 žingsnis: Funkcijos **į** `struct` vidų, `main()` nepakeistas → `01_NC_Naive`

> Failas: `11_Discovering_CPP_Struct/01_NC_Naive/usestack.cpp`

!!! quote "Mintis / ketinimas"
    „Perkeliu funkcijų apibrėžimus į `struct Stack {}` vidų. `main()` lieka nepakeistas – turėtų vis tiek veikti, ne?"

???+ "📄 `usestack.cpp` – funkcijos viduje, `main()` C stiliumi"
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
        init(&st1);                // ← C stilius – globaliai ieško 'init'
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
        `static void reset(...)` viduje `struct` kompiliatorius priima be skundo.  
        `static` klasės viduje reiškia visai **ką kitą** nei C kalboje – tai klasės lygio narys, ne egzemplioriaus. Prie šio `static` grįšime vėliau.

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
## 1️⃣2️⃣ Etapas: `12_ENCAPSULATING_in_CPP_Struct`

> [`12_ENCAPSULATING_in_CPP_Struct`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/12_ENCAPSULATING_in_CPP_Struct/)

!!! note "🎯 Tikslas"
    Išmesti `struct Stack *pst` parametrą – suprasti `this`. Ir pamatyti, kad `struct` su metodais dar **nesuteikia** information hiding.

---

### 1 žingsnis: Išmetame `pst` parametrą → `01_OK`

!!! quote "Mintis / ketinimas"
    "`st1.init(&st1)` – absurdas. Objektas jau žino, kas jis yra – išmeskime `&st1` kaip parametrą!"

???+ "📄 `usestack.cpp`"
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
            if (!isFull()) stack[top++] = c;
        }
        char pop() {
            if (!isEmpty()) return stack[--top];
            return '\0';
        }
    };
    int main(void) {
        char c;
        struct Stack st1;
        Stack st2;              // ← C++ leidžia be "struct"!
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
    Kompiliatorius *automatiškai* perduoda nuorodą į kviečiantį objektą. Tas paslėptas parametras vadinamas **`this`**.

??? info "Kas yra `this`?"
    ```cpp
    // Tai ką mes rašome:
    void init() { top = 0; }

    // Tai ką kompiliatorius „mato" po gaubtu:
    void init(Stack* const this) { this->top = 0; }
    ```
    Kai rašome `st1.init()` – kompiliatorius išverčia į `Stack::init(&st1)`.  
    `this` visada rodo į **tą objektą**, kuris kvietė metodą.

---

### 2 žingsnis: Ataka vis dar pavyksta → `02_OK_Attack`

!!! quote "Mintis / ketinimas"
    „Turime `struct` su metodais – ar duomenys dabar saugūs?"

???+ "📄 Atakos kodas `main()` dalyje"
    ```cpp
    // ... (tas pats struct Stack su metodais)
    int main(void) {
        // ... įprastas naudojimas su st1 ...
        st2.init();
        while ((!st2.isFull()) && ('\n' != (c = getchar()))) st2.push(c);
    //// ATTACK!
    st2.stack[st2.top++] = '!';
    ////
        while (!st2.isEmpty()) putchar(st2.pop());
        putchar('\n');
        return 0;
    }
    ```

=== "🔨 = ⚙️➔🔗➔🚀"
    ```bash
    clang usestack.cpp -o app   # ✅ kompiliuoja!
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

### 1 žingsnis: Plikas `class` be specifikatorių → `01_NC_Naive`

> Failas: `13_HIDING_in_CLASS/01_NC_Naive/usestack.cpp`

!!! quote "Mintis / ketinimas"
    „C++ turi specialų raktažodį objektams – `class`. Pakeičiu `struct` į `class` ir viskas turėtų veikti kaip anksčiau."

???+ "📄 `usestack.cpp` – tik `struct` → `class`"
    ```cpp
    #include <stdio.h>
    #define SIZE 5

    class Stack {       // ← tik šis pakeitimas!
        char stack[SIZE];
        int top;

        void init()       { top = 0; }
        int isEmpty()     { return 0 == top; }
        int isFull()      { return SIZE == top; }
        void push(char c) { if (!isFull()) stack[top++] = c; }
        char pop()        { if (!isEmpty()) return stack[--top]; return '\0'; }
    };

    int main(void) {
        char c;
        Stack st1, st2;
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
    clang usestack.cpp -o app
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

### 2 žingsnis: Pridedame `public:` → `02_OK`

> Failas: `13_HIDING_in_CLASS/02_OK/usestack.cpp`

!!! quote "Mintis / ketinimas"
    „Metodai turi būti pasiekiami iš išorės – dedu `public:`. Duomenys tegul lieka `private:`."

???+ "📄 `usestack.cpp` – su `public:`/`private:`"
    ```cpp
    #include <stdio.h>
    #define SIZE 5

    class Stack {
    private:                        // ← duomenys paslėpti
        char stack[SIZE];
        int top;
    public:                         // ← metodai prieinami
        void init()       { top = 0; }
        int isEmpty()     { return 0 == top; }
        int isFull()      { return SIZE == top; }
        void push(char c) { if (!isFull()) stack[top++] = c; }
        char pop()        { if (!isEmpty()) return stack[--top]; return '\0'; }
    };

    int main(void) {
        char c;
        Stack st1, st2;
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

### 3 žingsnis: Ataka → `03_NC_Attack`

> Failas: `13_HIDING_in_CLASS/03_NC_Attack/usestack.cpp`

!!! quote "Mintis / ketinimas"
    „I dalyje su C `struct` ataka **pavyko** (06 etapas). Patikriname ar čia tas pats..."

???+ "📄 Atakos kodas `main()` dalyje"
    ```cpp
    // ... (tas pats class Stack su private:/public:)

    int main(void) {
        // ...
        //// ATTACK!
        st2.stack[st2.top++] = '!';
        ////
        // ...
    }
    ```

=== "🔨 Kompiliavimas"
    ```bash
    clang usestack.cpp -o app
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
