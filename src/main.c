#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include "lcd.h"

volatile uint32_t g_seconds = 0;

typedef struct {
    const char *name;
    uint16_t weight;
} Customer;

Customer customers[] = {
    {"Harry", 5000},
    {"Farmor Anka", 3000},
    {"Svarte Petter", 1500},
    {"Langben", 4000},
    {"IOT Reklambyra", 1000}
};

#define CUSTOMER_COUNT (sizeof(customers) / sizeof(customers[0]))

int pick_customer(int last_index) {
    uint16_t total_weight = 0;

    for (uint8_t i = 0; i < CUSTOMER_COUNT; i++) {
        if (i != last_index) {
            total_weight += customers[i].weight;
        }
    }

    uint16_t r = rand() % total_weight;

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

const char *pick_message(uint8_t customer_index) {
    switch (customer_index) {
        case 0: {
            uint8_t r = rand() % 3;
            if (r == 0) return "Kop bil hos Harry [SCROLL]";
            if (r == 1) return "En god bilaffar (for Harry!) [TEXT]";
            return "Hederlige Harrys Bilar [BLINK]";
        }

        case 1: {
            uint8_t r = rand() % 2;
            if (r == 0) return "Kop paj hos Farmor Anka [SCROLL]";
            return "Skynda innan Marten atit alla pajer [TEXT]";
        }

        case 2: {
            uint32_t minutes = g_seconds / 60;

            if ((minutes % 2) == 0) {
                return "Lat Petter bygga at dig [SCROLL]";
            } else {
                return "Bygga svart? Ring Petter [TEXT]";
            }
        }

        case 3: {
            uint8_t r = rand() % 2;
            if (r == 0) return "Mysterier? Ring Langben [TEXT]";
            return "Langben fixar biffen [TEXT]";
        }

        case 4:
            return "Synas har? IOT:s Reklambyra [TEXT]";
    }

    return "Okant meddelande";
}

void display_ad(const char *customer_name, const char *message) {
    lcd_clear();
    _delay_ms(50);

    lcd_set_cursor(0, 0);
    lcd_puts(customer_name);

    lcd_set_cursor(1, 0);
    lcd_puts(message);
}

int main(void){
    lcd_init();

    srand(1234);

    int last_customer = -1;

    while (1) {
        int current = pick_customer(last_customer);
        const char *message = pick_message(current);

        display_ad(customers[current].name, message);

        for (uint8_t i = 0; i < 20; i++){
            _delay_ms(1000);
            g_seconds++;
        }
        last_customer = current;
    }

    return 0;
}