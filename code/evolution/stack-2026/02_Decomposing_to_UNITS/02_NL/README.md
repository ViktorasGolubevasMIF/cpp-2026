# 02_NL - Multiple Definition Error

> **Statusas:** ❌ NL (Not Linking) - linkavimo klaida  
> **Tikslas:** Demonstruoti **multiple definition** klaidą  
> **Pamoka:** `#include "file.c"` + kompiliuoti atskirai = dvigubos definicijos

---

## 🔥 Problema

**Kodas:**
```c
// user.c
#include "stack.c"  // ← KLAIDA!

int main(void) {
    init();
    // ...
}
```

**Kompiliavimas:**
```bash
gcc stack.c user.c -o app
```

---

## ❌ Linkavimo Klaida

```
multiple definition of `init'
multiple definition of `isEmpty'
multiple definition of `isFull'
multiple definition of `push'
multiple definition of `pop'
multiple definition of `stack'
multiple definition of `top'
```

---

## 🔍 Kas Nutiko?

1. **`user.c` faile:** `#include "stack.c"` → nukopijuoja visą turinį
2. **Kompiliavimas:**
   - `gcc -c stack.c` → sukuria `stack.o` su visomis funkcijomis
   - `gcc -c user.c` → sukuria `user.o` **SU TOM PAČIOM** funkcijomis
3. **Linkavimas:** Linker mato kiekvieną funkciją **du kartus** → klaida!

---

## 💡 Pamoka

### Deklaracija vs Apibrėžimas:
- **Apibrėžimas** = sukuria atmintį/kodą (vieną kartą!)
- **Deklaracija** = sako, kad egzistuoja (daug kartų OK)

### Problema:
`.c` failai turi **apibrėžimus** → negalima nukopijuoti kelis kartus!

---

## ✅ Sprendimas

**03 etapas** - teisingas modulis su **header** failu!

---

## 🔗 Nuorodos

- [← Teisingas variantas](../03_OK_Wrong/)
- [↑ Atgal į Etapą](../)
- [→ Sprendimas (03 etapas)](../../03_Discovering_C_MODULE/)
