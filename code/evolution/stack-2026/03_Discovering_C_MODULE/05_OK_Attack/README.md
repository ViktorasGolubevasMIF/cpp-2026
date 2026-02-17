# 05_OK_Attack - Ataka Pavyksta!

> **Statusas:** ⚠️ Kompiliuojasi, bet NESAUGUS  
> **Tikslas:** Demonstruoti, kad **moduliavimas ≠ information hiding**

---

## 🔥 Atakos Scenarijus

**Problema:** Globalūs `stack[]` ir `top` prieinami su `extern`!

---

## 📋 Failai

```
05_OK_Attack/
├── stack.h         ← Tas pats kaip 04_OK
├── stack.c         ← Tas pats kaip 04_OK
└── user.c          ← 🔥 Su atakos kodu!
```

---

## 🔧 Kompiliavimas

```bash
gcc stack.c user.c -o app
```

**Rezultatas:** ✅ Kompiliuojasi be klaidų!

---

## 💣 Atakos Kodas (user.c)

```c
#include <stdio.h>
#include "stack.h"

extern char stack[];  // 🔥 Tiesioginis prieiga!
extern int top;       // 🔥

int main(void) {
    init();
    
    // Normaliai naudojame
    push('A');
    push('B');
    push('C');
    
    // 🔥 ATAKA: tiesiogiai modifikuojame duomenis!
    stack[top++] = '!';
    
    // Išvedame
    while (!isEmpty()) {
        putchar(pop());
    }
    putchar('\n');
    
    return 0;
}
```

---

## 🎯 Testuok

### Kompiliuok ir vykdyk:
```bash
gcc stack.c user.c -o app
./app
```

### Rezultatas:
```
!CBA
```

**Pastaba:** '!' buvo įterptas **tiesiogiai**, **apeinant** `push()` funkciją!

---

## 💡 Kas Nutiko?

1. **Normaliai:** `push('A')`, `push('B')`, `push('C')`
   - Stekas: `['A', 'B', 'C']`
   - `top = 3`

2. **Ataka:** `stack[top++] = '!';`
   - Stekas: `['A', 'B', 'C', '!']`
   - `top = 4`
   - **Apeita** `push()` funkcija!
   - **Nėra** `isFull()` patikrinimo!

3. **Pop:** `!`, `C`, `B`, `A`

---

## 📖 Pagrindinė Pamoka

### Moduliavimas ≠ Information Hiding

**Modulis** (header + implementation) = **organizavimo** mechanizmas  
**Information hiding** = **apsaugos** mechanizmas

- ✅ Turime modulį
- ❌ **Neturime** information hiding
- ⚠️ Duomenys **vieši** → galima modifikuoti

---

## 🛡️ Kodėl Tai Bloga?

1. **Pažeidžiama abstrakcija** - tiesioginis duomenų keitimas
2. **Apeinama validacija** - nėra `isFull()` patikrinimo
3. **Galimi bagai** - `top` gali būti neteisingas
4. **Nenumatomas elgesys** - kiti kodai gali sugadinti būseną

---

## ❓ Refleksijos Klausimai

### Studentams:
1. **Kodėl** kompiliatorius **neleidžia** šios atakos?
2. **Kaip** galėtume **apsaugoti** `stack[]` ir `top`?
3. **Ar** galima turėti modulį **be** information hiding?

### Atsakymai:
1. Nes `extern` **legalus** C sintaksėje - kompiliatorius nemato problemos
2. **04 etapas** - su `static` internal linkage!
3. **Taip** - kaip šis pavyzdys. **Modulis** ≠ **saugumas**!

---

## 🎓 Mokymosi Rezultatas

Po šio scenarijaus suprasi:
- ⚠️ **Global variables** = pavojus
- ⚠️ **`extern`** = bet kas gali pasiekti
- ✅ **Modulis** ≠ **information hiding**
- ✅ Reikia **papildomų** mechanizmų apsaugai

---

## ➡️ Sprendimas

**Etapas 04:** [Protecting_IMPLEMENTATION](../../04_Protecting_IMPLEMENTATION/)
- `static` internal linkage
- Ataka **nepavyks**!
- `extern` grąžins **linker error**!

---

## 🔗 Nuorodos

- [← Tinkamas variantas](../04_OK/)
- [↑ Atgal į Etapą](../)
- [→ Sprendimas (04 etapas)](../../04_Protecting_IMPLEMENTATION/)

---

**Atakos scenarijai = geriausias būdas suprasti saugumo principus!** 🛡️
