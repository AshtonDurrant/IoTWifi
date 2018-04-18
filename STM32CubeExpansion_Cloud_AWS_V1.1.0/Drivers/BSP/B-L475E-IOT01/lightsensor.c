/* This will deal with communicating with the APDS-9301 ambient light sensor.
 * This file contains the function calls associated with sensor communication.
 * This is modeled after existing ARM code.
 * lightsensor.c
 *
 * Created on: Apr 2, 2018
 * Author: Ashton
 */

#include "lightsensor.h"


LSensor_DriverTypeDef LSensorDriver = {
		init_light_sensor,
		read_light_sensor_data
};

/*This function initializes the APDS-9301 ambient light sensor. This is modeled heavily
 * after HTS221_T_Init.
 * @param address:	I2C device address of the light sensor
 */
void init_light_sensor(uint16_t address){
	uint8_t tmp;

	  /* Read control register */
	  tmp = SENSOR_IO_Read_I2C1(address, ADPS9301_READ_CONTROL_REGISTER);

	  /* Enable BDU */
	  //tmp &= ~HTS221_BDU_MASK;
	  //tmp |= (1 << HTS221_BDU_BIT);

	  /* Set default ODR */
	  //tmp &= ~HTS221_ODR_MASK;
	  //tmp |= (uint8_t)0x01; /* Set ODR to 1Hz */

	  /* Activate the device */
	  //tmp |= HTS221_PD_MASK;

	  /* Apply settings to CTRL_REG1 */
	  //SENSOR_IO_Write(DeviceAddr, HTS221_CTRL_REG1, tmp);
}

float read_light_sensor_data(uint16_t address){

	return 0.0;
}

