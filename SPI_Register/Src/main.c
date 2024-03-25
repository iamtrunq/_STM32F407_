
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stm32f407xx.h>
#include <stdio.h>
#include "systick.h"
#include "spi.h"
#include "GPIO.h"
uint8_t NoDecode[11]={0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,0x7F,0x7B,0x00};
uint16_t count = 0;


void SPI_Transmit_16bits( uint8_t* TX_Data, uint16_t TX_Size);
void Max7219_init();
void HELLO();
void SendData(uint8_t Address, uint8_t Data);
void Put_Number_Left(uint16_t value);
void Put_Number_Right(uint16_t value);


int main(void)
{

	SPI2_Init();
	Max7219_init();
	GPIO_Toggle_Init();
	HELLO();
	SYS_TICK_Delay(1000);

	SendData(0x05,0x00);
	SendData(0x06,0x00);
	SendData(0x07,0x00);
	SendData(0x08,0x00);

	while(1){
		count++;
		if (count > 9999) count = 0;
		Toggle();
		Put_Number_Left(count);
		SYS_TICK_Delay(500);
	}


}

void SendData(uint8_t Address, uint8_t Data){

	uint8_t Buffer[2];
    Buffer[0] = Address;
    Buffer[1] = Data;
    SPI_Transmit_16bits(Buffer,2);

}
void Max7219_init(){
    /* Set  no decode mode: 0x0000 */
	SendData(0x09,0x00);
    /* Set intensity: 0x0A09 */
	SendData(0x0A,0x09);
    /* Set scanlimit */
	SendData(0x0B,0x07);
    /* No shutdown, turn off display test */
	SendData(0x0C,0x01);
	SendData(0x0F,0x00);
}
void HELLO(){
	uint8_t mssv[8] = {0x08,0x37,0x4F,0x0E,0x0E,0x7E,0x08,0x00};
    for(int i=0;i<8;i++){
        SendData(i+1, mssv[7-i]);
    }
}

void Put_Number_Left(uint16_t value){
	uint8_t donvi;
	uint8_t chuc;
	uint8_t tram;
	uint8_t nghin;
	if(value < 10){
		donvi = value;
		SendData(0x01,NoDecode[donvi]);
		SendData(0x02,NoDecode[11]);
		SendData(0x03,NoDecode[11]);
		SendData(0x04,NoDecode[11]);
	}else if(value < 100 ){
		chuc = value / 10;
		donvi = value % 10;
		SendData(0x01,NoDecode[donvi]);
		SendData(0x02,NoDecode[chuc]);
		SendData(0x03,NoDecode[11]);
		SendData(0x04,NoDecode[11]);
	}else if(value < 1000){
		tram = value / 100;
		chuc = (value - tram*100) /10;
		donvi = (value - tram*100) % 10;

		SendData(0x01,NoDecode[donvi]);
		SendData(0x02,NoDecode[chuc]);
		SendData(0x03,NoDecode[tram]);
		SendData(0x04,NoDecode[11]);
	}else{
		nghin = value / 1000;
		tram = (value - nghin*1000) / 100;
		chuc = (value - nghin*1000 - tram*100) / 10;
		donvi = (value - nghin*1000 - tram*100) % 10;

		SendData(0x01,NoDecode[donvi]);
		SendData(0x02,NoDecode[chuc]);
		SendData(0x03,NoDecode[tram]);
		SendData(0x04,NoDecode[nghin]);

	}

}
