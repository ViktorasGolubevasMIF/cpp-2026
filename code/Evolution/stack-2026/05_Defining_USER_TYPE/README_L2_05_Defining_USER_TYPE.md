# Etapas 05: Defining_USER_TYPE

> **Koncepcija:** `struct Stack` - vartotojo apibrėžiamas tipas  
> **Problema:** NC klaidos (`static` struktūroje), RT klaidos (pass-by-value)  
> **Sprendimas:** `struct` + rodyklės = kelių egzempliorių galimybė

---

## 🎯 Tikslas

Sukurti **tipą**, kad galėtume turėti **kelis** stekas:
- `struct Stack st1, st2, st3;` - kiek norime!
- Kiekvienas stekas - **nepriklausomas**

**Bet:** Kelyje sutinkame **klaidas**, kurios moko!

---

## 📂 Subdirektorijos

| Subdirektorija | Rezultatas | Aprašymas |
|----------------|-----------|-----------|
| [01_NC/](01_NC/) | ❌ NC klaida | `static` struktūros nariams - **draudžiama** C! |
| [02_RT/](02_RT/) | ⚠️ RT klaida | Pass-by-value - duomenys neįsimena! |
| [03_OK/](03_OK/) | ✅ Veikia | Teisingas `struct` + rodyklės |

---

## 🔧 Kompiliavimo Instrukcijos

### Scenarijus 1: NC klaida (01_NC):
```bash
cd 01_NC
gcc stack.c -o app
```

**Rezultatas:** ❌ Kompiliavimo klaida!
```
error: expected specifier-qualifier-list before 'static'
error: type name does not allow storage class to be specified
```

---

### Scenarijus 2: RT klaida (02_RT):
```bash
cd 02_RT
gcc stack.c -o app
./app
```

**Rezultatas:** ✅ Kompiliuojasi, bet **neteisingai veikia**!
```
Input:  123
Output: (tuščia)  ← Duomenys nepasikeitė!
```

---

### Scenarijus 3: Teisingai (03_OK):
```bash
cd 03_OK
gcc stack.c -o app
./app
```

**Rezultatas:** ✅ Veikia!

---

## 📖 Pagrindinės Pamokos

### ❌ Klaida #1: `static` struktūroje (01_NC)

**Bandymas:**
```c
struct Stack {
    static char stack[SIZE];  // ❌ C neleidžia!
    static int top;           // ❌
};
```

**Kodėl klaida?**
- C kalba **draudžia** storage-class specifikatorius struktūros nariams
- `struct` = tipo šablonas, o `static` = kintamojo savybė
- **C++ turi** `static` narius, bet **C - NE**!

**Pamoka:** C ir C++ **skiriasi**!

---

### ⚠️ Klaida #2: Pass-by-value (02_RT)

**Bandymas:**
```c
struct Stack {
    char stack[SIZE];
    int top;
};

void init(struct Stack st) {  // ❌ Kopija!
    st.top = 0;  // Modifikuoja tik kopiją!
}

void push(struct Stack st, char c) {  // ❌ Kopija!
    st.stack[st.top++] = c;  // Neįsimena!
}
```

**Kas nutiko?**
```
main():
  st.top = 999  (niekada nepakeistas)

init(st):  ← Gauna KOPIJĄ
  st.top = 0   ← Keičia kopiją
  [funkcija baigiasi, kopija išmetama]

main():
  st.top = 999  ← Vis dar nepakeistas!
```

**Pamoka:** C struktūros perduodamos **by value** (kopijuojamos)!

---

### ✅ Sprendimas: Rodyklės (03_OK)

**Teisingai:**
```c
struct Stack {
    char stack[SIZE];
    int top;
};

void init(struct Stack *pst) {  // ✅ Rodyklė!
    pst->top = 0;  // Keičia originalą!
}

void push(struct Stack *pst, char c) {  // ✅ Rodyklė!
    pst->stack[pst->top++] = c;  // Įsimena!
}

int main(void) {
    struct Stack st1, st2;  // ✅ Du stekai!
    
    init(&st1);  // ← Perduodame adresą
    push(&st1, 'A');
    
    init(&st2);
    push(&st2, 'B');
}
```

**Pamoka:** **Pass-by-pointer** = keičiame originalą!

---

## 💡 C vs C++ Palyginimas

| C | C++ |
|---|-----|
| `struct Stack { ... };` | `class Stack { ... };` |
| Nėra `static` narių | Turi `static` narius |
| Pass-by-pointer manual | Pass-by-reference auto |
| `struct Stack s1;` | `Stack s1;` (trumpiau) |

---

## 💡 Refleksijos Klausimai

### Studentams:
1. **Kodėl** C neleidžia `static` struktūros nariams?
2. **Kas** nutinka, kai perduodi struktūrą **by value**?
3. **Kuo** skiriasi `struct Stack st` nuo `struct Stack *pst`?

### Atsakymai:
1. `struct` = **tipo** apibrėžimas, `static` = **kintamojo** savybė → nesiderina
2. Funkcija gauna **kopiją** → pakeitimai neįsimena
3. `st` = objektas, `pst` = **rodyklė** į objektą (adreso perdavimas)

---

## 🎓 Mokymosi Rezultatas

Po šio etapo suprasi:
- ❌ C **neturi** `static` struktūros narių
- ⚠️ **Pass-by-value** vs **Pass-by-pointer**
- ✅ `struct` = vartotojo tipas
- ✅ **Kelių egzempliorių** galimybė

---

## ➡️ Kitas Žingsnis

**Etapas 06:** [ENCAPSULATING_UDT_in_Module](../06_ENCAPSULATING_UDT_in_Module/)
- Perkelkime `struct` į modulį
- Bet struktūra bus **vieša** header'yje
- Atakos vėl pavyks... 🔓

---

## 🔗 Nuorodos

- [← Atgal į 04 Etapą](../04_Protecting_IMPLEMENTATION/)
- [↑ Atgal į Root](../)
- [→ Kitas Etapas](../06_ENCAPSULATING_UDT_in_Module/)

---

**`struct` = kelių objektų galimybė!** 📦📦📦
