#define F_CPU 16000000UL  // Klockfrekvens för AVR

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include "lcd.h"

#include <string.h>

volatile uint32_t g_seconds = 0;   // Håller koll på sekunder
uint8_t scroll = 0;
uint8_t blink = 1;

// Kund med namn och vikt
typedef struct {
    const char *name;
    uint16_t weight;
} Customer;

// Kunder och deras vikt
Customer customers[] = {
    {"  Harrys Bilar", 5000},
    {"Kop hos Farmor Anka ", 3000},
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
        if ((int) i != last_index) {
            total_weight += customers[i].weight;
        }
    }

    if (total_weight == 0) {
        return 0;
    }

    uint16_t r = rand() % total_weight;

    // Välj kund utifrån slumpvärdet
    for (uint8_t i = 0; i < CUSTOMER_COUNT; i++) {
        if ((int) i == last_index) {
            continue;
        }

        if (r < customers[i].weight) {
            return i;
        }

        r -= customers[i].weight;
    }

    return 0;
}

// Väljer meddelande för vald kund
const char *pick_message(uint8_t customer_index) {
    switch (customer_index) {
        case 0: {
            uint8_t r = rand() % 3;
            if (r == 0) return "Kop bil hos Harry. En god bilaffar (for Harry!)  ";
            if (r == 1) return "God bilaffar, Hederlige Harrys Bilar    ";
            return "Harrys Bilar";
        }

        case 1: {
            uint8_t r = rand() % 2;
            if (r == 0) return "Kop paj hos F";
            return "Kop paj hos Farmor Anka  ";
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


void update_scroll(int len){
    scroll = (scroll + 1) % len;
}

void update_blink(){
    blink = !blink;
}

void render_message(const char *message){
    char buffer[21];
    int len = strlen(message);

    for (int i = 0; i < 20; i++) {
        buffer[i] = message[(scroll + i) % len];
    }
    buffer[20] = '\0';

    lcd_set_cursor(0, 1);
    lcd_puts(buffer);

}

void render_blink(const char *text){
    lcd_set_cursor(0, 0);
    
    if (blink) {
        lcd_puts(text);
    } else {
        lcd_puts("                    ");
    }
}

// Visar kund och meddelande på LCD
void display_ad(const char *customer_name, const char *message, uint8_t seconds) {
    lcd_clear();



    scroll = 0;
    blink = 1;

    int len = strlen(message);

    for (uint16_t tick = 0; tick < seconds * 10; tick++) {
        if (tick > 0 && tick % 5 == 0) {
            update_blink();
        }
        update_scroll(len);

        render_blink(customer_name);
        render_message(message);

        _delay_ms(250);
    }

    g_seconds += seconds;
}

int main(void){
    lcd_init();  // Starta LCD
    srand(TCNT0); // Startvärde för slump

    int last_customer = -1;

    while (1) {
        int current = pick_customer(last_customer);
        const char *message = pick_message(current);

        display_ad(customers[current].name, message, 20);

        last_customer = current;
    }

    return 0;
}