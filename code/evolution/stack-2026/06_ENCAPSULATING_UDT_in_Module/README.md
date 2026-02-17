# Etapas 06: ENCAPSULATING_UDT_in_Module

> **Koncepcija:** Modulinis UDT - struktūra + funkcijos modulyje  
> **Problema:** Struktūra vieša header'yje → atakos pavyksta!  
> **Sprendimas:** Encapsulation ≠ Information Hiding (dar ne tikras ADT!)

---

## 🎯 Tikslas

Perkelti UDT (`struct Stack`) į modulį:
- `struct Stack` apibrėžimas **header'yje**
- Funkcijos **implementation** faile
- Keli stekai veikia!

**Bet:** Struktūros laukai **vieši** → galima tiesiogiai keisti!

---

## 📂 Subdirektorijos

| Subdirektorija | Rezultatas | Aprašymas |
|----------------|-----------|-----------|
| [01_OK/](01_OK/) | ✅ Veikia | Modulinis UDT - teisingai naudojamas |
| [02_OK_Attack/](02_OK_Attack/) | ⚠️ Ataka pavyksta! | Tiesioginis prieiga prie `st.stack[]`, `st.top` |

---

## 🔧 Kompiliavimo Instrukcijos

### Tinkamas variantas (01_OK):
```bash
cd 01_OK
gcc stack.c user.c -o app
./app
```

**Rezultatas:** ✅ Veikia!

---

### Atakos scenarijus (02_OK_Attack):
```bash
cd 02_OK_Attack
gcc stack.c user_attack.c -o app
./app
```

**Rezultatas:** ✅ Kompiliuojasi, ataka **pavyksta**!
```
Input 1st Stack:
1234
4321

Input 2nd Stack:
4567
!7654  ← '!' įterptas tiesiogiai!
```

---

## 📖 Pagrindinės Pamokos

### ✅ Kas Pagerėjo (lyginant su 05 etapu)?

1. **Modulinis dizainas** - UDT header'yje
2. **Aiški struktūra** - `struct Stack` apibrėžimas matomas
3. **Kelių egzempliorių** palaikymas
4. **Gražus kodas** - organizuotas

---

### ❌ Kas Vis Dar Problematiška?

**Struktūra header'yje = vieša!**

```c
// stack.h
struct Stack {
    char stack[SIZE];  // ← VIEŠA!
    int top;           // ← VIEŠA!
};
```

**Vartotojas gali:**
```c
struct Stack st;
st.top = 999;           // ← Tiesioginis modifikavimas!
st.stack[0] = '!';      // ← Apeinama validacija!
```

---

### 🔥 Atakos Kodas (02_OK_Attack)

```c
#include <stdio.h>
#include "stack.h"

int main(void) {
    struct Stack st1, st2;
    
    // Normalus naudojimas st1
    init(&st1);
    push(&st1, 'A');
    push(&st1, 'B');
    
    // Normalus naudojimas st2
    init(&st2);
    push(&st2, '1');
    push(&st2, '2');
    
    // 🔥 ATAKA: tiesioginis laukų modifikavimas!
    st2.stack[st2.top++] = '!';
    
    // Išvedame
    while (!isEmpty(&st2)) {
        putchar(pop(&st2));
    }
}
```

**Rezultatas:** `!21` - ataka pavyko!

---

## 📖 Encapsulation ≠ Information Hiding

### Encapsulation (Inkapsuliacija):
> Duomenys + funkcijos logiškai **sugrupuoti** kartu

**Pavyzdys:**
```c
struct Stack {      // ← Duomenys
    char stack[];
    int top;
};
void push(...);     // ← Funkcijos
char pop(...);
```

**Turime?** ✅ Taip!

---

### Information Hiding (Informacijos slėpimas):
> Duomenys **paslėpti** - negalima tiesiogiai pasiekti

**Pavyzdys:**
```c
struct Stack;  // ← Forward declaration, pilnas apibrėžimas paslėptas
```

**Turime?** ❌ Ne!

---

## 💡 Kodėl Tai Svarbu?

### Tiesioginis prieiga = problemos:

1. **Pažeidžiama abstrakcija** - naudotojas mato vidinius laukus
2. **Apeinama validacija** - nėra `isFull()` patikrinimo
3. **Galimi bagai** - `top` gali tapti neteisingu
4. **Sunku keisti** - jei pakeisite struktūrą → suges naudotojo kodas

---

## 💡 Refleksijos Klausimai

### Studentams:
1. **Kuo** skiriasi **encapsulation** nuo **information hiding**?
2. **Kodėl** struktūros header'yje = **nesaugu**?
3. **Kaip** galėtume **paslėpti** struktūros laukus?

### Atsakymai:
1. **Encapsulation** = grupavimas, **hiding** = apsauga nuo prieigos
2. Nes bet kas gali `st.top = 999;` → apeidama abstrakcija
3. **07 etapas** - opaque pointer (forward declaration)!

---

## 🎓 Mokymosi Rezultatas

Po šio etapo suprasi:
- ✅ **Modulinį** UDT dizainą
- ⚠️ **Encapsulation** ≠ **Information Hiding**
- ❌ Struktūra header'yje = **vieša**
- ✅ Kodėl reikia **tikro** slėpimo

---

## ➡️ Kitas Žingsnis

**Etapas 07:** [HIDING_UDT_Information](../07_HIDING_UDT_Information/)
- **Opaque pointer** - forward declaration
- Struktūros laukai **paslėpti**!
- Atakos **nepavyks** (NC klaida)! 🔒

---

## 🔗 Nuorodos

- [← Atgal į 05 Etapą](../05_Defining_USER_TYPE/)
- [↑ Atgal į Root](../)
- [→ Kitas Etapas](../07_HIDING_UDT_Information/)

---

**Encapsulation ≠ Information Hiding!** 📦🔒
