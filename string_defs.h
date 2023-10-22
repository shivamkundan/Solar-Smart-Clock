#ifndef STRING_DEFS_H
	#define STRING_DEFS_H
#endif

#ifndef Shivam_TXT_H
	#include <Shivam_TXT.h>
#endif

#ifndef TYPES_H
	#include "types.h"
#endif

// Init TXT objects
Shivam_TXT timestr = Shivam_TXT(185,62,MAIN_TIME_FONT,1,TXT_COLOR,BG_COLOR);
Shivam_TXT sec_str = Shivam_TXT(390,40,SECOND_FONT,1,TXT_COLOR,BG_COLOR);
Shivam_TXT am_pm_str = Shivam_TXT(400,62,AM_PM_FONT,1,TOP_BAR_COLOR,BG_COLOR);

Shivam_TXT timestr_screen_one = Shivam_TXT(0,15,MAIN_TIME_FONT_SCR_ONE,1,TOP_BAR_COLOR,BG_COLOR);

Shivam_TXT current_str=Shivam_TXT(0,SCREEN_H-10,CURRENT_STR_FONT,1,BOTTOM_BAR_COLOR,BG_COLOR);
Shivam_TXT day_str=Shivam_TXT(0,25,DAY_STR_FONT,1,TOP_BAR_COLOR,BG_COLOR);
Shivam_TXT date_str=Shivam_TXT(0,60-1,DATE_STR_FONT,1,TOP_BAR_COLOR,BG_COLOR);

Shivam_TXT touch_count_str=Shivam_TXT(0,25,DATE_STR_FONT,1,TOP_BAR_COLOR,BG_COLOR);
// ---
uint16_t start_row=135;
Shivam_TXT time_remaining=Shivam_TXT(100,start_row,TIME_REMAIN_FONT_SCR_ONE,1,TXT_COLOR,BG_COLOR);
Shivam_TXT event_name_str=Shivam_TXT(100,start_row+80,EVENT_NAME_SCR_ONE,1,TXT_COLOR,BG_COLOR);
Shivam_TXT next_event_str=Shivam_TXT(100,start_row+80+50+5,EVENT_TIME_SCR_ONE,1,EEE,BG_COLOR);
Shivam_TXT date_str_scr_one=Shivam_TXT(0,15,DATE_SCR_ONE,1,TOP_BAR_COLOR,BG_COLOR);

Shivam_TXT TILL_label=Shivam_TXT(30,210,TILL_STR_SIZE,1,EEE,BG_COLOR);
// ---
#define START_ROW 100
#define START_COL 75

Shivam_TXT hours_remaining=Shivam_TXT(START_COL,START_ROW,TIME_REMAIN_FONT_SCR_ONE,1,TXT_COLOR,BG_COLOR);
Shivam_TXT hours=Shivam_TXT(START_COL+100,START_ROW-30,FreeMonoBold18pt7b,1,TXT_COLOR,BG_COLOR);

Shivam_TXT mins_remaining=Shivam_TXT(START_COL+100,START_ROW+55,FreeMonoBold38pt7b,1,TXT_COLOR,BG_COLOR);
Shivam_TXT minutes=Shivam_TXT(START_COL+100+100,START_ROW+55-25,FreeMonoBold16pt7b,1,TXT_COLOR,BG_COLOR);


Shivam_TXT center_time=Shivam_TXT(START_COL+100+100,START_ROW+55-25+50,TIME_REMAIN_FONT_SCR_ONE,1,HX8357_CYAN,BG_COLOR);

Shivam_TXT pwm_str=Shivam_TXT(0,SCREEN_H-10,CURRENT_STR_FONT,1,BOTTOM_BAR_COLOR,BG_COLOR);

#define NUM_STRINGS 19
Shivam_TXT* LL[NUM_STRINGS]={&timestr, &sec_str,&am_pm_str,
							 &current_str,
							 &day_str,&date_str,
							 &timestr_screen_one,
							 &touch_count_str,
							 &hours_remaining,&hours,&mins_remaining,&minutes,
							 &time_remaining,&event_name_str,&next_event_str,&date_str_scr_one,
							 &pwm_str,&center_time,
							 &TILL_label
						};

void init_TXTs(Adafruit_HX8357 tft)
/* Initializes all TXT objects */
{

	for (uint8_t i=0;i<NUM_STRINGS;i++)
		LL[i]->begin(tft);
}