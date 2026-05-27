\---

# OOP C++ egzaminas (2026)

## Galutinis kurso vertinimas

|Dalis|Balai|
|-|:-:|
|Pratybų užduotys (be bonus)|≤5|
|Egzamino užduotys (be bonus)|≤5|
|Pratybų bonus|≤1|
|Egzamino bonus?|≤1|
|**Iš viso**|≤12|
|**Galutinis**|≤10|

## Egzamino tikslas

Egzamino tikslas — parodyti, kad gebate perskaityti, suprasti ir parašyti bazinį C++ kodą, taikydami objektinio ir bendrinio (generinio) programavimo sąvokas ir primityvus.

\---

## Egzamino struktūra (100 + 20)

|Dalis|Turinys|Taškai|Užduočių|
|-|-|:-:|:-:|
|I|Testas|~30|8-10|
|II|Kodo rašymas|~30|2|
|III|Kodo analizė|~40|3-4|
|Bonus|Papildoma užduotis|~+20|1|

\---

## I dalis — Testas (30 tšk.)

### Formatas

Trumpi klausimai su pateiktais atsakymų variantais.

> Pažymėkite *vieną arba daugiau teisingų atsakymų* (jei nenurodyta kitaip).

Kai kuriuose klausimuose gali būti pateiktas trumpas kodo fragmentas.

\---

### Tikrina

* bazinių sąvokų supratimą;
* gebėjimą perskaityti trumpą C++ fragmentą;
* gebėjimą atpažinti tinkamą sprendimą.

\---

### Rekomenduojamos kartojimo temos

* `struct` ir `class` — skirtumai, prieigos modifikatoriai
* reikšmė / rodyklė / nuoroda — perdavimo semantika
* `has-a` ir `is-a` — kompozicija vs paveldėjimas
* konstruktoriai / destruktoriai — gyvavimo ciklas, `static` nariai
* inicializavimo sąrašas — `const` nariai, efektyvumas
* `virtual`, `override` — statinis vs dinaminis susiejimas
* baziniai modern C++ elementai — `auto`, range-for, `nullptr`

\---

## II dalis — Kodo rašymas (30 tšk.)

### Formatas

Pateikiamas nepilnas programos fragmentas. `// TODO` stiliumi pažingsniui rašysite kodą, užpildydami tuščias vietas.

Gali būti prašoma:

> Užbaigti klasės apibrėžimą
> 
> Parašyti trūkstamą metodą
> 
> Sukurti objektus ir pademonstruoti funkcionalumą

Užduočių forma analogiška pratybų užduotims (tik mažiau detalizuota).

\---

### Tikrina

* gebėjimą parašyti veikiantį C++ kodą;
* gebėjimą taikyti kurso sąvokas;
* klasės projektavimo pagrindus.

\---

### Rekomenduojamos kartojimo temos

* klasės — nariai, prieigos modifikatoriai, `const` metodai
* parametrinis konstruktorius su inicializavimo sąrašu
* getter / setter
* `static` nariai ir metodai
* kompozicija — sub-objektas kaip narys
* paveldėjimas — konstruktorių grandinė, `override`
* bazinis išimčių naudojimas — `throw`, `try/catch`
* `vector<T>` su rodyklėmis į bazinius objektus

\---

### Užduočių pavyzdžiai

* Klasė su kompozicija (pvz., `Engine` + `Car`)
* Paveldėjimas, `virtual`, `throw`

\---

## III dalis — Kodo analizė (40 tšk.)

### Formatas

Pateikiamas programos arba jos fragmento kodas.

Gali būti prašoma:

> Kas bus išvesta?
> 
> Ar programa kompiliuosis? Ar susilinkins?
> 
> Jei ne — kur problema?
> 
> Kaip būtų galima pataisyti?
> 
> Trumpai pakomentuoti arba pagrįsti atsakymą.

Ne visose užduotyse bus visi šie punktai.

\---

### Tikrina

* gebėjimą skaityti kodą;
* objekto gyvavimo ciklo supratimą;
* vykdymo eigos numatymą;
* gebėjimą pastebėti projektavimo ar realizacijos problemas.

\---

### Rekomenduojamos kartojimo temos

* konstruktoriai / destruktoriai — tvarka, `static` skaitikliai
* objektų kopijavimas — shallow vs deep copy, *Rule of Three*
* kompozicija (*has-a*) ir agregacija — skirtumas, gyvavimo ciklai
* paveldėjimas (*is-a*) — konstruktorių grandinė, *upcasting*
* *object slicing* — kas prarandama ir kodėl
* polimorfizmas — `virtual` metodai, abstrakčios klasės
* statinis vs dinaminis susiejimas (*binding*)
* virtualus destruktorius — kodėl būtinas polimorfizme
* išimtys — `throw`/`try`/`catch`, *stack unwinding*
* RAII principas — ryšys su destruktoriais ir išimtimis

\---

### Užduočių pavyzdžiai (užvadinimai)

* Konstruktoriai, destruktoriai, `static` skaitiklis
* `virtual` ir *static vs dynamic binding*
* `try`/`catch` ir išimtinė (alternatyvi) vykdymo eiga
* *Upcasting* ir *object slicing*
* Konstruktorių grandinė paveldėjime
* *Shallow copy* ir *Rule of Three*
* `throw` ir steko *unwinding* su objektais
* Virtualus destruktorius ir paveldėjimas

\---

## Bonus (+20)

### Formatas

Pasirenkama viena papildoma užduotis.

Tikslas — parodyti gilesnį supratimą, o ne kompensuoti bazinių žinių trūkumą.

\---

### Galimos temos

* RAII — resursų valdymas per konstruktorių/destruktorių
* `unique_ptr` — išmaniosios rodyklės pagrindai
* `template<typename T>` — šablonų sintaksė ir naudojimas

\---

# (Ne)Leidžiama

✓ viena A4 formato atmintinė (abi pusės)

\---

# Vertinimo principai

* vertinama idėja ir argumentavimas;
* daliniai balai galimi;
* smulkios sintaksės klaidos nėra esminės.

\---

# Ko nebus

✗ sintaksės kalimo

✗ retų STL detalių

✗ dirbtinių „trick" klausimų

✗ temų už kurso ribų

\---
