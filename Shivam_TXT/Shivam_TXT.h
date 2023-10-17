#pragma once
#ifndef Shivam_TXT_h
#define Shivam_TXT_h

#include "Arduino.h"
#include <Shivam_PicoW_TFT.h>

class Shivam_TXT {

	private:
		int16_t x,y;
		uint16_t w,h;
		uint16_t prev_w,prev_h;
		uint8_t font_size;
		int txt_color,bg_color;
		GFXfont font;
		Adafruit_HX8357 tft = Adafruit_HX8357(1, 1, 1);

	public:
		Shivam_TXT(int16_t x,  int16_t y, GFXfont font, uint8_t font_size, int txt_color, int bg_color);

		void begin(Adafruit_HX8357 tft);

		// Update width and height of text
		void update_wh(String in_text);

		// Printing functions
		void print_tft_ralign(String in_text);
		void print_tft(String in_text);
		void print_tft_no_prev_bg(String in_text);
		void print_tft_middle(String in_text);

		// Fill BG behind prev text
		void fill_background(int x,int y);
};

#endif