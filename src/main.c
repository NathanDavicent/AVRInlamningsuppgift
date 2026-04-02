#define F_CPU 16000000UL  // Klockfrekvens för AVR

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include "lcd.h"

volatile uint32_t g_seconds = 0;   // Håller koll på sekunder

// Kund med namn och vikt
typedef struct {
    const char *name;
    uint16_t weight;
} Customer;

// Kunder och deras vikt
Customer customers[] = {
    {"Harry", 5000},
    {"Farmor Anka", 3000},
    {"Svarte Petter", 1500},
    {"Langben", 4000},
    {"IOT Reklambyra", 1000}
};

#define CUSTOMER_COUNT (sizeof(customers) / sizeof(customers[0]))  // Antal kunder

// Väljer en kund slumpmässigt efter vikt
// Samma kund får inte väljas två gånger i rad
int pick_customer(int last_index) {
    uint16_t total_weight = 0;

    // Räkna total vikt utan senaste kunden
    for (uint8_t i = 0; i < CUSTOMER_COUNT; i++) {
        if (i != last_index) {
            total_weight += customers[i].weight;
        }
    }

    uint16_t r = rand() % total_weight;

    // Välj kund utifrån slumpvärdet
    for (uint8_t i = 0; i < CUSTOMER_COUNT; i++) {
        if (i == last_index) {
            continue;
        }

        if (r < customers[i].weight) {
            return i;
        }

        r -= customers[i].weight;
    }

    return -1;
}

// Väljer meddelande för vald kund
const char *pick_message(uint8_t customer_index) {
    switch (customer_index) {
        case 0: {
            uint8_t r = rand() % 3;
            if (r == 0) return "Kop bil hos H";
            if (r == 1) return "God bilaffar";
            return "Harrys Bilar";
        }

        case 1: {
            uint8_t r = rand() % 2;
            if (r == 0) return "Kop paj hos F";
            return "Skynda, paj slut";
        }

        case 2: {
            uint32_t minutes = g_seconds / 60;

            if ((minutes % 2) == 0) {
                return "Petter bygger";
            } else {
                return "Ring Petter";
            }
        }

        case 3: {
            uint8_t r = rand() % 2;
            if (r == 0) return "Ring Langben";
            return "Fixar biffen";
        }

        case 4:
            return "Synas har?";
    }

    return "Okant meddelande";
}

// Visar kund och meddelande på LCD
void display_ad(const char *customer_name, const char *message) {
    lcd_clear();
    _delay_ms(50);

    // Rensa båda raderna
    lcd_set_cursor(0, 0);
    lcd_puts("                "); // 16 spaces

    lcd_set_cursor(0, 0);
    lcd_puts("                "); // 16 spaces

    // Skriv kundnamn på rad 1
    lcd_set_cursor(1, 0);
    lcd_puts(customer_name);

    // Skriv meddelande på rad 2
    lcd_set_cursor(1, 0);
    lcd_puts(message);
}

int main(void){
    lcd_init();  // Starta LCD
    srand(1234); // Startvärde för slump

    int last_customer = -1;

    while (1) {
        int current = pick_customer(last_customer);
        const char *message = pick_message(current);

        display_ad(customers[current].name, message);

        // Vänta 20 sekunder
        for (uint8_t i = 0; i < 20; i++){
            _delay_ms(1000);
            g_seconds++;
        }
        last_customer = current;
    }

    return 0;
}