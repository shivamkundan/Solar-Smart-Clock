// See: https://www.timeanddate.com/sun/usa/carbondale

#define LEAP_YEAR(Y)     ( (Y>0) && !(Y%4) && ( (Y%100) || !(Y%400) ))     // from time-lib

int my_dayOfWeek(uint16_t year, uint8_t month, uint8_t day)
{
	uint16_t months[] =
	{
		0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 // days until 1st of month
	};

	uint32_t days = year * 365;   // days until year
	for (uint16_t i = 4; i < year; i+=4)
	{
		if (LEAP_YEAR(i)) days++;  // adjust leap years
	}

	days += months[month-1] + day;
	if ((month > 2) && LEAP_YEAR(year)) days++;
	return days % 7;
}

String get_day_name(uint8_t dayOfWeek){
	switch(dayOfWeek){
	case 1:
		return ("Sun");
	case 2:
		return ("Mon");
	case 3:
		return ("Tue");
	case 4:
		return ("Wed");
	case 5:
		return ("Thu");
	case 6:
		return ("Fri");
	case 0:
		return ("Sat");
	default:
		return("Err");
	}
}
// ==========================================

#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC

DS3232RTC myRTC;

// These vars save current time vals
uint8_t HH;
uint8_t MM;
uint8_t ss;
char am_pm;
bool AM=false;

// These vars save previous time vals
uint8_t prev_HH=99;
uint8_t prev_MM=99;
uint8_t prev_ss=99;

#define TIME_ZONE_DIFF_HOURS -5
#define TIME_ZONE_DIFF_SECS TIME_ZONE_DIFF_HOURS*3600

// ---------------- //
#define OCTOBER 10
#define NUM_DAYS_MONTH 31

#define NUM_ITEMS_PER_DAY 5 //number of time values stored for each day
const uint16_t START_INDEX=70; // to prevent reading too many old dates

#define PADDING_VAL 23
#define LINE_SPACING 20
#define NUM_FUTURE_EVENTS 5

// These values are for UTC-5
uint32_t october_2023[]={
	1696159560,1696161120,1696182360,1696203600,1696205160,
	1696245960,1696247580,1696268760,1696289880,1696291440,
	1696332420,1696334040,1696355100,1696376220,1696377780,
	1696418880,1696420440,1696441500,1696462500,1696464060,
	1696505340,1696506900,1696527900,1696548840,1696550400,
	1696591800,1696593360,1696614240,1696635120,1696636680,
	1696678260,1696679820,1696700640,1696721460,1696723020,
	1696764720,1696766280,1696787040,1696807740,1696809300,
	1696851180,1696852740,1696873440,1696894080,1696895640,
	1696937640,1696939200,1696959780,1696980360,1696981980,
	1697024040,1697025660,1697046180,1697066700,1697068260,
	1697110500,1697112120,1697132580,1697152980,1697154600,
	1697196960,1697198580,1697218980,1697239320,1697240880,
	1697283420,1697285040,1697305320,1697325660,1697327220,
	1697369880,1697371500,1697391720,1697411940,1697413560,
	1697456340,1697457900,1697478120,1697498280,1697499840,
	1697542800,1697544360,1697564520,1697584560,1697586180,
	1697629260,1697630820,1697650920,1697670900,1697672520,
	1697715720,1697717280,1697737260,1697757240,1697758800,
	1697802180,1697803740,1697823660,1697843520,1697845140,
	1697888640,1697890200,1697910060,1697929860,1697931480,
	1697975100,1697976660,1697996460,1698016200,1698017760,
	1698061560,1698063120,1698082860,1698102540,1698104100,
	1698148020,1698149580,1698169260,1698188820,1698190440,
	1698234480,1698236040,1698255600,1698275160,1698276780,
	1698320880,1698322500,1698342000,1698361500,1698363120,
	1698407340,1698408960,1698428400,1698447840,1698449460,
	1698493800,1698495420,1698514800,1698534120,1698535740,
	1698580260,1698581880,1698601200,1698620460,1698622080,
	1698666720,1698668340,1698687600,1698706800,1698708420,
	1698753180,1698754800,1698774000,1698793140,1698794760
};

String get_event_type(uint8_t event_type){
	String outstr;

	switch (event_type){
		case 0:
			return "first light";
		case 1:
			return "sunrise";
		case 2:
			return "solar noon";
		case 3:
			return "sunset";
		case 4:
			return "last light";
	}
	return ("err: "+String(event_type));
}
// -----------------------------------------------------------------------------
String secondsToHMS( const uint32_t seconds)//, uint16_t &h, uint8_t &m, uint8_t &s )
{
	uint32_t t = seconds + (TIME_ZONE_DIFF_SECS);

	uint16_t s = t % 60;

	t = (t - s)/60;
	uint16_t m = t % 60;

	t = (t - m)/60;
	uint16_t h = t;

	char now_time_str[40];
	sprintf(now_time_str,"%02d:%02d:%02d",h,m,s);

	return now_time_str;
}

String secondsToHM( const uint32_t seconds)//, uint16_t &h, uint8_t &m, uint8_t &s )
{
	uint32_t t = seconds + (TIME_ZONE_DIFF_SECS);

	uint16_t s = t % 60;

	t = (t - s)/60;
	uint16_t m = t % 60;

	t = (t - m)/60;
	uint16_t h = t;

	char now_time_str[80];
	sprintf(now_time_str,"%02d:%02d",h,m);

	return now_time_str;
}

String secondsToHH( const uint32_t seconds)//, uint16_t &h, uint8_t &m, uint8_t &s )
{
	uint32_t t = seconds + (TIME_ZONE_DIFF_SECS);

	uint16_t s = t % 60;

	t = (t - s)/60;
	uint16_t m = t % 60;

	t = (t - m)/60;
	uint16_t h = t;

	char now_hh_str[40];
	sprintf(now_hh_str,"%02d",h);
	return now_hh_str;
}

String secondsToMM( const uint32_t seconds)//, uint16_t &h, uint8_t &m, uint8_t &s )
{
	uint32_t t = seconds + (TIME_ZONE_DIFF_SECS);
	uint16_t s = t % 60;

	t = (t - s)/60;
	uint16_t m = t % 60;

	char now_mm_str[40];
	sprintf(now_mm_str,"%02d",m);

	return now_mm_str;
}

String secondsToSS( const uint32_t seconds)//, uint16_t &h, uint8_t &m, uint8_t &s )
{
	uint32_t t = seconds + (TIME_ZONE_DIFF_SECS);
	uint16_t s = t % 60;

	char now_ss_str[40];
	sprintf(now_ss_str,"%02d",s);
	return now_ss_str;
}
// -----------------------------------------------------------------------------
uint16_t calc_time_diff(time_t inTime)
/* Compares inTime with stored list */
{
	uint16_t index=0;
	char buf[80];

	for (index=START_INDEX;index<(NUM_DAYS_MONTH*NUM_ITEMS_PER_DAY);index++){
		int diff = inTime - october_2023[index];
		if (diff<0)
			break;
	}

	return index;
}

time_t convert_to_local_time(time_t utcTime)
/* Convert from UTC to local time */
{
	time_t out=utcTime+TIME_ZONE_DIFF_SECS;
	return out;
}

time_t get_rtc_time()
/* Retrieve UTC time from RTC module */
{
	time_t t = myRTC.get();
	time_t out_unixTime; // a time stamp

	tmElements_t te;
	te.Second = second(t);
	te.Hour = hour(t);
	te.Minute = minute(t);
	te.Day = day(t);
	te.Month = month(t);
	te.Year = year(t)-1970;

	out_unixTime =  makeTime(te);

	return out_unixTime;
}
// -----------------------------------------------------------------------------
time_t get_local_time()
/* Retrieve UTC from RTC module, then convert to local time */
{
	time_t utcTime=get_rtc_time();
	time_t localTime=convert_to_local_time(utcTime);
	return localTime;
}

tmElements_t convert_to_tmelements(time_t inTime){
	tmElements_t te;
	te.Second = second(inTime);
	te.Hour = hour(inTime);
	te.Minute = minute(inTime);
	te.Day = day(inTime);
	te.Month = month(inTime);
	te.Year = year(inTime)-1970;

	return te;
}
// -----------------------------------------------------------------------------
String get_readable_time(time_t inTime){
	tmElements_t te=convert_to_tmelements(inTime);

	int hour_12hr=0;
	String am_pm;
	String outstr;

	if (isAM(inTime)==1){
		am_pm=" AM";
		hour_12hr=(te.Hour==0 ? 12 : te.Hour);
	}
	else{
		am_pm=" PM";
		hour_12hr=(te.Hour==12 ? 12 : te.Hour-12);
	}
	char buf[80];
	sprintf(buf,"%d:%02d",hour_12hr,te.Minute);
	outstr=buf+am_pm;

	return outstr;
}

String get_readable_datetime(time_t inTime)
/* Returns preferred readable date time string */
{
	tmElements_t te=convert_to_tmelements(inTime);

	int hour_12hr=0;
	String am_pm;
	if (isAM(inTime)==1){
		am_pm=" AM";
		if (te.Hour==0)
			hour_12hr=12;
		else
			hour_12hr=te.Hour;
	}
	else{
		am_pm=" PM";
		if (te.Hour==12)
			hour_12hr=12;
		else
			hour_12hr=te.Hour-12;
	}
	char buf[80];
	sprintf(buf,", %s %d %02d:%02d",monthShortStr(te.Month),te.Day,hour_12hr,te.Minute);

	int day=my_dayOfWeek(te.Year+1970,te.Month,te.Day);

	String dayname=get_day_name(day);

	return (dayname+String(buf)+=am_pm);
}

String get_readable_day(time_t inTime)
/* Returns day name */
{
	tmElements_t te=convert_to_tmelements(inTime);

	int day=my_dayOfWeek(te.Year+1970,te.Month,te.Day);
	String dayname=get_day_name(day);

	return (dayname);
}

String get_readable_date(time_t inTime)
/* Returns day name */
{
	tmElements_t te=convert_to_tmelements(inTime);

	char buf[40];
	sprintf(buf,"%s %d",monthShortStr(te.Month),te.Day);

	return (buf);
}
// -----------------------------------------------------------------------------
void get_individual_time(time_t inTime, uint8_t &HH, uint8_t &MM, uint8_t &SS, bool &AM){
	tmElements_t te=convert_to_tmelements(inTime);

	int hour_12hr=0;

	AM=isAM(inTime);

	if (AM==1){
		hour_12hr=(te.Hour==0 ? 12 : te.Hour);
	}
	else{
		hour_12hr=(te.Hour==12 ? 12 : te.Hour-12);
	}

	HH=hour_12hr;
	MM=te.Minute;
	SS=te.Second;
}
