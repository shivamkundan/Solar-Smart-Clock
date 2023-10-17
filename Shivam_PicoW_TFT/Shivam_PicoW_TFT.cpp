#include "Shivam_PicoW_TFT.h"

Shivam_PicoW_TFT::Shivam_PicoW_TFT(uint16_t pwm_lvl, uint8_t scr_rot){
	this->pwm_lvl=pwm_lvl;
	this->scr_rot=scr_rot;
}

void Shivam_PicoW_TFT::begin()
{
	start_pwm();
	this->tft.begin();
	this->tft.setRotation(this->scr_rot);
}

void Shivam_PicoW_TFT::println(String in_text){
	this->tft.println(in_text);
}

void Shivam_PicoW_TFT::clear_screen(int bg_color){
	this->tft.fillScreen(bg_color);
	this->tft.setCursor(0,0);
}

void Shivam_PicoW_TFT::set_pwm_lvl(uint16_t pwm_lvl){
	this->pwm_lvl=pwm_lvl;
}

void Shivam_PicoW_TFT::start_pwm(){
	// setup pwm
	gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
	uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
	pwm_set_wrap(slice_num, 65535);
	pwm_set_chan_level(slice_num, PWM_CHAN_A, this->pwm_lvl);
	pwm_set_enabled(slice_num, true);
}