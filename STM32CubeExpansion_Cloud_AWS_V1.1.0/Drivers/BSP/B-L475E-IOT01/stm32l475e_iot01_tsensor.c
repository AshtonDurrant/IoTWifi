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
 #define APDS9301_CONTROL 				0x0
 #define APDS9301_TIMING 				0x1
 #define APDS9301_THRESHLOWLOW 			0x2
 #define APDS9301_THRESHLOWHIGH 		0x3
 #define APDS9301_THRESHHIGHLOW 		0x4
 #define APDS9301_THRESHHIGHHIGH 		0x5
 #define APDS9301_INTERRUPT 			0x6
 #define APDS9301_ID 					0xa
 #define APDS9301_DATA0LOW 				0xc
 #define APDS9301_DATA0HIGH 			0xd
 #define APDS9301_DATA1LOW 				0xe
 #define APDS9301_DATA1HIGH 			0xf

 #define APDS9301_READ_BYTE_CONTROL_REGISTER		0xc0
 #define APDS9301_WRITE_BYTE_CONTROL_REGISTER		0xc0
 #define APDS9301_ACTIVATE							0x03

#define LIGHT_SENSOR_ADDRESS			0x39		//Floating

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
	address = 40;	//For testing only

	  /* Read control register */
//	  tmp = SENSOR_IO_Read_I2C1(address, APDS9301_READ_BYTE_CONTROL_REGISTER);

	  /* Enable BDU */
	  //tmp &= ~HTS221_BDU_MASK;
	  //tmp |= (1 << HTS221_BDU_BIT);

	  /* Set default ODR */
	  //tmp &= ~HTS221_ODR_MASK;
	  //tmp |= (uint8_t)0x01; /* Set ODR to 1Hz */

	  /* Activate the device */
	  //tmp |= HTS221_PD_MASK;

	  while(1){
	   /*Apply settings to control register: activate light sensor*/
		  SENSOR_IO_I2C1_Write((address << 1), APDS9301_WRITE_BYTE_CONTROL_REGISTER, APDS9301_ACTIVATE);
	  }
	  /*Check to make sure that the write was successful*/
	  tmp = SENSOR_IO_Read_I2C1(address, APDS9301_READ_BYTE_CONTROL_REGISTER);
}

float read_light_sensor_data(uint16_t address){

	return 0.0;
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
