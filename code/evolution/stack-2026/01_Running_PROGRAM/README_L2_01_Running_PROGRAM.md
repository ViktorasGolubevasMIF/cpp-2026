# Etapas 01: Running_PROGRAM

> **Koncepcija:** MONOLITH - visa logika vienoje vietoje  
> **Problema:** Globalūs kintamieji, nelankstus, neskalabiluojamas  
> **Sprendimas:** Veikia, bet reikia evoliucijos!

---

## 🎯 Tikslas

Parodyti **paprasčiausią** steko implementaciją:
- Visa logika **viename faile**
- **Globalūs** kintamieji
- **main()** funkcija čia pat

**Tikslas:** Suprasti, kad **veikiantis kodas ≠ geras kodas**

---

## 📂 Struktūra

```
01_Running_PROGRAM/
└── stack.c      ← Viskas viename faile!
```

**Nėra subdirektorijų** - vienas failas, vienas scenarijus.

---

## 🔧 Kompiliavimas

```bash
cd 01_Running_PROGRAM
gcc stack.c -o app
./app
```

**Rezultatas:** ✅ Veikia!

---

## 📖 Kodas (santrauka)

```c
#include <stdio.h>
#define SIZE 5

char stack[SIZE];  // ← Globalus masyvas
int top = 0;       // ← Globalus indeksas

void init(void) { top = 0; }
int isEmpty(void) { return 0 == top; }
int isFull(void) { return SIZE == top; }
void push(char c) { /* ... */ }
char pop(void) { /* ... */ }

int main(void) {
    // Naudojimas čia pat
}
```

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

1. **Paprastas** - viskas vienoje vietoje
2. **Lengva suprasti** - pradedantiesiems
3. **Greitas** prototipavimas

---

## ❌ Trūkumai

1. **Globalūs kintamieji** - name pollution
2. **Tik vienas stekas** - nelankstus
3. **Neskalabiluojamas** - sunku plėsti
4. **Neperkeliamas** - negalima panaudoti kituose projektuose
5. **Sunku testuoti** - viskas susimaišę

---

## 📖 Pagrindinė Pamoka

### Veikiantis Kodas ≠ Geras Kodas

**Programuotojai pradedantieji** dažnai mano:
> "Jei kompiliuojasi ir veikia → viskas gerai!"

**Patyrę programuotojai** žino:
> "Kodas turi būti **maintainable**, **scalable**, **reusable**!"

---

## 💡 Refleksijos Klausimai

### Studentams:
1. **Kodėl** globalūs kintamieji **blogi**?
2. **Kaip** sukurti **du** nepriklausomus stekas šiame kode?
3. **Ką** reikštų **panaudoti** šį steką **kitame projekte**?

### Atsakymai:
1. Name pollution, hard to test, unclear ownership
2. **Neįmanoma** - reikia tipo (struct)! → **05 etapas**
3. Reikėtų **kopijuoti-klijuoti** kodą → **blogas sprendimas**!

---

## 🎓 Mokymosi Rezultatas

Po šio etapo suprasi:
- ✅ Kaip veikia **paprasčiausias** stekas
- ⚠️ **Monolito** problemas
- ✅ **Kodėl** reikia moduliavimo

---

## ➡️ Kitas Žingsnis

**Etapas 02:** [Decomposing_to_UNITS](../02_Decomposing_to_UNITS/)
- Bandysime **atskirti** logiką į failus
- Susidurkime su **NL klaidomis**!
- Išmoksime **antipattern** `#include "file.c"`

---

## 🔗 Nuorodos

- [↑ Atgal į Root](../)
- [→ Kitas Etapas](../02_Decomposing_to_UNITS/)
- [Pilnas Gidas](../../../docs/evolution/Stack_Evolution_Guide_9_Stages_FINAL.md)

---

**Pradžia visada paprasta - evoliucija ateina vėliau!** 🌱→🌳
