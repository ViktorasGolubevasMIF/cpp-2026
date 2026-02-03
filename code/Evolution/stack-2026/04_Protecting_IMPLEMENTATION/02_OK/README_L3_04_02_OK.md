# 02_OK - Teisingas `static` Naudojimas

> **Statusas:** ✅ Veikia teisingai  
> **Koncepcija:** Information hiding su `static` internal linkage

---

## 📋 Failai

```
02_OK/
├── stack.h      ← Tik funkcijų prototipai (be extern stack/top!)
├── stack.c      ← static kintamieji + implementacija
└── user.c       ← Teisingas naudojimas
```

---

## 🔧 Kompiliavimas

```bash
gcc stack.c user.c -o app
./app
```

**Rezultatas:** ✅ Veikia!

---

## 🔒 Header (stack.h)

```c
#define SIZE 5

// ✅ TIKTAI funkcijų prototipai!
void init(void);
int isEmpty(void);
int isFull(void);
void push(char c);
char pop(void);

// ❌ NĖRA: extern char stack[]; 
// ❌ NĖRA: extern int top;
```

**Pastaba:** Nėra `extern` - duomenys paslėpti!

---

## 🔒 Implementation (stack.c)

```c
#include "stack.h"

// ✅ static = internal linkage!
static char stack[SIZE];
static int top = 0;

// ✅ Private funkcija
static void reset(void) {
    top = 0;
}

void init(void) {
    top = 0;
}

int isEmpty(void) {
    return 0 == top;
}

// ... kitos funkcijos
```

**Apsauga:**
- `stack[]` - **nematoma** už `stack.c` ribų
- `top` - **nematomas** už `stack.c` ribų
- `reset()` - **nemataoma** už `stack.c` ribų

---

## ✅ Teisingas Naudojimas (user.c)

```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;
    init();
    
    while ((!isFull()) && ('\n' != (c = getchar()))) {
        push(c);
    }
    
    while (!isEmpty()) {
        putchar(pop());
    }
    
    return 0;
}
```

**Gražu:**
- Naudoja **tik** funkcijas iš header'io
- **Nežino** apie `stack[]` ar `top`
- **Abstraction** veikia!

---

## 🎯 Testuok

### Įvestis:
```
12345
```

### Tikėtina išvestis:
```
54321
```

---

## ✅ Privalumai

1. **Information hiding** - duomenys paslėpti
2. **Saugumas** - `extern` atakos neveikia
3. **Abstrakcija** - naudotojas nežino implementacijos
4. **Clean interface** - header aiškus

---

## ❌ Trūkumai

**Tik vienas stekas!**

```c
init();      // Inicializuoja VIENĄ steką
push('A');
init();      // Reset'ina TĄ PATĮ steką!
push('B');   // Prarandame 'A'!
```

**Problema:** `static` kintamieji = **globalūs** (bet paslėpti)

---

## 💡 Pamoka

### `static` Veikia, Bet...

**Pasiekta:**
- ✅ Information hiding

**Nepassiekta:**
- ❌ Kelių egzempliorių galimybė

---

## ➡️ Sprendimas

**Etapas 05:** [Defining_USER_TYPE](../../05_Defining_USER_TYPE/)
- `struct Stack` tipas
- Keli stekai: `st1`, `st2`, `st3`
- Bet susidurkime su NC/RT klaidomis! 🐛

---

## 🔗 Nuorodos

- [← Atakos bandymas](../01_NL_Attack/)
- [↑ Atgal į Etapą](../)
- [→ Kitas Etapas (05)](../../05_Defining_USER_TYPE/)

---

**`static` = hiding veikia, bet vienas egzempliorius!** 🔒1️⃣
