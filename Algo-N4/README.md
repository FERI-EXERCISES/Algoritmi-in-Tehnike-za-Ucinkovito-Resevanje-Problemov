# Vaja 4: Pirati (2023/2024)

Implementirajte rešitev podane naloge z dinamičnim programiranjem. Program naj prebere vhodne podatke iz datoteke, rezultat pa izpiše v konzolo. Oddajte ZIP datoteko izvorne kode (brez direktorijev bin, obj, Debug, Release, itd...).

Primer zagona programa: ./vaja4.exe testni_primer1.txt

Dovoljeni programski jeziki: C++, Python.
(C++ implementacije bodo prevedene in preizkušene v Visual Studiu 2022, Python implementacije pa pognane s Python različico >= 3.8)

Vrednost vaje: 15 točk.

Rok za oddajo: 19. 4. 2024 ob 6h zjutraj, rok za zagovor: 19. 4. 2024.
(Po roku za oddajo bodo avtomatsko vnešene ocene, če vaša naloga deluje vsaj za oba testna primera 1 in 2 v celoti. V nasprotnem primeru morate zagovarjati nalogo!)

Pravilni rezultati testnih primerov:
- testni_primer1: 6 14 20 32 44 56 69 82 95 110 140 195
- testni_primer2: 5 10 24 38 51 71 94 118 170 210 254 309 364 469 569 674 788 921
- testni_primer3: rešitev na voljo v priponki

Opomba:
- Pri tej nalogi priporočamo implementacijo v jeziku C++.
- Za izračun rešitve 3. testnega primera znotraj časovne omejitve boste morali samostojno dodatno implementirati dinamično programiranje za premikanje med otoki v vrstici. (NAMIG: ustvarite si ločene tabele (slične RESULT in A) in izvedite tabeliranje izračunov minimalnih časov za obisk [0, ..., N] otokov zgolj znotraj i-te vrstice (N = št. otokov znotraj i-te vrstice). Ta postopek je sličen postopku reševanja zgleda v priloženem PDF-u. Shranjene vrednosti v novo ustvarjenih tabelah v i-ti vrstici nato uporabite za končne posodobitve izračunov v glavnih tabelah RESULT in A.)
- Celotna implementacija z grobo silo ali 'hardcode'-ana rešitev dinamičnega programiranja bo ocenjena z 0 točkami.