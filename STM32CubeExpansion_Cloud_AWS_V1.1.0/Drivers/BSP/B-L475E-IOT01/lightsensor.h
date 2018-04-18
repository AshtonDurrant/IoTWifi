/*This is my custom header file for interfacing with the light sensor.
 * This sensor will use I2C1 to connect to the STM32L475.
 * lightsensor.h
 *
 * Created on: Apr 2, 2018
 * Author: Ashton
 */

#ifndef LIGHTSENSOR_H_
#define LIGHTSENSOR_H_

#include <stdio.h>	//For access to types (uint16_t)

//Defines for ADPS-9301 control and data registers
#define APDS9301_CONTROL 		0x0
#define APDS9301_TIMING 		0x1
#define APDS9301_THRESHLOWLOW 	0x2
#define APDS9301_THRESHLOWHIGH 	0x3
#define APDS9301_THRESHHIGHLOW 	0x4
#define APDS9301_THRESHHIGHHIGH 0x5
#define APDS9301_INTERRUPT 		0x6
#define APDS9301_ID 			0xa
#define APDS9301_DATA0LOW 		0xc
#define APDS9301_DATA0HIGH 		0xd
#define APDS9301_DATA1LOW 		0xe
#define APDS9301_DATA1HIGH 		0xf

#define APDS9301_READ_CONTROL_REGISTER	0xc0


typedef struct
{
	void	(*Init)(uint16_t);	//Initialization function (16-bit value is address)
	float	(*ReadData)(uint16_t);	//Reads data. 16-bit value is address of sensor.
}LSensor_DriverTypeDef;

void init_light_sensor(uint16_t address);	//Initializes the light sensor.

float read_light_sensor_data(uint16_t address);	//Reads data from the light sensor.

#endif /* APPLICATION_COMMON_LIGHTSENSOR_H_ */
