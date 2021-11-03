#include "Watchy_Phil.h"

#define DARKMODE true

const uint8_t BATTERY_SEGMENT_WIDTH = 7;
const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
const uint8_t BATTERY_SEGMENT_SPACING = 9;
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

WatchyPhil::WatchyPhil() {} //constructor

void WatchyPhil::drawWatchFace() {
	display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
	display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	drawTime();
	drawDate();
	drawSteps();
	drawWorkState();
	drawWeather();
	drawBattery();
	display.drawBitmap(120, 77, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	if (BLE_CONFIGURED) {
		display.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	}
}

void WatchyPhil::drawTime() {
	display.setFont(&DSEG7_Classic_Bold_53);
	display.setCursor(5, 53 + 5);
	int displayHour;
	if (HOUR_12_24 == 12) {
		displayHour = ((currentTime.Hour + 11) % 12) + 1;
	}
	else {
		displayHour = currentTime.Hour;
	}
	if (displayHour < 10) {
		display.print("0");
	}
	display.print(uint32_t(displayHour));
	display.print(":");
	if (currentTime.Minute < 10) {
		display.print("0");
	}
	display.println(currentTime.Minute);
}

void WatchyPhil::drawDate() {
	display.setFont(&Seven_Segment10pt7b);

	int16_t  x1, y1;
	uint16_t w, h;

	String dayOfWeek = dayStr(currentTime.Wday);
	display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);
	display.setCursor(85 - w, 85);
	display.println(dayOfWeek);

	String month = monthShortStr(currentTime.Month);
	display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
	display.setCursor(85 - w, 110);
	display.println(month);

	display.setFont(&DSEG7_Classic_Bold_25);
	display.setCursor(5, 120);
	if (currentTime.Day < 10) {
		display.print("0");
	}
	display.println(currentTime.Day);
	display.setCursor(5, 150);
	display.println(currentTime.Year + YEAR_OFFSET);// offset from 1970, since year is stored in uint8_t
}
void WatchyPhil::drawSteps() {
	// reset step counter at midnight
	if (currentTime.Hour == 0 && currentTime.Minute == 0) {
		sensor.resetStepCounter();
	}
	uint32_t stepCount = sensor.getCounter();
	display.drawBitmap(10, 165, steps, 19, 23, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	display.setCursor(35, 190);
	display.println(stepCount);
}
void WatchyPhil::drawWorkState() {
	display.setFont(&FreeMonoBold9pt7b);
	display.setCursor(100, 154);
	if (workState == VIBING_STATE) {
		display.println("VIBING");
	}
	else if (workState == HYPERVISING_STATE) {
		display.println("HYPERV.");
	}
	else if (workState == MANAGING_STATE) {
		display.println("MANAGING");
	}
	else if (workState == WORKING_STATE) {
		display.println("WORKING");
	}
}
void WatchyPhil::drawBattery() {
	display.drawBitmap(154, 73, battery, 37, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	display.fillRect(159, 78, 27, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);//clear battery segments
	int8_t batteryLevel = 0;
	float VBAT = getBatteryVoltage();
	if (VBAT > 4.1) {
		batteryLevel = 3;
	}
	else if (VBAT > 3.95 && VBAT <= 4.1) {
		batteryLevel = 2;
	}
	else if (VBAT > 3.80 && VBAT <= 3.95) {
		batteryLevel = 1;
	}
	else if (VBAT <= 3.80) {
		batteryLevel = 0;
	}

	for (int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++) {
		display.fillRect(159 + (batterySegments * BATTERY_SEGMENT_SPACING), 78, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	}
}

void WatchyPhil::drawWeather() {

	weatherData currentWeather = getWeatherData();

	int8_t temperature = currentWeather.temperature;
	int16_t weatherConditionCode = currentWeather.weatherConditionCode;


	display.setFont(&Seven_Segment10pt7b);
	display.setCursor(159, 115);
    // #TODO for simulation:
	//display.println(std::to_string(temperature) + (strcmp(TEMP_UNIT, "metric") == 0 ? " C" : " F"));
	display.println(String(temperature) + (strcmp(TEMP_UNIT, "metric") == 0 ? " C" : " F"));

	const unsigned char* weatherIcon;

	//https://openweathermap.org/weather-conditions
	if (weatherConditionCode > 801) {//Cloudy
		weatherIcon = cloudy;
	}
	else if (weatherConditionCode == 801) {//Few Clouds
		weatherIcon = cloudsun;
	}
	else if (weatherConditionCode == 800) {//Clear
		weatherIcon = sunny;
	}
	else if (weatherConditionCode >= 700) {//Atmosphere
		weatherIcon = cloudy;
	}
	else if (weatherConditionCode >= 600) {//Snow
		weatherIcon = snow;
	}
	else if (weatherConditionCode >= 500) {//Rain
		weatherIcon = rain;
	}
	else if (weatherConditionCode >= 300) {//Drizzle
		weatherIcon = rain;
	}
	else if (weatherConditionCode >= 200) {//Thunderstorm
		weatherIcon = rain;
	}
	else
		return;
	display.drawBitmap(100, 100, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}
