# 01_NC - Incomplete Type Klaida

> **Statusas:** ❌ Not Compiling - kompiliavimo klaida  
> **Tikslas:** Parodyti, kad opaque pointer = tikras information hiding!  
> **Pamoka:** Forward declaration → incomplete type → NC klaida

---

## 🔒 Opaque Pointer Koncepcija

**Header (stack.h):**
```c
struct Stack;  // ← Forward declaration (incomplete type!)

void init(struct Stack *s);
void push(struct Stack *s, char c);
// ... kitos funkcijos
```

**Implementation (stack.c):**
```c
struct Stack {  // ← Pilnas apibrėžimas TIK čia!
    char stack[SIZE];
    int top;
};
```

---

## 📋 Failai

```
01_NC/
├── stack.h         ← Forward declaration
├── stack.c         ← Pilnas struct apibrėžimas
├── _user.c         ← Bandymas deklaruoti: struct Stack st;
└── user_attack.c   ← Bandymas atakuoti
```

---

## 🔧 Bandymas #1: Normalus Naudojimas (_user.c)

```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    // ❌ Bandome deklaruoti objektą
    struct Stack st1, st2;
    
    init(&st1);
    push(&st1, 'A');
    
    return 0;
}
```

### Kompiliavimas:
```bash
gcc stack.c _user.c -o app
```

### ❌ Kompiliavimo Klaida:
```
error: storage size of 'st1' isn't known
error: storage size of 'st2' isn't known
error: variable has incomplete type 'struct Stack'
```

---

## 💣 Bandymas #2: Ataka (user_attack.c)

```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    // ❌ Bandome deklaruoti
    struct Stack st;
    
    init(&st);
    push(&st, 'A');
    
    // 🔥 Bandome atakuoti
    st.stack[st.top++] = '!';  // ❌ NC klaida!
    
    return 0;
}
```

### Kompiliavimas:
```bash
gcc stack.c user_attack.c -o app
```

### ❌ Kompiliavimo Klaida:
```
error: storage size of 'st' isn't known
error: variable has incomplete type 'struct Stack'
```

**Ataka NEPAVYKO!** 🎉

---

## 🔍 Kodėl NC Klaida?

### Forward Declaration = Incomplete Type

```c
struct Stack;  // ← Kompiliatorius žino: "tipas egzistuoja"
               //   Bet NEŽINO: koks dydis!
```

### Negalima:
```c
struct Stack st;  // ❌ Kompiliatorius: "Kiek atmintės skirti? 🤷"
```

### Galima:
```c
struct Stack *pst;  // ✅ Rodyklė: visada 8 bytes (64-bit)
```

---

## 📖 Pagrindinė Pamoka

### Incomplete Type Apsauga

**Kompiliatorius:**
> "Negaliu sukurti objekto, nes nežinau jo dydžio!"

**Rezultatas:**
- ✅ **Compile-time** apsauga
- ✅ Atakos **neįmanomos** (ne tik nepavyksta!)
- ✅ Tikras **information hiding**

---

## 💡 Opaque Pointer vs struct Header'yje

| 06 etapas (struct header'yje) | 07 etapas (opaque pointer) |
|-------------------------------|---------------------------|
| `struct Stack { ... };` matomas | `struct Stack;` tik deklaracija |
| `struct Stack st;` ✅ veikia | `struct Stack st;` ❌ NC klaida |
| `st.stack[st.top++] = '!';` ✅ ataka pavyksta | `st.stack[...];` ❌ NC klaida |
| **Encapsulation** ✅ | **Information Hiding** ✅ |

---

## ⚠️ Problema: Kaip Sukurti Objektą?

### Negalima:
```c
struct Stack st;  // ❌ Incomplete type!
```

### Sprendimas?
**Factory pattern** → **08 etapas**!

```c
struct Stack* create(void);  // ← Funkcija žino dydį!
```

---

## 🎯 Testuok

Pabandyk kompiliuoti ir pamatysi klaidas:

```bash
cd 01_NC
gcc stack.c _user.c -o app
# ❌ error: storage size of 'st1' isn't known
```

---

## 💡 Refleksija

**Klausimas:** Kodėl `struct Stack *pst;` veikia, bet `struct Stack st;` neveikia?

**Atsakymas:** 
- **Rodyklė** = fiksuotas dydis (8 bytes) - kompiliatorius žino!
- **Objektas** = nežinomas dydis (incomplete type) - kompiliatorius **nežino**!

---

## ➡️ Sprendimas

**Etapas 08:** [Acquiring_RESOURCES](../../08_Acquiring_RESOURCES/)
- Factory funkcijos: `create()` / `destroy()`
- Dynamic allocation: `malloc` / `free`
- Pilnas lifecycle management!

---

## 🔗 Nuorodos

- [↑ Atgal į Etapą](../)
- [→ Sprendimas (08 etapas)](../../08_Acquiring_RESOURCES/)

---

**Incomplete type = compile-time apsauga!** 🔒✨
