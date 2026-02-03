---

marp: true

---

# Parametrų perdavimo mechanizmai
## **Value** vs. **Pointer** vs. **Reference**

---

## 1. Perdavimas pagal reikšmę (_call/pass by value_)

Tai vienintelis "tikras" C kalbos mechanizmas.

*   Į funkciją perduodamos **kopijos**.
*   Funkcija turi savo lokalius kintamuosius (`temp`, `x`, `y`).
*   Originalūs kintamieji `main` funkcijoje **NĖRA** keičiami.

<div class="admonition note">
Tai saugu, bet neefektyvu dideliems objektams ir netinka, kai norime pakeisti originalą.
</div>

```cpp
void swap_val(int x, int y) { // Gauna KOPIJAS (x=1, y=2)
    int temp = x;
    x = y;
    y = temp;
} // Čia kopijos sunaikinamos. Originalai a ir b nepakito.
```

[👉 Python Tutor](https://pythontutor.com/render.html#code=%23include%20%3Ciostream%3E%0Avoid%20swap_val%28int%20x,%20int%20y%29%20%7B%20//%20Gauna%20KOPIJAS%20%28x%3D1,%20y%3D2%29%0A%20%20%20%20int%20temp%20%3D%20x%3B%0A%20%20%20%20x%20%3D%20y%3B%0A%20%20%20%20y%20%3D%20temp%3B%0A%7D%0Aint%20main%28%29%20%7B%0A%20%20int%20a%3D1,%20b%3D2%3B%0A%20%20swap_val%28a,b%29%3B%0A%20%20/*%20a%3D1,b%3D2%20*/%0A%7D&cumulative=false&curInstr=9&heapPrimitives=nevernest&mode=display&origin=opt-frontend.js&py=cpp_g%2B%2B9.3.0&rawInputLstJSON=%5B%5D&textReferences=false)

---

<div style="page-break-after: always;"></div>

## 2. C rodyklėmis imituojamas perdavimas pagal nuorodą ("_call/pass by reference_")

C kalboje norėdami pakeisti originalą, turime "gudrauti" naudodami adresus.

1.  Parametrai tampa rodyklėmis (`int*`).
2.  Naudojame **išadresavimo** (dereference) operatorių `*`, kad pasiektume reikšmę.
3.  Kviečiant funkciją, siunčiame **adresus** (`&a`).

```cpp
void swap_ptr(int* x, int* y) { // Gauna ADRESUS
    int temp = *x; // Paimk reikšmę iš adreso x
    *x = *y;       // Įrašyk y reikšmę į x adresą
    *y = temp;     // Įrašyk temp į y adresą
}

// Kvietimas: swap_ptr(&a, &b);
```

[👉 Vizualizuoti atmintį (Python Tutor)](https://pythontutor.com/render.html#code=%23include%20%3Ciostream%3E%0Avoid%20swap_blogas%28int%20x,%20int%20y%29%20%7B%20//%20Gauna%20KOPIJAS%20%28x%3D1,%20y%3D2%29%0A%20%20%20%20int%20temp%20%3D%20x%3B%0A%20%20%20%20x%20%3D%20y%3B%0A%20%20%20%20y%20%3D%20temp%3B%0A%7D%0Aint%20main%28%29%20%7B%0A%20%20int%20a%3D1,%20b%3D2%3B%0A%20%20swap_blogas%28a,b%29%3B%0A%20%20/*%20a%3D1,b%3D2%20*/%0A%7D&cumulative=false&curInstr=0&heapPrimitives=nevernest&mode=display&origin=opt-frontend.js&py=cpp_g%2B%2B9.3.0&rawInputLstJSON=%5B%5D&textReferences=false)

---

<div style="page-break-after: always;"></div>

## 3. C++ perdavimas pagal nuorodas (_call/pass by reference_)

C++ įveda **tikrąjį** perdavimą pagal nuorodą.
Nuoroda (`int&`) – tai **pseudonimas** (alias). Tai lyg antras vardas tam pačiam kintamajam.

*   Sintaksė švaresnė (nereikia `*` ir `&` funkcijos viduje).
*   Kompiliatorius užtikrina, kad nuoroda visada į kažką rodytų.

```cpp
void swap_cpp(int& x, int& y) { // x yra 'a' pseudonimas
    int temp = x;
    x = y;         // Keičia patį originalą
    y = temp;
}

// Kvietimas: swap_cpp(a, b);  <- Atrodo paprastai!
```

[👉 Išbandyti kode (Godbolt)](https://godbolt.org/z/K1Tcs6r3h)

---

<div style="page-break-after: always;"></div>

<!-- _class: invert -->

## Apibendrinimas: 3 būdai

| Savybė | By Value (C/C++) | Pointer (C stilius) | Reference (C++ stilius) |
| :--- | :--- | :--- | :--- |
| **Deklaracija** | `void f(int x)` | `void f(int* x)` | `void f(int& x)` |
| **Kvietimas** | `f(a)` | `f(&a)` | `f(a)` |
| **Veiksmas** | Dirba su kopija | Dirba su adresu | Dirba su originalu |
| **Sintaksė** | Paprasta | Sudėtinga (`*`, `&`) | Paprasta |
| **Verdiktas** | Saugus, bet "lokalus" | Galingas, bet pavojingas | **Modernus standartas** |

```cpp
// Tik C++ leidžia rašyti taip švariai:
int main() {
    int a=1, b=2;
    swap_cpp(a, b); // a=2, b=1
}

```

