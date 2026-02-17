# Etapas 08: Acquiring_RESOURCES

> **Koncepcija:** Factory Pattern - `create()` / `destroy()`  
> **Problema:** Kaip sukurti objektą su opaque pointer?  
> **Sprendimas:** Dynamic allocation + manual lifecycle management

---

## 🎯 Tikslas

Sukurti **factory funkcijas** resursų valdymui:
- `create()` - sukuria objektą (`malloc`)
- `destroy()` - sunaikina objektą (`free`)
- **Manual** lifecycle management

**Rezultatas:** Pilnai veikiantis ADT su information hiding!

---

## 📂 Subdirektorijos

| Subdirektorija | Rezultatas | Aprašymas |
|----------------|-----------|-----------|
| [02_OK/](02_OK/) | ✅ Veikia | Factory pattern su `create()`/`destroy()` |

---

## 🔧 Kompiliavimas

```bash
cd 02_OK
gcc stack.c user.c -o app
./app
```

**Rezultatas:** ✅ Veikia!

---

## 📖 Pagrindinės Pamokos

### ✅ Kas Pagerėjo (lyginant su 07 etapu)?

**07 etapas (problema):**
```c
struct Stack;  // ← Forward declaration
struct Stack st;  // ❌ NC: incomplete type - kaip sukurti?
```

**08 etapas (sprendimas):**
```c
struct Stack;  // ← Forward declaration

// Factory funkcijos
struct Stack* create(void);  // ← Sukuria objektą
void destroy(struct Stack*); // ← Sunaikina objektą

// Naudojimas
struct Stack *pst = create();  // ✅ Veikia!
destroy(pst);                  // ✅ Atlaisvina
```

---

## 🏭 Factory Pattern

### Header (stack.h):
```c
#define SIZE 5

struct Stack;  // ← Opaque pointer

// Factory funkcijos
struct Stack* create(void);
void destroy(struct Stack *s);

// Operacijos
void init(struct Stack *s);
void push(struct Stack *s, char c);
char pop(struct Stack *s);
int isEmpty(struct Stack *s);
int isFull(struct Stack *s);
```

### Implementation (stack.c):
```c
#include <stdlib.h>
#include "stack.h"

struct Stack {  // ← Pilnas apibrėžimas TIK čia!
    char stack[SIZE];
    int top;
};

struct Stack* create(void) {
    struct Stack *s = malloc(sizeof(struct Stack));
    if (s != NULL) {
        s->top = 0;
    }
    return s;
}

void destroy(struct Stack *s) {
    free(s);
}

// ... kitos funkcijos
```

### Naudojimas (user.c):
```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    // Sukuriame du stekas
    struct Stack *pst1 = create();
    struct Stack *pst2 = create();
    
    init(pst1);
    push(pst1, 'A');
    
    init(pst2);
    push(pst2, 'B');
    
    // ... naudojame
    
    // Atlaisviname atmintį
    destroy(pst1);
    destroy(pst2);
    
    return 0;
}
```

---

## 💡 Factory Pattern Privalumai

1. **Paslėpta implementacija** - naudotojas nežino `struct` struktūros
2. **Kontroliuojamas lifecycle** - `create()` ir `destroy()`
3. **Dynamic allocation** - kiek reikia egzempliorių
4. **Encapsulation** - visos operacijos per funkcijas

---

## 🔄 Lifecycle Management

### Manual vs Automatic:

**C (08 etapas):**
```c
Stack *s = create();   // ← Manual allocation
// ... naudojimas
destroy(s);            // ← Manual deallocation
```

**C++ (RAII):**
```cpp
{
    Stack s;           // ← Auto construction
    // ... naudojimas
}                      // ← Auto destruction
```

**Pamoka:** C++ automatizuoja tai, ką C daro rankiniu būdu!

---

## 💡 C vs C++ Palyginimas

| C (Factory) | C++ (Constructor/Destructor) |
|-------------|------------------------------|
| `Stack* create()` | `Stack()` constructor |
| `void destroy(Stack*)` | `~Stack()` destructor |
| Manual `malloc`/`free` | Automatic memory management |
| `Stack *s = create();` | `Stack s;` (auto) |
| `destroy(s);` | Auto when out of scope |

---

## ⚠️ Kas Dar Trūksta?

### Error Handling:
```c
Stack *s = create();
if (s == NULL) {  // ← Reikia tikrinti!
    // Handle error
}
```

### Validation:
```c
void push(Stack *s, char c) {
    if (s == NULL) return;  // ← Defensive programming
    // ...
}
```

**Sprendimas:** 09 etapas - Lifecycle completion!

---

## 💡 Refleksijos Klausimai

### Studentams:
1. **Kodėl** reikia `create()` funkcijos su opaque pointer?
2. **Kas** nutiktų, jei pamirštume `destroy()`?
3. **Kuo** skiriasi C factory nuo C++ konstruktoriaus?

### Atsakymai:
1. Nes `struct Stack st;` neveikia (incomplete type) → reikia dynamic allocation
2. **Memory leak** - atmintis neatlaisvinta
3. C - **manual** (reikia kviesti), C++ - **automatic** (auto)

---

## 🎓 Mokymosi Rezultatas

Po šio etapo suprasi:
- ✅ **Factory pattern** principus
- ✅ **Dynamic allocation** su opaque pointer
- ✅ **Manual lifecycle** management
- ✅ C → C++ tiltas (RAII)

---

## ➡️ Kitas Žingsnis

**Etapas 09:** [Completing_LIFECYCLE](../09_Completing_LIFECYCLE/)
- Pilnas lifecycle completion
- Error handling
- Defensive programming
- Finalas C evoliucijoje!

---

## 🔗 Nuorodos

- [← Atgal į 07 Etapą](../07_HIDING_UDT_Information/)
- [↑ Atgal į Root](../)
- [→ Kitas Etapas](../09_Completing_LIFECYCLE/)

---

**Factory = C kalbos konstruktoriai!** 🏭
