#include "stm32f407xx.h"
#include "i2c.h"
/*
 * PB6 : SCL
 * PB9 : SDA
 *
 * */


void I2C_Init(void){
	/*--------------------------GPIO CONFIGURE-------------------------------*/
	/*Enable clock*/
	RCC->AHB1ENR |=(1U<<1);			/*Enable clock for port B*/

	/*Aternate function mode*/
	GPIOB->MODER |= (0x2<<12);		/*Alternate Mode*/
	GPIOB->MODER |=(0x2<<18);		/*Alternate Mode*/

	/*Output Open Drain*/
	GPIOB->OTYPER |=(1U<<6);
	GPIOB->OTYPER |=(1U<<9);

	/*Speed configure*/
	GPIOB->OSPEEDR |=(0x3<<12);		/*Very High Speed*/
	GPIOB->OSPEEDR |=(0x3<<18);		/*Very High Speed*/

	/*Output Push-Pull*/
	GPIOB->PUPDR |=(0x1<<12);
	GPIOB->PUPDR |=(0x1<<18);

	/*Select Alternate Function*/
	GPIOB->AFR[0] |=(0x4<<24);		/*AF4*/
	GPIOB->AFR[1] |=(0x4<<4);		/*AF4*/


	/*--------------------------I2C CONFIGURE---------------------------------*/
	/**/
	RCC->APB1ENR |=APB1ENR_I2C1EN;
	/*Configure mode for I2C*/
	I2C1->CCR &=~ (1U<<15);  //Sm Mode

	/*Configure Frequency*/
	I2C1->CR2 |= (0x10<<0); // 16Mhz

	/*Configure CCR*/
	I2C1->CCR |= (0x50<<0); // 80 Dec

	/*Configure TRISE
	 * Note : READ MANUAL*/
	I2C1->TRISE = 0;
	I2C1->TRISE |=(0x11<<0);

	/*Enable ACK*/
	I2C1->CR1 |= CR1_ACK;

	/*Enable I2C1*/
	I2C1->CR1 |= CR1_PE;

}
void I2C_MasterSendData(uint8_t Addrres,uint8_t *pTXbuffer, uint32_t Len, uint8_t StartRepeat){
	/*1. Generate Start condition*/
	I2C1->CR1 |= CR1_START;
	/*2. While start condition complete*/
	/*Note : until SB is cleared SCL will be stretched (pull to LOW)*/
	while(!(I2C1->SR1 & SR1_SB));
	/*3. Send the Address of the slave with R/W bit -> 8bit*/
	I2C1->DR = (Addrres << 1) ; //Write
	/*4. Wait address phase is completed*/
	while( !(I2C1->SR1 & SR1_ADDR) );
	/*5, Clear ADDR flag*/
	/*Note uintil ADDR is cleared then SCL will be stretched (pulled to LOW)*/
	uint8_t dumpy = I2C1->SR1  & I2C1->SR2;
	/*6. Send Data until Len = 0*/

	while(Len > 0){
		while(!(I2C1->SR1 & SR1_TXE));
		I2C1->DR = *pTXbuffer;
		pTXbuffer++;
		Len--;
	}

	/* 7. When Len = 0 , wait TXE = 1 and BTF = 1 before generate STOP condition */
	/*Note TXE = 1 , BTF = 1, mean both Shift Register (SR) and Data Register(DR) are empty and next transmission should begin and auto clear by Stop condition*/
	/*When BTF = 1 SCL will be stretched (pulled to LOW)*/
	while(!(I2C1->SR1 & SR1_TXE));
	while(!(I2C1->SR1 & SR1_BTF));

	/*8. Generate STOP condition and master neend not to wait for the completion of stop condition*/
	/*Note: Generateing STOP, automatically clears the BTF*/
	if(StartRepeat == DISABLE){
		I2C1->CR1 |= CR1_STOP;
	}
}

void I2C_MasterReceiveData(uint8_t SlaveAddress,uint8_t *pRxBuffer, uint32_t Len,uint8_t StartRepeat){
	/*1. Generate Start condition*/
	I2C1->CR1 |= CR1_START;
	/*2. Confirm that start generation is completed by checking the SB flag
	 * Note : Until SB is cleared , SCL will be stretched (pull to LOW)*/
	while(!(I2C1->SR1 & SR1_SB));
	/*3. Send the address of the slave with R/W = 1*/
	I2C1->DR = (SlaveAddress << 1) | 1;  //Read
	/*4. Wait until address phase is completed by checking ADDR flag*/
	while(!(I2C1->SR1 & SR1_ADDR));
	/*Master want to read one byte from Slave*/

	if(Len ==1 ){
		/*Disable ACKing*/
		I2C1->CR1 &=~ CR1_ACK;
		/*Clear ADRR FLAG*/

		/*Generate STOP condition*/
		I2C1->CR1 |= CR1_STOP;

		uint16_t dumpy = I2C1->SR1 | I2C1->SR2;
		/*Wait until RXNE is set 1*/
		while(!(I2C1->SR1 & SR1_RXNE));

		/*Read the Data*/
		*pRxBuffer = I2C1->DR;

		return;
	}

	/*Master want to read more bytes from Slave*/
	else{
		/*Clear the ADDR Flag*/

		/*Read Data until Len = 0*/
		for(uint32_t i = Len; i > 0; i--){
			/*Wait until RXNE is set 1*/
			while(!(I2C1->SR1 & SR1_RXNE));

			/*Last second byte data*/
			if (i == 2){
				/*Clear ACK bit*/
				I2C1->CR1 &=~ CR1_ACK;

				/*Generate STOP condition*/
				if(StartRepeat == DISABLE){
					I2C1->CR1 |= CR1_STOP;
				}
			}

			/*Read Data from Data Register*/
			*pRxBuffer = I2C1->DR;

			/*Increment the buffer address*/
			pRxBuffer++;

		}
	}
	/*Re-enable ACKing */
	I2C1->CR1 |= CR1_ACK;
}


void I2C_Read_Memory(uint8_t Slave_Address, uint8_t Memory_Address, int Len ,uint8_t *Data){
	volatile int temp;

	/*Wait I2C not busy*/
	while (I2C1->SR2 & (SR2_BUSY)){}

	/*Generate start condition*/
	I2C1->CR1 |= CR1_START;

	/*Wait start bit generated*/
	while( !(I2C1->SR1 & SR1_SB) ){}

	/*Transmit slave address + Write*/
	I2C1->DR = Slave_Address << 1 + 0;

	/*Wait uintil addr flag is set*/
	while( !(I2C1->SR1 & SR1_ADDR) ){}

	/*Clear addr flag*/
	temp = I2C1->SR2 ;

	/*Wait until transmit empty*/
	while ( !(I2C1->SR1 & SR1_TXE) ){}

	/*Send memory address*/
	I2C1->DR = Memory_Address;



	/*Generate restart*/
	I2C1->CR1 |= CR1_START;

	/*Wait until start flag is set*/
	while( !(I2C1->SR1 & SR1_SB) ){}

	/*Transmit slave address + Read*/
	I2C1->DR = (Slave_Address <<1) |1;

	/*Wait uintil addr flag is set*/
	while( !(I2C1->SR1 & SR1_ADDR) ){}

	/*Clear addr flag*/
	temp = I2C1->SR2 | I2C1->SR1 ;
	if(Len ==1 ){
		/*Disable ACKing*/
		I2C1->CR1 &=~ CR1_ACK;
		/*Clear ADRR FLAG*/

		/*Generate STOP condition*/
		I2C1->CR1 |= CR1_STOP;

		uint16_t dumpy = I2C1->SR1 | I2C1->SR2;
		/*Wait until RXNE is set 1*/
		while(!(I2C1->SR1 & SR1_RXNE));

		/*Read the Data*/
		*Data = I2C1->DR;

		//return;
	}

	/*Master want to read more bytes from Slave*/
	else{
		/*Clear the ADDR Flag*/

		/*Read Data until Len = 0*/
		for(uint32_t i = Len; i > 0; i--){
			/*Wait until RXNE is set 1*/
			while(!(I2C1->SR1 & SR1_RXNE));

			/*Last second byte data*/
			if (i == 2){
				/*Clear ACK bit*/
				I2C1->CR1 &=~ CR1_ACK;

				/*Generate STOP condition*/

				I2C1->CR1 |= CR1_STOP;

			}

			/*Read Data from Data Register*/
			*Data = I2C1->DR;

			/*Increment the buffer address*/
			Data++;

		}
	}
	/*Re-enable ACKing */
	I2C1->CR1 |= CR1_ACK;

}
void I2C_Write_Memory(uint8_t saddr, uint8_t maddr, int n, uint8_t *Data){
	/*Generate Start Condition*/
	I2C1->CR1 |=CR1_START;

	/*Wait start bit generated*/
	while(!(I2C1->SR1 & SR1_SB));

	/*Sent Slave Address + Write*/
	I2C1->DR = saddr << 1;

	/*Wait ADDR bit set*/
	while(!(I2C1->SR1 & SR1_ADDR));

	/*NOTE: Clear ADDR bit*/
	uint8_t dumpy = I2C1->SR1 | I2C1->SR2;

	/*Wait Data Transmit Register is empty*/
	while(!(I2C1->SR1 & SR1_TXE));

	/*Sent Memory Adresss*/
	I2C1->DR = maddr;

	for(int i = 0; i < n; i++){

		/*Wait Data Transmit Register is empty*/
		while(!(I2C1->SR1 & SR1_TXE));

		/*Send Data */
		I2C1->DR = *Data++;

	}
	/*Wait The last byte was sent*/
	while (!(I2C1->SR1 & SR1_BTF));

	/*Generate Stop condition*/
	I2C1->CR1 |=CR1_STOP;
}


