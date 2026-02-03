# 01_NL_Attack - static Apsauga Veikia!

> **Statusas:** ❌ Not Linking - ataka **NEPAVYKO**!  
> **Tikslas:** Parodyti, kad `static` apsaugo nuo `extern` atakų  
> **Pamoka:** Internal linkage = compile-time apsauga!

---

## 🛡️ Atakos Bandymas

**Kodas (user_attack.c):**
```c
#include <stdio.h>
#include "stack.h"

extern char stack[];     // 🔥 Bandome pasiekti
extern int top;          // 🔥
extern void reset(void); // 🔥 Private funkciją

int main(void) {
    init();
    push('A');
    
    // 🔥 ATAKA: tiesioginis prieiga!
    stack[top++] = '!';
    
    // 🔥 ATAKA: private funkcija
    reset();
    
    return 0;
}
```

---

## 🔧 Kompiliavimas

```bash
gcc stack.c user_attack.c -o app
```

---

## ❌ Linkavimo Klaida!

```
undefined reference to `stack'
undefined reference to `top'
undefined reference to `reset'
```

**Ataka NEPAVYKO!** 🎉

---

## 🔒 Kodėl Nepavyko?

### stack.c su `static`:
```c
static char stack[SIZE];    // ← Internal linkage!
static int top = 0;         // ← Internal linkage!
static void reset(void) {   // ← Internal linkage!
    top = 0;
}
```

### Kas nutiko:

1. **`static`** = **internal linkage**
2. Simboliai matomi **TIK** `stack.c` viduje
3. **Linker** nežino apie šiuos simbolius
4. `extern` bandymas → **undefined reference**

---

## 📖 Pagrindinė Pamoka

### `static` = C kalbos `private`!

| C (`static`) | C++ (`private`) |
|-------------|-----------------|
| Internal linkage | Access control |
| Linker-level | Compiler-level |
| File scope | Class scope |

**Rezultatas:** Informacijos slėpimas ✅

---

## 💡 External vs Internal Linkage

### External (default):
```c
char stack[SIZE];  // ← Matoma VISUR (su extern)
```

### Internal (`static`):
```c
static char stack[SIZE];  // ← Matoma TIK šiame faile!
```

---

## 🎯 Testuok

Pabandyk sukompiliuoti ir pamatysi klaidas:

```bash
cd 01_NL_Attack
gcc stack.c user_attack.c -o app
# ❌ undefined reference klaidos!
```

---

## 💡 Refleksija

**Klausimas:** Kodėl `extern` neveikia su `static` kintamaisiais?

**Atsakymas:** `static` = **internal linkage** → simbolis **neeksportuojamas** linker'iui → `extern` negali jo rasti!

---

## ✅ Teisingas Variantas

Žiūrėk: [../02_OK/](../02_OK/) - teisingas naudojimas be atakų!

---

## ➡️ Sprendimas

**Rezultatas:** `static` veikia! Bet vis dar **tik vienas stekas** → **05 etapas** su `struct`!

---

**`static` = linker'io nematomumas!** 🔒
