
#include "blink.h"
#include <stdint.h>




void led_init(void)
{
    uint32_t *pGPIOD_ODR = (uint32_t*)0x40020C14;
    uint32_t *pRCCABH1ENR = (uint32_t*)0x40023830;
    uint32_t *pGPIOD_MODER = (uint32_t*) 0x40020C00;
    
    // Enable clock for port D
	*pRCCABH1ENR |= ( 1 << 3);
	// Configure ouput mode for GPIO D12 D13 D14 D15
	*pGPIOD_MODER |= ( 1 << (24));
	*pGPIOD_MODER |= ( 1 << (26));
	*pGPIOD_MODER |= ( 1 << (28));
	*pGPIOD_MODER |= ( 1 << (30));
    //Turn off all led
    *pGPIOD_ODR &=~ ( 1 << 12);
    *pGPIOD_ODR &=~ ( 1 << 13);
    *pGPIOD_ODR &=~ ( 1 << 14);
    *pGPIOD_ODR &=~ ( 1 << 15);

}

void turn_on_led(uint8_t LED){
    uint32_t *pGPIOD_ODR = (uint32_t*)0x40020C14;
    *pGPIOD_ODR |= ( 1 << LED);
}

void turn_off_led(uint8_t LED){
    uint32_t *pGPIOD_ODR = (uint32_t*)0x40020C14;
    *pGPIOD_ODR &=~ ( 1 << LED);
}

void toggle_led(uint8_t LED){
    uint32_t *pGPIOD_ODR = (uint32_t*)0x40020C14;
    *pGPIOD_ODR ^= ( 1 << LED);
}

