# Etapas 09: Completing_LIFECYCLE

> **Koncepcija:** Lifecycle Completion - pilnas objekto gyvavimo ciklas  
> **Problema:** Error handling, validation, robustness  
> **Sprendimas:** Defensive programming, pilnas C ADT!

---

## 🎯 Tikslas

Užbaigti ADT evoliuciją C kalboje:
- **Error handling** - `NULL` patikrinimas
- **Defensive programming** - validacijos
- **Robustness** - saugus kodas

**Rezultatas:** Pilnai užbaigtas C ADT - pasiruošęs C++!

---

## 📂 Subdirektorijos

| Subdirektorija | Rezultatas | Aprašymas |
|----------------|-----------|-----------|
| [01_OK/](01_OK/) | ✅ Veikia | Pilnas lifecycle su error handling |

---

## 🔧 Kompiliavimas

```bash
cd 01_OK
gcc stack.c user.c -o app
./app
```

**Rezultatas:** ✅ Veikia su pilnu lifecycle!

---

## 📖 Pagrindinės Pamokos

### ✅ Kas Pagerėjo (lyginant su 08 etapu)?

**08 etapas (bazinis):**
```c
void push(Stack *s, char c) {
    if (!isFull(s)) {  // ← Bet jei s == NULL?
        s->stack[s->top++] = c;
    }
}
```

**09 etapas (defensive):**
```c
void push(Stack *s, char c) {
    if (s == NULL) return;     // ← NULL check!
    if (isFull(s)) return;     // ← Validation
    s->stack[s->top++] = c;
}

int isEmpty(const Stack *s) {
    return (s == NULL) || (s->top == 0);  // ← NULL-safe
}
```

---

## 🛡️ Defensive Programming

### NULL Pointer Checks:
```c
Stack* create(void) {
    Stack *s = malloc(sizeof(Stack));
    if (s != NULL) {  // ← Tikrinti malloc!
        s->top = 0;
    }
    return s;
}

void destroy(Stack *s) {
    if (s != NULL) {  // ← Safe free
        free(s);
    }
}

void push(Stack *s, char c) {
    if (s == NULL) return;  // ← Prevent crash
    // ...
}
```

---

### Const Correctness:
```c
int isEmpty(const Stack *s) {  // ← const = nemodifikuoja
    return (s == NULL) || (s->top == 0);
}

int isFull(const Stack *s) {   // ← const = read-only
    return (s != NULL) && (s->top == SIZE);
}
```

**Pamoka:** `const` = **compile-time** garantija, kad funkcija nemodifikuos!

---

## 🎓 C ADT - Pasiekta!

### 9 Etapų Kelionė:

```
01 (Monolitas)
  ↓ Problema: globalūs, nelankstus
02 (Decomposing)
  ↓ Problema: NL klaidos, antipattern
03 (MODULE)
  ↓ Problema: duomenys vieši
04 (static)
  ↓ Problema: tik vienas stekas
05 (struct)
  ↓ Problema: NC/RT klaidos
06 (UDT in Module)
  ↓ Problema: struktūra vieša
07 (Opaque Pointer)
  ↓ Problema: kaip sukurti objektą?
08 (Factory)
  ↓ Problema: error handling?
09 (Lifecycle Completion)
  ✓ PASIEKTA: Pilnas C ADT!
```

---

## ✅ Ką Pasiekėme?

### C ADT su:
- ✅ **Information hiding** (opaque pointer)
- ✅ **Encapsulation** (data + operations)
- ✅ **Multiple instances** (factory)
- ✅ **Lifecycle management** (create/destroy)
- ✅ **Error handling** (NULL checks)
- ✅ **Const correctness** (read-only garantijos)

---

## ❌ Ko Dar Trūksta (C++ suteiks)?

### C Trūkumai:
- ❌ **Manual lifecycle** - reikia `create()`/`destroy()`
- ❌ **No RAII** - nėra automatinio cleanup
- ❌ **No operator overloading** - negalima `s1 + s2`
- ❌ **No templates** - negalima `Stack<int>`
- ❌ **No exceptions** - tik error codes

### C++ Privalumai:
- ✅ **Automatic lifecycle** (constructor/destructor)
- ✅ **RAII** - Resource Acquisition Is Initialization
- ✅ **Operator overloading** - `s1 + s2` galimas
- ✅ **Templates** - `Stack<T>` generics
- ✅ **Exceptions** - `try`/`catch` error handling

---

## 💡 C vs C++ Palyginimas

| C (09 etapas) | C++ (klasė) |
|---------------|-------------|
| `Stack *s = create();` | `Stack s;` (auto) |
| `destroy(s);` manual | Auto destructor |
| `push(s, 'A');` | `s.push('A');` |
| Error codes (`if (s == NULL)`) | Exceptions (`try`/`catch`) |
| `void*` generic | Templates `<T>` |
| `const Stack *s` | `const Stack& s` |

---

## 💡 Refleksijos Klausimai

### Studentams:
1. **Kodėl** reikia `NULL` patikrinimų?
2. **Ką** reiškia `const Stack *s`?
3. **Kodėl C++ reikalingas**, jei C gali pasiekti tą patį?

### Atsakymai:
1. **Defensive programming** - apsauga nuo crash'ų (`malloc` gali grąžinti `NULL`)
2. `const` = funkcija **garantuoja**, kad nemodifikuos objekto
3. C++ **automatizuoja** lifecycle, prideda **type safety**, **exceptions**, **templates**!

---

## 🎓 Mokymosi Rezultatas

Po **9 etapų** suprasi:
- ✅ Pilną **C ADT** evoliuciją
- ✅ **Information hiding** principus
- ✅ **Encapsulation** vs **Hiding** skirtumą
- ✅ **Factory pattern** prasmę
- ✅ **Defensive programming** būtinybę
- ✅ **C → C++ tiltą** - kodėl OOP reikalingas

---

## ➡️ Kas Toliau?

### Advanced Techniques (10A-10D):
- **10A:** [typedef](../../../docs/evolution/Stack_Advanced_Techniques_10A-10D.md#10a) - Type aliasing
- **10B:** [const correctness](../../../docs/evolution/Stack_Advanced_Techniques_10A-10D.md#10b) - Const methods
- **10C:** [function pointers](../../../docs/evolution/Stack_Advanced_Techniques_10A-10D.md#10c) - Polymorphism
- **10D:** [void* generic](../../../docs/evolution/Stack_Advanced_Techniques_10A-10D.md#10d) - Templates bridge

### C++ Perėjimas:
- `class Stack` su `private`/`public`
- Konstruktoriai/Destruktoriai
- RAII principas
- Operator overloading
- Templates

---

## 🔗 Nuorodos

- [← Atgal į 08 Etapą](../08_Acquiring_RESOURCES/)
- [↑ Atgal į Root](../)
- [Advanced Techniques](../../../docs/evolution/Stack_Advanced_Techniques_10A-10D.md)
- [Pilnas Gidas](../../../docs/evolution/Stack_Evolution_Guide_9_Stages_FINAL.md)

---

## 🎬 Finišas!

> **9 etapų kelionė nuo naivaus monolito iki profesionalaus C ADT!**  
> Kiekvienas žingsnis sprendė konkrečią problemą.  
> **Klaidos** = mokymo įrankis, ne kliūtis!

**Dabar pasiruošęs C++!** 🚀

---

**C ADT = rankinis darbas. C++ = automatizavimas + elegancija!** ✨
