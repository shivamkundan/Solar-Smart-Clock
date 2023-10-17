
#pragma once
#ifndef Shivam_PicoW_TFT_h
#define Shivam_PicoW_TFT_h

#include "Arduino.h"
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define TFT_CS 17
#define TFT_DC 15
#define TFT_RST -1 // RST can be set to -1 if you tie it to Arduino's reset
#define PWM_PIN 0

#define SCREEN_H 320
#define SCREEN_W 480

// ============================== //
class Shivam_PicoW_TFT
{
	public:

		Shivam_PicoW_TFT(uint16_t pwm_lvl, uint8_t scr_rot);

		void begin();

		void set_pwm_lvl(uint16_t pwm_lvl);

		void start_pwm();

		void println(String in_text);

		void clear_screen(int bg_color);

		Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

	private:
		uint16_t pwm_lvl; // Screen brightness
		uint8_t scr_rot;

};

// ============================== //

#endif
