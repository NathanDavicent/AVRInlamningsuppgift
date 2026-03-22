#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    const char *name;
    int weight;
} Customer;

Customer customers[] = {
    {"Harry", 5000},
    {"Farmor Anka", 3000},
    {"Svarte Petter", 1500},
    {"Langben", 4000},
    {"IOT Reklambyra", 1000}
};

int customer_count = sizeof(customers) / sizeof(customers[0]);

int pick_customer(int last_index) {
    int total_weight = 0;

    for (int i = 0; i < customer_count; i++) {
        if (i != last_index) {
            total_weight += customers[i].weight;
        }
    }

    int r = rand() % total_weight;

    for (int i = 0; i < customer_count; i++) {
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

const char *pick_message(int customer_index) {
    switch (customer_index) {
        case 0: {
            int r = rand() % 3;
            if (r == 0) return "Kop bil hos Harry [SCROLL]";
            if (r == 1) return "En god bilaffar (for Harry!) [TEXT]";
            return "Hederlige Harrys Bilar [BLINK]";
        }

        case 1: {
            int r = rand() % 2;
            if (r == 0) return "Kop paj hos Farmor Anka [SCROLL]";
            return "Skynda innan Marten atit alla pajer [TEXT]";
        }

        case 2: {
            time_t now = time(NULL);
            struct tm *t = localtime(&now);

            if (t->tm_min % 2 == 0) {
                return "Lat Petter bygga at dig [SCROLL]";
            } else {
                return "Bygga svart? Ring Petter [TEXT]";
            }
        }

        case 3: {
            int r = rand() % 2;
            if (r == 0) return "Mysterier? Ring Langben [TEXT]";
            return "Langben fixar biffen [TEXT]";
        }

        case 4:
            return "Synas har? IOT:s Reklambyra [TEXT]";
    }

    return "Okant meddelande";
}

void display_ad(const char *customer_name, const char *message) {
    printf("Kund: %s\n", customer_name);
    printf("Meddelande: %s\n", message);
    printf("-------------------------\n");
}


int main(void){

    srand((unsigned int)time(NULL));

    int last_customer = -1;

    for (int i = 0; i < 15; i++) {
        int current = pick_customer(last_customer);
        const char *message = pick_message(current);

        display_ad(customers[current].name, message);
        last_customer = current;
    }

    return 0;
}