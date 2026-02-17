# 📚 Stack ADT: Advanced C Techniques (Appendix)
## Etapai 10A–10D: "C jau viską turėjo!"

> **Versija:** 2026.01  
> **Tikslas:** Parodyti pažangias C technikas, kurios vėliau taps C++ funkcionalumu  
> **Naudojimas:** Grįžti prie šių temų **C++ paskaitų metu** ir parodyti evoliuciją

---

## 🎯 Apžvalga: 4 Advanced Techniques

| Etapas | Technika | C koncepcija | C++ analogas | Kada naudoti C++ paskaitose |
|--------|----------|--------------|--------------|------------------------------|
| **10A** | `typedef` Aliasing | Type alias | `using` alias, type aliases | Pristatant C++ type system |
| **10B** | `const` Correctness | `const` qualifier | `const` member functions | Pristatant C++ const correctness |
| **10C** | Function Pointers | Function pointers | Virtual functions, `std::function` | Pristatant polymorphism |
| **10D** | Generic (`void*`) | Type erasure | Templates | Pristatant C++ templates |

---

## 💡 Kaip naudoti šį appendix?

### Mokymo strategija:
1. **09 etape užbaikite C evoliuciją** (Factory + Lifecycle)
2. **C++ paskaitų metu grįžkite** prie 10A-10D
3. **Pasakykite:** *"Žiūrėkite, C jau turėjo tokią techniką!"*
4. **Parodykite:** Kaip C++ supaprastina/pagerina tą techniką

### Pavyzdys:
> **C++ paskaita apie Templates:**  
> *"Prieš mokydamiesi templates, pažiūrėkime, kaip C sprendė generic programavimą... Etapas 10D!"*

---

# 🚀 ETAPAS 10A: TYPEDEF Aliasing

## 📌 Koncepcija
**Type Aliasing** - sintaksinis cukrus, trumpesni tipų pavadinimai

## Problema su `struct Stack`
Visur rašome `struct Stack` - ilga, nepatogu:
```c
struct Stack *pst = create();
void push(struct Stack *s, char c);
```

## Sprendimas: `typedef`
```c
typedef struct Stack Stack;  // ← Alias!

// Dabar galima rašyti:
Stack *pst = create();
void push(Stack *s, char c);
```

---

## 10A Katalogo struktūra
```
10A_TYPEDEF_Aliasing/
├── 01_Without_Typedef/
│   ├── stack.h
│   ├── stack.c
│   └── user.c
├── 02_With_Typedef/
│   ├── stack.h
│   ├── stack.c
│   └── user.c
└── README.md
```

---

## 01_Without_Typedef: Ilgas kodas

### Kodas
**stack.h:**
```c
#define SIZE 5

struct Stack;

struct Stack* create(void);
void destroy(struct Stack *s);
void init(struct Stack *s);
void push(struct Stack *s, char c);
char pop(struct Stack *s);
int isEmpty(struct Stack *s);
int isFull(struct Stack *s);
```

**user.c:**
```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    struct Stack *pst1 = create();  // ← ilgas!
    struct Stack *pst2 = create();
    
    init(pst1);
    push(pst1, 'A');
    
    while (!isEmpty(pst1)) {
        putchar(pop(pst1));
    }
    
    destroy(pst1);
    destroy(pst2);
    return 0;
}
```

❌ **Problema:** `struct Stack` per ilgas, pasikartoja

---

## 02_With_Typedef: Trumpesnis kodas

### Kodas
**stack.h:**
```c
#define SIZE 5

// ✅ typedef alias
typedef struct Stack Stack;

Stack* create(void);
void destroy(Stack *s);
void init(Stack *s);
void push(Stack *s, char c);
char pop(Stack *s);
int isEmpty(Stack *s);
int isFull(Stack *s);
```

**user.c:**
```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    Stack *pst1 = create();  // ✅ trumpa!
    Stack *pst2 = create();
    
    init(pst1);
    push(pst1, 'A');
    
    while (!isEmpty(pst1)) {
        putchar(pop(pst1));
    }
    
    destroy(pst1);
    destroy(pst2);
    return 0;
}
```

✅ **Privalumai:**
- Trumpesnis kodas
- Lengviau skaityti
- Panašus į C++ klasės sintaksę

---

## C vs C++ palyginimas

| C (`typedef`) | C++ (`class`) |
|---------------|---------------|
| `typedef struct Stack Stack;` | `class Stack { ... };` |
| `Stack *s = create();` | `Stack s;` (auto construction) |
| `destroy(s);` | Auto destructor |

### 💡 C++ paskaitoje pasakykite:
> *"C++ `class` = C `typedef struct` + automatinis lifecycle!"*

---

# 🚀 ETAPAS 10B: CONST Correctness

## 📌 Koncepcija
**Const Correctness** - funkcijos, kurios **nemodifikuoja** objekto, žymimos `const`

## Problema
Funkcijos `isEmpty()` ir `isFull()` **nesako**, kad jos nemodifikuoja steko:
```c
int isEmpty(Stack *s);  // Gal modifikuoja? Nežinai!
int isFull(Stack *s);
```

## Sprendimas: `const`
```c
int isEmpty(const Stack *s);  // ✅ Garantuoja, kad NEMODIFIKUOS!
int isFull(const Stack *s);
```

---

## 10B Katalogo struktūra
```
10B_CONST_Correctness/
├── 01_No_Const/
│   ├── stack.h
│   ├── stack.c
│   └── user.c
├── 02_Const_Pointer/
│   ├── stack.h
│   ├── stack.c
│   └── user.c
├── 03_Const_Methods/
│   ├── stack.h
│   ├── stack.c
│   └── user.c
└── README.md
```

---

## 01_No_Const: Nėra garantijų

### Kodas
**stack.h:**
```c
typedef struct Stack Stack;

Stack* create(void);
void destroy(Stack *s);
void push(Stack *s, char c);      // Modifikuoja
char pop(Stack *s);                // Modifikuoja
int isEmpty(Stack *s);             // ❌ Neaišku!
int isFull(Stack *s);              // ❌ Neaišku!
```

❌ **Problema:** Nėra garantijos, kad `isEmpty`/`isFull` nemodifikuos

---

## 02_Const_Pointer: Garantijos

### Kodas
**stack.h:**
```c
typedef struct Stack Stack;

Stack* create(void);
void destroy(Stack *s);
void push(Stack *s, char c);           // Modifikuoja
char pop(Stack *s);                    // Modifikuoja
int isEmpty(const Stack *s);          // ✅ NEMODIFIKUOS!
int isFull(const Stack *s);           // ✅ NEMODIFIKUOS!
```

**stack.c:**
```c
int isEmpty(const Stack *s) {
    if (s == NULL) return 1;
    return 0 == s->top;
    
    // s->top = 0;  // ← KOMPILIAVIMO KLAIDA!
}
```

✅ **Privalumai:**
- Kompiliatorius **apsaugo** nuo atsitiktinio modifikavimo
- Aiškiau, kuri funkcija **keičia**, o kuri - **ne**

---

## 03_Const_Methods: Pilnas pavyzdys

### Kodas
**stack.h:**
```c
typedef struct Stack Stack;

// Modifikuojančios
Stack* create(void);
void destroy(Stack *s);
void init(Stack *s);
void push(Stack *s, char c);
char pop(Stack *s);

// Nemodifikuojančios (const)
int isEmpty(const Stack *s);
int isFull(const Stack *s);
int size(const Stack *s);      // ✅ Naujas metodas
void print(const Stack *s);    // ✅ Naujas metodas
```

**stack.c:**
```c
int size(const Stack *s) {
    return (s != NULL) ? s->top : 0;
}

void print(const Stack *s) {
    if (s == NULL) return;
    
    printf("Stack[%d]: ", s->top);
    for (int i = 0; i < s->top; i++) {
        putchar(s->stack[i]);
    }
    putchar('\n');
}
```

---

## C vs C++ palyginimas

| C (`const` pointer) | C++ (`const` method) |
|---------------------|----------------------|
| `int isEmpty(const Stack *s);` | `bool isEmpty() const;` |
| Manual const discipline | Compiler-enforced |

### 💡 C++ paskaitoje pasakykite:
> *"C++ `const` metodai = C `const` pointeriai + patogesnė sintaksė!"*

---

# 🚀 ETAPAS 10C: FUNCTION Pointers (Polymorphism)

## 📌 Koncepcija
**Function Pointers** - funkcijų rodyklės, leidžiančios "runtime dispatch"

## Problema
Norime turėti **skirtingas** steko versijas (pvz., su logging, be logging), bet vieną sąsają.

## Sprendimas: Function Pointers
```c
typedef void (*PushFunc)(Stack*, char);
typedef char (*PopFunc)(Stack*);

struct StackVTable {
    PushFunc push;
    PopFunc pop;
};
```

---

## 10C Katalogo struktūra
```
10C_FUNCTION_Pointers/
├── 01_Static_Dispatch/
│   ├── stack.h
│   ├── stack.c
│   └── user.c
├── 02_Function_Pointers/
│   ├── stack.h
│   ├── stack.c
│   ├── stack_logging.c
│   └── user.c
├── 03_Callback_Pattern/
│   ├── stack.h
│   ├── stack.c
│   └── user.c
└── README.md
```

---

## 01_Static_Dispatch: Paprastas kodas

### Kodas (įprastas)
```c
void push(Stack *s, char c) {
    if (!isFull(s)) {
        s->stack[s->top++] = c;
    }
}
```

❌ **Problema:** Vienas būdas - negalima keisti elgesio runtime

---

## 02_Function_Pointers: Dynamic Dispatch

### Kodas
**stack.h:**
```c
typedef struct Stack Stack;

typedef void (*PushFunc)(Stack*, char);
typedef char (*PopFunc)(Stack*);

struct StackVTable {
    PushFunc push;
    PopFunc pop;
};

struct Stack {
    char stack[SIZE];
    int top;
    struct StackVTable *vtable;  // ✅ Virtual table!
};

// Factory funkcijos
Stack* create_normal(void);
Stack* create_logging(void);
```

**stack.c:**
```c
// Normalus push
void push_normal(Stack *s, char c) {
    if (!isFull(s)) {
        s->stack[s->top++] = c;
    }
}

// Logging push
void push_logging(Stack *s, char c) {
    printf("[PUSH] '%c'\n", c);  // ✅ Logging!
    if (!isFull(s)) {
        s->stack[s->top++] = c;
    }
}

// Normalus pop
char pop_normal(Stack *s) {
    if (!isEmpty(s)) {
        return s->stack[--s->top];
    }
    return '\0';
}

// Logging pop
char pop_logging(Stack *s) {
    char c = '\0';
    if (!isEmpty(s)) {
        c = s->stack[--s->top];
        printf("[POP] '%c'\n", c);  // ✅ Logging!
    }
    return c;
}

// VTables
static struct StackVTable vtable_normal = {
    .push = push_normal,
    .pop = pop_normal
};

static struct StackVTable vtable_logging = {
    .push = push_logging,
    .pop = pop_logging
};

// Factory
Stack* create_normal(void) {
    Stack *s = malloc(sizeof(Stack));
    s->top = 0;
    s->vtable = &vtable_normal;  // ✅ Assign vtable
    return s;
}

Stack* create_logging(void) {
    Stack *s = malloc(sizeof(Stack));
    s->top = 0;
    s->vtable = &vtable_logging;  // ✅ Assign vtable
    return s;
}

// Generic push/pop (dispatch per vtable)
void push(Stack *s, char c) {
    s->vtable->push(s, c);  // ✅ Runtime dispatch!
}

char pop(Stack *s) {
    return s->vtable->pop(s);  // ✅ Runtime dispatch!
}
```

**user.c:**
```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    Stack *s1 = create_normal();
    Stack *s2 = create_logging();
    
    printf("Normal stack:\n");
    push(s1, 'A');
    push(s1, 'B');
    
    printf("\nLogging stack:\n");
    push(s2, 'X');  // ← [PUSH] 'X'
    push(s2, 'Y');  // ← [PUSH] 'Y'
    
    printf("\nPopping:\n");
    pop(s2);        // ← [POP] 'Y'
    
    destroy(s1);
    destroy(s2);
    return 0;
}
```

✅ **Privalumai:**
- **Runtime polymorphism** C kalboje!
- Galima keisti elgesį be kodo keitimo

---

## 03_Callback_Pattern: Event Handling

### Kodas
**stack.h:**
```c
typedef void (*OnPushCallback)(Stack*, char);
typedef void (*OnPopCallback)(Stack*, char);

struct Stack {
    char stack[SIZE];
    int top;
    OnPushCallback on_push;  // ✅ Callback!
    OnPopCallback on_pop;
};

void set_push_callback(Stack *s, OnPushCallback cb);
void set_pop_callback(Stack *s, OnPopCallback cb);
```

**user.c:**
```c
void my_push_handler(Stack *s, char c) {
    printf(">> Pushed: %c\n", c);
}

void my_pop_handler(Stack *s, char c) {
    printf("<< Popped: %c\n", c);
}

int main(void) {
    Stack *s = create();
    
    set_push_callback(s, my_push_handler);
    set_pop_callback(s, my_pop_handler);
    
    push(s, 'A');  // → >> Pushed: A
    push(s, 'B');  // → >> Pushed: B
    pop(s);        // → << Popped: B
    
    destroy(s);
    return 0;
}
```

✅ **Privalumai:**
- **Event-driven** programavimas
- Callback pattern

---

## C vs C++ palyginimas

| C (Function Pointers) | C++ (Virtual Functions) |
|-----------------------|-------------------------|
| Manual vtable | Compiler-generated vtable |
| `s->vtable->push(s, c)` | `s->push(c)` |
| `typedef void (*Func)(...)` | `virtual void func()` |

### 💡 C++ paskaitoje pasakykite:
> *"C++ `virtual` funkcijos = C function pointers + automatinis vtable!"*

---

# 🚀 ETAPAS 10D: GENERIC (void* Type Erasure)

## 📌 Koncepcija
**Generic Programming** - vienas kodas daugeliui tipų (`void*` type erasure)

## Problema
Turime `Stack` tik `char` tipui. Norime `int`, `float`, `struct`...

## Sprendimas: `void*`
```c
void push(Stack *s, void *data, size_t size);
void* pop(Stack *s);
```

---

## 10D Katalogo struktūra
```
10D_GENERIC_VoidPointer/
├── 01_Typed_Stack/
│   ├── stack_char.h
│   ├── stack_char.c
│   ├── stack_int.h
│   ├── stack_int.c
│   └── user.c
├── 02_Generic_Stack/
│   ├── stack.h
│   ├── stack.c
│   └── user.c
└── README.md
```

---

## 01_Typed_Stack: Dubliavimas

### Problema
Reikia rašyti **atskirą** steką kiekvienam tipui:

**stack_char.h:**
```c
typedef struct StackChar StackChar;

StackChar* create_char(void);
void push_char(StackChar *s, char c);
char pop_char(StackChar *s);
```

**stack_int.h:**
```c
typedef struct StackInt StackInt;

StackInt* create_int(void);
void push_int(StackInt *s, int val);
int pop_int(StackInt *s);
```

❌ **Problema:** Kodo dubliavimas!

---

## 02_Generic_Stack: void* sprendimas

### Kodas
**stack.h:**
```c
typedef struct Stack Stack;

Stack* create(size_t element_size, size_t capacity);
void destroy(Stack *s);

void push(Stack *s, const void *data);
void* pop(Stack *s);
int isEmpty(const Stack *s);
int isFull(const Stack *s);
```

**stack.c:**
```c
#include <stdlib.h>
#include <string.h>
#include "stack.h"

struct Stack {
    void *data;           // ✅ Generic buffer!
    size_t element_size;
    size_t capacity;
    size_t top;
};

Stack* create(size_t element_size, size_t capacity) {
    Stack *s = malloc(sizeof(Stack));
    s->data = malloc(element_size * capacity);
    s->element_size = element_size;
    s->capacity = capacity;
    s->top = 0;
    return s;
}

void destroy(Stack *s) {
    free(s->data);
    free(s);
}

void push(Stack *s, const void *data) {
    if (isFull(s)) return;
    
    void *dest = (char*)s->data + (s->top * s->element_size);
    memcpy(dest, data, s->element_size);  // ✅ Kopiuojame
    s->top++;
}

void* pop(Stack *s) {
    if (isEmpty(s)) return NULL;
    
    s->top--;
    return (char*)s->data + (s->top * s->element_size);
}

int isEmpty(const Stack *s) {
    return s->top == 0;
}

int isFull(const Stack *s) {
    return s->top == s->capacity;
}
```

**user.c:**
```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    // ✅ char stekas
    Stack *s_char = create(sizeof(char), 10);
    char c1 = 'A', c2 = 'B';
    push(s_char, &c1);
    push(s_char, &c2);
    
    char *pc = (char*)pop(s_char);
    printf("Popped char: %c\n", *pc);  // → B
    
    // ✅ int stekas
    Stack *s_int = create(sizeof(int), 10);
    int i1 = 42, i2 = 99;
    push(s_int, &i1);
    push(s_int, &i2);
    
    int *pi = (int*)pop(s_int);
    printf("Popped int: %d\n", *pi);  // → 99
    
    // ✅ struct stekas
    typedef struct { int x; int y; } Point;
    Stack *s_point = create(sizeof(Point), 10);
    
    Point p1 = {10, 20}, p2 = {30, 40};
    push(s_point, &p1);
    push(s_point, &p2);
    
    Point *pp = (Point*)pop(s_point);
    printf("Popped Point: (%d, %d)\n", pp->x, pp->y);  // → (30, 40)
    
    destroy(s_char);
    destroy(s_int);
    destroy(s_point);
    
    return 0;
}
```

✅ **Privalumai:**
- **Vienas kodas** daugeliui tipų!
- Generic programming C kalboje

❌ **Trūkumai:**
- **Nėra type safety** - kompiliatorius neleidžia užkirsti kelio klaidoms
- **Manual casting** - reikia `(int*)`, `(char*)` cast'ų
- **Runtime errors** - klaidos tik vykdymo metu

---

## C vs C++ palyginimas

| C (`void*`) | C++ (Templates) |
|-------------|-----------------|
| `void push(Stack *s, void *data)` | `template<T> void push(T data)` |
| Manual casting: `(int*)pop(s)` | Auto type: `T value = pop()` |
| Runtime errors | **Compile-time errors** |
| No type safety | **Type safety** |

### 💡 C++ paskaitoje pasakykite:
> *"C++ templates = C `void*` + type safety + zero runtime overhead!"*

---

# 🎓 APIBENDRINIMAS: 4 Advanced Techniques

## Lentelė

| Etapas | Technika | C problema | C sprendimas | C++ pagerinimas |
|--------|----------|------------|--------------|-----------------|
| **10A** | typedef | `struct Stack` ilgas | `typedef struct Stack Stack;` | `class Stack` (auto) |
| **10B** | const | Nėra garantijų | `const Stack *s` | `const` methods |
| **10C** | Function Pointers | Static dispatch | `vtable` + function pointers | `virtual` functions |
| **10D** | void* Generic | Kodo dubliavimas | `void*` type erasure | **Templates** |

---

## 🎯 Mokymo strategija

### Kada grįžti prie 10A-10D?

1. **10A (typedef)** → C++ paskaita: *"Type Aliases & using"*
2. **10B (const)** → C++ paskaita: *"Const Correctness & const Methods"*
3. **10C (function pointers)** → C++ paskaita: *"Polymorphism & Virtual Functions"*
4. **10D (void*)** → C++ paskaita: *"Templates & Generic Programming"*

### Kaip pristatyti?

> *"Prieš išmokstant [C++ feature], pažiūrėkime, kaip C sprendė šią problemą...  
> [Parodote 10X etapą]  
> Dabar pažiūrėkime, kaip C++ tai supaprastina!"*

---

## 📝 Studentų užduotys (10A-10D)

### Užduotis 1: typedef Refactoring
- Paimkite 09 etapo kodą
- Pritaikykite `typedef` visur
- Palyginkite kodo skaitomumą

### Užduotis 2: const Audit
- Pažymėkite visas funkcijas `const`, kurios nemodifikuoja
- Pabandykite modifikuoti `const` objekte → kompiliavimo klaida
- Refleksija: *"Kodėl const svarbu?"*

### Užduotis 3: Polymorphism
- Sukurkite 3 steko variantus:
  - Normal
  - Logging
  - Bounded (su max size check)
- Naudokite function pointers vtable
- Runtime pasirinkimas

### Užduotis 4: Generic Queue
- Sukurkite generic Queue su `void*`
- Testuokite su `int`, `double`, `struct`
- Palyginkite su C++ `template<T>` Queue

---

## 🎬 Galutinė mintis

> **"C jau viską turėjo... Tik nebuvo taip patogu!"** 😄

**C++ = C technikas + automatizavimas + safety + elegancija**

---

**Versija:** 2026.01 (Advanced Techniques)  
**Licenzija:** Laisvai naudojamas mokymo tikslams  
**Grįžti į:** [Main Guide (01-09)](Stack_Evolution_Guide_9_Stages_FINAL.md)
