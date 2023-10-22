#ifndef SCREENS_H
	#define SCREENS_H
#endif

#ifndef STRING_DEFS_H
	#include "string_defs.h"
#endif

#ifndef TIME_STUFF_H
	#include "time_stuff.h"
#endif

#ifndef GLOBALS_H
	#include "globals.h"
#endif

#ifndef Shivam_TXT_H
	#include <Shivam_TXT.h>
#endif

#define SCREEN_NEXT_FIVE_EVENTS 0
#define SCREEN_NEXT_EVENT 1
#define SCREEN_NEXT_PLUS_1_EVENT 2

void print_fancy_time_remain(int diff);
void print_time_screen_one();
void screen_one_main(time_t TimeStamp,time_t localTime, uint16_t index_offset);
void screen_one(time_t TimeStamp,time_t localTime, uint16_t index_offset);

void print_concise(uint16_t index, time_t inTime, uint16_t col, uint16_t row);
void print_info(time_t TimeStamp, time_t localTime);
void screen_zero_main(time_t TimeStamp,time_t localTime);
void screen_zero(time_t TimeStamp,time_t localTime);

void print_HH_MM();
void print_second();
void print_am_pm();
void print_day_date(time_t localTime);
void init_screens(uint8_t screen_num,time_t TimeStamp,time_t localTime);

// ----------------------------------------------------------------------------- //
// Helpers

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

void print_day_date(time_t localTime)
/* Show day+date on top left of screen */
{
	day_str.print_tft(get_readable_day(localTime));
	date_str.print_tft(get_readable_date(localTime));
}

// ----------------------------------------------------------------------------- //

void screen_zero(time_t TimeStamp,time_t localTime)
/* Shows the next 5 events */
{
	if (init_screen){
		init_screen=false;
		print_HH_MM();
		print_am_pm();
	}

	print_second();
	if (prev_MM!=MM)
		print_HH_MM();
	if (prev_HH!=HH)
		print_am_pm();

	// Update time diff every 1 minutec
	if (prev_ss==59){
		screen_zero_main(TimeStamp, localTime);
	}
}

void screen_zero_main(time_t TimeStamp,time_t localTime){
	print_info (TimeStamp, localTime);
	print_day_date(localTime);
}

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

// ----------------------------------------------------------------------------- //
void screen_one(time_t TimeStamp,time_t localTime, uint16_t index_offset)
/* Shows only the (next+offset) event */
{
	print_time_screen_one();
	if (prev_ss==59)
		screen_one_main(TimeStamp,localTime, index_offset);
}

void screen_one_main(time_t TimeStamp,time_t localTime, uint16_t index_offset){

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

void print_time_screen_one()
/* Shows current time in small font */
{
	// Print current time
	char buf[40];
	sprintf(buf, "%02d:%02d:%02d %cM\n",HH,MM,ss,am_pm);

	// timestr_screen_one.print_tft_middle(buf);
	timestr_screen_one.print_tft_ralign(buf);
}

// ----------------------------------------------------------------------------- //
void init_screens(uint8_t screen_num,time_t TimeStamp,time_t localTime)
/* Setup */
{

	// Serial.println("screen_num: "+String(screen_num));
	myTFT.tft.fillScreen(HX8357_BLACK);

	// Print everything else
	switch (screen_num){
		case SCREEN_NEXT_FIVE_EVENTS: {
			screen_zero_main(TimeStamp, localTime);
			screen_zero(TimeStamp,localTime);
			break;
		}
		case SCREEN_NEXT_EVENT: {
			screen_one_main(TimeStamp,localTime,screen_num-1);
			screen_one(TimeStamp,localTime,screen_num-1);
			break;
		}
		case SCREEN_NEXT_PLUS_1_EVENT: {
			// print_time_screen_one();
			screen_one_main(TimeStamp,localTime,screen_num-1);
			screen_one(TimeStamp,localTime,screen_num-1);
			break;
		}
	}
}