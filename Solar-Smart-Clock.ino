// Custom libs
#include <hp_BH1750.h>  //inlude the library
// #include <Shivam_TXT.h>
#include "string_defs.h"
// #include <Shivam_PicoW_TFT.h>
#include <Adafruit_INA219.h>
#include "time_stuff.h"
#include "piezo_stuff.h"
// #include "types.h"
#include "TS_params.h"

// #define OFFSET_T_SECS 11*60*60+41*60+15*60

// Init current sensor
Adafruit_INA219 ina219;

// Init light sensor
hp_BH1750 BH1750;
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
uint16_t prev_screen_change_time=0;
bool cycle_screens = false;

// Timing
unsigned long prevMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

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


	bh1750_avail = BH1750.begin(BH1750_TO_GROUND);
	if (bh1750_avail)
		BH1750.start();

	// Measure room brightness
	light_sensor_async_read();
	pwm_str.print_tft_ralign(String(PWM_LVL)+"/"+String(curr_lux));

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

	// -------- Manage touch input -------- //
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
	// ------------------------------------ //

	// Update time display every 1 second
	if (prev_ss!=ss){

		check_screen_brightness();

		// Measure room brightness
		light_sensor_async_read();

		pwm_str.print_tft_ralign(String(PWM_LVL)+"/"+String(curr_lux));

		if (prev_ss==59){
			check_clear_screen();
		}

		// Print everything else
		switch (screen_num){
			case 0:{
				print_second();
				// Print current time
				if (prev_MM!=MM){
					print_HH_MM();
					// touch_count_str.print_tft_middle(String(touch_count));
				}
				// Update every 1 hour
				if (prev_HH!=HH){
					single_beep();
					print_am_pm();
				}
				screen_zero(TimeStamp,localTime);
				break;
			}
			case 1:
			{
				if (prev_ss==59){ 							// Update time diff every 1 minute
					screen_one(TimeStamp,localTime,0);
				}
				else{
					// // Calc time diff from event
					// uint16_t index=calc_time_diff(TimeStamp);
					// String event_name = get_event_type(index%NUM_ITEMS_PER_DAY);
					// time_t inTime = convert_to_local_time(TimeStamp);
					// int diff = inTime - october_2023[index];	// Both are in UTC
					// Serial.println(secondsToHMS(abs(diff))+" << "+String(abs(diff)+TIME_ZONE_DIFF_SECS));
					// // if (abs(diff)<20000)
					// // 	mins_remaining.print_tft(secondsToSS(abs(diff)));
				}
				print_time_screen_one();
				break;
			}
			case 2:
			{
				if (prev_ss==59){ 							// Update time diff every 1 minute
					screen_one(TimeStamp,localTime,1);
				}
				else {
					print_time_screen_one();
				}
				break;
			}
		}
	}
	update_previous_vals();
}

// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
void print_HH_MM(){
	// Print current time
	char buf[40];
	// sprintf(buf, "%02d:%02d:%02d %cM\n",HH,MM,ss,am_pm);
	sprintf(buf, "%02d:%02d\n",HH,MM);
	timestr.print_tft(buf);
	// timestr.print_tft_ralign(buf);
}

void print_second(){
	char ss_str[20];
	sprintf(ss_str, "%02d",ss);
	sec_str.print_tft_ralign(ss_str);
}

void print_am_pm(){
	if (AM)
		am_pm_str.print_tft_ralign("AM");
	else
		am_pm_str.print_tft_ralign("PM");
}
// -----------------------------------------------------------------------------

void init_screens(uint8_t screen_num,time_t TimeStamp,time_t localTime)
/* Setup */
{

	// Serial.println("screen_num: "+String(screen_num));
	myTFT.tft.fillScreen(HX8357_BLACK);

	// Print everything else
	switch (screen_num){
		case 0: {
			print_second();
			print_HH_MM();
			print_am_pm();
			print_current();
			screen_zero(TimeStamp,localTime);
			print_info (TimeStamp, localTime);
			print_day_date(localTime);
			break;
		}
		case 1: {
			// print_time_screen_one();
			screen_one(TimeStamp,localTime,screen_num-1);
			break;
		}
		case 2: {
			// print_time_screen_one();
			screen_one(TimeStamp,localTime,screen_num-1);
			break;
		}
	}
}

void screen_zero(time_t TimeStamp,time_t localTime)
/* Shows the next 5 events */
{
	// Print voltage-current
	print_current();

	// Update time diff every 1 minutec
	if (prev_ss==59){
		print_info (TimeStamp, localTime);
		print_day_date(localTime);
	}
}

void screen_one(time_t TimeStamp,time_t localTime, uint16_t index_offset)
/* Shows only the (next+offset) event */
{
	print_time_screen_one();

	// Print voltage-current
	print_current();

	uint16_t index=calc_time_diff(TimeStamp);
	index+=index_offset;

	// Calc time diff from event
	String event_name = get_event_type(index%NUM_ITEMS_PER_DAY);
	time_t inTime = convert_to_local_time(TimeStamp);
	int diff = inTime - october_2023[index];	// Both are in UTC

	// // Line 1
	// if (abs(diff)<120){
	// 		mins_remaining.print_tft(String(abs(diff))+"seconds");
	// 	}
	// else
		print_fancy_time_remain(diff);

	// Line 2
	event_name_str.print_tft_middle(event_name);

	// Line 3.1
	TILL_label.print_tft("till");
	time_t TT=convert_to_local_time(october_2023[index]);
	next_event_str.print_tft_middle(get_readable_time(TT));

	// Line 3.2
	date_str_scr_one.print_tft(get_readable_day(localTime)+", "+get_readable_date(localTime));

	// Show index# for event type
	Shivam_TXT index_str=Shivam_TXT(0,SCREEN_H-35,CURRENT_STR_FONT,1,BOTTOM_BAR_COLOR,BG_COLOR);
	index_str.begin(myTFT.tft);
	index_str.print_tft_no_prev_bg("screen: "+String(screen_num));
}

void print_fancy_time_remain(int diff)
/* Print remaining time in staggered order*/
{
	String HH=secondsToHH(abs(diff));
	String MM=secondsToMM(abs(diff));
	String hms;

	if (MM=="00" && prev_MM!=0)	//Beep if time remaining is round number
		single_beep();

	if (HH=="00"){
			if (prev_HH!=0)
				clear_screen=true;
			if (MM[0]=='0')
				hms=String(MM[1])+" minutes";
			else
				hms=String(MM)+" minutes";

			time_remaining.print_tft_middle(hms);
	}
	else {
		hours_remaining.print_tft(HH);
		if (HH=="01")
			hours.print_tft("hour");
		else
			hours.print_tft("hours");
		mins_remaining.print_tft(MM);
		minutes.print_tft("minutes");
	}
}

void print_time_screen_one(){
	// Print current time
	char buf[40];
	sprintf(buf, "%02d:%02d:%02d %cM\n",HH,MM,ss,am_pm);

	// timestr_screen_one.print_tft_middle(buf);
	timestr_screen_one.print_tft_ralign(buf);
}
// -----------------------------------------------------------------------------
void print_concise(uint16_t index, time_t inTime, uint16_t col, uint16_t row)
/* Print time remaining till next event */
{
	String event_name = get_event_type(index%NUM_ITEMS_PER_DAY);
	inTime = convert_to_local_time(inTime);
	int diff = inTime - october_2023[index];	// Both are in UTC

	String hms=secondsToHM(abs(diff));
	String temp_str=hms+" till "+event_name+" ";

	// add padding for alignment
	if (temp_str.length()<PADDING_VAL)
		for (int i=0;i<(temp_str.length()-(PADDING_VAL));i++)
			temp_str+=" ";

	Shivam_TXT outstr=Shivam_TXT(col,row,FreeMonoBold9pt7b,1,TXT_COLOR,BG_COLOR);
	outstr.begin(myTFT.tft);

	outstr.print_tft_no_prev_bg(temp_str+get_readable_datetime(convert_to_local_time(october_2023[index])));
}

void print_info(time_t TimeStamp, time_t localTime)
/* Print current day/date/time */
{
	uint16_t index=calc_time_diff(TimeStamp);
	int row=100;
	for (uint8_t i=0;i<NUM_FUTURE_EVENTS;i++){
		print_concise(index+i, TimeStamp,0,row);
		row+=LINE_SPACING+20;
	}
}

void print_day_date(time_t localTime){
	day_str.print_tft(get_readable_day(localTime));
	date_str.print_tft(get_readable_date(localTime));
}

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

void print_current(void)
/* Print current & voltage */
{
	float shuntvoltage = 0;
	float busvoltage = 0;
	float current_mA = 0;
	float loadvoltage = 0;
	float power_mW = 0;

	shuntvoltage = ina219.getShuntVoltage_mV();
	busvoltage = ina219.getBusVoltage_V();
	current_mA = ina219.getCurrent_mA();
	power_mW = ina219.getPower_mW();
	loadvoltage = busvoltage + (shuntvoltage / 1000);

	char buf[40];
	sprintf(buf,"%.2fV     %.2fmA      %.2fmW",loadvoltage,current_mA,power_mW);

	current_str.print_tft(buf);
}