# Kompozicija „ant pirštų"
## Objektų ryšiai/santykiai: Asociacija, Agregacija, Kompozicija

!!! abstract "Šios paskaitos tikslas"
    Suprasti kaip C++ objektai gali **turėti** vienas kitą arba **žinoti** vienas apie kitą.  
    Naudosime `Student` ir `Course` klases — paprastus, bet realistiškus pavyzdžius.

    Pagrindiniai klausimai:
    
    - Kuo skiriasi **asociacija**, **agregacija** ir **kompozicija**?
    - Kas valdo objekto **gyvavimo ciklą** (_lifecycle_)?
    - Kas nutinka destruktoriuje — kas **atsako** už sunaikinimą?

    > **Kompiliavimo klaidos ir netikėtos išvestys — tai mokymosi įrankis!**

---

## 🗺️ Didysis paveikslas

Prieš rašant kodą — svarbu suprasti **tris santykių tipus**:

```
Santykių STIPRUMAS (nuo silpniausio iki stipriausio):

  Asociacija  ──►       Agregacija        ──►      Kompozicija
   „žino"         „naudoja/turi (nesavą)"        „talpina/valdo (savą)"
```

??? tip "STIPRUMO Vizualizacija"
    ```mermaid
    flowchart LR
        A[Asociacija] --> B[Agregacija]
        B --> C[Kompozicija]
        
        subgraph A
            A1[const T&]
        end
        subgraph B
            B1[T*]
        end
        subgraph C
            C1[vector<T>]
        end
    ```

??? tip "STIPRUMO Vizualizacija 2"
    ```mermaid
    flowchart LR
        A["Asociacija<br/><code>const T&</code><br/>parametras"] 
        -->|"silpnesnis"| B["Agregacija<br/><code>T*</code><br/>rodyklė"]
        -->|"stipresnis"| C["Kompozicija<br/><code>vector&lt;T&gt;</code><br/>tiesioginis narys"]
        
        style A fill:#e1f5fe,stroke:#01579b,stroke-width:2px
        style B fill:#fff3e0,stroke:#e65100,stroke-width:2px
        style C fill:#ffebee,stroke:#b71c1c,stroke-width:2px
    ```

---

| Kriterijus | Asociacija | Agregacija | Kompozicija |
|---|---|---|---|
| **Ryšys** | „žino apie" | „naudoja/turi (nesavą)" | „talpina/valdo (savą)" |
| **Gyvavimas** | Nepriklausomas | Nepriklausomas | Priklausomas |
| **Savininkas** | Nėra | Išorinis | Pats objektas |
| **C++ tipas** | `&` parametras | `T*` narys | `T` narys |
| **Destruktorius** | Nieko nedaro | `delete` **ne** | `delete` **taip** (arba auto) |
| **UML** | `──────►` | `◇────►` | `◆────►` |

??? tip "UML Vizualizacija"
    ```mermaid
    classDiagram
    class Destytojas
    class Studentas
    class Universitetas
    class Kursas
    
    Destytojas ..> Studentas : <code>(const Studentas&)</code><br/>parametras
    Universitetas o-- Studentas : <code>Studentas*</code>
    Kursas *-- Studentas : <code>vector<Studentas></code>
    
    note for Destytojas "Asociacija<br/>Žino apie<br/>nekuria ir nenaikina"
    note for Universitetas "Agregacija<br/>Naudoja (nesavą)<br/>neatsako už gyvavimą"
    note for Kursas "Kompozicija<br/>Valdo (savą)<br/>sukuria ir sunaikina"    
    ```

??? tip "UML Vizualizacija 2"
    ```mermaid
    classDiagram
    class Destytojas
    class Studentas
    class Universitetas
    class Kursas
    
    Destytojas ..> Studentas : asociacija\n(const T&)
    Universitetas o-- Studentas : agregacija\n(T*)
    Kursas *-- Studentas : kompozicija\n(vector<T>)
    
    note for Destytojas "Žino apie\nnekuria/nenaikina"
    note for Universitetas "Turi (nesavą)\nneatsako už gyvavimą"
    note for Kursas "Valdo (savą)\nsukuria ir sunaikina"    
    ```

---

## 1️⃣ Asociacija — „žino apie"

!!! note "Kontekstas"
    Viena klasė **naudoja** kitą, bet **nevaldo** jos gyvavimo ciklo.  
    Paprasčiausias ir laisvasis ryšys — tiesiog parametras arba nuoroda.

### Pavyzdys: Dėstytojas ir Studentas

=== "asociacija.cpp"
    ```cpp linenums="1" hl_lines="17-19"
    #include <iostream>
    #include <string>

    class Studentas {
    public:
        std::string vardas;
        Studentas(const std::string& v) : vardas(v) {
            std::cout << "Studentas '" << vardas << "' sukurtas\n";
        }
        ~Studentas() {
            std::cout << "Studentas '" << vardas << "' sunaikintas\n";
        }
    };

    class Destytojas {
        std::string vardas;
    public:
        Destytojas(const std::string& v) : vardas(v) {}

        // Asociacija: metodas GAUNA Studentas& — bet jo nekuria, nenaikina!
        void patikrintiEgzamina(const Studentas& s) const {
            std::cout << vardas << " tikrina " << s.vardas << " egzaminą\n";
        }
    };

    int main() {
        Studentas s("Jonas");       // Jonas gyvas čia
        Destytojas d("Dr. Smith");

        d.patikrintiEgzamina(s);    // Dėstytojas „žino" apie Joną
                                    // bet jo nevaldo!
        return 0;
    }
    ```

=== "🖥️"
    ```
    Studentas 'Jonas' sukurtas
    Dr. Smith tikrina Jonas egzaminą
    Studentas 'Jonas' sunaikintas
    ```

---

#### ✅ Veikia! Ir...

???+ success "Savybės"
    - `Destytojas` **neturi** `Studentas` kaip nario
    - `Studentas` egzistuoja **nepriklausomai** nuo `Destytojas`
    - Jei `Destytojas` dings — `Studentas` **išgyvena**
    - Dėstytojas gauna studentą per parametrą — tik **laikinai naudoja**

???+ info "Atpažinimo ženklas kode"
    ```cpp
    void patikrintiEgzamina(const Studentas& s);  // ← & parametras = asociacija
    ```
    Nėra `Studentas` kaip klasės nario. Tik metodų parametruose.

??? note "📌 Priminimas: ką reiškia `const Studentas& s`?"
    Šis užrašas gali atrodyti neįprastai — bet jis labai dažnas C++ kode ir verta jį atpažinti.

    ```cpp
    void patikrintiEgzamina(const Studentas& s)
    //                       ^^^^^ ^^^^^^^^^ ^
    //                       nekeisti  tipas  nuoroda (ne kopija)
    ```

    - **`&`** — parametras perduodamas **nuoroda** (_reference_): objektas **nekopijuojamas**,
      metodas dirba su originaliu objektu tiesiogiai
    - **`const`** — metodas **žada nekeisti** gauto objekto; tai apsauga ir aiški intencija

    Tai „universalus" C++ sprendimas perduodant objektus į funkcijas/metodus:
    efektyvu (be kopijos) ir saugu (negalima netyčia pakeisti).

    !!! tip "Išsamiau"
        Nuorodos, `const` taisyklingumo (_const correctness_) ir susijusios temos
        nagrinėjamos atskirai — skyriuje **„Geresnis C"** (_Better C_).

---

## 2️⃣ Agregacija — „naudoja/turi (nesavą)"

!!! note "Kontekstas"
    Klasė **saugo rodyklę** į kitą objektą, bet **neatsako** už jo sunaikinimą.  
    Komponentas gali egzistuoti ir be savininko.

### Pavyzdys: Universitetas ir Studentai

=== "agregacija.cpp"
    ```cpp linenums="1" hl_lines="22-23 27"
    #include <iostream>
    #include <string>

    class Studentas {
    public:
        std::string vardas;
        Studentas(const std::string& v) : vardas(v) {
            std::cout << "Studentas '" << vardas << "' sukurtas\n";
        }
        ~Studentas() {
            std::cout << "Studentas '" << vardas << "' sunaikintas\n";
        }
    };

    class Universitetas {
        std::string pavadinimas;
        Studentas* studentas;   // rodyklė — ne tiesioginis narys!
    public:
        // Universitetas NEGAUNA studentų nuosavybės — tik naudoja
        Universitetas(const std::string& p, Studentas* s)
            : pavadinimas(p), studentas(s) {
            std::cout << "Universitetas '" << pavadinimas << "' sukurtas\n";
        }
        ~Universitetas() {
            std::cout << "Universitetas '" << pavadinimas
                      << "' sunaikintas (studento NENAIKINA)\n";
            // studentas čia NETRINAME — jis ne mūsų!
        }
        void registruoti() const {
            std::cout << studentas->vardas
                      << " registruotas " << pavadinimas << "\n";
        }
    };

    int main() {
        Studentas* s = new Studentas("Jonas");  // Jonas gyvas nepriklausomai

        {
            Universitetas u("VU", s);   // U naudoja Joną
            u.registruoti();
        }   // ← Universitetas sunaikintas, BET Jonas gyvas!

        std::cout << "Jonas vis dar egzistuoja: " << s->vardas << "\n";
        delete s;   // Jono savininkas — main()!
        return 0;
    }
    ```

=== "🖥️"
    ```
    Studentas 'Jonas' sukurtas
    Universitetas 'VU' sukurtas
    Jonas registruotas VU
    Universitetas 'VU' sunaikintas (studento NENAIKINA)
    Jonas vis dar egzistuoja: Jonas
    Studentas 'Jonas' sunaikintas
    ```

---

#### ✅ Veikia! Ir...

???+ success "Savybės"
    - `Universitetas` saugo `Studentas*` — **rodyklę**, ne objektą
    - `Studentas` sukuriamas **išorėje** (`main`) — ne universiteto viduje
    - Destruktorius **nedelete** studento
    - Jei universitetas užsidaro — studentai **neišnyksta** (realistiškai! ✓)

???+ info "Atpažinimo ženklas kode"
    ```cpp
    Studentas* studentas;   // ← rodyklė = agregacija
    ```
    Rodyklė saugoma kaip narys, bet **konstruktorius gauna jau sukurtą** objektą.

---

### ⚠️ Klasikinė klaida agregacijoje

> `agregacija_RT.cpp`

!!! quote "Sumanymas/ketinimas"
    „Jei turiu rodyklę, tai destruktoriuje reikia `delete`..." — **KLAIDINGA!**  
    Šiuo atveju — tas pats objektas ištrinamas **du kartus**.

=== "agregacija_RT.cpp"
    ```cpp linenums="1" hl_lines="9 22 27"
    #include <iostream>
    #include <string>

    class Studentas {
    public:
        std::string vardas;
        Studentas(const std::string& v) : vardas(v) {
            std::cout << "Studentas '" << vardas << "' sukurtas\n";
        }
        ~Studentas() {
            std::cout << "Studentas '" << vardas << "' sunaikintas\n";
        }
    };

    class Universitetas {
        std::string pavadinimas;
        Studentas* studentas;
    public:
        Universitetas(const std::string& p, Studentas* s)
            : pavadinimas(p), studentas(s) {}

        ~Universitetas() {
            delete studentas;  // ← KLAIDA: mes ne savininkai!
        }
    };

    int main() {
        Studentas* s = new Studentas("Jonas");  // savininkas — main()

        {
            Universitetas u("VU", s);   // u gauna rodyklę — ne nuosavybę!
        }   // ← u destruktorius padaro delete s  (1-as kartas)

        delete s;   // ← main() irgi delete  (2-as kartas!) → CRASH
        return 0;
    }
    ```

=== 🖥️❌ Runtime Error"
    ```
    Studentas 'Jonas' sukurtas
    Studentas 'Jonas' sunaikintas
    free(): double free detected in tcache 2
    Aborted (core dumped)
    ```

---

#### ❌ Runtime klaida!

??? bug "RT: double free!"
    `delete` iškviestas **du kartus** ant to paties atminties adreso:  
    pirmą kartą — `Universitetas` destruktoriuje, antrą — `main()`.  
    Tai **neapibrėžtas elgesys** (_undefined behavior_) — programa gali griūti, tyliai sugadinti atmintį arba elgtis atsitiktinai.

??? danger "Taisyklė"
    **Kas sukūrė su `new` — tas ir turi `delete`.**  
    Agregacijoje savininkas yra **išorinis** kodas — klasė tik **naudoja**, nevaldo.

---

## 3️⃣ Kompozicija — „talpina/valdo (savą)"

!!! note "Kontekstas"
    Klasė turi kitą kaip **tiesioginį narį** — sudėtinę dalį.  
    Komponentas **negali egzistuoti** be savininko.  
    Gyvavimo ciklą valdo savininkas — **automatiškai**.

??? note "📖 Oficialus apibrėžimas: „has-a" ryšys"
    Kompozicija OOP literatūroje aprašoma per **„has-a"** (_turi_) ryšį:

    > *„A **has a** B"* — A klasė **turi** B kaip neatsiejamą dalį;
    > B egzistuoja tik A kontekste ir kartu su juo sunaikinama.

    Pavyzdžiui: *„Kursas **has a** Studentų sąrašą"*,
    *„Automobilis **has a** Variklį"*.

    Tai vienas iš pagrindinių OOP projektavimo principų, skiriamas nuo
    **„is-a"** (_paveldėjimo_) ryšio — kurį nagrinėsime 10-oje paskaitoje.

    Išsamiau: [cppreference — OOP](https://en.cppreference.com/w/) arba
    klasikinis šaltinis: *„Object-Oriented Analysis and Design"* (Grady Booch).

### Pavyzdys: Kursas ir Studentai

=== "kompozicija.cpp"
    ```cpp linenums="1" hl_lines="16 20-23 27"
    #include <iostream>
    #include <string>
    #include <vector>

    class Studentas {
    public:
        std::string vardas;
        Studentas() : vardas("(tuščias)") {}
        Studentas(const std::string& v) : vardas(v) {
            std::cout << "Studentas '" << vardas << "' sukurtas\n";
        }
        ~Studentas() {
            std::cout << "Studentas '" << vardas << "' sunaikintas\n";
        }
    };

    class Kursas {
        std::string pavadinimas;
        std::vector<Studentas> studentai;   // TIESIOGINIS narys — ne rodyklė!
    public:
        // Kompozicija: Kursas SUKURIA ir VALDO studentus
        Kursas(const std::string& p) : pavadinimas(p) {
            std::cout << "Kursas '" << pavadinimas << "' sukurtas\n";
        }
        ~Kursas() {
            std::cout << "Kursas '" << pavadinimas << "' sunaikintas\n";
            // studentai sunaikinami AUTOMATIŠKAI — tai mūsų dalis!
        }
        void prideti(const Studentas& s) {
            studentai.push_back(s);
        }
        void spausdinti() const {
            std::cout << "Kursas: " << pavadinimas << "\n";
            for (const auto& s : studentai)
                std::cout << "  - " << s.vardas << "\n";
        }
    };

    int main() {
        {
            Kursas k("OOP C++");
            k.prideti(Studentas("Jonas"));
            k.prideti(Studentas("Marta"));
            k.spausdinti();
        }   // ← Kursas sunaikintas + VISI studentai sunaikinami kartu!

        std::cout << "--- Po bloko ---\n";
        return 0;
    }
    ```

=== "🖥️"
    ```
    Kursas 'OOP C++' sukurtas
    Studentas 'Jonas' sukurtas
    Studentas 'Jonas' sunaikintas
    Studentas 'Marta' sukurtas
    Studentas 'Jonas' sunaikintas
    Studentas 'Marta' sunaikintas
    Kursas: OOP C++
      - Jonas
      - Marta
    Kursas 'OOP C++' sunaikintas
    Studentas 'Jonas' sunaikintas
    Studentas 'Marta' sunaikintas
    --- Po bloko ---
    ```

---

#### ✅ Veikia! Ir...

???+ success "Savybės"
    - `Kursas` saugo `std::vector<Studentas>` — **tiesioginis narys**
    - Studentai sukuriami ir **priklausomi** nuo kurso
    - Destruktorius sunaikina studentus **automatiškai** (per `vector`)
    - Studentai **negali egzistuoti** be kurso — tai kurso dalis

???+ info "Atpažinimo ženklas kode"
    ```cpp
    std::vector<Studentas> studentai;   // ← tiesioginis narys = kompozicija
    ```
    Ne rodyklė — **pats objektas** (arba konteineris objektų) kaip narys.

??? info "O kodėl ne fiksuotas masyvas?"
    Techniškai galima ir taip — `Studentas studentai[100]` irgi būtų kompozicija:
    komponentai yra tiesioginiai nariai, sunaikinami kartu su savininku.

    Tačiau `std::vector<Studentas>` yra patogiau dėl dinaminio dydžio.  
    Svarbiausia — `vector` pats yra RAII konteineris: jis **valdo savo elementų
    gyvavimo ciklą** lygiai taip pat kaip šiame pavyzdyje `Kursas` valdo studentus.

    !!! warning "Reikalavimas `Studentas` klasei"
        `std::vector` viduje kopijuoja elementus (pvz., per `push_back`).  
        Todėl `Studentas` turi turėti **kopijavimo konstruktorių** (_copy constructor_).  
        Šiame pavyzdyje kompiliatorius jį sugeneruoja automatiškai — bet tai **užbėgimas į priekį**: 7-oje paskaitoje pamatysime, kada to nepakanka ir reikia rašyti patiems.

---

??? tip "SVARBU!: kodėl protokole 2 konstruktoriai, bet 5 destruktoriai?"

    Paleiskite šį kodą ir suskaičiuokite konstruktorius ir destruktorius:

    ```cpp linenums="1"
    #include <iostream>
    #include <string>
    #include <vector>

    class Studentas {
    public:
        std::string vardas;
        Studentas(const std::string& v) : vardas(v) {
            std::cout << "[CTOR]  Studentas '" << vardas << "' sukurtas\n";
        }
        ~Studentas() {
            std::cout << "[DTOR]  Studentas '" << vardas << "' sunaikintas\n";
        }
    };

    int main() {
        std::vector<Studentas> studentai;
        studentai.push_back(Studentas("Jonas"));
        studentai.push_back(Studentas("Marta"));
        return 0;
    }
    ```

    === "🖥️ (tikėtasi)"
        ```
        [CTOR]  Studentas 'Jonas' sukurtas
        [CTOR]  Studentas 'Marta' sukurtas
        [DTOR]  Studentas 'Marta' sunaikintas
        [DTOR]  Studentas 'Jonas' sunaikintas
        ```

    === "🖥️ (realybė)"
        ```
        [CTOR]  Studentas 'Jonas' sukurtas
        [DTOR]  Studentas 'Jonas' sunaikintas
        [CTOR]  Studentas 'Marta' sukurtas
        [DTOR]  Studentas 'Jonas' sunaikintas
        [DTOR]  Studentas 'Marta' sunaikintas
        [DTOR]  Studentas 'Jonas' sunaikintas
        [DTOR]  Studentas 'Marta' sunaikintas
        ```

    **2 konstruktorių iškvietimas, bet 5 destruktoriai!** Kur dingo kiti?

    Atsakymas: `push_back` **kopijuoja** objektą į `vector` vidų.  
    Kopijavimas naudoja **kopijavimo konstruktorių** (_copy constructor_) —  
    bet mes jo neužregistravome su `std::cout`, tad jo iškvietimų **nematome**.

    Norėdami pamatyti visą paveikslą, reikia pridėti copy constructor su logging'u:

    ```cpp linenums="1" hl_lines="10-13"
    class Studentas {
    public:
        std::string vardas;
        Studentas(const std::string& v) : vardas(v) {
            std::cout << "[CTOR]  Studentas '" << vardas << "' sukurtas\n";
        }
        ~Studentas() {
            std::cout << "[DTOR]  Studentas '" << vardas << "' sunaikintas\n";
        }
        // Kopijavimo konstruktorius — kompiliatorius jį kuria automatiškai,
        // bet tada jo nematome protokole!
        Studentas(const Studentas& kitas) : vardas(kitas.vardas) {
            std::cout << "[COPY]  Studentas '" << vardas << "' nukopijuotas\n";
        }
    };
    ```

    === "🖥️ su [COPY]"
    ```
        [CTOR]  Studentas 'Jonas' sukurtas
        [COPY]  Studentas 'Jonas' nukopijuotas   ← push_back kopijuoja!
        [DTOR]  Studentas 'Jonas' sunaikintas    ← laikinas objektas ištrinamas
        [CTOR]  Studentas 'Marta' sukurtas
        [COPY]  Studentas 'Marta' nukopijuotas   ← push_back kopijuoja!
        [COPY]  Studentas 'Jonas' nukopijuotas   ← vector perskirsto atmintį!
        [DTOR]  Studentas 'Jonas' sunaikintas    ← senas Jonas (prieš realokaciją)
        [DTOR]  Studentas 'Marta' sunaikintas    ← laikinas objektas ištrinamas
        [DTOR]  Studentas 'Jonas' sunaikintas    ← vector išvalomas
        [DTOR]  Studentas 'Marta' sunaikintas    ← vector išvalomas
    ```

    Dabar protokolas pilnas — **kiekvienas sukurtas objektas turi lygiai vieną destruktorių**:
    2 `[CTOR]` + 3 `[COPY]` = 5 sukūrimų → 5 `[DTOR]` sunaikinimų. ✓

    !!! note "Ką tai reiškia?"
        - `push_back` **kopijuoja** elementą — ne „perkelia"
        - `vector` gali **perskirsti atmintį** ir kopijuoti **visus** esamus elementus
        - Copy constructor yra **būtinas** kai objektai keliauja į konteinerius
        - **7-oje paskaitoje** išmoksime rašyti copy constructor teisingai  
          (ir 13-oje — kaip to išvengti su `std::move`)

---

## 🔬 Gyvavimo ciklo palyginimas

Kas **sukuria** ir kas **sunaikina** — tai ir nustato santykio tipą.

=== "Asociacija"
    ```cpp
    // Tiek sukuria, tiek sunaikina — IŠORINIS kodas
    Studentas s("Jonas");
    Destytojas d("Smith");
    d.patikrintiEgzamina(s);   // d tik naudoja s — nieko nekuria/nenaikina
    ```

=== "Agregacija"
    ```cpp
    // Sukuria — IŠORINIS kodas
    Studentas* s = new Studentas("Jonas");
    Universitetas u("VU", s);
    // Sunaikina — IRGI IŠORINIS kodas
    delete s;   // u destruktorius to nedaro!
    ```

=== "Kompozicija"
    ```cpp
    {
        Kursas k("OOP");
        k.prideti(Studentas("Jonas"));
        // Sukuria — KURSAS (per vector::push_back)
        // Sunaikina — KURSAS (automatiškai destruktoriuje)
    }   // ← Jonas sunaikinamas čia, kartu su kursu
    ```

---

## 🧠 Kaip atsiminti?

Pagalvokite apie **realaus gyvenimo analogijas**:

| Analogija | Tipas | Kodėl |
|---|---|---|
| 🤝 Dėstytojas ir Studentas | **Asociacija** | Abu egzistuoja nepriklausomai |
| 🏛️ Universitetas ir Studentai | **Agregacija** | Universitetas „turi" studentų, bet jei užsidaro — studentai neišnyksta |
| ❤️ Žmogus ir Širdis | **Kompozicija** | Širdis negali egzistuoti be žmogaus — tai neatsiejama dalis |
| 📚 Knyga ir Puslapiai | **Kompozicija** | Puslapiai neegzistuoja be knygos |
| 🚗 Garažas ir Automobilis | **Agregacija** | Automobilis gali išvažiuoti ir egzistuoti be garažo |

---

## ⚡ Greitoji santrauka

```
Asociacija:   Destytojas ──────► Studentas
              metodas gauna & parametrą, nieko nevaldo

Agregacija:   Universitetas ◇───► Studentas*
              naudoja/turi (nesavą) — NEKURIA, NENAIKINA

Kompozicija:  Kursas ◆───► [Studentas]
              talpina/valdo (savą) — SUKURIA ir SUNAIKINA
```

!!! danger "Trys taisyklės"
    1. **Asociacija** — jei tik metodų parametruose: `const T&` arba `T*`
    2. **Agregacija** — jei narys yra `T*`, bet **konstruktorius gauna** jau sukurtą objektą
    3. **Kompozicija** — jei narys yra `T` arba `std::vector<T>` — **savininkas talpina ir valdo viską**

---

## 🏋️ Užduotys

!!! example "Užduotis 1 — Atpažinkite tipą"
    Pažiūrėkite į šį kodą ir pasakykite: asociacija, agregacija ar kompozicija?

    ```cpp
    class Biblioteka {
        Knyga* knyga;
    public:
        Biblioteka(Knyga* k) : knyga(k) {}
        ~Biblioteka() { /* nieko */ }
    };
    ```
    
    ??? success "Atsakymas"
        **Agregacija** — saugoma rodyklė `Knyga*`, konstruktorius gauna jau sukurtą objektą, destruktorius nieko nenaikina.

!!! example "Užduotis 2 — Atpažinkite tipą"

    ```cpp
    class Automobilis {
        Variklis variklis;   // int galia;
    public:
        Automobilis(int galia) : variklis(galia) {}
    };
    ```

    ??? success "Atsakymas"
        **Kompozicija** — `Variklis` yra tiesioginis narys (ne rodyklė). Automobilis sukuria variklį ir automatiškai sunaikina.

!!! example "Užduotis 3 — Sukurkite"
    Sukurkite `Grupe` klasę, kuri:

    - Turi **kompoziciškai** `std::vector<Studentas>` narius
    - Turi metodą `void priimti(const std::string& vardas)` — sukuria naują studentą
    - Turi metodą `void spausdinti()` — išveda visus studentus
    - Destruktoriuje **nieko rašyti nereikia** — kodėl?

---

*[ADT]: Abstract Data Type — Abstraktus Duomenų Tipas
*[NC]: Not Compiling — Nesikompiliuoja
*[NL]: Not Linking — Nesulinkuoja
*[RT]: Runtime Error — Vykdymo klaida