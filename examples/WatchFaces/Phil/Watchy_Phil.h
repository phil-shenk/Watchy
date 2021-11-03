#ifndef WATCHY_PHIL_H
#define WATCHY_PHIL_H

// #TODO for deployment:
#include <Watchy.h>
// for simulation:
//#include "Watchy.h"
#include "Seven_Segment10pt7b.h"
#include "DSEG7_Classic_Regular_15.h"
#include "DSEG7_Classic_Bold_25.h"
#include "DSEG7_Classic_Regular_39.h"
#include "Watchy_Phil_Icons.h"

class WatchyPhil : public Watchy {
public:
	WatchyPhil();
	void drawWatchFace();
	void drawTime();
	void drawDate();
	void drawSteps();
	void drawWorkState();
	void drawWeather();
	void drawBattery();
};

#endif