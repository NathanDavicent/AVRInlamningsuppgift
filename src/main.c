#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lcd.h"

#define LCD_COLS 20

volatile uint32_t g_seconds = 0;

uint8_t scroll = 0;
uint8_t blink = 1;

typedef struct {
    const char *name;
    uint16_t weight;
} Customer;

typedef enum {
    MSG_SCROLL,
    MSG_TEXT,
    MSG_BLINK
} MessageType;

typedef struct {
    const char *text;
    MessageType type;
} Message;

Customer customers[] = {
    {"Hederlige Harrys Bilar", 5000},
    {"Farmor Ankas Pajer AB", 3000},
    {"Svarte Petters Svartbyggen", 1500},
    {"Langben detektivbyra", 4000},
    {"IOT Reklambyra", 1000}
};

#define CUSTOMER_COUNT (sizeof(customers) / sizeof(customers[0]))

int pick_customer(int last_index) {
    uint16_t total_weight = 0;

    for (uint8_t i = 0; i < CUSTOMER_COUNT; i++) {
        if ((int)i != last_index) {
            total_weight += customers[i].weight;
        }
    }

    uint16_t r = rand() % total_weight;

    for (uint8_t i = 0; i < CUSTOMER_COUNT; i++) {
        if ((int)i == last_index) {
            continue;
        }

        if (r < customers[i].weight) {
            return i;
        }

        r -= customers[i].weight;
    }

    return 0;
}

Message pick_message(uint8_t customer_index) {
    Message msg;

    switch (customer_index) {
        case 0: {
            uint8_t r = rand() % 3;

            if (r == 0) {
                msg.text = "Kop bil hos Harry    ";
                msg.type = MSG_SCROLL;
            } else if (r == 1) {
                msg.text = "En god bilaffar for Harry!";
                msg.type = MSG_TEXT;
            } else {
                msg.text = "Hederlige Harrys Bilar";
                msg.type = MSG_BLINK;
            }

            return msg;
        }

        case 1: {
            uint8_t r = rand() % 2;

            if (r == 0) {
                msg.text = "Kop paj hos Farmor Anka    ";
                msg.type = MSG_SCROLL;
            } else {
                msg.text = "Skynda innan Marten atit alla pajer";
                msg.type = MSG_TEXT;
            }

            return msg;
        }

        case 2: {
            uint32_t minutes = g_seconds / 60;

            if ((minutes % 2) == 0) {
                msg.text = "Lat Petter bygga at dig    ";
                msg.type = MSG_SCROLL;
            } else {
                msg.text = "Bygga svart? Ring Petter";
                msg.type = MSG_TEXT;
            }

            return msg;
        }

        case 3: {
            uint8_t r = rand() % 2;

            if (r == 0) {
                msg.text = "Mysterier? Ring Langben";
            } else {
                msg.text = "Langben fixar biffen";
            }

            msg.type = MSG_TEXT;
            return msg;
        }

        case 4:
            msg.text = "Synas har? IOT Reklambyra";
            msg.type = MSG_TEXT;
            return msg;
    }

    msg.text = "Okant meddelande";
    msg.type = MSG_TEXT;
    return msg;
}

void update_scroll(int len) {
    scroll = (scroll + 1) % len;
}

void update_blink(void) {
    blink = !blink;
}

void make_fixed_buffer(char *buffer, const char *text) {
    for (int i = 0; i < LCD_COLS; i++) {
        if (text[i] != '\0') {
            buffer[i] = text[i];
        } else {
            buffer[i] = ' ';
        }
    }

    buffer[LCD_COLS] = '\0';
}

void print_line(uint8_t row, const char *text) {
    char buffer[LCD_COLS + 1];

    make_fixed_buffer(buffer, text);

    lcd_set_cursor(0, row);
    lcd_puts(buffer);
}

void render_scroll_message(const char *message) {
    char buffer[LCD_COLS + 1];
    int len = strlen(message);

    for (int i = 0; i < LCD_COLS; i++) {
        buffer[i] = message[(scroll + i) % len];
    }

    buffer[LCD_COLS] = '\0';

    lcd_set_cursor(0, 1);
    lcd_puts(buffer);
}

void render_blink_message(const char *message) {
    char buffer[LCD_COLS + 1];

    if (blink) {
        make_fixed_buffer(buffer, message);
    } else {
        for (int i = 0; i < LCD_COLS; i++) {
            buffer[i] = ' ';
        }

        buffer[LCD_COLS] = '\0';
    }

    lcd_set_cursor(0, 1);
    lcd_puts(buffer);
}

void display_ad(const char *customer_name, Message message, uint8_t seconds) {
    lcd_clear();

    scroll = 0;
    blink = 1;

    print_line(0, customer_name);

    int len = strlen(message.text);

    for (uint16_t tick = 0; tick < seconds * 4; tick++) {

        if (message.type == MSG_SCROLL) {
            render_scroll_message(message.text);
            update_scroll(len);
        } 
        else if (message.type == MSG_BLINK) {
            if (tick > 0 && tick % 2 == 0) {
                update_blink();
            }

            render_blink_message(message.text);
        } 
        else {
            print_line(1, message.text);
        }

        _delay_ms(250);
    }

    g_seconds += seconds;
}

int main(void) {
    lcd_init();

    srand(1234);

    int last_customer = -1;

    while (1) {
        int current = pick_customer(last_customer);
        Message message = pick_message(current);

        display_ad(customers[current].name, message, 20);

        last_customer = current;
    }

    return 0;
}