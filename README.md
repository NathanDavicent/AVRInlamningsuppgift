# AVRInlamningsuppgift

# Reklamskylt (AVR Projekt)

## Beskrivning

Detta projekt är en enkel reklamskylt byggd i C för en AVR-mikrokontroller (t.ex. ATmega328P).
Programmet visar olika reklammeddelanden på en 16x2 LCD.

Meddelandena visas baserat på hur mycket varje kund har betalat, där högre betalning ger större chans att visas.

## Funktioner

* Slumpmässigt val av kund baserat på vikt (betalning)
* Samma kund visas aldrig två gånger i rad
* Olika meddelanden för varje kund
* Tidsbaserad logik (t.ex. Svarte Petter på jämna/ojämna minuter)
* Varje reklam visas i 20 sekunder
* LCD-display används för att visa text

---

## Teknik

* Programmeringsspråk: C
* Plattform: AVR (ATmega328P)
* Display: 16x2 LCD
* Bibliotek:

  * <avr/io.h>
  * <util/delay.h>
  * lcd.h

---

## Hur det fungerar

1. Programmet startar och initierar LCD:n
2. En kund väljs slumpmässigt baserat på vikt
3. Ett meddelande väljs för kunden
4. Meddelandet visas på LCD i 20 sekunder
5. Processen upprepas

---

## Att köra projektet

1. Kompilera koden med AVR-GCC
2. Ladda upp till din mikrokontroller (t.ex. med Arduino/ISP)
3. Koppla en 16x2 LCD enligt din koppling
4. Starta programmet

---

## Förbättringar (framtida arbete)

* Scrollande text
* Blinkande text
* Bättre slumpgenerator
* Mer dynamisk hantering av meddelanden
