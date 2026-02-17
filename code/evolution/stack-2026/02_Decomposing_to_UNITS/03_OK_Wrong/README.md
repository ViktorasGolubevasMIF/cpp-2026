# 03_OK_Wrong - Antipattern (veikia, bet blogai)

> **Statusas:** ⚠️ Veikia, bet **ANTIPATTERN**  
> **Tikslas:** Parodyti, kodėl `#include "file.c"` = blogas stilius  
> **Pamoka:** Tai, kad veikia ≠ teisingai!

---

## ⚠️ Kodas

```c
// user.c
#include "stack.c"  // ← ANTIPATTERN!

int main(void) {
    init();
    push('A');
    // ...
}
```

---

## 🔧 Kompiliavimas

```bash
gcc user.c -o app  # ← Tik user.c!
```

**Rezultatas:** ✅ Veikia!

```
Input:  123
Output: 321
```

---

## 🔍 Kodėl Veikia?

1. `#include "stack.c"` → nukopijuoja **visą** `stack.c` turinį į `user.c`
2. Kompiliuojame **TIK** `user.c` → vienas object failas
3. Viename object faile → **nėra** multiple definition
4. Veikia! ✅

---

## ❌ Kodėl BLOGAI?

### 1. **Antipattern**
`.c` failai **neturėtų** būti include'inami!

### 2. **Implicit Function Declarations**
Jei funkcijos nenaudoja prototipų → compiler warnings

### 3. **Negalima Atskirai Kompiliuoti**
```bash
gcc -c stack.c  # ← Bet nebenaudojame
gcc -c user.c   # ← Viskas čia
```

### 4. **Blogas Stilius**
Professionali praktika: **header** failai deklaracijoms!

---

## 💡 Pamoka

### Veikiantis Kodas ≠ Geras Kodas

**Veikia:** ✅  
**Teisingai:** ❌

**Teisingas būdas:**
```c
// stack.h
void init(void);
void push(char c);

// user.c
#include "stack.h"  // ← Teisingai!
```

---

## ✅ Sprendimas

**03 etapas** - [Discovering_C_MODULE](../../03_Discovering_C_MODULE/) su **header** failu!

---

## 🔗 Nuorodos

- [← Multiple Definition klaida](../02_NL/)
- [↑ Atgal į Etapą](../)
- [→ Sprendimas (03 etapas)](../../03_Discovering_C_MODULE/)

---

**Antipattern = veikia, bet neteisingai!** ⚠️
