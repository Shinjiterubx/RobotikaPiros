# Robotika 2023 gyakorlat Piros csapat

## Csapattagok

- Kiss Barnabás (DDWUPK)
- Kaponya Martin (SND69K)
- Mihály Balázs (D05D90)
- Szegedi Tamás (BZCMF2)


A csapat választott beadandó feladata egy kaputelefon.
A kaputelefonon egy keypaden beírt 4 számjegyű kóddal vagy egy rfid taggel lehet bejutni, ahol a sikeres azonosítás után egy szervómotorral szimulált ajtónyitás történik.

A bejutás a kaputelefonon a * gomb lenyomása után írt lakáskóddal lehetséges, ezután kéri a jelszót amit, ha sikeresen beütöttünk beenged a rendszer.
Ha kód helyett rfid taggel szeretnénk bejutni, csak érintsük oda az rfid olvasóhoz.

A kaputelefon kijelzőjét egy mozgásérzékelő által érzékelt mozgás hatására megvilágítjuk, mely 5 másodpercenként figyeli, hogy történt-e mozgás.

Amennyiben elrontottuk a kódot, vagy 5 másodpercig nem nyúltunk a kaputelefonhoz úgy a rendszer visszaáll alvó állapotba.

Ha megszeretnénk változtatni a kódot akkor # gomb lenyomása után írt lakáskóddal lehetséges, ezután kéri a jelszót, amit ha sikeresen beütöttünk, új jelszót írhatunk be. Ha a jelszót nem adjuk meg 5 másodpercen belül akkor nem történik módosítás.

A projekthez szükséges alkatrészek:
- Mozgásérzékelő
- RFID olvasó
- Szervómotor
- Kijelző
- IR remote

![Kapcsolási rajz](1.png)
![Bekötési rajz](2.png)
![Állapot átmeneti rajz](3.png)