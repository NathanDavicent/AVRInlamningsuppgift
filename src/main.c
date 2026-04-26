#define F_CPU 16000000UL  // Klockfrekvens för AVR

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include "lcd.h"

#include <string.h>

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
            if (r == 0) return "Kop bil hos H, God bilaffar";
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
    uint8_t seconds;
    char buffer[17];
    int len = strlen(message);

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_puts(customer_name);


    for (int offset = 0; offset < len; offset++) {
        for (int i = 0; i < 16; i++) {
            buffer[i] = message[(offset + i) % len];
        }

        buffer[16] = '\0';

        // Visar txt
        lcd_set_cursor(0, 0);
        lcd_puts(buffer);
        _delay_ms(500);

        // dölj txt
        lcd_set_cursor(0, 0);
        lcd_puts("                ");
        _delay_ms(500);

        g_seconds++;
    }
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