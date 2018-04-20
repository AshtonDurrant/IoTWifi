/**
  ******************************************************************************
  * @file    stm32l475e_iot01_tsensor.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the temperature sensor
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l475e_iot01_tsensor.h"
#include <math.h>	//Needed the pow() function in determining the lux value

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32L475E_IOT01
  * @{
  */

/** @defgroup STM32L475E_IOT01_TEMPERATURE TEMPERATURE
  * @{
  */

/** @defgroup STM32L475E_IOT01_TEMPERATURE_Private_Variables TEMPERATURE Private Variables 
  * @{
  */ 
static TSENSOR_DrvTypeDef *tsensor_drv;  

//Defines for ADPS-9301 control and data registers

#define APDS9301_READ_BYTE_CONTROL_REGISTER			0xc0
#define APDS9301_WRITE_BYTE_CONTROL_REGISTER		0xc0
#define APDS9301_ACTIVATE							0x03
#define APDS9301_DEACTIVATE							0x00
#define APDS9301_READ_ID_REGISTER					0xca

#define APDS9301_READ_CH0_LOW_BYTE					0xcc
#define APDS9301_READ_CH0_HIGH_BYTE					0xcd
#define APDS9301_READ_CH1_LOW_BYTE					0xce
#define APDS9301_READ_CH1_HIGH_BYTE					0xcf

#define LIGHT_SENSOR_ADDRESS			0x39		//Floating

static float lux;

static float determine_lux_value(uint16_t channel0, uint16_t channel1);

 typedef struct
 {
 	void	(*Init)(uint16_t);	//Initialization function (16-bit value is address)
 	float	(*ReadData)(uint16_t);	//Reads data. 16-bit value is address of sensor.
 }LSensor_DriverTypeDef;

/**
  * @}
  */

/** @defgroup STM32L475E_IOT01_TEMPERATURE_Private_Functions TEMPERATURE Private Functions
  * @{
  */

/**
  * @brief  Initializes peripherals used by the I2C Temperature Sensor driver.
  * @retval TSENSOR status
  */
uint32_t BSP_TSENSOR_Init(void)
{  
  uint8_t ret = TSENSOR_ERROR;

#ifdef USE_LPS22HB_TEMP
  tsensor_drv = &LPS22HB_T_Drv;
#else /* USE_HTS221_TEMP */
  tsensor_drv = &HTS221_T_Drv; 
#endif

  /* Low level init */
  SENSOR_IO_Init();	//Initializes I2C2 and I2C1

  /* TSENSOR Init */   
  tsensor_drv->Init(TSENSOR_I2C_ADDRESS, NULL);
  init_light_sensor(LIGHT_SENSOR_ADDRESS);

  /*Light sensor Init*/


  ret = TSENSOR_OK;
  
  return ret;
}

/**
  * @brief  Read Temperature register of TS751.
  * @retval STTS751 measured temperature value.
  */
float BSP_TSENSOR_ReadTemp(void)
{ 
  return tsensor_drv->ReadTemp(TSENSOR_I2C_ADDRESS);
}

/*This function initializes the APDS-9301 ambient light sensor. This is modeled heavily
 * after HTS221_T_Init.
 * @param address:	I2C device address of the light sensor
 */
void init_light_sensor(uint16_t address){
	uint8_t tmp;
	uint8_t tmp2;
	uint16_t total;

	  /* Read control register */
//	  tmp = SENSOR_IO_Read_I2C1(address, APDS9301_READ_BYTE_CONTROL_REGISTER);

	  /* Enable BDU */
	  //tmp &= ~HTS221_BDU_MASK;
	  //tmp |= (1 << HTS221_BDU_BIT);

	  /* Set default ODR */
	  //tmp &= ~HTS221_ODR_MASK;
	  //tmp |= (uint8_t)0x01; /* Set ODR to 1Hz */

	  /* Activate the light sensor */
	  SENSOR_IO_I2C1_Write((address << 1), APDS9301_WRITE_BYTE_CONTROL_REGISTER, APDS9301_ACTIVATE);

	  /*Read channel 0, first time*/
	 // tmp = SENSOR_IO_Read_I2C1((address << 1), APDS9301_READ_CH0_LOW_BYTE);
	 // tmp2 = SENSOR_IO_Read_I2C1((address << 1), APDS9301_READ_CH0_HIGH_BYTE);
	  //total = (tmp2 << 8) + tmp;	//Total value
	  /*Read channel 0, second time (covered by my hand)*/
	  //tmp = SENSOR_IO_Read_I2C1((address << 1), APDS9301_READ_CH0_LOW_BYTE);
	  //tmp2 = SENSOR_IO_Read_I2C1((address << 1), APDS9301_READ_CH0_HIGH_BYTE);
	  //total = (tmp2 << 8) + tmp;	//Total value

	  /*Test code for reading registers and checking to make sure that I2C1 works correctly*/
	  /*SENSOR_IO_I2C1_Write((address << 1), APDS9301_WRITE_BYTE_CONTROL_REGISTER, APDS9301_ACTIVATE);

	  tmp = SENSOR_IO_Read_I2C1((address << 1), APDS9301_READ_BYTE_CONTROL_REGISTER);

	  SENSOR_IO_I2C1_Write((address << 1), APDS9301_WRITE_BYTE_CONTROL_REGISTER, APDS9301_DEACTIVATE);

	  tmp = SENSOR_IO_Read_I2C1((address << 1), APDS9301_READ_BYTE_CONTROL_REGISTER);

	  tmp = SENSOR_IO_Read_I2C1((address << 1), APDS9301_READ_ID_REGISTER);*/
}

float read_light_sensor_data(void){
	uint16_t address = LIGHT_SENSOR_ADDRESS;
	uint8_t low_byte;				//Used to hold the low byte of read-in data
	uint8_t high_byte;				//Used to hold the high byte of read-in data
	uint16_t channel_0_raw_value;	//Channel 0 raw data value
	uint16_t channel_1_raw_value;	//Channel 1 raw data value

	//Read Channel 0
	low_byte = SENSOR_IO_Read_I2C1((address << 1), APDS9301_READ_CH0_LOW_BYTE);
	high_byte = SENSOR_IO_Read_I2C1((address << 1), APDS9301_READ_CH0_HIGH_BYTE);
	channel_0_raw_value = (high_byte << 8) + low_byte;	//Channel 0 read

	//Read Channel 1
	low_byte = SENSOR_IO_Read_I2C1((address << 1), APDS9301_READ_CH1_LOW_BYTE);
	high_byte = SENSOR_IO_Read_I2C1((address << 1), APDS9301_READ_CH1_HIGH_BYTE);
	channel_1_raw_value = (high_byte << 8) + low_byte;	//Channel 1 read

	//Calculate lux value from raw data.
	lux = determine_lux_value(channel_0_raw_value, channel_1_raw_value);

	return lux;
}

/*This function is based off of the empirical formula for calculating lux, given in the
 * APDS9301 datasheet. This function takes in the raw data read from Channels 0 and 1 on the
 * APDS9301 and calculates the lux detected by that sensor. The lux value is calculated from the
 * raw data and is returned from this function.
 */
float determine_lux_value(uint16_t channel0, uint16_t channel1){
	float decision = (float)channel1 / channel0;	//Hopefully this cast avoids int division
	float lux_value;

	if(decision <= 0.50){	// CH1/CH0 <= 0.50
		lux_value = (0.0304 * channel0) - (0.062 * channel0 * pow(((float)channel1/channel0), 1.4));
	}

	else if(decision > 0.50 && decision <= 0.61){	// 0.50 < CH1/CH0 <= 0.61
		lux_value = (0.0224 * channel0) - (0.031 * channel1);
	}

	else if(decision > 0.61 && decision <= 0.80){	// 0.61 < CH1/CH0 <= 0.80
		lux_value = (0.0128 * channel0) - (0.0153 * channel1);
	}

	else if(decision > 0.80 && decision <= 1.30){	// 0.80 < CH1/CH0 <= 1.30
		lux_value = (0.00146 * channel1) - (0.00112 * channel1);
	}
	else{	// CH1/CH0 > 1.30
		lux_value = 0.0;
	}

	return lux_value;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
