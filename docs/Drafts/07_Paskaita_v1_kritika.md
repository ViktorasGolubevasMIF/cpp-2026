# Paveldėjimas — kada naudoti ir kada ne

!!! abstract "Apie šį dokumentą"
    Tai **papildoma medžiaga** prie [P07: Paveldėjimas](07_Paskaita_v1.md).
    
    Techninę pusę jau išmokome. Dabar — **projektavimo pusė**:
    kada paveldėjimas yra teisingas sprendimas, o kada jis tampa problema.
    
    Čia rasite:
    
    - **LSP** — Liskov Substitution Principle: ką reiškia "tikras" is-a
    - **Klasikų nuomonės** — Stroustrup, Eckel, Booch, Martin
    - **Anti-pavyzdžiai** — kada paveldėjimas apgauna
    - **Alternatyva** — has-a vietoj is-a

---

## 1. Kas yra "tikras" is-a?

Mes sakome, kad `Circle IS-A Shape`. Bet ar visada galima pasitikėti šia intuicija?

Tarkime, ekspromtu tartas **paveldėjimo** ryšio apibūdinimas:

> "Kažkas **YRA kažkieno rūšis** tik tuomet, kai abstraguodamiesi identifikuojame,
> kad tas kažkas **TURI panašias į kažkieno savybes ir elgseną**."

byloja, kad tai ne tiek **sintaksės** klausimas, kiek — **semantikos** klausimas. Ir čia prasideda problemos.

---

## 2. Liskov Substitution Principle (LSP)

**Barbara Liskov, 1987:**

> "Jei `S` yra `T` potipis, tai kiekvienas `T` tipo objektas gali būti
> pakeistas `S` tipo objektu — nepakeičiant programos teisingumo."

Paprastai: jei rašote `Shape* p = new Circle(...)` — programa turi veikti
**lygiai taip pat gerai** kaip su `Shape* p = new Shape(...)`.

### Klasikinis LSP pažeidimo pavyzdys: `Square IS-A Rectangle`

Matematiškai — teisingai. Kvadratas **yra** stačiakampis.
Programavime — **problema**:

```cpp
class Rectangle {
public:
    virtual void setWidth(double w)  { width = w; }
    virtual void setHeight(double h) { height = h; }
    double area() { return width * height; }
protected:
    double width, height;
};

class Square : public Rectangle {
public:
    void setWidth(double w) override  { width = height = w; }  // ← abi pusės!
    void setHeight(double h) override { width = height = h; }  // ← abi pusės!
};
```

```cpp
// Vartotojas tikisi Rectangle elgsenos:
void resize(Rectangle* r) {
    r->setWidth(4);
    r->setHeight(3);
    assert(r->area() == 12);  // ← su Square: FAIL! area() = 9
}

Rectangle* r = new Square(5);
resize(r);  // ← LSP pažeidimas — Square "apgauna" vartotoją
```

**Išvada:** matematinis "is-a" ≠ programavimo "is-a".
Paveldėjimas teisingas tik kai **elgsena** taip pat paveldima be staigmenų.

---

## 3. Klasikų nuomonės

!!! note "Apibendrinanti frazė"
    **"Inheritance models what something *is*. Composition models what something *does*."**

---

### Grady Booch

> „Paveldėjimas – tai klasifikavimo priemonė: jis sako, kad vienas objektas yra specializuota kito forma. Tačiau geri modeliai remiasi ne paveldėjimo kiekiu, o **semantiniu tikslumu**."

> „Naudok is-a tik tada, kai gali pagrįsti substituciją – kai vieną tipą gali pakeisti kitu nepakeisdamas sistemos elgsenos."

> „Dauguma blogų dizainų atsiranda tada, kai paveldėjimas naudojamas **kodui pernaudoti**, o ne reikšmei modeliuoti."

**Interpretacija:**
- `is-a` = taksonomija (klasifikacija)
- `has-a` = kompozicija (struktūra)
- Klaida: naudoti paveldėjimą kaip "shortcut reuse"

---

### Bruce Eckel

> „Paveldėjimas turėtų būti **paskutinė priemonė**, o ne pirmas pasirinkimas."

> „Jei dvejoji tarp is-a ir has-a, beveik visada rinkis has-a."

> „Kompozicija yra lankstesnė – ji leidžia keisti elgseną **vykdymo metu**, o paveldėjimas ją užfiksuoja **kompiliavimo metu**."

**Interpretacija:**
- `is-a` → naudok tik kai tikrai natūralus santykis
- `has-a` → numatytasis pasirinkimas
- Paveldėjimas = stiprus ryšys (tight coupling)

---

### Bjarne Stroustrup

> „Paveldėjimas yra galingas, bet pavojingas – jis įveda **glaudų ryšį** tarp tipų."

> „Naudok paveldėjimą, kai nori išreikšti sąsają ir bendrą elgseną, ne tik dalintis implementacija."

> „Kompozicija dažnai yra paprastesnė ir saugesnė alternatyva."

**Interpretacija:**
- `is-a` → interfeisas + polimorfizmas
- `has-a` → implementacijos pakartotinis naudojimas be rizikos
- Svarbu: value semantics vs hierarchy complexity

---

### Robert C. Martin (Uncle Bob)

> „Favor composition over inheritance."

> „Inheritance sukuria trapias bazines klases (**fragile base class problem**)."

**Fokusas:**
- `has-a` → mažesnis coupling
- `is-a` → tik kai laikaisi Liskov substitucijos principo

---

### Sandi Metz

> „Paveldėjimas yra **spėjimas apie ateitį**."

> „Kompozicija leidžia atidėti sprendimus."

**Fokusas:**
- Paveldėjimas = ankstyvas įsipareigojimas
- Kompozicija = lankstumas

---

## 4. Kada paveldėjimas TEISINGAS

✅ Kai paveldėtojas **nesulaužo** bazinės klasės elgsenos  
✅ Kai `virtual` metodai perašomi **laikantis** originalaus kontrakto  
✅ Kai hierarchija stabili ir mažai kinta  
✅ Kai polimorfizmas iš tikrųjų reikalingas  

```
Shape → Circle    ✅ Circle nesulaužo Shape elgsenos
Shape → Rectangle ✅ Rectangle nesulaužo Shape elgsenos
```

---

## 5. Kada paveldėjimas KLAIDINGAS

❌ Kai naudojamas tik kodui pakartotinai naudoti (code reuse)  
❌ Kai paveldėtojas **sulaužo** bazinės elgsenos lūkesčius (LSP)  
❌ Kai hierarchija tampa per gili (> 2-3 lygiai)  
❌ Kai "is-a" teisingas tik matematiškai, ne elgsenos požiūriu  

```
Rectangle → Square  ❌ Square sulaužo setWidth/setHeight elgseną
Bird → Penguin      ❌ Penguin negali fly() — LSP pažeidimas
```

---

## 6. Lyginamasis kontekstas: is-a vs has-a

=== "IS-A (paveldėjimas)"

    **Naudok, kai:**

    - „A yra B rūšis" — ir galioja substitucija (LSP)
    - Nori polimorfizmo
    - Santykis **stabilus** ir mažai kinta

    **Pavyzdys:** `Dog is-a Animal`, `Circle is-a Shape`

    **Rizikos:**
    - Trapios hierarchijos
    - Per didelis coupling
    - Sunku keisti vėliau

=== "HAS-A (kompozicija)"

    **Naudok, kai:**

    - „A turi B"
    - Elgsena sudedama iš dalių
    - Nori **lankstumo** — keisti vykdymo metu

    **Pavyzdys:** `Car has-a Engine`, `Room has-a Window`

    **Privalumai:**
    - Lankstumas
    - Mažesnis coupling
    - Lengviau testuoti ir keisti

---

## 7. Alternatyva: has-a vietoj is-a

Kai paveldėjimas netinkamas — dažnai **kompozicija** yra geresnis sprendimas:

```cpp
// ❌ Paveldėjimas — Square sulaužo Rectangle:
class Square : public Rectangle { ... };

// ✅ Kompozicija — Square turi savo logiką:
class Square {
    double side;
public:
    double area() { return side * side; }
    double perimeter() { return 4 * side; }
};
```

**"Prefer composition over inheritance"** — vienas žinomiausių OOP principų
(GoF: Design Patterns, 1994).

---

## 8. Ryšys su polimorfizmu

TODO: papildyti po P08

Paveldėjimas be `virtual` — tai tik kodo struktūrizavimas.
**Tikras** paveldėjimo potencialas atsiskleidžia su polimorfizmu:

```cpp
// Tik tada Shape* tikrai "veikia" skirtingai:
for (Shape* s : shapes) {
    s->area();  // ← kviečia Circle::area() arba Rectangle::area()
                //   (su virtual — P08)
}
```

---

*[LSP]: Liskov Substitution Principle
*[SOLID]: Single responsibility, Open-closed, Liskov substitution, Interface segregation, Dependency inversion
*[GoF]: Gang of Four — Design Patterns autoriai
