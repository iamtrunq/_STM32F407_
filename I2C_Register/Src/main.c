
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stm32f407xx.h>
#include <stdio.h>
#include "GPIO.h"
#include "systick.h"
#include "i2c.h"




#define MPU6050_ADDRESS 	0x68
#define SMPLRT_DIV_REG		0x19		//Sample Rate Divider
#define GYRO_CONFIG_REG		0x1B		//Gyroscope Configuration
#define ACCEL_CONFIG_REG	0x1C		//Accelerometer Configuration
#define ACCEL_XOUT_H_REG	0x3B
#define TEMP_OUT_H_REG		0x41
#define GYRO_XOUT_H_REG		0x43
#define PWR_MGMT_1_REG		0x6B
#define WHO_AM_I_REG		0x75

int16_t Accel_X_Raw	= 0;
int16_t Accel_Y_Raw	= 0;
int16_t Accel_Z_Raw	= 0;

int16_t Gyro_X_Raw = 0;
int16_t Gyro_Y_Raw = 0;
int16_t Gyro_Z_Raw = 0;

float Ax,Ay,Az,Gx,Gy,Gz;

uint8_t RX_Buffer;
uint8_t TX_Buffer[20];
uint8_t Data[15]={0x75,0x88,0x89,0x55,0x56,0x58,0x89,0x12,0x15,0x78,0x29,0x23,0x29,0x55,0x14};
uint8_t DataMe[1]={0x55};

void MPU6050_Init();
void MPU6050_Read();
int main(void)
{


	GPIO_Toggle_Init();
	I2C_Init();
	MPU6050_Init();
	//I2C_Write_Memory(0x68,0x27,1,&DataMe);
	//I2C_Read_Memory(0x68,0x75,&RX_Buffer);
	while(1){
		SYS_TICK_Delay(500);
		Toggle();
		MPU6050_Read();
	}


}
void USART2_IRQHandler(void){
	/*Check which interrupt*/
	if( USART2->SR & (1<<5) ){

		UART_Transmit(TX_Buffer,20);

		//Toggle();
	}
	/*Clear interrupt flag*/
	USART2->SR &=~ (1<<5);


}

void MPU6050_Init(){
	uint8_t check;
	uint8_t Data;
	I2C_Read_Memory(MPU6050_ADDRESS,WHO_AM_I_REG,1,&check);

	if (check == 104){
		Data = 0x00;
		I2C_Write_Memory(MPU6050_ADDRESS,PWR_MGMT_1_REG,1,Data);

		Data = 0x07;
		I2C_Write_Memory(MPU6050_ADDRESS,SMPLRT_DIV_REG,1,Data);

		Data = 0x00;
		I2C_Write_Memory(MPU6050_ADDRESS,ACCEL_CONFIG_REG,1,Data);

		Data = 0x00;
		I2C_Write_Memory(MPU6050_ADDRESS,GYRO_CONFIG_REG,1,Data);

	}
}

void MPU6050_Read(){
	uint8_t RX_Data[6];

	I2C_Read_Memory(MPU6050_ADDRESS,ACCEL_XOUT_H_REG,6,RX_Data);

	Accel_X_Raw = (int16_t) (RX_Data[0] << 8 | RX_Data[1] );
	Accel_Y_Raw = (int16_t) (RX_Data[2] << 8 | RX_Data[3] );
	Accel_Z_Raw = (int16_t) (RX_Data[4] << 8 | RX_Data[5] );

	Ax = Accel_X_Raw /16384.0;
	Ay = Accel_Y_Raw /16384.0;
	Az = Accel_Z_Raw /16384.0;


}
