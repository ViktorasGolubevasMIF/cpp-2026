# 📚 Stack ADT Evoliucija: Nuo C iki C++
## Pedagoginis vadovas - 9 etapai

> **Versija:** 2026.01 (GALUTINĖ)  
> **Kodas:** `code/evolution/stack-2026/`  
> **Etapai:** 01 → 09 (pilna C evoliucija)  
> **Tikslas:** Parodyti tikrą ADT evoliuciją su **kompiliavimo/linkavimo klaidomis**

---

## 🎯 Subdirektorijų žymėjimai

| Žymėjimas | Reikšmė | Kas nutinka |
|-----------|---------|-------------|
| **NC** | **N**ot **C**ompiling | Kompiliavimo klaida (syntax/type error) |
| **NL** | **N**ot **L**inking | Linkavimo klaida (multiple definition/undefined reference) |
| **OK** | Kompiliuojasi ir veikia | Bet gali būti **dizaino problema** |
| **OK_Attack** | Veikia, bet **ataka pavyksta** | Parodo saugumo spragą |
| **RT** | **R**un**T**ime error | Kompiliuojasi, bet crash/neteisingi rezultatai |

---

## 📖 9 ETAPŲ APŽVALGA

```
01_Running_PROGRAM                    → Monolitas
02_Decomposing_to_UNITS              → Fizinis padalijimas (NL klaidos!)
03_Discovering_C_MODULE              → Modulis (atakos pavyksta!)
04_Protecting_IMPLEMENTATION         → static slėpimas (NL ataka nepavyksta!)
05_Defining_USER_TYPE                → struct (NC, RT klaidos!)
06_ENCAPSULATING_UDT_in_Module       → Modulinis UDT (atakos pavyksta!)
07_HIDING_UDT_Information            → Opaque pointer (NC ataka nepavyksta!)
08_Acquiring_RESOURCES               → Factory (create/destroy)
09_Completing_LIFECYCLE              → Pilnas lifecycle
```

---

# 🚀 ETAPAS 01: Running_PROGRAM

### 📂 Katalogo struktūra
```
01_Running_PROGRAM/
└── stack.c
```

### 📌 Koncepcija
**MONOLITH** - visa logika vienoje vietoje. Globalūs kintamieji. **Veikia, bet...**

### 💻 Kodas
**Failas:** `stack.c`

```c
#include <stdio.h>
#define SIZE 5

char stack[SIZE];  // ← globalus
int top = 0;

void init(void) {
    top = 0;
}

int isEmpty(void) {
    return 0 == top;
}

int isFull(void) {
    return SIZE == top;
}

void push(char c) {
    if (!isFull()) {
        stack[top++] = c;
    }
}

char pop(void) {
    if (!isEmpty()) {
        return stack[--top];
    }
    return '\0';
}

int main(void) {
    char c;
    init();
    
    while ((!isFull()) && ('\n' != (c = getchar()))) 
        push(c);
    
    while (!isEmpty()) 
        putchar(pop());
    
    putchar('\n');
    return 0;
}
```

### 🔧 Kompiliavimas
```bash
gcc stack.c -o stack
```

### ✅ Veikia!
```
Input:  12345
Output: 54321
```

### ❌ Problemos
1. Globalūs `stack[]` ir `top` - matomi visur
2. Tik **vienas** stekas sistemoje
3. Viskas viename faile - negalima pakartotinio naudojimo
4. Jokio inkapsuliavimo

### 💡 Pamoka
> **Veikiantis kodas ≠ geras kodas**

---

# 🚀 ETAPAS 02: Decomposing_to_UNITS

### 📂 Katalogo struktūra
```
02_Decomposing_to_UNITS/
├── 02_NL/              ← Linkavimo klaida!
│   ├── stack.c
│   └── user.c
└── 03_OK_Wrong/        ← Veikia, bet BLOGAI
    ├── stack.c
    └── user.c
```

### 📌 Koncepcija
**DECOMPOSING** - bandome atskirti logiką į failus

---

## 02_NL: Linkavimo klaida

### 💻 Kodas
**Failas:** `02_NL/stack.c`
```c
#define SIZE 5

char stack[SIZE];
int top = 0;

void init(void) {
    top = 0;
}

int isEmpty(void) {
    return 0 == top;
}

int isFull(void) {
    return SIZE == top;
}

void push(char c) {
    if (!isFull()) {
        stack[top++] = c;
    }
}

char pop(void) {
    if (!isEmpty()) {
        return stack[--top];
    }
    return '\0';
}
```

**Failas:** `02_NL/user.c`
```c
#include <stdio.h>
#include "stack.c"  // ⚠️ KLAIDA: include .c failą!

int main(void) {
    char c;
    init();
    
    while ((!isFull()) && ('\n' != (c = getchar()))) 
        push(c);
    
    while (!isEmpty()) 
        putchar(pop());
    
    putchar('\n');
    return 0;
}
```

### 🔧 Kompiliavimas
```bash
gcc stack.c user.c -o app
```

### ❌ **NL** - Linkavimo klaida!
```
multiple definition of `init'
multiple definition of `isEmpty'
multiple definition of `isFull'
multiple definition of `push'
multiple definition of `pop'
multiple definition of `stack'
multiple definition of `top'
```

### 🔍 Kodėl?
- `user.c` daro `#include "stack.c"` → visos funkcijos nukopijuojamos
- Tada kompiliuojame **ir** `stack.c` atskirai
- Linkeris mato **dvigubas** definicijas!

---

## 03_OK_Wrong: Veikia, bet BLOGAI

### 💻 Kodas
**Failas:** `03_OK_Wrong/user.c`
```c
#include <stdio.h>
#include "stack.c"  // ⚠️ ANTIPATTERN!

int main(void) {
    char c;
    init();
    
    while ((!isFull()) && ('\n' != (c = getchar()))) 
        push(c);
    
    while (!isEmpty()) 
        putchar(pop());
    
    putchar('\n');
    return 0;
}
```

### 🔧 Kompiliavimas
```bash
gcc user.c -o app  # ← Tik user.c!
```

### ✅ Veikia!
```
Input:  123
Output: 321
```

### ❌ Kodėl BLOGAI?
- `#include "stack.c"` yra **antipattern**!
- `.c` failai neturėtų būti include'inami
- Nėra headerio → funkcijų prototipai "implicit"

### 💡 Pamoka
> **`#include "modulis.c"` = ANTIPATTERN!**  
> **Teisingai:** `#include "modulis.h"` + kompiliuoti atskirai

---

# 🚀 ETAPAS 03: Discovering_C_MODULE

### 📂 Katalogo struktūra
```
03_Discovering_C_MODULE/
├── 04_OK/              ← Tinkamas modulis
│   ├── stack.h
│   ├── stack.c
│   └── user.c
└── 05_OK_Attack/       ← Ataka pavyksta!
    ├── stack.h
    ├── stack.c
    └── user.c
```

### 📌 Koncepcija
**MODULE** - header (.h) + implementation (.c)

---

## 04_OK: Tinkamas modulis

### 💻 Kodas
**Failas:** `04_OK/stack.h`
```c
#define SIZE 5

// ✅ Tik deklaracijos
extern char stack[SIZE];
extern int top;

void init(void);
int isEmpty(void);
int isFull(void);
void push(char c);
char pop(void);
```

**Failas:** `04_OK/stack.c`
```c
#include "stack.h"

// ✅ Apibrėžimai .c faile
char stack[SIZE];
int top = 0;

void init(void) {
    top = 0;
}

int isEmpty(void) {
    return 0 == top;
}

int isFull(void) {
    return SIZE == top;
}

void push(char c) {
    if (!isFull()) {
        stack[top++] = c;
    }
}

char pop(void) {
    if (!isEmpty()) {
        return stack[--top];
    }
    return '\0';
}
```

**Failas:** `04_OK/user.c`
```c
#include <stdio.h>
#include "stack.h"  // ✅ Teisingai!

int main(void) {
    char c;
    init();
    
    while ((!isFull()) && ('\n' != (c = getchar()))) 
        push(c);
    
    while (!isEmpty()) 
        putchar(pop());
    
    putchar('\n');
    return 0;
}
```

### 🔧 Kompiliavimas
```bash
gcc stack.c user.c -o app
```

### ✅ Veikia!
```
Input:  123456
Output: 654321
```

---

## 05_OK_Attack: Ataka pavyksta!

### 💻 Kodas
**Failas:** `05_OK_Attack/user.c`
```c
#include <stdio.h>
#include "stack.h"

extern char stack[];  // 🔥 Tiesioginis prieiga!
extern int top;       // 🔥

int main(void) {
    char c;
    init();
    
    while ((!isFull()) && ('\n' != (c = getchar()))) 
        push(c);
    
    stack[top++] = '!';  // 🔥 ATAKA!
    
    while (!isEmpty()) 
        putchar(pop());
    putchar('\n');
    
    return 0;
}
```

### ✅ Kompiliuojasi! Ataka **PAVYKO**!
```
Input:  123456
Output: !654321  ← '!' įterptas!
```

### 💡 Pamoka
> **Moduliavimas ≠ Saugumas**  
> Duomenys vis dar **neslėpti**!

---

# 🚀 ETAPAS 04: Protecting_IMPLEMENTATION

### 📂 Katalogo struktūra
```
04_Protecting_IMPLEMENTATION/
├── 01_NL_Attack/       ← Ataka NEPAVYKSTA! (NL)
│   ├── stack.h
│   ├── stack.c
│   └── user_attack.c
└── 02_OK/              ← Veikia teisingai
    ├── stack.h
    ├── stack.c
    └── user.c
```

### 📌 Koncepcija
**STATIC** - internal linkage, **information hiding**

---

## 02_OK: Tinkamas kodas

### 💻 Kodas
**Failas:** `02_OK/stack.h`
```c
#define SIZE 5

// ✅ TIKTAI funkcijų prototipai (nėra extern stack/top!)
void init(void);
int isEmpty(void);
int isFull(void);
void push(char c);
char pop(void);
```

**Failas:** `02_OK/stack.c`
```c
#include "stack.h"

#define SIZE 5

static char stack[SIZE];    // ✅ static = internal linkage!
static int top = 0;         // ✅ Paslėpta!

static void reset(void) {   // ✅ Private funkcija
    top = 0;
}

void init(void) {
    top = 0;
}

int isEmpty(void) {
    return 0 == top;
}

int isFull(void) {
    return SIZE == top;
}

void push(char c) {
    if (!isFull()) {
        stack[top++] = c;
    }
}

char pop(void) {
    if (!isEmpty()) {
        return stack[--top];
    }
    return '\0';
}
```

**Failas:** `02_OK/user.c`
```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;
    init();
    
    while ((!isFull()) && ('\n' != (c = getchar()))) 
        push(c);
    
    while (!isEmpty()) 
        putchar(pop());
    
    putchar('\n');
    return 0;
}
```

### 🔧 Kompiliavimas
```bash
gcc stack.c user.c -o app
```

### ✅ Veikia!

---

## 01_NL_Attack: Ataka nepavyksta!

### 💻 Kodas
**Failas:** `01_NL_Attack/user_attack.c`
```c
#include <stdio.h>
#include "stack.h"

extern char stack[];     // 🔥 Bandome pasiekti
extern int top;          // 🔥
extern void reset(void); // 🔥 Bandome iškviesti private funkciją

int main(void) {
    char c;
    init();
    
    while ((!isFull()) && ('\n' != (c = getchar()))) 
        push(c);
    
    stack[top++] = '!';  // 🔥 ATAKA!
    
    while (!isEmpty()) 
        putchar(pop());
    putchar('\n');
    
    reset();  // 🔥 ATAKA!
    return 0;
}
```

### 🔧 Kompiliavimas
```bash
gcc stack.c user_attack.c -o app
```

### ❌ **NL** - Linkavimo klaida!
```
undefined reference to `reset'
undefined reference to `stack'
undefined reference to `top'
```

### ✅ ATAKA **NEPAVYKO**! 🎉

### 💡 Pamoka
> **`static` = internal linkage**  
> Matoma **TIK** savo `.c` faile!  
> Tai C kalbos **information hiding** mechanizmas!

### ⚠️ Problema
Vis tiek **tik vienas stekas** sistemoje!

---

# 🚀 ETAPAS 05: Defining_USER_TYPE

### 📂 Katalogo struktūra
```
05_Defining_USER_TYPE/
├── 01_NC_Static_in_Struct/  ← NC klaida!
├── 02_RT_PassByValue/        ← RT klaida!
└── 03_OK/                    ← Veikia!
```

### 📌 Koncepcija
**USER-DEFINED TYPE** - `struct` tipas, kelių egzempliorių galimybė

---

## 01_NC_Static_in_Struct: Kompiliavimo klaida

### 💻 Kodas (konceptualus)
```c
struct Stack {
    static char stack[SIZE];  // ❌ C neleidžia!
    static int top;           // ❌
};
```

### ❌ **NC** - Kompiliavimo klaida!
```
error: expected specifier-qualifier-list before 'static'
error: type name does not allow storage class to be specified
```

### 💡 Pamoka
> **C draudžia `static` struktūros nariams!**  
> C++ turi `static` narius, bet C - **NE**

---

## 02_RT_PassByValue: Runtime klaida

### 💻 Kodas (konceptualus)
```c
struct Stack {
    char stack[SIZE];
    int top;
};

// ❌ Pass-by-value - modifikuoja tik kopiją!
void init(struct Stack st) {
    st.top = 0;  // Neįsimena!
}

void push(struct Stack st, char c) {
    if (!isFull(st)) {
        st.stack[st.top++] = c;  // Neįsimena!
    }
}
```

### ✅ Kompiliuojasi, bet **RT** - neteisingi rezultatai!
```
Input:  123
Output: (tuščia)  ← Duomenys nepasikeitė!
```

### 💡 Pamoka
> **Pass-by-value** - C struktūros kopijuojamos!  
> Reikia naudoti **pointers**!

---

## 03_OK: Tinkamas variantas

### 💻 Kodas (konceptualus)
```c
struct Stack {
    char stack[SIZE];
    int top;
};

// ✅ Pass-by-pointer!
void init(struct Stack *pst) {
    pst->top = 0;
}

void push(struct Stack *pst, char c) {
    if (!isFull(pst)) {
        pst->stack[pst->top++] = c;
    }
}

int main(void) {
    struct Stack st1, st2;  // ✅ Du nepriklausomi stekai!
    
    init(&st1);
    push(&st1, 'A');
    
    init(&st2);
    push(&st2, 'B');
    
    // ...
}
```

### ✅ Veikia! Dabar turime **kelis** stekus!

### 💡 Pamoka
> **Struktūra = vartotojo tipas**  
> `struct Stack s1, s2, s3;` - kiek norime egzempliorių!

---

# 🚀 ETAPAS 06: ENCAPSULATING_UDT_in_Module

### 📂 Katalogo struktūra
```
06_ENCAPSULATING_UDT_in_Module/
├── 01_OK/              ← Veikia teisingai
│   ├── stack.h
│   ├── stack.c
│   └── user.c
└── 02_OK_Attack/       ← Ataka pavyksta!
    ├── stack.h
    ├── stack.c
    └── user_attack.c
```

### 📌 Koncepcija
**ENCAPSULATION** - UDT modulyje, bet struktūra **vieša**

---

## 01_OK: Modulinis UDT

### 💻 Kodas
**Failas:** `01_OK/stack.h`
```c
#define SIZE 5

// ✅ Struktūra header'yje
struct Stack {
    char stack[SIZE];
    int top;
};

void init(struct Stack *s);
int isEmpty(struct Stack *s);
int isFull(struct Stack *s);
void push(struct Stack *s, char c);
char pop(struct Stack *s);
```

**Failas:** `01_OK/stack.c`
```c
#include "stack.h"

void init(struct Stack *s) {
    s->top = 0;
}

int isEmpty(struct Stack *s) {
    return 0 == s->top;
}

int isFull(struct Stack *s) {
    return SIZE == s->top;
}

void push(struct Stack *s, char c) {
    if (!isFull(s)) {
        s->stack[s->top++] = c;
    }
}

char pop(struct Stack *s) {
    if (!isEmpty(s)) {
        return s->stack[--s->top];
    }
    return '\0';
}
```

**Failas:** `01_OK/user.c`
```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;
    struct Stack st1, st2;
    
    printf("Input 1st Stack:\n");
    init(&st1);
    while ((!isFull(&st1)) && ('\n' != (c = getchar()))) 
        push(&st1, c);
    
    while (!isEmpty(&st1)) 
        putchar(pop(&st1));
    putchar('\n');
    printf("1st Stack is empty\n\n");
    
    printf("Input 2nd Stack:\n");
    init(&st2);
    while ((!isFull(&st2)) && ('\n' != (c = getchar()))) 
        push(&st2, c);
    
    while (!isEmpty(&st2)) 
        putchar(pop(&st2));
    putchar('\n');
    printf("2nd Stack is empty\n\n");
    
    return 0;
}
```

### ✅ Veikia!

---

## 02_OK_Attack: Ataka pavyksta!

### 💻 Kodas
**Failas:** `02_OK_Attack/user_attack.c`
```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;
    struct Stack st1, st2;
    
    printf("Input 1st Stack:\n");
    init(&st1);
    while ((!isFull(&st1)) && ('\n' != (c = getchar()))) 
        push(&st1, c);
    
    while (!isEmpty(&st1)) 
        putchar(pop(&st1));
    putchar('\n');
    printf("1st Stack is empty\n\n");
    
    printf("Input 2nd Stack:\n");
    init(&st2);
    while ((!isFull(&st2)) && ('\n' != (c = getchar()))) 
        push(&st2, c);
    
    //// 🔥 ATAKA!
    st2.stack[st2.top++] = '!';
    ////
    
    while (!isEmpty(&st2)) 
        putchar(pop(&st2));
    putchar('\n');
    printf("2nd Stack is empty\n\n");
    
    return 0;
}
```

### ✅ Kompiliuojasi! Ataka **PAVYKO**!
```
Input 1st Stack:
1234
4321
1st Stack is empty

Input 2nd Stack:
4567
!7654  ← '!' įterptas!
2nd Stack is empty
```

### 💡 Pamoka
> **Encapsulation ≠ Information Hiding**  
> Struktūra matoma → **galima modifikuoti** laukus!

---

# 🚀 ETAPAS 07: HIDING_UDT_Information

### 📂 Katalogo struktūra
```
07_HIDING_UDT_Information/
└── 01_NC[incomplete_type]/  ← NC ataka!
    ├── stack.h
    ├── stack.c
    ├── _user.c          ← NC: incomplete type
    └── user_attack.c    ← NC: bandymas atakuoti
```

### 📌 Koncepcija
**OPAQUE POINTER** - forward declaration, **incomplete type**

---

## 01_NC[incomplete_type]: Ataka nepavyksta!

### 💻 Kodas
**Failas:** `01_NC/stack.h`
```c
#define SIZE 5

// ✅ INCOMPLETE TYPE - tik deklaracija!
struct Stack;

void init(struct Stack *s);
int isEmpty(struct Stack *s);
int isFull(struct Stack *s);
void push(struct Stack *s, char c);
char pop(struct Stack *s);
```

**Failas:** `01_NC/stack.c`
```c
#include "stack.h"

// ✅ Pilnas apibrėžimas TIK .c faile!
struct Stack {
    char stack[SIZE];
    int top;
};

void init(struct Stack *s) {
    s->top = 0;
}

int isEmpty(struct Stack *s) {
    return 0 == s->top;
}

int isFull(struct Stack *s) {
    return SIZE == s->top;
}

void push(struct Stack *s, char c) {
    if (!isFull(s)) {
        s->stack[s->top++] = c;
    }
}

char pop(struct Stack *s) {
    if (!isEmpty(s)) {
        return s->stack[--s->top];
    }
    return '\0';
}
```

**Failas:** `01_NC/_user.c` (Bandymas naudoti)
```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;
    
    // ❌ KLAIDA: negalima deklaruoti
    struct Stack st1, st2;  // ← NC klaida!
    
    // ...
    
    return 0;
}
```

### ❌ **NC** - Kompiliavimo klaida!
```
error: storage size of 'st1' isn't known
error: storage size of 'st2' isn't known
error: variable has incomplete type 'struct Stack'
```

---

**Failas:** `01_NC/user_attack.c` (Bandymas atakuoti)
```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;
    struct Stack st1, st2;  // ← NC!
    
    init(&st1);
    push(&st1, 'A');
    
    //// 🔥 BANDYMAS ATAKUOTI!
    st2.stack[st2.top++] = '!';  // ← NC!
    ////
    
    return 0;
}
```

### ❌ Kompiliavimo klaida!
```
error: storage size of 'st1' isn't known
error: storage size of 'st2' isn't known
```

### ✅ ATAKA **NEPAVYKO**! 🎉

### 💡 Pamoka
> **Opaque pointer = tikras information hiding**  
> Vartotojas **nežino** struktūros apibrėžimo!  
> **Forward declaration** (`struct Stack;`) = incomplete type

### ⚠️ Problema
Kaip sukurti steką?  
`struct Stack st;` - **neveikia** (incomplete type)!  
**Sprendimas:** `create()` factory funkcija!

---

# 🚀 ETAPAS 08: Acquiring_RESOURCES

### 📂 Katalogo struktūra
```
08_Acquiring_RESOURCES/
└── 02_OK/
    ├── stack.h
    ├── stack.c
    └── user.c
```

### 📌 Koncepcija
**FACTORY PATTERN** - `create()` / `destroy()` funkcijos

---

## 02_OK: Factory Pattern

### 💻 Kodas
**Failas:** `02_OK/stack.h`
```c
#define SIZE 5

struct Stack;  // ← opaque type

// ✅ Factory funkcijos
struct Stack* create(void);
void destroy(struct Stack *s);

void init(struct Stack *s);
int isEmpty(struct Stack *s);
int isFull(struct Stack *s);
void push(struct Stack *s, char c);
char pop(struct Stack *s);
```

**Failas:** `02_OK/stack.c`
```c
#include <stdlib.h>
#include "stack.h"

struct Stack {
    char stack[SIZE];
    int top;
};

// ✅ Factory: sukūrimas
struct Stack* create(void) {
    struct Stack *s = (struct Stack*)malloc(sizeof(struct Stack));
    if (s != NULL) {
        s->top = 0;
    }
    return s;
}

// ✅ Factory: sunaikinimas
void destroy(struct Stack *s) {
    free(s);
}

void init(struct Stack *s) {
    if (s != NULL) {
        s->top = 0;
    }
}

int isEmpty(struct Stack *s) {
    return (s != NULL) && (0 == s->top);
}

int isFull(struct Stack *s) {
    return (s != NULL) && (SIZE == s->top);
}

void push(struct Stack *s, char c) {
    if ((s != NULL) && !isFull(s)) {
        s->stack[s->top++] = c;
    }
}

char pop(struct Stack *s) {
    if ((s != NULL) && !isEmpty(s)) {
        return s->stack[--s->top];
    }
    return '\0';
}
```

**Failas:** `02_OK/user.c`
```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;
    
    // ✅ Sukuriame per factory
    struct Stack *pst1 = create();
    struct Stack *pst2 = create();
    
    printf("Input 1st Stack:\n");
    init(pst1);
    while ((!isFull(pst1)) && ('\n' != (c = getchar()))) 
        push(pst1, c);
    
    while (!isEmpty(pst1)) 
        putchar(pop(pst1));
    putchar('\n');
    printf("1st Stack is empty\n\n");
    
    printf("Input 2nd Stack:\n");
    init(pst2);
    while ((!isFull(pst2)) && ('\n' != (c = getchar()))) 
        push(pst2, c);
    
    while (!isEmpty(pst2)) 
        putchar(pop(pst2));
    putchar('\n');
    printf("2nd Stack is empty\n\n");
    
    // ✅ Atlaisviname atmintį
    destroy(pst1);
    destroy(pst2);
    
    return 0;
}
```

### ✅ Veikia!
```
Input 1st Stack:
1234
4321
1st Stack is empty

Input 2nd Stack:
4567
7654
2nd Stack is empty
```

### 💡 Pamoka
> **Factory pattern** - `create()` / `destroy()`  
> Prototipai C++ **konstruktoriams/destruktoriams**!  
> **Manual lifecycle management**

---

# 🚀 ETAPAS 09: Completing_LIFECYCLE

### 📂 Katalogo struktūra
```
09_Completing_LIFECYCLE/
└── 01_OK/
    ├── stack.h
    ├── stack.c
    └── user.c
```

### 📌 Koncepcija
**LIFECYCLE COMPLETION** - pilnas objekto gyvavimo ciklo valdymas

> **Pastaba:** Šis etapas papildo 08 etapą su dar griežtesniu lifecycle valdymu  
> (pvz., error handling, resursų tikrinimas, ir pan.)

### 💡 C dalies išvada

```
✅ Pasiekta (9 etapai):
  • Information hiding (opaque pointer)
  • Lifecycle management (create/destroy)
  • Multiple instances
  • True Abstract Data Type
  • Factory pattern

❌ Trūksta (C++ suteiks):
  • Automatinio lifecycle (RAII)
  • Type safety (konstruktorius/destruktorius)
  • Operators (operatorių perkrovimas)
  • Templates (generics)
```

---

# 🎓 APIBENDRINIMAS

## 9 Etapų Lentelė

| Nr | Katalogo pavadinimas | Pagrindinė koncepcija | Info Hiding? | Lifecycle? | Subdirektorijos |
|----|---------------------|---------------------|--------------|------------|-----------------|
| 01 | `Running_PROGRAM` | Monolitas | ❌ | ❌ | - |
| 02 | `Decomposing_to_UNITS` | Fizinis padalijimas | ❌ | ❌ | `02_NL`, `03_OK_Wrong` |
| 03 | `Discovering_C_MODULE` | C modulis | ❌ | ❌ | `04_OK`, `05_OK_Attack` |
| 04 | `Protecting_IMPLEMENTATION` | `static` linkage | ✅ (bet 1 stekas) | ❌ | `01_NL_Attack`, `02_OK` |
| 05 | `Defining_USER_TYPE` | `struct` tipas | ❌ | ❌ | `01_NC`, `02_RT`, `03_OK` |
| 06 | `ENCAPSULATING_UDT_in_Module` | Modulinis UDT | ❌ | ❌ | `01_OK`, `02_OK_Attack` |
| 07 | `HIDING_UDT_Information` | Opaque pointer | ✅ | ❌ | `01_NC[incomplete_type]` |
| 08 | `Acquiring_RESOURCES` | Factory pattern | ✅ | ✅ (manual) | `02_OK` |
| 09 | `Completing_LIFECYCLE` | Lifecycle completion | ✅ | ✅ | `01_OK` |

---

## 🎯 Mokymosi rezultatai

### C kalboje studentai išmoko:
- ✅ **Moduliavimą** - header (.h) + implementation (.c)
- ✅ **Information hiding** - `static` internal linkage
- ✅ **User-defined types** - `struct` ir rodyklės
- ✅ **Opaque pointer idiom** - forward declaration
- ✅ **Factory pattern** - manual resource management
- ✅ **Kompiliavimo/linkavimo klaidas** - kaip įrankis mokymuisi

### Principai:
- ✅ **Encapsulation vs Information Hiding** - skirtumas
- ✅ **ADT** (Abstract Data Type) - tikra prasmė
- ✅ **Separation of Concerns** - modulinis dizainas
- ✅ **Lifecycle management** - sukūrimas/sunaikinimas

---

## 📝 Studentų užduotys

### Užduotis 1: Evoliucijos analizė
Kiekvienam etapui (01-09):
1. Paaiškinkite, **kas pagerėjo** lyginant su ankstesniu
2. Nurodykite, **kodėl ankstesnis** sprendimas buvo nepakankamas
3. Parodykite **kompiliavimo/linkavimo klaidą** ir jos priežastį

### Užduotis 2: Klaidų kategorijos
Klasifikuokite subdirektorijas:
- **NC** (Not Compiling) - kokios klaidos?
- **NL** (Not Linking) - kodėl linker nesuranda simbolių?
- **OK_Attack** - kodėl ataka pavyksta?

### Užduotis 3: Savarankiškas ADT
Sukurkite **Queue** (eilę) ADT naudodami:
- Etapą 04: su `static` (viena eilė)
- Etapą 07: su opaque pointer (kelios eilės)
- Etapą 08: su factory pattern

### Užduotis 4: Refleksija
> **"Kodėl C++ reikalingas, jei C gali pasiekti tą patį (09 etapas)?"**

---

## ⚙️ Kompiliavimo instrukcijos

### C programos
```bash
gcc -Wall -Wextra -std=c11 -o prog stack.c user.c
```

---

## 🎬 Galutinė išvada

> **9 etapų kelionė** - nuo **naivaus monolito** link **profesionalaus ADT**.  
> Kiekvienas žingsnis **išsprendžia konkrečią problemą**, vedantis link **OOP principų**.

**Svarbiausias takeaway:**
- ✅ Suprasti **KODĖL**, o ne tik **KAIP**
- ✅ C++ = **C evoliucija** (ne konkurentas!)
- ✅ **Kompiliavimo klaidos** = mokymo įrankis
- ✅ **Atakų scenarijai** = saugumo demonstracija

---

**Vėliau grįšime prie `typedef`, `const correctness`, `function pointers` C++ paskaitose! 😄**

---

**Sėkmės mokantis! 🚀**  
**Versija:** 2026.01 (9 etapai)  
**Licenzija:** Laisvai naudojamas mokymo tikslams
