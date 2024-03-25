/*
 * Description:
 * Use UART2 combine with Interrupt to transmit and receive data
 * When Data was read then Data will be transmited to Computer
 * */


#include <stdio.h>
#include <stdint.h>
#include <uart2.h>
#include <systick.h>
#include <stm32f407xx.h>

uint8_t Data[3] = {};
uint8_t mess[50]= "Hello world, UART Protocol test \n";
void NVIC_UART2(void);
void UART_Transmit( uint8_t* TX_Data, uint8_t TX_Size);
void UART_Receive(uint8_t *RX_Data,uint8_t RX_Size);

int main(void)
{
	RCC->AHB1ENR |= (1<<3);   //ENABLE clock for port D
	GPIOD->MODER |= (0x55<<24);  // Output for D12 ,D13, D14, D15
	GPIOD->ODR &=~ (0xF<<12);

	NVIC_UART2();
	UART2_Init();
	UART_Transmit(mess,50);
	SYS_TICK_Delay(500);

	while(1){

		SYS_TICK_Delay(1000);
		GPIOD->ODR ^=(1<<15);
		GPIOD->ODR ^=(1<<12);

	}
}

void NVIC_UART2(void){
	/*Clear Pending*/
	NVIC->ICPR[USART2_IRQn/32] |=(1U << (USART2_IRQn % 32));
	/*Enable interrupt */
	NVIC->ISER[USART2_IRQn/32] |=(1U << (USART2_IRQn % 32));
	/*Set priority 1*/
	NVIC->IP[USART2_IRQn/4] |= (0x01 << (8 * (USART2_IRQn%4) ) );
}


/*Interrupt UART2 handling*/
void USART2_IRQHandler(){
	/*Check flag status interrupt*/
	if(USART2->SR & (1<<5)){
		/*Receive data and then transmit backward*/
		UART_Receive(Data,3);
		GPIOD->ODR ^=(1<<14);
		GPIOD->ODR ^=(1<<13);

		/*Transmit data */
		UART_Transmit(Data,3);

	}
	USART2->SR &=~(1<<5);			//Clear Pending
}

