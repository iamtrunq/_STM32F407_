


#ifndef BLINK_H_
#define BlINK_H_
#include <stdint.h>

#define Blue 15
#define Red 14
#define Green 12
#define Orange 13



void led_init(void);
void turn_on_led(uint8_t LED);
void turn_off_led(uint8_t LED);
void toggle_led(uint8_t LED);

#endif 