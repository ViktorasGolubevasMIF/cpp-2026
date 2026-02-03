# Etapas 04: Protecting_IMPLEMENTATION

> **Koncepcija:** `static` internal linkage - Information Hiding  
> **Problema:** Tik vienas stekas sistemoje  
> **Sprendimas:** Duomenys paslėpti, bet ne objektinio tipo

---

## 🎯 Tikslas

Paslėpti implementacijos detales naudojant **`static`**:
- `static char stack[]` - matoma **TIK** `stack.c` faile
- `static int top` - matoma **TIK** `stack.c` faile
- `static void reset()` - private funkcija

**Rezultatas:** Atakos su `extern` **nepavyks**!

---

## 📂 Subdirektorijos

| Subdirektorija | Rezultatas | Aprašymas |
|----------------|-----------|-----------|
| [01_NL_Attack/](01_NL_Attack/) | ❌ NL klaida | Ataka **nepavyksta** - `undefined reference` |
| [02_OK/](02_OK/) | ✅ Veikia | Tinkamas information hiding su `static` |

---

## 🔧 Kompiliavimo Instrukcijos

### Tinkamas variantas (02_OK):
```bash
cd 02_OK
gcc stack.c user.c -o app
./app
```

**Rezultatas:** ✅ Veikia!

---

### Atakos bandymas (01_NL_Attack):
```bash
cd 01_NL_Attack
gcc stack.c user_attack.c -o app
```

**Rezultatas:** ❌ Linkavimo klaida!
```
undefined reference to `stack'
undefined reference to `top'
undefined reference to `reset'
```

**Ataka NEPAVYKO!** 🎉

---

## 📖 Pagrindinės Pamokos

### ✅ Kas Pagerėjo (lyginant su 03 etapu)?

**03 etapas (blogo):**
```c
// stack.h
extern char stack[SIZE];  // ← Vieši!
extern int top;

// user.c
extern char stack[];      // ← Ataka pavyksta!
extern int top;
stack[top++] = '!';       // ← Veikia!
```

**04 etapas (gerai):**
```c
// stack.c
static char stack[SIZE];  // ← PRIVATE!
static int top = 0;       // ← PRIVATE!

// user.c
extern char stack[];      // ← Banlo pasiekti
extern int top;
stack[top++] = '!';       // ← NL KLAIDA!
```

---

### 🔒 `static` Internal Linkage

**`static` C kalboje:**
- Globalus kintamasis/funkcija matoma **TIK** tame `.c` faile
- Kiti failai **negali** pasiekti net su `extern`
- Tai C kalbos **information hiding** mechanizmas

**Analogija C++:** `static` ≈ `private` klasėje

---

## ❌ Kas Vis Dar Problematiška?

### Tik Vienas Stekas!

```c
// stack.c
static char stack[SIZE];  // ← Vienas globalus masyvas
static int top = 0;

// Negalima:
init();     // Inicializuoja VIENĄ steką
init();     // Resets tą patį steką!
```

**Problema:** Negalime turėti `st1`, `st2` egzempliorių!

**Sprendimas:** 05 etapas - `struct Stack`!

---

## 💡 Refleksijos Klausimai

### Studentams:
1. **Kodėl** `extern` **neveikia** su `static` kintamaisiais?
2. **Kuo** skiriasi **external linkage** nuo **internal linkage**?
3. **Kaip** galėtume turėti **du** stekas šiame kode?

### Atsakymai:
1. Nes `static` = **internal linkage** → simbolis **nematomas** linker'iui
2. **External** = visible across files, **Internal** = visible only in file
3. **Neįmanoma** - reikia tipo (`struct`)! → **05 etapas**

---

## 🎓 Mokymosi Rezultatas

Po šio etapo suprasi:
- ✅ **`static`** internal linkage
- ✅ **Information hiding** principą
- ✅ **Saugumas** nuo `extern` atakų
- ⚠️ **Ribojimas** - tik vienas egzempliorius

---

## ➡️ Kitas Žingsnis

**Etapas 05:** [Defining_USER_TYPE](../05_Defining_USER_TYPE/)
- `struct Stack` tipas
- Kelių egzempliorių galimybė
- NC/RT klaidos - mokymasis iš klaidų!

---

## 🔗 Nuorodos

- [← Atgal į 03 Etapą](../03_Discovering_C_MODULE/)
- [↑ Atgal į Root](../)
- [→ Kitas Etapas](../05_Defining_USER_TYPE/)

---

**`static` = C kalbos `private`!** 🔒
