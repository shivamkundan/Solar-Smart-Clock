#include "TouchScreen.h"
#include <Shivam_PicoW_TFT.h>

// Touch pins
#define YP A0  // must be an analog pin, use "An" notation!
#define XM A1  // must be an analog pin, use "An" notation!
#define YM 12   // can be a digital pin
#define XP 13   // can be a digital pin

#define TOUCH_SQUARE_SIDE 50

#define TOUCH_PRESSURE_THRESHOLD 800

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it    <<<<<< TO DO
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 295);
// ts.pressureThreshhold=300;

// Stores the adjusted x and y coords
int ax=0;
int ay=0;

void translate_coords(uint16_t curr_x, uint16_t curr_y )
/* Translate co-ordinates from TS panel to LCD */
{
	switch (SCR_ROT){
		case 0:
			// for orientation no. 0 / portrait
			ax=map(curr_x, 151, 861, 0, 319);
			ay=map(curr_y, 99, 922, 0, 479);
			break;

		case 1:
			// for orientation no. 1 / landscape
			ay=map(curr_x, 230-10, 835-10, 319, 0);
			ax=map(curr_y, 120-10, 889-10, 0, 479);
			break;

		case 3:
			// for orientation no. 3 / landscape
			ay=map(curr_x, 230-10, 835-10, 0, 319);
			ax=map(curr_y, 120-10, 889-10, 479, 0);
			break;
	}
}

// ============================ HELPERS ============================ //
// x = 151 to 861
int min_x=1000;
int max_x=0;
int curr_x=0;

// y = 109 to 925
int min_y=1000;
int max_y=0;
int curr_y=0;

void update_touch_limits(int curr_x, int curr_y)
/* To track max and min values for x and y */
{
	if (curr_x<min_x)
		min_x=curr_x;
	else if (curr_x>max_x)
		max_x=curr_x;

	if (curr_y<min_y)
		min_y=curr_y;
	else if (curr_y>max_y)
		max_y=curr_y;
}

void print_touch_limits(int px, int py, Adafruit_HX8357 tft)
/* Print max and min values for x and y */
{
	tft.setCursor(10, 200);
	tft.println("X = ("+String(px)+","+String(min_x)+","+String(max_x)+")");
	tft.println(" ");
	tft.println(" ");
	tft.println("Y = ("+String(py)+","+String(min_y)+","+String(max_y)+")");
	tft.println(" ");
	tft.println(" ");
}

void show_touch_point(int curr_x, int curr_y, int curr_z, Adafruit_HX8357 tft)
/* Draw a square around the touch point */
{
	// For finding max/min x and y
	update_touch_limits((int) curr_x,(int) curr_y);
	// print_touch_limits((int) curr_x, (int) curr_y,tft);

	// Translate TS co-ords to current LCD orientation
	translate_coords((int) curr_x,(int) curr_y);

	// // Blit to screen
	// tft.println("(X,Y) = ("+String(ax)+","+String(ay)+")");
	// tft.println(" ");
	// tft.println("p.z: "+String(curr_z));
	// tft.println(" ");

	// Draw square around touch point
	tft.drawRect(ax-TOUCH_SQUARE_SIDE/2, ay-TOUCH_SQUARE_SIDE/2, TOUCH_SQUARE_SIDE, TOUCH_SQUARE_SIDE, HX8357_WHITE);
}



