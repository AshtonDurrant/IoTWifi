/*This is my custom header file for interfacing with the light sensor.
 * This sensor will use I2C1 to connect to the STM32L475.
 * lightsensor.h
 *
 * Created on: Apr 2, 2018
 * Author: Ashton
 */

#ifndef APPLICATION_COMMON_LIGHTSENSOR_H_
#define APPLICATION_COMMON_LIGHTSENSOR_H_

void activate_light_sensor(void);	//Activates the light sensor so that readings will begin.

#endif /* APPLICATION_COMMON_LIGHTSENSOR_H_ */
