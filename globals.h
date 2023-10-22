#ifndef GLOBALS_H
	#define GLOBALS_H
#endif

#ifndef Shivam_TXT_H
	#include <Shivam_TXT.h>
#endif

#ifndef Shivam_PicoW_TFT_H
	#include <Shivam_PicoW_TFT.h>
#endif


#ifndef SENSOR_CURRENT_H
	#include "sensor_current.h"
#endif


#ifndef SENSOR_LIGHT_H
	#include "sensor_light.h"
#endif


// Init current sensor
Adafruit_INA219 ina219;

// // Init light sensor
// hp_BH1750 BH1750;
bool bh1750_avail=false;
float curr_lux=0;
float prev_lux=0;


// Init TFT
Shivam_PicoW_TFT myTFT = Shivam_PicoW_TFT(PWM_LVL,SCR_ROT);

uint16_t new_pwm;

// Touchscreen control
int touch_countdown = 0;
uint touch_count = 0;
bool clear_screen=false;

// Display control
#define NUM_SCREENS 3
#define SCR_CHANGE_TIME 1*60*1000
uint8_t screen_num = 1;
uint8_t prev_screen_num = screen_num;
uint16_t prev_screen_change_time=0;
bool cycle_screens = false;
bool init_screen=false;

// Timing
unsigned long prevMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
