#include "Shivam_TXT.h"

Shivam_TXT::Shivam_TXT(int16_t x,  int16_t y, GFXfont font, uint8_t font_size, int txt_color, int bg_color)
/* Constructor */
{
	this->x = x;
	this->y = y;
	this->font=font;
	this->font_size=font_size;
	this->txt_color=txt_color;
	this->bg_color=bg_color;
	this->w=0;
	this->h=0;
}

void Shivam_TXT::begin(Adafruit_HX8357 tft)
/* Begin */
{
	this->tft=tft;
}

void Shivam_TXT::update_wh(String in_text)
/* Update width and height of text */
{
	this->prev_w=this->w;
	this->prev_h=this->h;

	int16_t x,y;
	uint16_t w, h;

	this->tft.setTextSize(font_size);
	this->tft.setFont(&font);
	this->tft.setTextColor(this->txt_color, this->bg_color);

	this->tft.getTextBounds(in_text, 0, 0, &x, &y, &w, &h);

	this->w=w;
	this->h=h;
}

void Shivam_TXT::print_tft_ralign(String in_text)
/* Right align text -> print to TFT display */
{
	update_wh(in_text);
	// Serial.println(String(this->x)+","+String(this->y));
	this->tft.setCursor(SCREEN_W-this->w-5,this->y);
	this->tft.setFont(&font);
	this->tft.setTextSize(font_size);
	fill_background(SCREEN_W-this->w-5,this->y);
	this->tft.println(in_text);
}

void Shivam_TXT::print_tft_middle(String in_text)
/* Right align text -> print to TFT display */
{
	update_wh(in_text);
	// Serial.println(String(this->x)+","+String(this->y));
	// Serial.println(String(SCREEN_W/2-(int)((this->w-10)/2)));

	uint col_num=SCREEN_W/2-(int)((this->w-10)/2);
	this->tft.setCursor(col_num,this->y);

	this->tft.setFont(&font);
	this->tft.setTextSize(font_size);
	// fill_background(SCREEN_W-this->w-10,this->y);

	// fill_background(col_num,this->y);
	this->tft.fillRect(col_num-20,y-this->h, this->w+5+40, this->h+5, this->bg_color);
	this->tft.println(in_text);
}

void Shivam_TXT::print_tft(String in_text)
/* Prints to TFT display */
{
	update_wh(in_text);

	this->tft.setCursor(this->x,this->y);
	this->tft.setFont(&font);
	fill_background(this->x,this->y);

	this->tft.println(in_text);
}

void Shivam_TXT::print_tft_no_prev_bg(String in_text)
/* Prints to TFT display without updating BG */
{
	update_wh(in_text);

	this->tft.setCursor(this->x,this->y);
	this->tft.setFont(&font);
	this->tft.fillRect(x,y-h, this->w+5, this->h+5, this->bg_color);
	this->tft.println(in_text);
}

void Shivam_TXT::fill_background(int x,int y)
/* Fill BG behind prev text */
{
	// Serial.println("fill_background");
	this->tft.fillRect(x-7,y-h-7, this->prev_w+15, this->h+15, this->bg_color);
}