# Stack ADT Evoliucija C kalboje
## I dalis: Nuo "naivaus" Monolito iki "profesionalaus" ADT C kalboje

---

### 🎯 Kodėl `Stack`? Kodėl evoliucija?

!!! abstract "ADT vs Primityvūs Tipai"
    **Abstraktus Duomenų Tipas (ADT)** fundamentaliai skiriasi nuo "mokyklinių" primityvių tipų (`int`, `char`, `float`).
    
    **Primityvūs tipai** - juos visų pirma suvokiame per **reikšmių aibę** ir taikytinas operacijas:

    - `int` → ..., -2, -1, 0, 1, 2, 3, ...
    
    - `char` → 'a', 'b', 'c', ..., 'z', ...
    
    - `float` → 3.14, -2.71, ...
    
    **ADT** - juos suvokiame tik per **operacijas**, o ne per reikšmes:
    
    - `Stack` → `push()`, `pop()`, `isEmpty()`, `isFull()`
    
    - Mums **nerūpi** kaip stekas atmintyje atrodo
    
    - Mums **svarbu** tik ką galime su juo daryti, t.y. žinoti jo elgseną.
!!! question "Kodėl pasirinktas Stack?"
    **Stack (Stekas)** - idealus ADT mokymosi pavyzdys:

    ✅ **Paprasta semantika:** LIFO (Last In, First Out) - "paskutinis įėjo, pirmas išėjo"
    
    ✅ **Minimalios operacijos:** Tik 2 pagrindinės - `push()` ir `pop()`
    
    ✅ **Aiški teisingo/blogo elgesio riba:** Lengva demonstruoti klaidas
    
    ✅ **Universalus:** Naudojamas visur - funkcijų iškvietimų stekas, expression evaluation, undo mechanizmai
    
    ✅ **Evolucionuoja į C++ klasę:** Puikus tiltas tarp C ir C++

!!! tip "Evoliucijos metodologija"
    Šioje kelionėje **kiekvienas etapas sprendžia ankstesnio problemą**, mokydamasis iš bandymų ir klaidų:
    
    - **Kompiliavimo klaidos** → mokome extern/static
    - **Linkavimo klaidos** → suprantame multiple definition
    - **Runtime klaidos** → išmokstame pass-by-pointer
    - **Saugumo problemos** → atrandame information hiding
    
    >**Kompiliavimo ir linkavimo klaidos bei perspėjimai yra mokymosi įrankis, o ne kliūtis!**

---

## 9-ių Etapų Evoliucija

!!! abstract ""
    **01-03:** Modulio gimimas (C modulis su header failais)
    
    **04:** Information hiding (`static`, internal linkage)
    
    **05-06:** User-Defined Type (`struct` + modulis)
    
    **07:** Opaque pointer (forward declaration)
    
    **08-09:** Pilnavertis ADT (factory pattern, lifecycle)

---

## 1️⃣Etapas: "Stekas Programoje"

> [`01_Running_PROGRAM`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/01_Running_PROGRAM/)

!!! note "🎯 Tikslas"
    Sukurti **paprasčiausią veikiančią** steko realizaciją - pradėti nuo monolitinės programos.

!!! info "🔍 Ką darysime"
    - Parašysime **visą kodą viename faile** - monolitas
    - Naudosime **globalius kintamuosius** steko duomenims
    - Suprasime **monolito privalumus** ir **ribotumus**

---

### 1 sprendimas: "Naivus"

> [`01_Running_PROGRAM/01_OK_Naive`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/01_Running_PROGRAM/01_OK_naive/)

!!! quote "sumanymas/ketinimas"
    Paprasčiausia įmanoma realizacija - viskas viename faile: duomenys, funkcijos, `main()`. Greitas prototipavimas!

=== "usestack.c"

    ???+ "📄 01_Running_PROGRAM/01_OK_naive/usestack.c"

        ```c linenums="1" hl_lines="6-7 39-58"
        --8<-- "code/evolution/stack-2026/01_Running_PROGRAM/01_OK_naive/usestack.c"
        ```

=== "🔨 = ⚙️➔🔗➔🚀"

    ```bash
    gcc usestack.c -o app  # ✅
    ./app
    ```
    arba (win)
    ```bash
    gcc usestack.c -o app
    app
    ```

=== "⌨️➔🖥️"

    ```
    123
    321
    ```

---

#### ✅ Veikia! Bet...

??? success "Privalumai"
    - **Kompiliatorius mato viską:** Vienas kompiliavimo vienetas - viską "mato"
    - **Paprastas build:** Viena komanda `gcc usestack.c -o app`
    - **Greitas prototipavimas:** Įdarbinimo pokalbio "quick solution"

??? danger "Problemos, pavojai..."
    - **Globalūs kintamieji:** `stack[]` ir `top` matomi visur - jokios apsaugos
    - **Tik vienas stekas:** Negalima turėti kelių stekų vienu metu
    - **Pakartotinis naudojimas:** Kaip kitas projektas panaudotų šį kodą?
    - **Prižiūrėjimas:** 1000+ eilučių viename faile → sunku orientuotis

---

### 💡 Pamokos

!!! tip "Monolitinė programa"
    - **Paprasčiausia pradžia** - viskas viename faile
    - **Globalūs duomenys** - prieinami be apsaugos
    - **Ribojimas** - tik vienas steko egzempliorius
    - **Sprendimas → Etapas 02:** Padalinti į failus

---

## 2️⃣Etapas: "Stekas Faile"

> [`02_Decomposing_to_UNITS`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/02_Decomposing_to_UNITS/)

!!! note "🎯 Tikslas"
    Išmokti **fizinio padalijimo** į failus - atskirti **paslaugą** (`stack.c`) nuo **vartotojo** (`user.c`).

!!! info "🔍 Ką darysime"
    - Padalinsime kodą į **du failus** - `stack.c` ir `user.c`
    - Susidursime su **kompiliavimo klaidomis** - trūksta aprašų
    - Susidursime su **linkinimo klaidomis** - per daug apibrėžimų
    - Isitikinsime, kad `#include "file.c"` yra antipattern
    - **Pamatysime skirtumus** tarp `gcc` ir `clang` kompiliatorių elgesio (tiesa - skirtumas kontroliuojamas)

---

### 1 sprendimas: Kodo padalijimas

> [`02_Decomposing_to_UNITS/01_NC`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/02_Decomposing_to_UNITS/01_NC/)

!!! quote "sumanymas/ketinimas"
    Tiesiog padalinsiu kodą į du failus ir kompiliuoju atskirai, kaip manęs ir reikalauja "proper" C.

=== "stack.c"

    ???+ "📄 02_Decomposing_to_UNITS/01_NC/stack.c"

        ```c linenums="1" 
        --8<-- "code/evolution/stack-2026/02_Decomposing_to_UNITS/01_NC/stack.c"
        ```

=== "user.c"

    ???+ "📄 02_Decomposing_to_UNITS/01_NC/user.c"

        ```c linenums="1" hl_lines="6-9"
        --8<-- "code/evolution/stack-2026/02_Decomposing_to_UNITS/01_NC/user.c"
        ```

=== "🔨 = ⚙️➔❌/⚠️"

    === "⚙️gcc → ⚠️warning"
    
        ```bash
        gcc -c stack.c # ✅
        gcc -c user.c  # ⚠️
        ```
        ```
        user.c: In function 'main':
        user.c:6:5: warning: implicit declaration of function 'init' [-Wimplicit-function-declaration]
            6 |     init();
            |     ^~~~
        user.c:7:37: warning: implicit declaration of function 'push' [-Wimplicit-function-declaration]
            7 |     while ('\n' != (c = getchar())) push(c);
            |                                     ^~~~
        user.c:8:13: warning: implicit declaration of function 'isEmpty' [-Wimplicit-function-declaration]
            8 |     while (!isEmpty()) putchar(pop());
            |             ^~~~~~~
        user.c:8:32: warning: implicit declaration of function 'pop'; did you mean 'popen'? [-Wimplicit-function-declaration]
            8 |     while (!isEmpty()) putchar(pop());
            |                                ^~~
            |                                popen
        ```
        ⚠️ gcc leidžia su perspėjimais - netiesioginis (sistemos parūpintas) funkcijos aprašas (`implicit declaration`)
    
    === "⚙️clang → ❌error"
    
        ```bash
        clang -c stack.c # ✅
        clang -c user.c  # ❌
        ```
        ```
        user.c:6:5: error: call to undeclared function 'init'; ISO C99 and later do not support implicit function declarations
            [-Wimplicit-function-declaration]
            6 |     init();
            |     ^
        user.c:7:37: error: call to undeclared function 'push'; ISO C99 and later do not support implicit function declarations
            [-Wimplicit-function-declaration]
            7 |     while ('\n' != (c = getchar())) push(c);
            |                                     ^
        user.c:8:13: error: call to undeclared function 'isEmpty'; ISO C99 and later do not support implicit function
            declarations [-Wimplicit-function-declaration]
            8 |     while (!isEmpty()) putchar(pop());
            |             ^
        user.c:8:32: error: call to undeclared function 'pop'; ISO C99 and later do not support implicit function declarations
            [-Wimplicit-function-declaration]
            8 |     while (!isEmpty()) putchar(pop());
            |                                ^
        4 errors generated.
        ```
        ❌ clang griežtesnis - iškart klaidos! - `call to undeclared function` ir dar primena, kad C99+ nepalaiko `implicit declaration`

---

#### ❌ Nesikompiliuoja (clang) / ⚠️ Perspėjimai (gcc)

??? bug "🔍 Diagnozė: Trūksta funkcijų aprašų (deklaracijų)"
    **Problema:** Kreipiamės į **neaprašytą** funkciją - kompiliatorius "nežino" kas tai per vardas, kokį veiksmą jis atstovauja.
    
    **Kodėl:**
    ```c
    // user.c kompiliuojant:
    init();     // ❌ Kas yra init? Kokio tipo? Nežinau!
    push(c);    // ❌ Kas yra push? Kokio tipo? Nežinau!
    ```
    
    **Skirtumas gcc vs clang:**
    
    - **gcc (traditional):** Leidžia su **warning** - "spėja" `int funkcija()` (implicit declaration)
    - **clang (strict):** Iškart **error** - ISO C99+ draudžia implicit declarations
    - Gali veikti su gcc, bet **dangerous** - spėjimas gali būti neteisingas!

---

### 2 sprendimas: `#include` `.c` failą

> [`02_Decomposing_to_UNITS/02_NL`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/02_Decomposing_to_UNITS/02_NL/)

!!! quote "sumanymas/ketinimas"
    Jei nemato vardų, o yra `#include`, tai jį ir panaudosiu - tada matys (kitur veikia pvz. `import`).

=== "user.c"

    ???+ "📄 02_Decomposing_to_UNITS/02_NL/user.c"

        ```c linenums="1" hl_lines="2"
        --8<-- "code/evolution/stack-2026/02_Decomposing_to_UNITS/02_NL/user.c"
        ```

=== "stack.c"

    ??? "📄 02_Decomposing_to_UNITS/02_NL/stack.c"

        ```c linenums="1"
        --8<-- "code/evolution/stack-2026/02_Decomposing_to_UNITS/02_NL/stack.c"
        ```

=== "🔨 = ⚙️➔🔗➔❌"

    === "⚙️gcc → 🔗gcc → ❌error"
    
        ```bash
        gcc -c stack.c -o stack.o  # ✅
        gcc -c user.c -o user.o    # ✅
        gcc stack.o user.o -o app  # ❌
        ```
        ```
        C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: user.o:user.c:(.text+0x0): multiple definition of `init'; stack.o:stack.c:(.text+0x0): first defined here
        C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: user.o:user.c:(.text+0x11): multiple definition of `isEmpty'; stack.o:stack.c:(.text+0x11): first defined here
        C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: user.o:user.c:(.text+0x25): multiple definition of `isFull'; stack.o:stack.c:(.text+0x25): first defined here
        C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: user.o:user.c:(.text+0x3a): multiple definition of `push'; stack.o:stack.c:(.text+0x3a): first defined here
        C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: user.o:user.c:(.text+0x76): multiple definition of `pop'; stack.o:stack.c:(.text+0x76): first defined here
        C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: user.o:user.c:(.bss+0x0): multiple definition of `stack'; stack.o:stack.c:(.bss+0x0): first defined here
        C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: user.o:user.c:(.bss+0x8): multiple definition of `top'; stack.o:stack.c:(.bss+0x8): first defined here
        collect2.exe: error: ld returned 1 exit status
        ```
        ❌ `multiple definition of ...`
    
    === "⚙️clang → 🔗clang → ❌error"
    
        ```bash
        clang -c stack.c -o stack.o  # ✅
        clang -c user.c -o user.o    # ✅
        clang stack.o user.o -o app  # ❌
        ```
        ```
        C:/mingw64/bin/ld: user.o:user.c:(.text+0x0): multiple definition of `init'; stack.o:stack.c:(.text+0x0): first defined here
        C:/mingw64/bin/ld: user.o:user.c:(.text+0x11): multiple definition of `isEmpty'; stack.o:stack.c:(.text+0x11): first defined here
        C:/mingw64/bin/ld: user.o:user.c:(.text+0x25): multiple definition of `isFull'; stack.o:stack.c:(.text+0x25): first defined here
        C:/mingw64/bin/ld: user.o:user.c:(.text+0x3a): multiple definition of `push'; stack.o:stack.c:(.text+0x3a): first defined here
        C:/mingw64/bin/ld: user.o:user.c:(.text+0x76): multiple definition of `pop'; stack.o:stack.c:(.text+0x76): first defined here
        C:/mingw64/bin/ld: user.o:user.c:(.bss+0x0): multiple definition of `stack'; stack.o:stack.c:(.bss+0x0): first defined here
        C:/mingw64/bin/ld: user.o:user.c:(.bss+0x8): multiple definition of `top'; stack.o:stack.c:(.bss+0x8): first defined here
        clang: error: linker command failed with exit code 1 (use -v to see invocation)
        ```
        ❌ multiple definition of ...

---

#### ❌ Nesilinkina

??? bug "🔍 Diagnozė: Daugybiniai apibrėžimai (multiple definitions)"
    **Problema:** `#include "stack.c"` įterpia visą turinį → visi `stack.c` **apibrėžimai** (funkcijos ir kintamieji) po kompiliavimo yra **abiejuose** `.o` failuose!
    
    **Kodėl:**
    ```c
    // user.c po preprocessor'iaus:
    #include "stack.c"  → [visas stack.c kodas įterpiamas čia]
    
    // Rezultatas:
    // stack.o turi: init(), push(), pop(), stack[], top  ← apibrėžimai
    // user.o TAIP PAT turi: init(), push(), pop(), stack[], top  ← apibrėžimai
    ```
    
    **Linkerio problema:**
    
    - Linkeris turi **sulinkinti** vardų kreipinius su jų **apibrėžimais**
    - Bet apibrėžimų **po du** - vienas `stack.o`, kitas `user.o`!
    - **Kurį pasirinkti?** Pagal kokį kriterijų?
    - Jei abu skirtingi - **kuris teisingas**?
    
    **Linkeris deleguoja sprendimą:** Griežtai informuoja apie "multiple definition" - autorius privalo pataisyti!
    
    **Prisiminkite terminologiją:**
    
    - **Aprašų** (declarations) gali būti daug ✅
    - **Apibrėžimų** (definitions) gali būti tik VIENAS ✅
    
    **Sprendimas:** `#include` skirtas `.h` failams (su aprašais), ne `.c` (su apibrėžimais)!

---

### 3 sprendimas: `#include` tą `.c` failą 2

> [`02_Decomposing_to_UNITS/03_OK_Wrong`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/02_Decomposing_to_UNITS/03_OK_Wrong/)

!!! quote "sumanymas/ketinimas"
    Jei nesilinkina dėl pasikartojaničių apibrėžimų, tai paliksiu `#include "stack.c"` bet nebelinkinsiu su `stack.o`.

=== "stack.c"

    ??? "📄 02_Decomposing_to_UNITS/03_OK_Wrong/stack.c"
    
        ```c linenums="1"
        --8<-- "code/evolution/stack-2026/02_Decomposing_to_UNITS/03_OK_Wrong/stack.c"
        ```

=== "user.c"

    ???+ "📄 02_Decomposing_to_UNITS/03_OK_Wrong/user.c"
    
        ```c linenums="1" hl_lines="2"
        --8<-- "code/evolution/stack-2026/02_Decomposing_to_UNITS/03_OK_Wrong/user.c"
        ```

=== "🔨 = ⚙️➔🔗➔🚀"

    ```bash
    gcc user.c -o app  # ✅
    ./app
    ```

=== "⌨️➔🖥️"

    ```
    123
    321
    ```

---

#### ✅ Veikia! Bet...

??? warning "🔍 Kodėl BLOGAI (antipattern)?"
    **Veikia, bet grįžome prie monolito problemos:**
    
    - **Grįžome prie monolito** - viskas vėl viename kompiliavimo vienete
    - **Negalimas pakartotinis naudojimas** - kaip kitas projektas panaudos?
    - **`#include` skirtas `.h`, ne `.c`** - prieš C konvenciją
    - **Kompiliavimo laikas** - `stack.c` kompiliuojamas **kiekvieną kartą** kai kompiliuojam `user.c`
    - **Priklausomybės** - pakeitus `stack.c` reikia perkompiliuoti `user.c`
    
    **Teisingas kelias:** Reikia **header failų** (`.h`) su **aprašais** (prototipais)!

---

### 💡 Pamokos

!!! tip "Aprašai vs Apibrėžimai - labai svarbu!"
    **Ką išmokome:**
    
    - **Aprašai (declarations):** Funkcijų prototipai - "sakome kad egzistuoja"
    - **Apibrėžimai (definitions):** Funkcijų realizacijos - "sakome kaip veikia"
    - **Aprašų** gali būti daug (įvairiuose failuose) ✅
    - **Apibrėžimų** gali būti tik **vienas** ✅
    
    **Kompiliavimo vs linkavimo klaidos:**
    
    - **Kompiliavimo:** Trūksta **aprašų** - implicit declaration
    - **Linkavimo:** Daug **apibrėžimų** - multiple definition
    
    **gcc vs clang skirtumai:**
    
    - **gcc:** Laisvesnis - leidžia implicit declarations su warning
    - **clang:** Griežtesnis - error pagal ISO C99+
    
    **Sprendimas → Etapas 03:** Tikras C modulis su `.h` failais (aprašais)!

---

??? info "📚 Dar kartą terminologija: Aprašas vs Apibrėžimas"
    **C kalboje kritiškai svarbu skirti du dalykus:**
    
    **Aprašas (Declaration)** - "sakome kompiliatoriui" kad kažkas egzistuoja:
    
    - `int add(int a, int b);` ← **funkcijos prototipas** (tai ir yra aprašas!)
    - `extern int counter;` ← kintamojo aprašas
    - Kompiliatorius žino **vardą ir tipą**, bet **ne kaip veikia** ar **kur saugoma**
    - **Gali būti daug** aprašų (įvairiuose failuose)
    
    **Apibrėžimas (Definition)** - "pasakome kompiliatoriui" KĄ daryti ir KUR saugoti:
    
    - `int add(int a, int b) { return a + b; }` ← funkcijos apibrėžimas
    - `int counter = 0;` ← kintamojo apibrėžimas  
    - Kompiliatorius žino **viską** - sugeneruoja kodą / paskiria (alokuoja) atmintį
    - **Gali būti tik VIENAS** apibrėžimas (vienoje vietoje)
    
    **Aukso taisyklė:** 
    
    - Aprašų (declarations) → **daug** ✅
    - Apibrėžimų (definitions) → **tik vienas** ✅

## 3️⃣Etapas: "Stekas Modulyje"

> [`03_Discovering_C_MODULE`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/03_Discovering_C_MODULE/)

!!! note "🎯 Tikslas"
    Sukurti **tikrą C modulį** su header failu (`.h`) - atskirti **interfeisą** (ką modulis siūlo) nuo **implementacijos** (kaip veikia). Išmokti **aprašų** (declarations) ir **apibrėžimų** (definitions) skirtumo praktiką.

!!! info "🔍 Ką darysime"
    - Sukursime **header failą** (`.h`) su funkcijų prototipais/aprašais ir kintamųjų aprašais
    - Suprasime **`extern`** raktažodžio vaidmenį
    - Pamatysime kad net ir "teisingas" modulis gali būti **nesaugus** - globalūs kintamieji prieinami
    - Pademonstruosime **ataką** su `extern` - tiesioginis duomenų manipuliavimas

!!! warning "Dar kartą!"
    **Aprašas (declaration)** ≠ **Apibrėžimas (definition)**
    
    - **Aprašas:** "Sakome kad egzistuoja" - `extern int top;` arba `void push(char c);`
    - **Apibrėžimas:** "Pasakome kaip veikia" - `int top = 0;` arba `void push(char c) { ... }`
    - **Taisyklė:** Aprašų daug ✅, apibrėžimas vienas ✅

**4 sprendimai** - per kompiliavimo ir linkavimo klaidas į tikrą modulį (_prasideda nuo 02_NL_).

---

### 1 sprendimas: į `.h` be `extern`

> [`03_Discovering_C_MODULE/02_NL`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/03_Discovering_C_MODULE/02_NL/)

!!! quote "sumanymas/ketinimas"
    Sukursiu tikrą C modulį: `.h` su deklaracijomis, `.c` su apibrėžimais. Tiesiog perkelsiu funkcijų prototipus ir kintamųjų deklaracijas į header failą.

=== "stack.h" 

    ???+ "📄 03_Discovering_C_MODULE/02_NL/stack.h"
    
        ```c linenums="1" hl_lines="3-4"
        --8<-- "code/evolution/stack-2026/03_Discovering_C_MODULE/02_NL/stack.h"
        ```

=== "stack.c"

    ???+ "📄 03_Discovering_C_MODULE/02_NL/stack.c"
    
        ```c linenums="1" hl_lines="2"
        --8<-- "code/evolution/stack-2026/03_Discovering_C_MODULE/02_NL/stack.c"
        ```

=== "user.c"

    ???+ "📄 03_Discovering_C_MODULE/02_NL/user.c"
    
        ```c linenums="1" hl_lines="2"
        --8<-- "code/evolution/stack-2026/03_Discovering_C_MODULE/02_NL/user.c"
        ```

=== "🔨 = ⚙️➔🔗➔❌"

    === "⚙️gcc ➔ 🔗gcc ➔ ❌error"
    
        ```bash
        gcc -c stack.c -o stack.o  # ✅
        gcc -c user.c -o user.o    # ✅
        gcc stack.o user.o -o app  # ❌
        ```
        ```
        C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: user.o:user.c:(.bss+0x0): multiple definition of `stack'; stack.o:stack.c:(.bss+0x0): first defined here
        C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: user.o:user.c:(.bss+0x64): multiple definition of `top'; stack.o:stack.c:(.bss+0x64): first defined here
        collect2.exe: error: ld returned 1 exit status
        ```
        ❌ multiple definition of ...
    
    === "⚙️clang ➔ 🔗clang ➔ ❌error"
    
        ```bash
        clang -c stack.c -o stack.o  # ✅
        clang -c user.c -o user.o    # ✅
        clang stack.o user.o -o app  # ❌
        ```
        ```
        C:/mingw64/bin/ld: user.o:user.c:(.bss+0x0): multiple definition of `top'; stack.o:stack.c:(.bss+0x0): first defined here
        C:/mingw64/bin/ld: user.o:user.c:(.bss+0x10): multiple definition of `stack'; stack.o:stack.c:(.bss+0x10): first defined here
        clang: error: linker command failed with exit code 1 (use -v to see invocation)
        ```
        ❌ multiple definition of ...

---

#### ❌ Nesilinkina

??? bug "Linkinimo klaida!"
    Kintamieji apibrėžti header'yje → abu failai sukuria kopijas. Tas pats kaip buvo, bet tik kintamiesiems. Reikia "aprašo formos" kintamiesiems. Pasitelkiam `extern`.

---

### 2 sprendimas: į `.h` su `extern`

> [`03_Discovering_C_MODULE/03_NL`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/03_Discovering_C_MODULE/03_NL/)

!!! quote "sumanymas/ketinimas"
    Išmokau, kad header'yje reikia `extern` kintamiesiems! Pridėsiu `extern` prie `stack[]` ir `top` deklaracijų.

=== "stack.h"

    ???+ "📄 03_Discovering_C_MODULE/03_NL/stack.h"
    
        ```c linenums="1" hl_lines="6-7"
        --8<-- "code/evolution/stack-2026/03_Discovering_C_MODULE/03_NL/stack.h"
        ```

=== "stack.c"

    ???+ "📄 03_Discovering_C_MODULE/03_NL/stack.c"
    
        ```c linenums="1" hl_lines="2"
        --8<-- "code/evolution/stack-2026/03_Discovering_C_MODULE/03_NL/stack.c"
        ```

=== "user.c"

    ???+ "📄 03_Discovering_C_MODULE/03_NL/user.c"
    
        ```c linenums="1" hl_lines="2"
        --8<-- "code/evolution/stack-2026/03_Discovering_C_MODULE/03_NL/user.c"
        ```

=== "🔨 = ⚙️➔🔗➔❌"

    === "⚙️gcc ➔ 🔗gcc ➔ ❌error"
    
        ```bash
        gcc -c stack.c -o stack.o  # ✅
        gcc -c user.c -o user.o    # ✅
        gcc stack.o user.o -o app  # ❌
        ```
        ```
        C:\Repos\Github\ViktorasGolubevasMIF\cpp-2026\code\evolution\stack-2026\03_Discovering_C_MODULE\03_NL>gcc user.c stack.c -o app
        C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccpza694.o:stack.c:(.rdata$.refptr.stack[.refptr.stack]+0x0): undefined reference to `stack'
        C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccpza694.o:stack.c:(.rdata$.refptr.top[.refptr.top]+0x0): undefined reference to `top'
        collect2.exe: error: ld returned 1 exit status
        ```
        ❌ multiple definition of ...
    
    === "⚙️clang ➔ 🔗clang ➔ ❌error"
    
        ```bash
        clang -c stack.c -o stack.o  # ✅
        clang -c user.c -o user.o    # ✅
        clang stack.o user.o -o app  # ❌
        ```
        ```
        C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/stack-34f531.o:stack.c:(.rdata$.refptr.stack[.refptr.stack]+0x0): undefined reference to `stack'
        C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/stack-34f531.o:stack.c:(.rdata$.refptr.top[.refptr.top]+0x0): undefined reference to `top'
        clang: error: linker command failed with exit code 1 (use -v to see invocation)

        ```
        ❌ `undefined reference to` ...

---

#### ❌ Nesilinkina

??? bug "Dar viena NL klaida!"
    **Netesigai nukopijuota - taisyti**
    `extern` header'yje gerai, bet `stack.c` TAIP PAT turi kintamųjų apibrėžimus! Dabar abu `.c` failai sukuria apibrėžimus. Header'yje turi būti TIK `extern` deklaracijos, o apibrėžimas – TIKTAI viename `.c` faile.

---

### 3 sprendimas: Teisingas modulis

> [`03_Discovering_C_MODULE/04_OK`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/03_Discovering_C_MODULE/04_OK/)

!!! quote "sumanymas/ketinimas"
    Supratau! Header'yje – `extern` deklaracijos, o apibrėžimas tik `stack.c` faile (be `extern`). User.c tik naudoja, bet neapibrėžia.

=== "stack.h"

    ???+ "📄 03_Discovering_C_MODULE/04_OK/stack.h"
    
        ```c linenums="1"
        --8<-- "code/evolution/stack-2026/03_Discovering_C_MODULE/04_OK/stack.h"
        ```

=== "stack.c"

    ???+ "📄 03_Discovering_C_MODULE/04_OK/stack.c"
    
        ```c linenums="1" hl_lines="5-6"
        --8<-- "code/evolution/stack-2026/03_Discovering_C_MODULE/04_OK/stack.c"
        ```

=== "user.c"

    ???+ "📄 03_Discovering_C_MODULE/04_OK/user.c"
    
        ```c linenums="1" hl_lines="2"
        --8<-- "code/evolution/stack-2026/03_Discovering_C_MODULE/04_OK/user.c"
        ```

=== "🔨 = ⚙️➔🔗➔🚀"

    ```bash
    gcc -c stack.c -o stack.o  # ✅
    gcc -c user.c -o user.o    # ✅
    gcc stack.o user.o -o app  # ✅
    ./app
    ```
    arba (win)
    ```bash
    gcc -c stack.c -o stack.o
    gcc -c user.c -o user.o
    gcc stack.o user.o -o app
    app
    ```

=== "⌨️➔🖥️"

    ```
    123
    321
    ```

---

#### ✅ Veikia! Bet...

??? success "Dabar teisingai!"
    - `extern` deklaracijos header'yje
    - Apibrėžimai stack.c
    - Tikras C modulis

??? danger "Problemos, pavojai..."
    - **Globalūs kintamieji viešai prieinami:** Kas nors gali rašyti `extern int top;` savo kode ir manipuliuoti!
    - **Nėra apsaugos:** Bet kas gali sugadinti `top` ar `stack[]`

---

### 4 sprendimas: "Atakos" bandymas

> [`03_Discovering_C_MODULE/05_OK_Attack`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/03_Discovering_C_MODULE/05_OK_Attack/)

!!! quote "sumanymas/ketinimas"
    Pademonstruoju, kad net ir "teisingas" modulis su `extern` nėra saugus – "attacker" gali pridėti `extern int top;` ir keisti steko būseną.

=== "stack.h"

    ??? "📄 03_Discovering_C_MODULE/05_OK_Attack/stack.h"
    
        ```c linenums="1"
        --8<-- "code/evolution/stack-2026/03_Discovering_C_MODULE/05_OK_Attack/stack.h"
        ```

=== "stack.c"

    ??? "📄 03_Discovering_C_MODULE/05_OK_Attack/stack.c"
    
        ```c linenums="1" hl_lines="5-6"
        --8<-- "code/evolution/stack-2026/03_Discovering_C_MODULE/05_OK_Attack/stack.c"
        ```

=== "user_attack.c"

    ???+ "📄 03_Discovering_C_MODULE/05_OK_Attack/user_attack.c"
    
        ```c linenums="1" hl_lines="4-5 13"
        --8<-- "code/evolution/stack-2026/03_Discovering_C_MODULE/05_OK_Attack/user_attack.c"
        ```

=== "🔨 = ⚙️➔🔗➔🚀"

    ```bash
    gcc -c stack.c -o stack.o
    gcc -c user_attack.c -o user_attack.o
    gcc stack.o user_attack.o -o app
    ./app  # ✅
    ```

=== "⌨️➔🖥️"

    ```
    123456
    !54321
    ```

---

#### ✅ "Ataka" pavyko!

??? danger "Globalūs kintamieji viešai prieinami"
    ```c
    extern int top;
    top = -1;  // ← Galima manipuliuoti!
    ```
    Vartotojas gali tiesiogiai pasiekti ir sugadinti vidinę steко būseną!
    
    **Sprendimas → Etapas 04:** `static` linkage

---

### 💡 Pamokos

!!! tip "C Modulio Anatomija"
    - **Modulis** = Interfeisas/Sąsaja + Realizacija/Implementacija = Header (antraštės failas) + Source (išeities failas)
    - **Modulis** = `.h` + `.c`
    - **Header (`.h`)**: `extern` deklaracijos globalių kintamųjų + funkcijų prototipai
    - **Source (`.c`)**: Kintamųjų apibrėžimai (be `extern`) + funkcijų realizacijos
    - **Problema**: Globalūs kintamieji su `extern` yra prieinami bet kam!

---
## 4️⃣Etapas: "Apsaugotas Stekas Modulyje"

> [`04_Protecting_IMPLEMENTATION`](https://github.com/ViktorasGolubevasMIF/cpp-2026/tree/main/code/evolution/stack-2026/04_Protecting_IMPLEMENTATION/)

!!! note "🎯 Tikslas"
    Išmokti **information hiding** techniką naudojant `static` arba internal linkage - paslėpti vidinę implementaciją nuo išorės.

!!! info "🔍 Ką darysime"
    - Suprasime kaip `static` saugo duomenis
    - Pamatysime likusią problemą - **tik vienas stekas**

---

### 1 sprendimas: Ataka su static

> [`04_Protecting_IMPLEMENTATION/01_NL_Attack`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/04_Protecting_IMPLEMENTATION/01_NL_Attack/)

!!! quote "sumanymas/ketinimas"
    Pabandysiu apsaugoti modulį - pridėsiu `static` prie kintamųjų `stack.c`.

=== "stack.h"

    ???+ "📄 04_Protecting_IMPLEMENTATION/01_NL_Attack/stack.h"
    
        ```c linenums="1" hl_lines="6-7"
        --8<-- "code/evolution/stack-2026/04_Protecting_IMPLEMENTATION/01_NL_Attack/stack.h"
        ```

=== "stack.c"

    ??? "📄 04_Protecting_IMPLEMENTATION/01_NL_Attack/stack.c"
    
        ```c linenums="1" hl_lines="2 5-6"
        --8<-- "code/evolution/stack-2026/04_Protecting_IMPLEMENTATION/01_NL_Attack/stack.c"
        ```

=== "user_attack.c"

    ??? "📄 04_Protecting_IMPLEMENTATION/01_NL_Attack/user_attack.c"
    
        ```c linenums="1" hl_lines="2 9-10"
        --8<-- "code/evolution/stack-2026/04_Protecting_IMPLEMENTATION/01_NL_Attack/user_attack.c"
        ```

=== "🔨 = ⚙️➔🔗➔❌"

    === "⚙️gcc ➔ 🔗gcc ➔ ❌error"
    
        ```bash
        gcc -c stack.c -o stack.o  # ✅
        gcc -c user_attack.c -o user_attack.o  # ✅
        gcc stack.o user_attack.o -o app  # ❌
        ```
        ```
        /usr/bin/ld: user_attack.o: in function `main':
        user_attack.c:(.text+0x7): undefined reference to `top'
        collect2: error: ld returned 1 exit status
        ```
        ❌ undefined reference to ...
    
    === "⚙️clang ➔ 🔗clang ➔ ❌error"
    
        ```bash
        clang -c stack.c -o stack.o  # ✅
        clang -c user_attack.c -o user_attack.o  # ✅
        clang stack.o user_attack.o -o app  # ❌
        ```
        ```
        /usr/bin/ld: user_attack.o: in function `main':
        user_attack.c:(.text+0x7): undefined reference to `top'
        clang: error: linker command failed with exit code 1
        ```
        ❌ `undefined reference to ...`

---

#### ❌ Nesilinkina - Ataka nepavyko!

??? bug "🔍 Diagnozė: Konfliktas tarp extern ir static"
    **Problema:** Header'yje deklaruojame `extern int top;`, bet `stack.c` apibrėžia `static int top = 0;`
    
    **Kodėl nevyksta:**
    - `extern` sako "ieškoti išorėje, matomas kitose kompiliavimo vieneto"
    - `static` sako "matomas **tik** šiame faile"
    - Linkeris negali rasti `top`, nes jis **internal linkage**!

---

### 2 sprendimas: Tikras information hiding

> [`04_Protecting_IMPLEMENTATION/02_OK`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/04_Protecting_IMPLEMENTATION/02_OK/)

!!! quote "sumanymas/ketinimas"
    Supratau! Pašalinsiu `extern` deklaracijas iš header'o. Header'yje - tik funkcijų prototipai, o kintamieji su `static` lieka **tik** `stack.c` viduje.

=== "stack.h"

    ???+ "📄 04_Protecting_IMPLEMENTATION/02_OK/stack.h"
    
        ```c linenums="1" hl_lines="6-10"
        --8<-- "code/evolution/stack-2026/04_Protecting_IMPLEMENTATION/02_OK/stack.h"
        ```

=== "stack.c"

    ??? "📄 04_Protecting_IMPLEMENTATION/02_OK/stack.c"
    
        ```c linenums="1" hl_lines="2 5-6"
        --8<-- "code/evolution/stack-2026/04_Protecting_IMPLEMENTATION/02_OK/stack.c"
        ```

=== "user.c"

    ??? "📄 04_Protecting_IMPLEMENTATION/02_OK/user.c"
    
        ```c linenums="1" hl_lines="2"
        --8<-- "code/evolution/stack-2026/04_Protecting_IMPLEMENTATION/02_OK/user.c"
        ```

=== "🔨 = ⚙️➔🔗➔🚀"

    ```bash
    gcc -c stack.c -o stack.o  # ✅
    gcc -c user.c -o user.o    # ✅
    gcc stack.o user.o -o app  # ✅
    ./app
    ```
    arba (win)
    ```bash
    gcc -c stack.c -o stack.o
    gcc -c user.c -o user.o
    gcc stack.o user.o -o app
    app
    ```

=== "⌨️➔🖥️"

    ```
    123
    321
    ```

---

#### ✅ Veikia! Bet...

??? success "`static` linkage saugo!"
    ```c
    static char stack[STACK_SIZE];  // Internal linkage
    static int top = 0;              // Nematomas išorėje
    ```
    - Ataka dabar **neveiks** - negalima `extern int top;`
    - Duomenys **apsaugoti** nuo išorinio manipuliavimo
    - **Information hiding** pasiektas!

??? danger "Problemos, pavojai..."
    - **Tik vienas stekas:** `static` kintamieji globalūs viduje modulio
    - **Negalima keliems egzemplioriams:** Reikia struktūros!

---

### 💡 Pamokos

!!! tip "Static Linkage"
    - **`static` kintamieji** - internal linkage (matomi tik tame faile)
    - **Information hiding:** Vartotojas negali pasiekti `top` ar `stack[]`
    - **Ribojimas:** Tik vienas stekas - reikia **user-defined type** (struct)!

---

## 5️⃣Etapas: "Tipizuotas Stekas"

> [`05_Defining_USER_TYPE`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/05_Defining_USER_TYPE/)

!!! note "🎯 Tikslas"
    Sukurti **user-defined type** (struct) - leisti **keliems stekų egzemplioriams** egzistuoti vienu metu.

!!! info "🔍 Ką darysime"
    - Grįšime prie monolito, bet su **struct**
    - Išmoksime `static` draudimą struktūrose
    - Suprasime **pass-by-value** vs **pass-by-pointer** skirtumus
    - Sukursime kelis stekų egzempliorius!

!!! warning "Koncepcija: 'Greitas entry point'"
    Tarsi įdarbinimo pokalbio "quick solution" - grįžtame prie monolito, bet su struktūra.

---

### 1 sprendimas: static struktūroje

> [`05_Defining_USER_TYPE/01_NC`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/05_Defining_USER_TYPE/01_NC/)

!!! quote "sumanymas/ketinimas"
    Sukursiu struct su `static` nariais - taip išlaikysiu `static` apsaugą iš etapo 04!

=== "usestack.c"

    ???+ "📄 05_Defining_USER_TYPE/01_NC/usestack.c"
    
        ```c linenums="1" hl_lines="6-9"
        --8<-- "code/evolution/stack-2026/05_Defining_USER_TYPE/01_NC/usestack.c"
        ```

=== "🔨 = ⚙️➔❌"

    ```bash
    gcc usestack.c -o app  # ❌
    ```
    ```
    usestack.c:7:5: error: storage class specified for 'stack'
    usestack.c:8:5: error: storage class specified for 'top'
    ```
    ❌ storage class specified for ...

---

#### ❌ Nesikompiliuoja

??? bug "🔍 Diagnozė: static draudžiamas struktūrose"
    **Problema:** C neleidžia `static` **storage class specifiers** struktūrų nariams!
    
    **Kodėl:**
    ```c
    struct Stack {
        static char stack[SIZE];  // ❌ Neleidžiama!
        static int top;           // ❌ Neleidžiama!
    };
    ```
    
    - `static` yra **storage class**, ne **type qualifier**
    - Struct nariai turi būti **instance data**, ne **class data**
    - C++ leidžia `static` narius klasėse, bet **ne C**!
    
    **Sprendimas:** Pašalinti `static` - naudoti įprastus narius.

---

### 2 sprendimas: Pass-by-value

> [`05_Defining_USER_TYPE/02_RT`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/05_Defining_USER_TYPE/02_RT/)

!!! quote "sumanymas/ketinimas"
    Pašalinsiu `static` ir perduosiu struktūrą funkcijoms pagal reikšmę (pass-by-value).

=== "usestack.c"

    ???+ "📄 05_Defining_USER_TYPE/02_RT/usestack.c"
    
        ```c linenums="1" hl_lines="11-13 16-20"
        --8<-- "code/evolution/stack-2026/05_Defining_USER_TYPE/02_RT/usestack.c"
        ```

=== "🔨 = ⚙️➔🔗➔🚀"

    ```bash
    gcc usestack.c -o app  # ✅
    ./app
    ```

=== "⌨️➔🖥️"

    ```
    
    (tuščia)
    ```

---

#### ✅ Kompiliuojasi, bet BLOGAI veikia!

??? bug "🔍 Diagnozė: Pass-by-value kopijuoja struktūrą"
    **Problema:** Runtime error - `pop()` negražina nieko!
    
    **Kodėl:**
    ```c
    void init(struct Stack st) {    // Kopija!
        st.top = 0;                 // Keičia KOPIJĄ
    }                               // Kopija sunaikinama
    
    void push(struct Stack st, char c) {  // Kopija!
        st.stack[st.top++] = c;           // Keičia KOPIJĄ
    }                                     // Kopija sunaikinama
    ```
    
    - **Pass-by-value** - funkcija gauna **kopiją** struktūros
    - Visi pakeitimai vyksta **kopijoje**
    - Originalas `main()` funkcijoje **nepasikeis**!
    
    **Sprendimas:** Naudoti **pass-by-pointer** - perduoti adresą.

---

### 3 sprendimas: Pass-by-pointer

> [`05_Defining_USER_TYPE/03_OK`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/05_Defining_USER_TYPE/03_OK/)

!!! quote "sumanymas/ketinimas"
    Naudosiu **pointer'ius** - funkcijos gaus struktūros adresą, galės keisti originalą!

=== "usestack.c"

    ???+ "📄 05_Defining_USER_TYPE/03_OK/usestack.c"
    
        ```c linenums="1" hl_lines="11-13 16-20 27-28"
        --8<-- "code/evolution/stack-2026/05_Defining_USER_TYPE/03_OK/usestack.c"
        ```

=== "🔨 = ⚙️➔🔗➔🚀"

    ```bash
    gcc usestack.c -o app  # ✅
    ./app
    ```

=== "⌨️➔🖥️"

    ```
    123
    321
    ```

---

#### ✅ Veikia!

??? success "Pass-by-pointer sprendžia problemą!"
    ```c
    void init(struct Stack *pst) {
        pst->top = 0;  // Keičia ORIGINALĄ
    }
    
    struct Stack st1, st2;  // Du stekai!
    init(&st1);
    init(&st2);
    ```
    - **Pass-by-pointer** - funkcija gauna **adresą**
    - Visi pakeitimai vyksta **originale**
    - Galime turėti **kelis egzempliorius**!

??? danger "Problemos, pavojai..."
    - **Struktūra vieša:** Vartotojas mato `struct Stack { char stack[100]; int top; };`
    - **Nariai prieinami:** Gali rašyti `st1.top = 999;` - sugadinti būseną
    - **Nėra information hiding:** Reikia **encapsulation** + modulio!

---

### 💡 Pamokos

!!! tip "User-Defined Type"
    - **Struct** leidžia kurti **naujus tipus**
    - **`static` draudžiamas** struktūrų nariuose C kalboje
    - **Pass-by-value** kopijuoja → pakeitimai neišlieka
    - **Pass-by-pointer** → galima keisti originalą
    - **Keli egzemplioriai:** `struct Stack st1, st2;` ✅

---

## 6️⃣Etapas: "Tipizuotas Stekas Modulyje"

> [`06_ENCAPSULATING_UDT_in_Module`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/06_ENCAPSULATING_UDT_in_Module/)

!!! note "🎯 Tikslas"
    **Encapsulation** - apjungti duomenis ir funkcijas modulyje. Sukurti **struct + modulis** kombinaciją.

!!! info "🔍 Ką darysime"
    - Apjungsime idėjas: **05 etapas** (struct) + **03 etapas** (modulis)
    - Sukursime modulinį user-defined type
    - Pamatysime kad **encapsulation ≠ information hiding**

!!! warning "Koncepcija: Apjungimas idėjų"
    Struct **iš 05** + Modulis **iš 03** = Struktūrizuotas modulis

---

### 1 sprendimas: Modulinis UDT

> [`06_ENCAPSULATING_UDT_in_Module/01_OK`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/06_ENCAPSULATING_UDT_in_Module/01_OK/)

!!! quote "sumanymas/ketinimas"
    Perkelsiu struct apibrėžimą į modulį - turėsiu **stack.h** su struktūros deklaracija ir funkcijų prototipais, **stack.c** su realizacija.

=== "stack.h"

    ???+ "📄 06_ENCAPSULATING_UDT_in_Module/01_OK/stack.h"
    
        ```c linenums="1" hl_lines="6-9 11-15"
        --8<-- "code/evolution/stack-2026/06_ENCAPSULATING_UDT_in_Module/01_OK/stack.h"
        ```

=== "stack.c"

    ??? "📄 06_ENCAPSULATING_UDT_in_Module/01_OK/stack.c"
    
        ```c linenums="1" hl_lines="2"
        --8<-- "code/evolution/stack-2026/06_ENCAPSULATING_UDT_in_Module/01_OK/stack.c"
        ```

=== "user.c"

    ??? "📄 06_ENCAPSULATING_UDT_in_Module/01_OK/user.c"
    
        ```c linenums="1" hl_lines="2 6-7"
        --8<-- "code/evolution/stack-2026/06_ENCAPSULATING_UDT_in_Module/01_OK/user.c"
        ```

=== "🔨 = ⚙️➔🔗➔🚀"

    ```bash
    gcc -c stack.c -o stack.o  # ✅
    gcc -c user.c -o user.o    # ✅
    gcc stack.o user.o -o app  # ✅
    ./app
    ```

=== "⌨️➔🖥️"

    ```
    123
    321
    ```

---

#### ✅ Veikia! Bet...

??? success "Modulinis UDT veikia!"
    - ✅ Struct modulyje
    - ✅ Keli egzemplioriai
    - ✅ Funkcijos dirba su pointer'iais
    - ✅ Pakartotinis naudojimas

??? danger "Problemos, pavojai..."
    - **Struktūros nariai VIEŠI:** Header'yje matoma visa struktūros anatomija
    - **Galima tiesioginė manipuliacija:** `st.top = 999;`
    - **Encapsulation ≠ Information hiding!**

---

### 2 sprendimas: Ataka pavyksta

> [`06_ENCAPSULATING_UDT_in_Module/02_OK_Attack`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/06_ENCAPSULATING_UDT_in_Module/02_OK_Attack/)

!!! quote "sumanymas/ketinimas"
    Pademonstruosiu, kad net ir modulinis UDT nėra saugus - "attacker" gali tiesiogiai keisti struktūros narius.

=== "stack.h"

    ??? "📄 06_ENCAPSULATING_UDT_in_Module/02_OK_Attack/stack.h"
    
        ```c linenums="1" hl_lines="6-9"
        --8<-- "code/evolution/stack-2026/06_ENCAPSULATING_UDT_in_Module/02_OK_Attack/stack.h"
        ```

=== "stack.c"

    ??? "📄 06_ENCAPSULATING_UDT_in_Module/02_OK_Attack/stack.c"
    
        ```c linenums="1"
        --8<-- "code/evolution/stack-2026/06_ENCAPSULATING_UDT_in_Module/02_OK_Attack/stack.c"
        ```

=== "user_attack.c"

    ???+ "📄 06_ENCAPSULATING_UDT_in_Module/02_OK_Attack/user_attack.c"
    
        ```c linenums="1" hl_lines="10-11"
        --8<-- "code/evolution/stack-2026/06_ENCAPSULATING_UDT_in_Module/02_OK_Attack/user_attack.c"
        ```

=== "🔨 = ⚙️➔🔗➔🚀"

    ```bash
    gcc -c stack.c -o stack.o
    gcc -c user_attack.c -o user_attack.o
    gcc stack.o user_attack.o -o app
    ./app  # ✅ bet rezultatas BLOGAS!
    ```

=== "⌨️➔🖥️"

    ```
    HACKED!
    321
    ```

---

#### ✅ Veikia, bet BLOGAI!

??? danger "Struktūros nariai prieinami"
    ```c
    struct Stack st;
    st.top = 999;  // ← Galima manipuliuoti!
    st.stack[0] = 'X';  // ← Galima keisti!
    ```
    
    **Encapsulation ≠ Information hiding:**
    - **Encapsulation** - duomenys + funkcijos kartu modulyje ✅
    - **Information hiding** - duomenų struktūra paslėpta ❌
    
    **Sprendimas → Etapas 07:** **Opaque pointer** (forward declaration)

---

### 💡 Pamokos

!!! tip "Encapsulation vs Information Hiding"
    - **Encapsulation:** Duomenys + funkcijos kartu (struct + modulis)
    - **Information hiding:** Struktūros detalės **paslėptos** nuo vartotojo
    - **Problema:** Header'yje matoma `struct Stack { ... };` - nariai prieinami!
    - **Reikia:** **Opaque pointer** - tik deklaracija, ne apibrėžimas!

---

## 7️⃣Etapas: "Paslėptas Stekas"

> [`07_HIDING_UDT_Information`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/07_HIDING_UDT_Information/)

!!! note "🎯 Tikslas"
    **Information hiding** - paslėpti struktūros detales nuo vartotojo. Naudoti **opaque pointer** (forward declaration).

!!! info "🔍 Ką darysime"
    - Panaudosime **incomplete type** - `struct Stack;` be apibrėžimo
    - Suprasime **forward declaration** koncepciją
    - Išsiaiškinsime kodėl reikalingi **pointer'iai**
    - Pamatysime **uninicializuotų pointer'ių** problemą

---

### 1 sprendimas: Incomplete type

> [`07_HIDING_UDT_Information/01_NC`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/07_HIDING_UDT_Information/01_NC/)

!!! quote "sumanymas/ketinimas"
    Perkelsiu struktūros **apibrėžimą** į `stack.c`, o header'yje palikusiu tik **deklaraciją** - `struct Stack;` (forward declaration).

=== "stack.h"

    ???+ "📄 07_HIDING_UDT_Information/01_NC/stack.h"
    
        ```c linenums="1" hl_lines="6"
        --8<-- "code/evolution/stack-2026/07_HIDING_UDT_Information/01_NC/stack.h"
        ```

=== "stack.c"

    ??? "📄 07_HIDING_UDT_Information/01_NC/stack.c"
    
        ```c linenums="1" hl_lines="4-7"
        --8<-- "code/evolution/stack-2026/07_HIDING_UDT_Information/01_NC/stack.c"
        ```

=== "user.c"

    ???+ "📄 07_HIDING_UDT_Information/01_NC/user_attack.c"
    
        ```c linenums="1" hl_lines="6-7"
        --8<-- "code/evolution/stack-2026/07_HIDING_UDT_Information/01_NC/user_attack.c"
        ```

=== "🔨 = ⚙️➔❌"

    ```bash
    gcc -c stack.c -o stack.o  # ✅
    gcc -c user.c -o user.o    # ❌
    ```
    ```
    user.c:7:19: error: storage size of 'st1' isn't known
    user.c:7:25: error: storage size of 'st2' isn't known
    ```
    ❌ storage size isn't known

---

#### ❌ Nesikompiliuoja

??? bug "🔍 Diagnozė: Incomplete type - nežinomas dydis"
    **Problema:** `user.c` bando sukurti `struct Stack st1, st2;` bet kompiliatorius nežino struktūros dydžio!
    
    **Kodėl:**
    ```c
    // user.c mato tik:
    struct Stack;  // Forward declaration - "incomplete type"
    
    // Bando sukurti:
    struct Stack st1;  // ❌ sizeof(Stack) nežinomas!
    ```
    
    - **Incomplete type** - kompiliatorius žino kad tipas egzistuoja, bet nežino jo struktūros
    - **Negalima alokuoti** stack'e - `sizeof()` nežinomas
    - **Galima tik pointer'iai** - `sizeof(pointer)` visada žinomas!
    
    **Sprendimas:** Naudoti **pointer'ius** - `struct Stack *pst1, *pst2;`

---

### 2 sprendimas: Bandymas su malloc

> [`07_HIDING_UDT_Information/02_NC`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/07_HIDING_UDT_Information/02_NC/)

!!! quote "sumanymas/ketinimas"
    Naudosiu **pointer'ius** ir `malloc()` - alokuosiu dinamiškai, nes `sizeof()` nežinomas kompiliavimo metu.

=== "stack.h"

    ???+ "📄 07_HIDING_UDT_Information/02_NC/stack.h"
    
        ```c linenums="1" hl_lines="6-10"
        --8<-- "code/evolution/stack-2026/07_HIDING_UDT_Information/02_NC/stack.h"
        ```

=== "stack.c"

    ??? "📄 07_HIDING_UDT_Information/02_NC/stack.c"
    
        ```c linenums="1" hl_lines="4-7"
        --8<-- "code/evolution/stack-2026/07_HIDING_UDT_Information/02_NC/stack.c"
        ```

=== "user.c"

    ???+ "📄 07_HIDING_UDT_Information/02_NC/user_attack.c"
    
        ```c linenums="1" hl_lines="6-9"
        --8<-- "code/evolution/stack-2026/07_HIDING_UDT_Information/02_NC/user_attack.c"
        ```

=== "🔨 = ⚙️➔❌"

    ```bash
    gcc -c stack.c -o stack.o  # ✅
    gcc -c user.c -o user.o    # ❌
    ```
    ```
    user.c:8:24: error: invalid application of 'sizeof' to incomplete type 'struct Stack'
    ```
    ❌ sizeof to incomplete type

---

#### ❌ Nesikompiliuoja

??? bug "🔍 Diagnozė: sizeof() incomplete type"
    **Problema:** `user.c` bando `malloc(sizeof(struct Stack))` bet `sizeof()` reikia pilno tipo!
    
    **Kodėl:**
    ```c
    // user.c mato tik:
    struct Stack;
    
    // Bando:
    malloc(sizeof(struct Stack));  // ❌ sizeof nežinomas!
    ```
    
    - **`malloc()` reikia dydžio** - kiek baitų alokuoti
    - **`sizeof()` reikia pilno tipo** - incomplete type nepakanka
    - **Vartotojas negali** alokuoti pats!
    
    **Sprendimas:** Reikia **factory funkcijos** modulyje - `create()` kuris žino `sizeof()`

---

### 3 sprendimas: Neininicializuotos rodyklės

> [`07_HIDING_UDT_Information/03_RT`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/07_HIDING_UDT_Information/03_RT/)

!!! quote "sumanymas/ketinimas"
    Pabandysiu su pointer'iais be alokavimo - gal veiks?

=== "stack.h"

    ???+ "📄 07_HIDING_UDT_Information/03_RT/stack.h"
    
        ```c linenums="1" hl_lines="6"
        --8<-- "code/evolution/stack-2026/07_HIDING_UDT_Information/03_RT/stack.h"
        ```

=== "stack.c"

    ??? "📄 07_HIDING_UDT_Information/03_RT/stack.c"
    
        ```c linenums="1"
        --8<-- "code/evolution/stack-2026/07_HIDING_UDT_Information/03_RT/stack.c"
        ```

=== "user.c"

    ???+ "📄 07_HIDING_UDT_Information/03_RT/user.c"
    
        ```c linenums="1" hl_lines="6-7 9-10"
        --8<-- "code/evolution/stack-2026/07_HIDING_UDT_Information/03_RT/user.c"
        ```

=== "🔨 = ⚙️➔🔗➔💥"

    ```bash
    gcc -c stack.c -o stack.o  # ✅
    gcc -c user.c -o user.o    # ✅
    gcc stack.o user.o -o app  # ✅
    ./app  # 💥
    ```
    ```
    Segmentation fault (core dumped)
    ```
    💥 Runtime crash!

---

#### ✅ Kompiliuojasi ir linkuojasi, bet CRASH!

??? bug "🔍 Diagnozė: Uninitialized pointers"
    **Problema:** Runtime crash - segmentation fault!
    
    **Kodėl:**
    ```c
    struct Stack *pst1, *pst2;  // Neininicializuotos rodyklės!
    init(pst1);  // 💥 pst1 rodo į niekur (garbage value)
    ```
    
    - **Pointer'iai neininicializuoti** - turi atsitiktines reikšmes
    - **Dereferencing** random adreso → segmentation fault
    - **Nėra atminties** - pointer'ius turi rodyti į **galiojančią atmintį**!
    
    **Sprendimas → Etapas 08:** **Factory pattern** - `create()` funkcija, kuri:
    - Alokuoja atmintį su `malloc()`
    - Inicializuoja struktūrą
    - Grąžina validų pointer'į

---

### 💡 Pamokos

!!! tip "Opaque Pointer Pattern"
    - **Forward declaration:** `struct Stack;` be apibrėžimo
    - **Incomplete type:** Kompiliatorius žino vardą, bet ne struktūrą
    - **Negalima:** `struct Stack st;` - nežinomas `sizeof()`
    - **Galima:** `struct Stack *pst;` - pointer'io dydis žinomas
    - **Problema:** Vartotojas negali alokuoti - reikia **factory!**
    - **Information hiding:** Struktūros detalės **paslėptos** ✅

---

## 8️⃣Etapas: "Stekas su Dinaminiu Valdymu"

> [`08_Acquiring_RESOURCES`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/08_Acquiring_RESOURCES/)

!!! note "🎯 Tikslas"
    **Factory pattern** - sukurti `create()` ir `destroy()` funkcijas, kurios valdo objektų **lifecycle** - kūrimą ir naikinimą.

!!! info "🔍 Ką darysime"
    - Sukursime `create()` - alokuoja ir inicializuoja
    - Sukursime `destroy()` - atlaisvina atmintį
    - Suprasime **resource acquisition** ir **cleanup** svarbą
    - Pamatysime **manual lifecycle management** C kalboje

---

### 1 sprendimas: Factory pattern

> [`08_Acquiring_RESOURCES/02_OK`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/08_Acquiring_RESOURCES/02_OK/)

!!! quote "sumanymas/ketinimas"
    Sukursiu **factory funkcijas** modulyje: `create()` alokuos ir inicializuos, `destroy()` atlaisvins atmintį. Vartotojas turės validius pointer'ius!

=== "stack.h"

    ???+ "📄 08_Acquiring_RESOURCES/02_OK/stack.h"
    
        ```c linenums="1" hl_lines="8-9"
        --8<-- "code/evolution/stack-2026/08_Acquiring_RESOURCES/02_OK/stack.h"
        ```

=== "stack.c"

    ???+ "📄 08_Acquiring_RESOURCES/02_OK/stack.c"
    
        ```c linenums="1" hl_lines="10-17 44-47"
        --8<-- "code/evolution/stack-2026/08_Acquiring_RESOURCES/02_OK/stack.c"
        ```

=== "user.c"

    ???+ "📄 08_Acquiring_RESOURCES/02_OK/user.c"
    
        ```c linenums="1" hl_lines="6-7 16-17"
        --8<-- "code/evolution/stack-2026/08_Acquiring_RESOURCES/02_OK/user.c"
        ```

=== "🔨 = ⚙️➔🔗➔🚀"

    ```bash
    gcc -c stack.c -o stack.o  # ✅
    gcc -c user.c -o user.o    # ✅
    gcc stack.o user.o -o app  # ✅
    ./app
    ```

=== "⌨️➔🖥️"

    ```
    123
    321
    ```

---

#### ✅ Veikia!

??? success "Factory pattern veikia!"
    ```c
    // Kūrimas (acquisition)
    struct Stack *pst1 = create();
    
    // Naudojimas
    push(pst1, '1');
    char c = pop(pst1);
    
    // Naikinimas (cleanup)
    destroy(pst1);
    ```
    
    **Privalumai:**
    - ✅ **Dinaminė alokacija** - `malloc()` modulyje
    - ✅ **Inicializacija** - `create()` užtikrina teisingą būseną
    - ✅ **Cleanup** - `destroy()` atlaisvina atmintį
    - ✅ **Keli egzemplioriai** - kiek tik reikia!
    - ✅ **Information hiding** - struktūra paslėpta

??? danger "Problemos, pavojai..."
    - **Manual lifecycle:** Vartotojas turi prisiminti `destroy()`
    - **Memory leak:** Užmiršus `destroy()` - memory leak
    - **Dangling pointer:** Po `destroy()` pointer'is invalid
    - **Double free:** Du kartus `destroy()` - undefined behavior

---

### 💡 Pamokos

!!! tip "Factory Pattern & Resource Management"
    - **`create()`** - alokuoja + inicializuoja → **acquisition**
    - **`destroy()`** - atlaisvina → **cleanup**
    - **Manual lifecycle:** Vartotojas atsakingas už `destroy()`
    - **C++ evoliucija:** Konstruktorius + destruktorius **automatiškai**!

---

## 9️⃣Etapas: "Stekas su Gyvavimo Ciklu"

> [`09_Completing_LIFECYCLE`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/09_Completing_LIFECYCLE/)

!!! note "🎯 Tikslas"
    Užbaigti **pilną ADT C kalboje** - pridėti visas lifecycle funkcijas, error handling, ir pasiekti **maksimalų abstrakcijos lygį**.

!!! info "🔍 Ką pasiekėme"
    - ✅ **Information hiding** - opaque pointer
    - ✅ **Encapsulation** - duomenys + funkcijos modulyje
    - ✅ **Multiple instances** - kiek tik reikia stekų
    - ✅ **Factory pattern** - `create()` / `destroy()`
    - ✅ **Pilnas lifecycle** - acquisition → usage → cleanup

---

### 1 sprendimas: Pilnas C ADT

> [`09_Completing_LIFECYCLE/01_OK`](https://github.com/ViktorasGolubevas/cpp2026-test/tree/main/code/evolution/stack-2026/09_Completing_LIFECYCLE/01_OK/)

!!! quote "sumanymas/ketinimas"
    Apibendrinsiu viską - pridėsiu visas reikalingas funkcijas pilnam ADT funkcionavimui.

=== "stack.h"

    ???+ "📄 09_Completing_LIFECYCLE/01_OK/stack.h"
    
        ```c linenums="1" hl_lines="8-14"
        --8<-- "code/evolution/stack-2026/09_Completing_LIFECYCLE/01_OK/stack.h"
        ```

=== "stack.c"

    ???+ "📄 09_Completing_LIFECYCLE/01_OK/stack.c"
    
        ```c linenums="1" hl_lines="10-17 44-47"
        --8<-- "code/evolution/stack-2026/09_Completing_LIFECYCLE/01_OK/stack.c"
        ```

=== "user.c"

    ???+ "📄 09_Completing_LIFECYCLE/01_OK/user.c"
    
        ```c linenums="1" hl_lines="6-7"
        --8<-- "code/evolution/stack-2026/09_Completing_LIFECYCLE/01_OK/user.c"
        ```

=== "🔨 = ⚙️➔🔗➔🚀"

    ```bash
    gcc -c stack.c -o stack.o  # ✅
    gcc -c user.c -o user.o    # ✅
    gcc stack.o user.o -o app  # ✅
    ./app
    ```

=== "⌨️➔🖥️"

    ```
    123
    321
    ```

---

#### ✅ Pilnas C ADT!

??? success "Visi ADT reikalavimai įvykdyti!"
    **ADT principai C kalboje:**
    
    1. ✅ **Information hiding** - struktūra paslėpta (opaque pointer)
    2. ✅ **Encapsulation** - duomenys + operacijos modulyje
    3. ✅ **Multiple instances** - `struct Stack *pst1, *pst2, ...;`
    4. ✅ **Lifecycle management** - `create()` → use → `destroy()`
    5. ✅ **Factory pattern** - kontroliuota kūrimas/naikinimas
    
    **Pilnas ADT API:**
    ```c
    struct Stack *create();        // Acquisition
    void init(struct Stack *pst);  // Initialization
    void push(struct Stack *pst, char c);
    char pop(struct Stack *pst);
    int isEmpty(struct Stack *pst);
    int isFull(struct Stack *pst);
    void destroy(struct Stack *pst);  // Cleanup
    ```

??? warning "C kalbos ribotumai"
    - **Manual lifecycle:** Reikia prisiminti `destroy()`
    - **No automatic cleanup:** Užmiršus - memory leak
    - **Syntax:** `push(pst, 'a')` vietoj `pst->push('a')`
    - **No operator overloading:** Negalima `pst1 = pst2;`

---

### 💡 Pamokos

!!! tip "C ADT → C++ Class Evolution"
    **C kalboje pasiekėme:**
    - ✅ Information hiding
    - ✅ Encapsulation
    - ✅ Factory pattern
    - ✅ Manual lifecycle management
    
    **C++ prideda:**
    - 🚀 **Konstruktorius** - automatic `create()`
    - 🚀 **Destruktorius** - automatic `destroy()`
    - 🚀 **Method syntax** - `s.push('a')`
    - 🚀 **Operator overloading** - `s1 = s2;`
    - 🚀 **RAII** - Resource Acquisition Is Initialization

---

## 🎯 Evoliucijos santrauka

### 9 etapų kelias

| Nr | Etapas | Failai | Sprendimų | Info Hiding? | Lifecycle? | Esmė |
|----|--------|--------|----------|--------------|------------|------|
| **01** | Running_PROGRAM | `usestack.c` | 1 | ❌ | ❌ | Monolitas |
| **02** | Decomposing_to_UNITS | `stack.c, user.c` | 3 | ❌ | ❌ | NL klaidos |
| **03** | Discovering_C_MODULE | `stack.h/c, user.c` | 4 | ❌ | ❌ | Modulis + extern |
| **04** | Protecting_IMPLEMENTATION | `stack.h/c, user.c` | 2 | ✅ (1 stekas) | ❌ | `static` linkage |
| **05** | Defining_USER_TYPE | `usestack.c` | 3 | ❌ | ❌ | Struct monolite |
| **06** | ENCAPSULATING_UDT | `stack.h/c, user.c` | 2 | ❌ | ❌ | Struct + modulis |
| **07** | HIDING_UDT_Information | `stack.h/c, user.c` | 3 | ✅ | ❌ | Opaque pointer |
| **08** | Acquiring_RESOURCES | `stack.h/c, user.c` | 1 | ✅ | ✅ (manual) | Factory |
| **09** | Completing_LIFECYCLE | `stack.h/c, user.c` | 1 | ✅ | ✅ | Pilnas ADT |

---

## 🚀 Šuolis į C++

### C ADT (Etapas 09):
```c
struct Stack *s = create();
push(s, 'a');
char c = pop(s);
destroy(s);  // ← Reikia prisiminti!
```

### C++ Class:
```cpp
Stack s;               // Konstruktorius (automatic create)
s.push('a');          // Method syntax
char c = s.pop();
// Destruktorius automatiškai (automatic destroy)
```

**Kitas žingsnis:** C++ Stack evoliucija! 🚀
