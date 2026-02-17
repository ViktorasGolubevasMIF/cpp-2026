# Etapas 02: Decomposing_to_UNITS

> **Koncepcija:** Fizinis padalijimas į failus  
> **Problema:** NL klaidos, antipattern `#include "file.c"`  
> **Sprendimas:** Išmokti **header** koncepcijos būtinybę

---

## 🎯 Tikslas

Bandyti **atskirti** steko logiką nuo naudotojo kodo:
- Steko funkcijos → `stack.c`
- Naudotojas → `user.c`

**Bet:** Susiduriame su **klaidomis**, kurios moko!

---

## 📂 Subdirektorijos

| Subdirektorija | Rezultatas | Aprašymas |
|----------------|-----------|-----------|
| [02_NL/](02_NL/) | ❌ NL klaida | `#include "stack.c"` → multiple definition |
| [03_OK_Wrong/](03_OK_Wrong/) | ⚠️ Antipattern | Veikia, bet **blogai** |

---

## 🔧 Kompiliavimo Instrukcijos

### Scenarijus 1: NL klaida (02_NL):
```bash
cd 02_NL
gcc stack.c user.c -o app
```

**Rezultatas:** ❌ Linkavimo klaida!
```
multiple definition of `init'
multiple definition of `isEmpty'
multiple definition of `stack'
multiple definition of `top'
...
```

---

### Scenarijus 2: Antipattern (03_OK_Wrong):
```bash
cd 03_OK_Wrong
gcc user.c -o app  # ← Tik user.c!
./app
```

**Rezultatas:** ✅ Veikia, bet **blogai**!

---

## 📖 Pagrindinės Pamokos

### ❌ Kas Blogai (02_NL)?

**Kodas:**
```c
// user.c
#include "stack.c"  // ⚠️ KLAIDA!
```

**Kompiliavimas:**
```bash
gcc stack.c user.c -o app
```

**Kas nutiko?**
1. `user.c` **nukopijuoja** visą `stack.c` turinį
2. Kompiliatorius mato funkcijas **du kartus**:
   - Iš `stack.c` failo
   - Iš `user.c` failo (per `#include`)
3. **Linker** mato **dvigubas** definicijas → **klaida**!

---

### ⚠️ Kas Blogai (03_OK_Wrong)?

**Kodas:**
```c
// user.c
#include "stack.c"  // ⚠️ ANTIPATTERN!

int main(void) {
    // Naudojame steką
}
```

**Kompiliavimas:**
```bash
gcc user.c -o app  # ← Kompiliuojame TIK user.c!
```

**Kodėl veikia?**
- `#include "stack.c"` **nukopijuoja** visą turinį
- Kompiliatorius mato **viską** `user.c` viduje
- **Tik vienas** object failas → nėra linkavimo

**Kodėl BLOGAI?**
- `.c` failai **neturėtų būti** include'inami!
- **Antipattern** - blogas stilius
- Funkcijų prototipai **implicit**
- Negali **atskirai** kompiliuoti

---

## 💡 Teisingas Sprendimas?

**03 etapas** - [Discovering_C_MODULE](../03_Discovering_C_MODULE/)!
- Header failas (`.h`) deklaracijoms
- Implementation failas (`.c`) apibrėžimams
- `#include "stack.h"` - **teisingai**!

---

## 📖 C Kalba: Deklaracijos vs Apibrėžimai

### Deklaracija:
```c
extern char stack[SIZE];  // "Egzistuoja kažkur"
void push(char c);        // "Funkcija bus apibrėžta"
```

### Apibrėžimas:
```c
char stack[SIZE];         // Sukuria atmintį
void push(char c) { /* */ } // Realizacija
```

**Header** = deklaracijos  
**Implementation** = apibrėžimai

---

## 💡 Refleksijos Klausimai

### Studentams:
1. **Kodėl** `#include "stack.c"` sukelia **NL klaidą** su `gcc stack.c user.c`?
2. **Kodėl** tas pats kodas **veikia** su `gcc user.c`?
3. **Kuo** skiriasi **deklaracija** nuo **apibrėžimo**?

### Atsakymai:
1. Nes funkcijos apibrėžtos **du kartus** → linker error
2. Nes kompiliuojamas **tik** `user.c` → vienas object failas
3. **Deklaracija** = "sako, kad egzistuoja", **apibrėžimas** = "sukuria"

---

## 🎓 Mokymosi Rezultatas

Po šio etapo suprasi:
- ❌ **Multiple definition** klaidos priežastį
- ⚠️ Kodėl `#include "file.c"` = **antipattern**
- ✅ **Deklaracijos** vs **Apibrėžimai**
- ✅ **Linker** veikimo principus

---

## ➡️ Kitas Žingsnis

**Etapas 03:** [Discovering_C_MODULE](../03_Discovering_C_MODULE/)
- **Teisingas** header + implementation
- `extern` deklaracijos
- Bet **atakos** vis dar pavyks... 🔓

---

## 🔗 Nuorodos

- [← Atgal į 01 Etapą](../01_Running_PROGRAM/)
- [↑ Atgal į Root](../)
- [→ Kitas Etapas](../03_Discovering_C_MODULE/)

---

**Klaidos = geriausias mokytojas!** 🐛→💡
