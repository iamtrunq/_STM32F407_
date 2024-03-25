#include <stdint.h>
#include "blink.h"
#include <stdio.h>


const uint32_t MAX_COUNT = 10000; 
uint32_t cycle = 0;

uint16_t dumpy = 10;
uint8_t dumpy1 = 10;





int main(){

    led_init();
    while(1){
        for(uint32_t i = 0; i < 100000; i++){}
        turn_on_led(Orange);
        turn_off_led(Green);
        for(uint32_t i = 0; i < 100000; i++){}
        turn_off_led(Orange);
        turn_on_led(Red);
        for(uint32_t i = 0; i < 100000; i++){}
        turn_off_led(Red);
        turn_on_led(Blue);
        for(uint32_t i = 0; i < 100000; i++){}
        turn_off_led(Blue);
        turn_on_led(Green);
        cycle ++;
    }
}