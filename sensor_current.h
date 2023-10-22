#ifndef SENSOR_CURRENT_H
	#define SENSOR_CURRENT_H
#endif

#ifndef _LIB_ADAFRUIT_INA219_
	#include <Adafruit_INA219.h>
#endif

// // Init current sensor
// Adafruit_INA219 ina219;

String print_current(void)
/* Print current & voltage */
{
// 	float shuntvoltage = 0;
// 	float busvoltage = 0;
// 	float current_mA = 0;
// 	float loadvoltage = 0;
// 	float power_mW = 0;

// 	shuntvoltage = ina219.getShuntVoltage_mV();
// 	busvoltage = ina219.getBusVoltage_V();
// 	current_mA = ina219.getCurrent_mA();
// 	power_mW = ina219.getPower_mW();
// 	loadvoltage = busvoltage + (shuntvoltage / 1000);

// 	char buf[40];
// 	sprintf(buf,"%.2fV     %.2fmA      %.2fmW",loadvoltage,current_mA,power_mW);

	return "buf";
	// return buf;
}


String print_current_byref(Adafruit_INA219* ina219)
/* Print current & voltage */
{
	float shuntvoltage = 0;
	float busvoltage = 0;
	float current_mA = 0;
	float loadvoltage = 0;
	float power_mW = 0;

	shuntvoltage = ina219->getShuntVoltage_mV();
	busvoltage = ina219->getBusVoltage_V();
	current_mA = ina219->getCurrent_mA();
	power_mW = ina219->getPower_mW();
	loadvoltage = busvoltage + (shuntvoltage / 1000);

	char buf[40];
	sprintf(buf,"%.2fV     %.2fmA      %.2fmW",loadvoltage,current_mA,power_mW);

	return buf;
}
