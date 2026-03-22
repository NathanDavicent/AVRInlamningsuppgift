//#include <util/delay.h>
//#include <avr/io.h>
//#include "lcd.h"

#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void display_ad(const char *sponsor[], unsigned int delay){
    //lcd_clear();
    //lcd_printf(sponsor);
    //_delay_ms(delay);

    printf("%s\n", sponsor);
    sleep(delay);

}

int main(void){
    while(1){
        display_ad("CocaCola", 5);
        display_ad("Amazon", 2);
        display_ad("Youtube", 5);
    }

    return 0;
}