# Etapas 03: Discovering C MODULE

> **Koncepcija:** C modulis = `.h` (header) + `.c` (implementation)  
> **Problema:** Duomenys vis dar vieši - galimos atakos!  
> **Sprendimas:** Teisingas header/implementation skaidymas

---

## 🎯 Tikslas

Išmokti **teisingai** atskirti:
- **Deklaracijas** (`.h` faile)
- **Apibrėžimus** (`.c` faile)

**Bet:** Duomenys (`stack[]`, `top`) vis dar **globalūs** → nesaugūs!

---

## 📂 Subdirektorijos

| Subdirektorija | Rezultatas | Aprašymas |
|----------------|-----------|-----------|
| [04_OK/](04_OK/) | ✅ Veikia | Tinkamas C modulis (header + implementation) |
| [05_OK_Attack/](05_OK_Attack/) | ⚠️ Ataka pavyksta! | Demonstruoja, kad moduliavimas ≠ saugumas |

---

## 🔧 Kompiliavimo Instrukcijos

### Tinkamas variantas (04_OK):
```bash
cd 04_OK
gcc stack.c user.c -o app
./app
```

**Rezultatas:** ✅ Veikia teisingai

---

### Atakos scenarijus (05_OK_Attack):
```bash
cd 05_OK_Attack
gcc stack.c user.c -o app
./app
```

**Rezultatas:** ✅ Kompiliuojasi, bet ataka **pavyksta**!

---

## 📖 Pagrindinės Pamokos

### ✅ Kas Pagerėjo (lyginant su 02 etapu)?
1. **Header failas** - aiški sąsaja
2. **Teisingas** `.h` ir `.c` skirstymas
3. **`extern`** deklaracijos header'yje
4. **Apibrėžimai** implementation faile
5. **Nėra** multiple definition klaidų!

### ❌ Kas Vis Dar Problematiška?
1. **Globalūs kintamieji** - `stack[]` ir `top` vieši
2. **Galima ataka** - su `extern` galima pasiekti
3. **Nėra information hiding**!

---

## 🔍 Kodų Palyginimas

### 04_OK: Tinkamas header
```c
// stack.h
#define SIZE 5

extern char stack[SIZE];  // ← Deklaracija
extern int top;

void init(void);
int isEmpty(void);
int isFull(void);
void push(char c);
char pop(void);
```

```c
// stack.c
#include "stack.h"

char stack[SIZE];  // ← Apibrėžimas
int top = 0;

void init(void) {
    top = 0;
}
// ... kitos funkcijos
```

---

### 05_OK_Attack: Atakos kodas
```c
// user.c
#include <stdio.h>
#include "stack.h"

extern char stack[];  // 🔥 Tiesioginis prieiga!
extern int top;       // 🔥

int main(void) {
    init();
    push('A');
    
    stack[top++] = '!';  // 🔥 ATAKA PAVYKO!
    
    while (!isEmpty()) {
        putchar(pop());
    }
    return 0;
}
```

**Rezultatas:** Išveda `!A` vietoj `A`!

---

## 💡 Refleksijos Klausimai

### Studentams:
1. **Kodėl 02_NL klaida** (iš 02 etapo) **čia nebeatsiranda**?
2. **Kuo skiriasi** `extern char stack[];` header'yje **nuo** `char stack[SIZE];` .c faile?
3. **Kodėl ataka** (05_OK_Attack) **pavyksta**?
4. **Kaip galėtume** **apsaugoti** duomenis?

### Atsakymai:
1. Nes dabar **teisingai** atskirtos deklaracijos (`.h`) ir apibrėžimai (`.c`)
2. `extern` = **deklaracija** (sako, kad egzistuoja), apibrėžimas = **sukuria** atmintį
3. Nes `stack[]` ir `top` **globalūs** → bet kas gali pasiekti su `extern`
4. **04 etapas** parodys - su `static`!

---

## 🎓 Mokymosi Rezultatai

Po šio etapo suprasi:
- ✅ **Header** vs **Implementation** failai
- ✅ **`extern`** deklaracijos prasmė
- ✅ **Modulinio** dizaino principai
- ✅ **Kodėl moduliavimas ≠ saugumas**

---

## ➡️ Kitas Žingsnis

**Etapas 04:** [Protecting_IMPLEMENTATION](../04_Protecting_IMPLEMENTATION/)
- Išmoksi `static` internal linkage
- Atakos **nepavyks**!
- Bet bus **kita** problema... 🤔

---

## 🔗 Nuorodos

- [← Atgal į 02 Etapą](../02_Decomposing_to_UNITS/)
- [↑ Atgal į Root](../)
- [Pilnas Gidas](../../../docs/evolution/Stack_Evolution_Guide_9_Stages_FINAL.md)

---

**Kompiliuok, eksperimentuok, mokykis iš klaidų!** 🚀
