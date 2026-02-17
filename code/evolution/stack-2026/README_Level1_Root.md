# Stack ADT Evoliucija: Nuo C iki C++
## 9 Etapų Kelionė

> **Projektas:** C → C++ ADT Evoliucija  
> **Tikslas:** Parodyti natūralią Stack ADT raidą su **tikromis klaidomis**  
> **Mokymui:** Kompiliavimo/linkavimo klaidos = mokymo įrankis

---

## 🎯 Subdirektorijų Žymėjimai

| Žymėjimas | Pavadinimas | Aprašymas |
|-----------|-------------|-----------|
| **NC** | Not Compiling | Kompiliavimo klaida (syntax/type error) |
| **NL** | Not Linking | Linkavimo klaida (multiple definition/undefined reference) |
| **OK** | Veikia | Kompiliuojasi ir veikia teisingai |
| **OK_Attack** | Veikia, bet ataka pavyksta | Demonstruoja saugumo spragą |
| **OK_Wrong** | Veikia, bet neteisingai | Antipattern ar dizaino klaida |
| **RT** | Runtime Error | Kompiliuojasi, bet runtime klaida |

---

## 📖 9 Etapų Apžvalga

| Nr | Etapas | Koncepcija | Info Hiding? | Subdirektorijos | Esmė |
|----|--------|------------|--------------|-----------------|------|
| **01** | [Running_PROGRAM](01_Running_PROGRAM/) | Monolitas | ❌ | - | Visa logika viename faile |
| **02** | [Decomposing_to_UNITS](02_Decomposing_to_UNITS/) | Fizinis padalijimas | ❌ | `02_NL`, `03_OK_Wrong` | Mokome `#include` klaidų |
| **03** | [Discovering_C_MODULE](03_Discovering_C_MODULE/) | C modulis (h+c) | ❌ | `04_OK`, `05_OK_Attack` | Header vs implementation |
| **04** | [Protecting_IMPLEMENTATION](04_Protecting_IMPLEMENTATION/) | `static` linkage | ✅ (1 stekas) | `01_NL_Attack`, `02_OK` | Information hiding |
| **05** | [Defining_USER_TYPE](05_Defining_USER_TYPE/) | `struct` tipas | ❌ | `01_NC`, `02_RT`, `03_OK` | Pass-by-value vs pointer |
| **06** | [ENCAPSULATING_UDT_in_Module](06_ENCAPSULATING_UDT_in_Module/) | Modulinis UDT | ❌ | `01_OK`, `02_OK_Attack` | Encapsulation ≠ hiding |
| **07** | [HIDING_UDT_Information](07_HIDING_UDT_Information/) | Opaque pointer | ✅ | `01_NC` | Forward declaration |
| **08** | [Acquiring_RESOURCES](08_Acquiring_RESOURCES/) | Factory pattern | ✅ | `02_OK` | create()/destroy() |
| **09** | [Completing_LIFECYCLE](09_Completing_LIFECYCLE/) | Lifecycle completion | ✅ | `01_OK` | Pilnas lifecycle |

---

## 🚀 Greitas Startas

### Kompiliavimo instrukcijos
```bash
cd <etapo_subdirektorija>
gcc stack.c user.c -o app
./app
```

### Pavyzdys (03 etapas):
```bash
cd 03_Discovering_C_MODULE/04_OK
gcc stack.c user.c -o app
./app
```

---

## 📚 Evoliucijos Logika

### Kiekvienas etapas sprendžia ankstesnio problemą:

```
01 (Monolitas)
  ↓ Problema: nelankstus, globalūs kintamieji
02 (Decomposing)
  ↓ Problema: NL klaidos, antipattern
03 (MODULE)
  ↓ Problema: duomenys vieši → ataka pavyksta
04 (static)
  ↓ Problema: tik vienas stekas
05 (struct)
  ↓ Problema: NC/RT klaidos
06 (UDT in Module)
  ↓ Problema: struktūra vieša → ataka pavyksta
07 (Opaque Pointer)
  ↓ Problema: kaip sukurti objektą?
08 (Factory)
  ↓ Problema: pilnas lifecycle?
09 (Lifecycle Completion)
  ✓ Pasiekta: pilnas C ADT!
```

---

## 🎓 Mokymosi Tikslai

### Po šių 9 etapų suprasi:
- ✅ **Moduliavimą** - `.h` vs `.c` failai
- ✅ **Information hiding** - `static`, opaque pointer
- ✅ **Encapsulation vs Hiding** - **skirtumas**!
- ✅ **ADT principus** - abstrakcija, sąsaja
- ✅ **Kompiliavimo/linkavimo klaidas** - kaip jas skaityti
- ✅ **Factory pattern** - resursų valdymas
- ✅ **C → C++ tiltą** - kodėl RAII, private, templates

---

## 📝 Kaip Naudoti

### Dėstytojams:
1. **Paskaitoje** - atidarykite konkretų etapą
2. **Kompiliuokite** su klaidomis (NC/NL)
3. **Demonstruokite** atakas (OK_Attack)
4. **Paaiškinkite** sprendimą

### Studentams:
1. **Skaitykite** kiekvieno etapo README.md
2. **Kompiliuokite** kiekvieną subdirektoriją
3. **Analizuokite** klaidas
4. **Užpildykite** užduočių lapus

---

## 🔗 Papildoma Medžiaga

- [Pilnas Gidas (9 Etapai)](../../docs/evolution/Stack_Evolution_Guide_9_Stages_FINAL.md)
- [Advanced Techniques (10A-10D)](../../docs/evolution/Stack_Advanced_Techniques_10A-10D.md)

---

## ⚙️ Kompiliavimo Aplinka

### Reikalavimai:
- **GCC** 5.0+ arba **Clang** 3.5+
- **C standart:** C11 (`-std=c11`)
- **Warnings:** `-Wall -Wextra`

### Rekomenduojama:
```bash
gcc -Wall -Wextra -std=c11 -o app stack.c user.c
```

---

## 🎯 Esminė Mintis

> **9 etapų kelionė** nuo **naivaus monolito** iki **profesionalaus ADT**.  
> Kiekvienas žingsnis **sprendžia konkrečią problemą**.  
> **Klaidos** = mokymo įrankis, ne kliūtis!

---

**Sėkmės mokantis!** 🚀  
**Versija:** 2026.01  
**Licenzija:** MIT (laisvas naudojimas mokymo tikslams)
