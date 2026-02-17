# 03 etapas – Antraštiniai failai ir modulio klaidos

Šiame etape nagrinėjamas **vienas svarbiausių C kalbos lūžių**:

> ❗**failas ≠ modulis**

> ❗**aprašas ≠ apibrėžimas**

Tai etapas, kuriame studentai dažniausiai susiduria su _linkerio klaidomis_ ir pradeda jausti, kad C programą sudaro *ne tiesiog "kažkokie" `.c` failai*, o **atskirai kompiliuojami vienetai** (_**separate compilation units**_).

## Etapo tikslas

- Suprasti skirtumą tarp **aprašo** (_**declaration**_) ir **apibrėžimo** (_**definition**_)
- Suprasti, kodėl *globalūs kintamieji* antraštiniuose failuose yra pavojingi
- Pamatyti tipines klaidas, dėl kurių *sukompiliuoja*, bet *nesilink’ina*

## Katalogo struktūra

```text
03_headers/
├─ n_nc/        # naivus, nesikompiliuoja
├─ w_nl/        # klaidingas, nesilink’ina (multiple definition)
├─ w_nl2/       # klaidingas, nesilink’ina (undefined reference)
├─ ok/          # teisingas sprendimas
└─ README.md
```

---

## Bandymai

### 🟥 `n_nc` – naivus bandymas

**Idėja:**
> „Jei `stack` reikalingas visur – įrašykime jį į `.h` failą.“

**Sprendimas:**
- antraštiniame faile apibrėžiamas globalus masyvas ir kintamasis

**Rezultatas:**
- ❌ **nesikompiliuoja** arba kompiliuojasi priklausomai nuo konteksto

**Problema:**
- `.h` failas nėra realizacijos vieta

---

### 🟥 `w_nl` – multiple definition

**Idėja:**
> „Jei veikia viename faile – veiks ir keliuose.“

**Sprendimas:**
- globalūs kintamieji apibrėžti `.h`
- `.h` įtrauktas į kelis `.c` failus

**Rezultatas:**
- ❌ **nesilink’ina** (`multiple definition of 'stack'`)

**Esminė pamoka:**
- apibrėžimas turi būti **vienas**

---

### 🟥 `w_nl2` – undefined reference

**Idėja:**
> „Naudokime `extern`, tada viskas susitvarkys.“

**Sprendimas:**
- `.h` faile – `extern` deklaracijos
- **nėra** atitinkamo apibrėžimo jokioje `.c` faile

**Rezultatas:**
- ❌ **nesilink’ina** (`undefined reference`)

**Esminė pamoka:**
- `extern` **nekuria objekto**

---

### 🟩 `ok` – teisingas sprendimas

**Idėja:**
> „Antraštė `.h` aprašo, `.c` – realizuoja.“

**Sprendimas:**
- `.h` faile:
  - funkcijų deklaracijos
  - `extern` globalūs objektai
- `.c` faile:
  - vieninteliai globalių objektų apibrėžimai

**Rezultatas:**
- ✅ sukompiliuoja
- ✅ susilink’ina
- ⚠️ realizacija **dar atvira** naudotojui

---

## Kompiliavimas

```sh
gcc stack.c user.c -o use_stack
```

---

## Ką išmokstame šiame etape

- C programa sudaryta iš **atskirai kompiliuojamų vienetų**
- `.h` failai skirti **aprašams**, ne apibrėžimams
- Linkerio klaidos yra:
  - ne sintaksės
  - o **programos struktūros** klaidos

---

## Ryšys su kitais etapais

- Ankstesnis: 02 – Failas ≠ modulis
- Kitas: 04 – Modulis su paslėpta realizacija (`static`)

---

## Analogija su C++

Šis etapas paaiškina, kodėl C++ turi:

```cpp
class Stack;      // deklaracija
class Stack { };  // apibrėžimas
```

ir kodėl `#include` nėra "teksto kopijavimas" konceptualiai.

---

> **Svarbiausia mintis:**  
> *Jei nesupranti, kur ir kiek kartų objektas apibrėžtas – programa jau pažeista.*

