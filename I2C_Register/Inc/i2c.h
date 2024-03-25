/*
 * i2c.h
 *
 *  Created on: Nov 23, 2023
 *      Author: nguye
 */

#ifndef I2C_H_
#define I2C_H_

#define ENABLE  	1
#define DISABLE		0

#define CR1_PE		(1U<<0)
#define	CR1_START	(1U<<8)
#define CR1_STOP	(1U<<9)
#define CR1_ACK		(1U<<10)
#define CR1_SWRST	(1U<<15)


#define CR2_FREQ	(0x3F<<0)

#define SR1_SB		(1U<<0)
#define SR1_ADDR	(1U<<1)
#define SR1_BTF		(1U<<2)
#define SR1_RXNE	(1U<<6)
#define SR1_TXE 	(1U<<7)

#define APB1ENR_I2C1EN (1<<21)


#define SR2_BUSY	(1U<<1)

void I2C_Init(void);
void I2C_MasterSendData(uint8_t Addrres,uint8_t *pTXbuffer, uint32_t Len, uint8_t StartRepeat);
void I2C_MasterReceiveData(uint8_t SlaveAddress,uint8_t *pRxBuffer, uint32_t Len,uint8_t StartRepeat);



#endif /* I2C_H_ */
