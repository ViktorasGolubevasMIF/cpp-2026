# 04_OK - Tinkamas C Modulis

> **Statusas:** ✅ Kompiliuojasi ir veikia  
> **Koncepcija:** Teisingas header (.h) + implementation (.c) skaidymas

---

## 📋 Failai

```
04_OK/
├── stack.h      ← Header (sąsaja)
├── stack.c      ← Implementation (realizacija)
└── user.c       ← Naudotojas
```

---

## 🔧 Kompiliavimas

```bash
gcc stack.c user.c -o app
```

**Rezultatas:** ✅ Veikia!

---

## 📖 Kaip Veikia?

### stack.h (Deklaracijos)
```c
#define SIZE 5

extern char stack[SIZE];  // ← Sako: "bus apibrėžta kitur"
extern int top;

void init(void);
int isEmpty(void);
// ... kitos funkcijos
```

### stack.c (Apibrėžimai)
```c
#include "stack.h"

char stack[SIZE];  // ← Čia sukuriama atmintis!
int top = 0;

void init(void) {
    top = 0;
}
// ... realizacijos
```

---

## ✅ Kas Gerai?

1. **Aiški sąsaja** - `stack.h` rodo, ką galima naudoti
2. **Nėra multiple definition** - kiekvienas simbolis apibrėžtas **vieną kartą**
3. **Modulinis** - galima naudoti keliuose projektuose

---

## ⚠️ Kas Vis Dar Problematiška?

**Duomenys vieši!**
- `extern char stack[];` gali panaudoti **bet kas**
- `extern int top;` irgi prieinamas
- **Žiūrėk:** [05_OK_Attack](../05_OK_Attack/) - atakos pavyzdį

---

## 🎯 Testuok

### Įvestis:
```
123456
```

### Tikėtina išvestis:
```
654321
```

---

## 💡 Refleksija

**Klausimas:** Kodėl `extern` header'yje, o ne .c faile?

**Atsakymas:** `extern` = **deklaracija** (sako kompiliatoriui, kad simbolis egzistuoja). Apibrėžimas (be `extern`) **sukuria** atmintį. Jei abu failai turėtų apibrėžimus → **multiple definition** klaida!

---

## ➡️ Toliau

- **Atakos scenarijus:** [05_OK_Attack](../05_OK_Attack/)
- **Sprendimas:** [Etapas 04 - static](../../04_Protecting_IMPLEMENTATION/)

---

**Kompiliavimo įrankis = geriausias mokytojas!** 🚀
