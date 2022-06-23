/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lpuart.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include <stdio.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define UART4          		LPUART4
#define UART4_CLK_FREQ 		BOARD_DebugConsoleSrcFreq()
#define PARAMETER_BYTE          	4
#define PARAMETER_COUNT      	    37
#define READ_INPUT_BUFFER_SIZE		8
#define READ_REG_BUFFER_SIZE		254

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t txbuff[]   = "Lpuart polling example\r\nBoard will send back received characters\r\n";
uint8_t rxbuff[20] = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/
//Union for Hexadecimal to Float Conversion
union hex2float
{
	uint8_t c[4];
	float f;
};
//Union for Hexadecimal to uint32_t Conversion
union hex2uint32
{
	uint8_t d[4];
	uint32_t g;
};
static const uint16_t wCRCTable[] = { 0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301,0X03C0, 0X0280, 0XC241, 0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1,
		0XC481, 0X0440, 0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81,0X0E40, 0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
		0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40, 0X1E00,0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41, 0X1400, 0XD4C1,
		0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641, 0XD201, 0X12C0, 0X1380,0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040, 0XF001, 0X30C0, 0X3180, 0XF141,
		0X3300, 0XF3C1, 0XF281, 0X3240, 0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501,0X35C0, 0X3480, 0XF441, 0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0,
		0X3E80, 0XFE41, 0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881,0X3840, 0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
		0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40, 0XE401,0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640, 0X2200, 0XE2C1,
		0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041, 0XA001, 0X60C0, 0X6180,0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240, 0X6600, 0XA6C1, 0XA781, 0X6740,
		0XA501, 0X65C0, 0X6480, 0XA441, 0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01,0X6FC0, 0X6E80, 0XAE41, 0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1,
		0XA881, 0X6840, 0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80,0XBA41, 0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
		0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640, 0X7200,0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041, 0X5000, 0X90C1,
		0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241, 0X9601, 0X56C0, 0X5780,0X9741, 0X5500, 0X95C1, 0X9481, 0X5440, 0X9C01, 0X5CC0, 0X5D80, 0X9D41,
		0X5F00, 0X9FC1, 0X9E81, 0X5E40, 0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901,0X59C0, 0X5880, 0X9841, 0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1,
		0X8A81, 0X4A40, 0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80,0X8C41, 0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
		0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

uint16_t ModbusCRC16(const uint8_t *nData, uint8_t wLength)
{
	uint8_t nTemp;
	uint16_t wCRCWord = 0xFFFF;
	while (wLength--)
	{
		nTemp = *nData++ ^ wCRCWord;
		wCRCWord >>= 8;
		wCRCWord ^= wCRCTable[nTemp];
	}
	return wCRCWord;
}
void delay_ms(uint32_t mSec)
{
	volatile uint32_t i = 0;
	i = mSec * 1000;
	SDK_DelayAtLeastUs(i , 600000000);
}
/*!
 * @brief Main function
 */
int main(void)
{
	uint8_t ch[10] = {0};
	lpuart_config_t config;

	//-----  Use meterReading  ------
	int i = 0;
	int j = 0;
	uint8_t modbusCommand[8] = {0};						  //Define Request String Buffer
	uint8_t rx[66]= {0};
	uint8_t slaveID = 0x01;								 // Modbus Slave Id
	uint8_t fx = 0x03;						   	   		// Modbus Function code
	uint16_t address = 0x0063;				      	   // 2 byte address of the coil/input/register
	uint16_t noOfInputsRead;				     	  // Variable to tell how many inputs to be read
	uint16_t noOfRegRead = 0x0014;					 // Variable to tell how many registers to be read
	uint8_t noOfRespBytes = (((0x0014) * 2) + 5);	// Variable to tell how many bytes as response we will get
	uint8_t readInputResp[READ_INPUT_BUFFER_SIZE];  // Buffer to hold response of Read Input CMD
	uint8_t readRegResp[READ_REG_BUFFER_SIZE];	   // Buffer to hold response of Read Reg CMD
	uint8_t tmpBuff[PARAMETER_COUNT*PARAMETER_BYTE] = {0};
	uint8_t bufferPosition = 0;
	uint8_t newbufpos = 0;
	union hex2float htf;				//Use union for hexadecimal to float conversion of data
	union hex2uint32 htu32;				//Use union for hexadecimal to uint32_t conversion of data
	struct meterreading *mreading;
	uint8_t parameterPosition = 0;
	uint16_t modbusCRC16 = 0;
	float EM_parameters[PARAMETER_COUNT] = {0};

	BOARD_ConfigMPU();
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();
	/*
	 * config.baudRate_Bps = 9600U;
	 * config.parityMode = kLPUART_ParityDisabled;
	 * config.stopBitCount = kLPUART_OneStopBit;
	 * config.txFifoWatermark = 0;
	 * config.rxFifoWatermark = 0;
	 * config.enableTx = false;
	 * config.enableRx = false;
	 */
	LPUART_GetDefaultConfig(&config);
	config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
	config.enableTx     = true;
	config.enableRx     = true;

	LPUART_Init(UART4, &config, UART4_CLK_FREQ);

	PRINTF("Lpuart polling example\r\nBoard will send back received characters\r\n");

	//Create Request String
	modbusCommand[0] = slaveID;		//Slave Id
	modbusCommand[1] =		fx;		//function Code
	modbusCommand[2] = ((address >> 8) & 0xFF);		//MSB
	modbusCommand[3] = (address & 0xFF);			//LSB
	//Seperate LSB and MSB of no of read Registers so that we can put it individually in the string for calculating CRC 16 Modbus
	modbusCommand[4] = ((noOfRegRead >> 8) & 0xFF);		//MSB
	modbusCommand[5] = (noOfRegRead & 0xFF);			//LSB

	//Getting CRC 16 Modbus
	modbusCRC16 = ModbusCRC16(modbusCommand, 6);
	//Seperate LSB and MSB f CRC 16 Modbus so that we can put it in request string for fetching data from Slave Modbus device
	modbusCommand[6] = (modbusCRC16 & 0xFF);			//LSB
	modbusCommand[7] = ((modbusCRC16 >> 8) & 0xFF);		//MSB


//	while (1)
//	{
			GPIO_PinWrite(GPIO4, 18U, 1U);
			LPUART_WriteBlocking(UART4, modbusCommand, 8);
			GPIO_PinWrite(GPIO4, 18U, 0U);
			LPUART_ReadBlocking(UART4, readRegResp, noOfRespBytes);

			PRINTF("First command Send.\r\n");
			for(i = 3; i < ((noOfRespBytes) - 2); i++)
			{
				tmpBuff[bufferPosition] = readRegResp[i];
			//	PRINTF(" \r\n |HEX| [%d] =  %x \r\n", i,readRegResp[i]);
				newbufpos = (bufferPosition + 1);

				if((newbufpos % PARAMETER_BYTE == 0) && ((i < 12) || (i > 15)))
				{
					j = (newbufpos - PARAMETER_BYTE);
					htf.c[1] = tmpBuff[j];
					htf.c[0] = tmpBuff[j + 1];
					htf.c[3] = tmpBuff[j + 2];
					htf.c[2] = tmpBuff[j + 3];
					EM_parameters[parameterPosition] = htf.f;
				//	PRINTF(" \r\n |Responce[%d] =  %f \r\n",parameterPosition ,htf.f);
					parameterPosition++;
				}
				bufferPosition++;
			}

			PRINTF("\r\n");
			for(i=0;i<10;i++)
			{
				PRINTF("|Meter_Reading_Modbus[%d] =  %f\r\n",i,EM_parameters[i]);
			}
//			memset(tmpBuff, '\0',sizeof(tmpBuff));
//			memset(readRegResp, '\0',sizeof(readRegResp));
//			memset(EM_parameters, '\0',sizeof(EM_parameters));
//			newbufpos=0;
//			bufferPosition =0;
//			parameterPosition=0;
			delay_ms(2000);
//	}

}
