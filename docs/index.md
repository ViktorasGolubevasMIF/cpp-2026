# Objektinis programavimas C++

Informatikos studijų programos 1 kurso dalykas (VU MIF). Kurse pereinama nuo procedūrinio C kodo prie abstrakčiųjų duomenų tipų ir objektinio programavimo. Kiekvienas naujas C++ mechanizmas atsiranda kaip atsakas į konkrečią ankstesnio sprendimo problemą: kompiliavimo ar susiejimo klaidą, atminties nuotėkį, projektavimo ribotumą.

## Kodėl toks kursas

Kursas parengtas iš naujo 2025–2026 m. m. Ankstesnė dalyko medžiaga siekė 2004 metus ir ilgą laiką nebuvo atnaujinama, o per tą laiką iš esmės pasikeitė ir pati C++ kalba (C++11 ir vėlesni standartai), ir programavimo praktika.

Naujasis kursas grindžiamas **evoliuciniu** principu: kalbos konstrukcijos nepateikiamos kaip sąrašas, kurį reikia išmokti. Kiekviena jų atsiranda tada, kai ankstesnis sprendimas atsiremia į savo ribas. Pavyzdžiui, `Stack` duomenų struktūra kurso pradžioje parašoma kaip monolitinė C programa, o vėliau žingsnis po žingsnio virsta moduliu, abstrakčiuoju duomenų tipu ir galiausiai C++ klase. Studentai patys pamato, kokią problemą sprendžia kiekvienas žingsnis.

!!! tip "Klaidos kaip mokymosi priemonė"
    Kompiliavimo klaidos, susiejimo nesėkmės ir dvigubas atminties atlaisvinimas čia nėra nesėkmės. Jos rodo, kodėl reikia geresnės abstrakcijos. Kas savo rankomis yra pamiršęs `delete`, tas supranta, kodėl egzistuoja RAII.

!!! note "Modern C++, bet ne technikų gausa"
    Kurse naudojamas šiuolaikinis C++: RAII, išmaniosios rodyklės, išimtys, šablonai. Tačiau tikslas yra ne aprėpti kuo daugiau kalbos galimybių ir bibliotekų, o išmokti rašyti **teisingą ir patikimą** kodą: aiškiai žinoti, kas valdo resursą, kas vyksta kopijuojant objektą ir kodėl bazinei klasei reikalingas virtualusis destruktorius. Standartinės bibliotekos algoritmai, projektavimo šablonai ir pažangesnės technikos šiame kurse neaprėpiami arba pateikiami tik papildomoje medžiagoje.

## Kurso struktūra

| Etapas | Paskaitos | Pratybos |
|---|---|---|
| **Nuo C link C++.** Parametrų perdavimas, `Stack` ADT evoliucija nuo monolitinio C iki C++ klasės | [P01](Paskaitos/01/Parameters_Passing.md), [P02–03](Paskaitos/02/02_Paskaita_Stack_Evolution_C_01_09.md) | [U1](Pratybos/Užduotys/U1.md) |
| **Klasė ir objektas.** Inkapsuliacija, kompozicija, RAII, objektų kopijavimas ir Trijų taisyklė | [P04](Paskaitos/04/04_Paskaita_v1.md), [P05](Paskaitos/05/05_Paskaita_v1.md) | [U2](Pratybos/Užduotys/U2_v2.md), [U3](Pratybos/Užduotys/U3.md), [U4](Pratybos/Užduotys/U4.md) |
| **Paveldėjimas ir polimorfizmas.** Operatorių perkrovimas, `is-a` ryšys, virtualieji metodai | [P06](Paskaitos/06/06_Paskaita_v1.md), [P07](Paskaitos/07/07_Paskaita_v2.md), [P08](Paskaitos/08/08_Paskaita_v2.md) | [U5](Pratybos/Užduotys/U5.md), [U6](Pratybos/Užduotys/U6.md) |
| **Patikimas ir apibendrintas kodas.** Išimtys, išmaniosios rodyklės, šablonai | [P09](Paskaitos/09/09_Paskaita_v3.md), [P10](Paskaitos/10/10_Paskaita_v2.md), [P11](Paskaitos/11/11_Paskaita_v2.md) | [U7](Pratybos/Užduotys/U7_v2.md), [U8](Pratybos/Užduotys/U8_v2.md) |

Prie kai kurių paskaitų pateikiama papildoma (*papildomai*) gilesnio lygio medžiaga. Ji neprivaloma ir egzamine nevertinama.

## Kaip dirbame

* **Pratybų užduotys** atliekamos ir pateikiamos fakulteto GitLab platformoje. Darbo tvarka aprašyta [užduočių atlikimo gide](Pratybos/Tvarka/UzduociuGidas_1_1.md).
* **Virtuali mokymosi aplinka (VMA)** naudojama registracijai, pranešimams ir vertinimams.
* **Egzaminui** pasiruošti padės [egzamino struktūros aprašas](Egzaminas/OP_CPP_Egzamino_struktura_v4.md) ir [pasirengimo atmintinė](Egzaminas/Pasirengimo_atmintine_v2.md).

## Naudingi įrankiai

| Įrankis | Kam naudojamas |
|---|---|
| [Compiler Explorer](https://godbolt.org/) | Kodo kompiliavimas naršyklėje ir sugeneruoto mašininio kodo peržiūra |
| [C++ Insights](https://cppinsights.io/) | Parodo, ką kompiliatorius „prirašo“ už jus: konstruktorius, konversijas, šablonų išskleidimą |
| [Python Tutor](https://pythontutor.com/cpp.html) | Steko, krūvos ir rodyklių vizualizacija vykdant programą žingsnis po žingsnio |
| [cppreference.com](https://en.cppreference.com/w/) | Kalbos ir standartinės bibliotekos žinynas |

## Atviras kursas

Visa kurso medžiaga yra atvira ir prieinama be prisijungimo. Atvira ir jos [GitHub repozitorija](https://github.com/ViktorasGolubevasMIF/cpp-2026). Joje saugomi ne tik tekstai, bet ir visi kodo pavyzdžiai (aplankas `code/`). Paskaitose matomas kodas įterpiamas tiesiai iš repozitorijos failų, todėl tekstas ir veikiantis kodas nesiskiria. Pavyzdžius galima atsisiųsti, kompiliuoti ir keisti patiems.

Svetainė sukurta su [MkDocs Material](https://squidfunk.github.io/mkdocs-material/) ir talpinama GitHub Pages. Kiekvienas medžiagos pakeitimas repozitorijoje automatiškai publikuojamas (GitHub Actions), todėl svetainėje visada matoma naujausia versija, o visa pakeitimų istorija išlieka repozitorijoje.
