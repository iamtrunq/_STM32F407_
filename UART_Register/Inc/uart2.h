/*
 * uart2.h
 *
 *  Created on: Oct 30, 2023
 *      Author: nguye
 */

/*
 * Clock input : 16MHZ
 * Baudrate : 9600 bps
 * PA2: TX
 * PA3: RX
 */
#include <stdio.h>
#include <stdint.h>
#include <stm32f407xx.h>

#ifndef UART2_H_
#define UART2_H_
void UART2_Init(void);
void UART_Transmit( uint8_t* TX_Data, uint8_t TX_Size);
void UART_Receive(uint8_t *RX_Data,uint8_t RX_Size);


#endif /* UART2_H_ */
