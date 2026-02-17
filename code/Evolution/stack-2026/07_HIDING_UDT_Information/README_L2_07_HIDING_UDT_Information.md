# Etapas 07: HIDING_UDT_Information

> **Koncepcija:** Opaque Pointer - forward declaration, incomplete type  
> **Problema:** Kaip sukurti objektą, jei nežinome dydžio?  
> **Sprendimas:** Tikras Information Hiding - atakos nepavyksta! 🔒

---

## 🎯 Tikslas

Paslėpti struktūros apibrėžimą naudojant **forward declaration**:
- `struct Stack;` header'yje - **incomplete type**
- Pilnas apibrėžimas **TIK** `.c` faile
- Naudotojas **nežino** vidinių laukų

**Rezultatas:** Tikras ADT - information hiding veikia!

---

## 📂 Subdirektorijos

| Subdirektorija | Rezultatas | Aprašymas |
|----------------|-----------|-----------|
| [01_NC/](01_NC/) | ❌ NC klaida | Bandymas deklaruoti `struct Stack st;` → **incomplete type** |

**Pastaba:** Tik viena subdirektorija, nes **teisingas** naudojimas reikalauja factory pattern → **08 etapas**!

---

## 🔧 Kompiliavimo Instrukcijos

### Bandymas naudoti (01_NC):
```bash
cd 01_NC
gcc stack.c _user.c -o app
```

**Rezultatas:** ❌ Kompiliavimo klaida!
```
error: storage size of 'st1' isn't known
error: variable has incomplete type 'struct Stack'
```

---

### Bandymas atakuoti (01_NC/user_attack.c):
```bash
gcc stack.c user_attack.c -o app
```

**Rezultatas:** ❌ Kompiliavimo klaida!
```
error: storage size of 'st1' isn't known
error: variable has incomplete type 'struct Stack'
```

**Ataka NEPAVYKO!** 🎉

---

## 📖 Pagrindinės Pamokos

### ✅ Kas Pagerėjo (lyginant su 06 etapu)?

**06 etapas (blogai):**
```c
// stack.h
struct Stack {
    char stack[SIZE];  // ← MATOMAS!
    int top;           // ← MATOMAS!
};

// user.c
st.stack[st.top++] = '!';  // ← Ataka pavyko!
```

**07 etapas (gerai):**
```c
// stack.h
struct Stack;  // ← Forward declaration, incomplete type

// stack.c
struct Stack {
    char stack[SIZE];  // ← PASLĖPTA!
    int top;           // ← PASLĖPTA!
};

// user.c
struct Stack st;           // ← NC klaida!
st.stack[st.top++] = '!';  // ← NC klaida!
```

---

## 🔒 Opaque Pointer Idiom

### Forward Declaration:
```c
// stack.h
struct Stack;  // ← Sako: "tipas egzistuoja, bet nežinau struktūros"
```

**Kas galima:**
```c
struct Stack *pst;  // ✅ Rodyklė - žinome adreso dydį (8 bytes)
```

**Kas NEgalima:**
```c
struct Stack st;  // ❌ Objektas - nežinome struktūros dydžio!
```

---

### Incomplete Type:

**Incomplete type** = tipas, kurio dydis **nežinomas**

**Kodėl negalima:**
```c
struct Stack st;  // Kompilatoriui reikia žinoti: kiek atmintės skirti?
```

**Kodėl galima:**
```c
struct Stack *pst;  // Rodyklė visada 8 bytes (64-bit sistemoje)
```

---

## ⚠️ Problema: Kaip Sukurti Objektą?

### Negalima:
```c
struct Stack st;  // ❌ NC klaida: incomplete type
```

### Sprendimas?
**Factory pattern** - 08 etapas!
```c
struct Stack* create(void);  // ← Funkcija sukuria objektą
void destroy(struct Stack*); // ← Funkcija sunaikina
```

---

## 💡 C vs C++ Palyginimas

| C (Opaque Pointer) | C++ (Private) |
|--------------------|---------------|
| `struct Stack;` forward declaration | `class Stack { private: ... };` |
| Negalima `struct Stack st;` | Galima `Stack s;` (auto construction) |
| Factory: `create()`/`destroy()` | Constructor/Destructor auto |
| Incomplete type compile-time check | Private access compile-time check |

**Pamoka:** C++ **automatizuoja** tai, ką C daro **rankiniu** būdu!

---

## 💡 Refleksijos Klausimai

### Studentams:
1. **Kas** yra **forward declaration**?
2. **Kodėl** negalima deklaruoti `struct Stack st;` su forward declaration?
3. **Kaip** galėtume sukurti steką, jei negalime `struct Stack st;`?

### Atsakymai:
1. Deklaracija, kuri sako "tipas egzistuoja", bet nepateikia pilno apibrėžimo
2. Kompiliatorius **nežino dydžio** → nežino, kiek atmintės skirti
3. **Factory funkcija** `create()` → 08 etapas!

---

## 🎓 Mokymosi Rezultatas

Po šio etapo suprasi:
- ✅ **Forward declaration** koncepcija
- ✅ **Incomplete type** prasmė
- ✅ **Opaque pointer** idiom
- ✅ **Tikras information hiding** - compile-time apsauga
- ⚠️ **Factory pattern** būtinybė

---

## ➡️ Kitas Žingsnis

**Etapas 08:** [Acquiring_RESOURCES](../08_Acquiring_RESOURCES/)
- **Factory funkcijos** - `create()` / `destroy()`
- Dynamic memory allocation
- Manual resource management
- Pilnas lifecycle valdymas!

---

## 🔗 Nuorodos

- [← Atgal į 06 Etapą](../06_ENCAPSULATING_UDT_in_Module/)
- [↑ Atgal į Root](../)
- [→ Kitas Etapas](../08_Acquiring_RESOURCES/)

---

**Opaque Pointer = tikras Information Hiding!** 🔒✨
