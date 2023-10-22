// Fonts
// SEE: https://rop.nl/truetype2gfx/

#ifndef TYPES_H
	#define TYPES_H
#endif

#ifndef Adafruit_GFX_H
	#include "Adafruit_GFX.h"
#endif


#include <Fonts/FreeSerifItalic24pt7b.h>

#include "fonts/OpenSansBold30pt7b.h"

#include "fonts/FreeSansBold46pt7b.h"
#include "fonts/FreeSansBold44pt7b.h"
#include "fonts/FreeSansBold40pt7b.h"
#include "fonts/FreeSansBold25pt7b.h"
#include "fonts/FreeSansBold20pt7b.h"
#include "fonts/FreeSansBold15pt7b.h"
#include "fonts/FreeSansBold12pt7b.h"
#include "fonts/FreeSansBold10pt7b.h"

#include "fonts/FreeMonoBold40pt7b.h"
#include "fonts/FreeMonoBold38pt7b.h"
#include "fonts/FreeMonoBold24pt7b.h"
#include "fonts/FreeMonoBold22pt7b.h"
#include "fonts/FreeMonoBold20pt7b.h"
#include "fonts/FreeMonoBold18pt7b.h"
#include "fonts/FreeMonoBold16pt7b.h"
#include "fonts/FreeMonoBold14pt7b.h"
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "fonts/FreeSerif45pt7b.h"
#include "fonts/FreeSerif30pt7b.h"

#include "fonts/FreeSans15pt7b.h"
#include "fonts/FreeSans14pt7b.h"

// min pwm: 1200
// Screen brightness pwm parameters
#define MIN_PWM 1400
#define MAX_PWM 65535
#define PWM_LIMIT 65535
#define MIN_LUX 2
#define MAX_LUX 50
#define MIN_LIGHT_DIFF 1
uint16_t PWM_LVL = MIN_PWM;

#define SCR_ROT 1

#define USB_DETECT_PIN 24

// =================== COLORS =================== //
// #define BG_COLOR HX8357_BLACK
// #define TXT_COLOR HX8357_YELLOW

// theme 0
#define BG_COLOR HX8357_BLACK
#define TXT_COLOR HX8357_YELLOW
#define EEE HX8357_CYAN
#define TOP_BAR_COLOR HX8357_CYAN
#define BOTTOM_BAR_COLOR HX8357_RED

// // theme 1
// #define BG_COLOR HX8357_BLACK
// #define TXT_COLOR HX8357_WHITE
// #define EEE HX8357_YELLOW

// // theme 2
// #define BG_COLOR HX8357_WHITE
// #define TXT_COLOR HX8357_BLACK
// #define EEE HX8357_YELLOW


// Set fonts
#define MAIN_TIME_FONT FreeSansBold44pt7b
#define SECOND_FONT FreeSansBold25pt7b
#define AM_PM_FONT FreeSansBold10pt7b

#define CURRENT_STR_FONT FreeMonoBold9pt7b
#define DAY_STR_FONT FreeMonoBold14pt7b
#define DATE_STR_FONT DAY_STR_FONT

// ----------------- Screen one ----------------- //
#define MAIN_TIME_FONT_SCR_ONE FreeSansBold10pt7b
#define DATE_SCR_ONE MAIN_TIME_FONT_SCR_ONE
#define TIME_REMAIN_FONT_SCR_ONE FreeSansBold40pt7b
#define EVENT_NAME_SCR_ONE FreeMonoBold24pt7b
#define EVENT_TIME_SCR_ONE FreeMonoBold22pt7b

#define TILL_STR_SIZE FreeMonoBold12pt7b