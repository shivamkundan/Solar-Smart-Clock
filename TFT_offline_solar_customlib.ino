// Custom libs
#include "libs.h"

// #define OFFSET_T_SECS 11*60*60+41*60+15*60

void setup(){
	prevMillis=millis();
	ts.pressureThreshhold=TOUCH_PRESSURE_THRESHOLD;
	Serial.begin(115200);
	init_piezo();
	init_tft();
	myRTC.begin();
	ina219.begin();
	init_TXTs(myTFT.tft);
	init_clock();

	// Measure room brightness
	light_sensor_async_read();
	pwm_str.print_tft_ralign(String(PWM_LVL)+"/"+String(curr_lux));
	check_screen_brightness();
}

void loop(){

	// Update time
	time_t TimeStamp = get_rtc_time();
	#ifdef OFFSET_T_SECS
		TimeStamp=TimeStamp+OFFSET_T_SECS;
	#endif
	time_t localTime = convert_to_local_time(TimeStamp);
	get_individual_time(localTime, HH, MM, ss, AM);

	// Cycle screens
	if (cycle_screens)
		screen_change(TimeStamp, localTime);

	//  Manage touch input
	touch_controller(TimeStamp,localTime);

	// ------------------------------------ //
	// Update time display every 1 second
	if (prev_ss!=ss){

		if (prev_screen_num!=screen_num)
			init_screen=true;
		prev_screen_num=screen_num;

		// Check for clear_flag signal
		if (prev_ss==59)
			check_clear_screen();

		// Print voltage-current
		current_str.print_tft(print_current_byref(&ina219));

		// beep
		if (prev_HH!=HH)
			single_beep();

		// touch_count_str.print_tft_middle(String(touch_count));

		// Print everything else
		switch (screen_num){
			case SCREEN_NEXT_FIVE_EVENTS:{
				screen_zero(TimeStamp,localTime);
				break;}
			case SCREEN_NEXT_EVENT:{
				screen_one(TimeStamp,localTime,0);
				break;}
			case SCREEN_NEXT_PLUS_1_EVENT:{
				screen_one(TimeStamp,localTime,1);
				break;}
		}
	}
	update_previous_vals();
}

// ----------------------------------------------------------------------------- //
// ------------------------------- Touch Controls ------------------------------ //
void touch_controller(time_t TimeStamp,time_t localTime){
	if (touch_countdown==0){
		TSPoint p = ts.getPoint();
		if (p.z > ts.pressureThreshhold) {
			touch_countdown=5;
			clear_screen=true;
			touch_count+=1;
			increment_screen();
			init_screens(screen_num,TimeStamp-60,localTime-60);
			show_touch_point(p.x, p.y, p.z,myTFT.tft);
			touch_count_str.print_tft_middle(String(touch_count));
		}
	}
	// Debouncing
	if (touch_countdown>0){
		touch_countdown-=1;
		delay(100);
	}
}

// ----------------------------------------------------------------------------- //
// ------------------------- Screen Display Management ------------------------- //
void screen_change(time_t TimeStamp, time_t localTime)
/* Cycles screens every SCR_CHANGE_TIME seconds */
{
	currentMillis=millis();
	if (currentMillis-prevMillis > SCR_CHANGE_TIME){
		prevMillis=currentMillis;
		increment_screen();
		init_screens(screen_num,TimeStamp-60,localTime-60);
	}
}

void increment_screen()
/* Circular increment */
{

	if (screen_num<NUM_SCREENS-1)
		screen_num+=1;
	else if (screen_num==NUM_SCREENS-1)
		screen_num=0;
}

void check_clear_screen()
/* Clears screen when "clear_screen" is set */
{
	if (clear_screen){
		myTFT.clear_screen(BG_COLOR);
		clear_screen=false;
	}
}

// ----------------------------------------------------------------------------- //
// ----------------------- Screen Brightness Management ------------------------ //
void update_screen_brightness(uint16_t new_pwm)
/* Set new PWM level */
{
	gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
	uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
	pwm_set_wrap(slice_num, PWM_LIMIT);
	pwm_set_chan_level(slice_num, PWM_CHAN_A, new_pwm);
	pwm_set_enabled(slice_num, true);
	PWM_LVL=new_pwm;
}

void check_screen_brightness()
/* Change screen brightness according to curr_lux */
{
	if (curr_lux<MIN_LUX){
		new_pwm=MIN_PWM;
		update_screen_brightness(new_pwm);
		pwm_str.print_tft_ralign(String(PWM_LVL)+"/"+String(curr_lux));
	}
	else if (curr_lux>MAX_LUX){
		new_pwm=MAX_PWM;
		update_screen_brightness(new_pwm);
		pwm_str.print_tft_ralign(String(PWM_LVL)+"/"+String(curr_lux));
		}
	else {
		float light_d=curr_lux-prev_lux;
		if (abs(light_d)>MIN_LIGHT_DIFF){
			new_pwm=map(curr_lux,MIN_LUX,MAX_LUX,MIN_PWM,MAX_PWM);
			update_screen_brightness(new_pwm);
			pwm_str.print_tft_ralign(String(PWM_LVL)+"/"+String(curr_lux));
		}
	}
}

void light_sensor_async_read()
/* Read ambient brightness from BH1750 */
{
	if (bh1750_avail){
		prev_lux=curr_lux;
		if (BH1750.hasValue() == true) {    // non blocking reading
			curr_lux = BH1750.getLux();
			// Serial.println(lux);
			BH1750.start();
		}

	}
}

// ----------------------------------------------------------------------------- //
void init_tft()
/* Initialize LCD display */
{
	myTFT.begin();
	myTFT.clear_screen(BG_COLOR);
}

void init_clock()
/* Initial time stamp */
{
	time_t TimeStamp = get_rtc_time();
	#ifdef OFFSET_T_SECS
		TimeStamp=TimeStamp+OFFSET_T_SECS;
	#endif
	time_t localTime = convert_to_local_time(TimeStamp);
	get_individual_time(localTime, HH, MM, ss, AM);
	update_am_pm();

	TimeStamp-=60;
	localTime-=60;

	init_screens(screen_num,TimeStamp,localTime);
	update_previous_vals();
}

// ----------------------------------------------------------------------------- //
// ----------------------------- Time Management ------------------------------- //
void update_am_pm()
/* For 12-hour format */
{
	single_beep();
	if (AM)
		am_pm='A';
	else
		am_pm='P';
}

void update_previous_vals()
/* Saves previous values of hh,mm,ss */
{
	prev_ss=ss;
	prev_MM=MM;
	prev_HH=HH;
}
