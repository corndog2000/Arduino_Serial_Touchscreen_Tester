//Serial Monitor Varible Input for use with Adafruit Capacitive Arduino Touchscreen shield
//Written by Joseph Schroedl
//August 25-26, 2017

#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

bool verboseMode = false;
long baudRate = 2000000; //If I assign baud rate this way the whole code slows down
long serialDelay = 100;
long tempSerialDelay = 0;

Adafruit_GFX_Button serialSetSpeedButton = Adafruit_GFX_Button();
Adafruit_GFX_Button OnButton = Adafruit_GFX_Button();
Adafruit_GFX_Button OffButton = Adafruit_GFX_Button();
//Number pad buttons
Adafruit_GFX_Button Number0 = Adafruit_GFX_Button();
Adafruit_GFX_Button Number1 = Adafruit_GFX_Button();
Adafruit_GFX_Button Number2 = Adafruit_GFX_Button();
Adafruit_GFX_Button Number3 = Adafruit_GFX_Button();
Adafruit_GFX_Button Number4 = Adafruit_GFX_Button();
Adafruit_GFX_Button Number5 = Adafruit_GFX_Button();
Adafruit_GFX_Button Number6 = Adafruit_GFX_Button();
Adafruit_GFX_Button Number7 = Adafruit_GFX_Button();
Adafruit_GFX_Button Number8 = Adafruit_GFX_Button();
Adafruit_GFX_Button Number9 = Adafruit_GFX_Button();
Adafruit_GFX_Button NumberDelete = Adafruit_GFX_Button();

Adafruit_FT6206 ts = Adafruit_FT6206();

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

int OnOffButtonXPos = 0;
int OnOffButtonYPos = 0;
int OnOffButtonWidth = 120;
int OnOffButtonHeight = 120;
bool OnorOff = false;
bool CountingUp = true;
int count = 0;

int SerialSpeedButtonXPos = 200;
int SerialSpeedButtonYPos = 0;
int SerialSpeedButtonWidth = 120;
int SerialSpeedButtonHeight = 120;


void clearScreen() {
	tft.fillScreen(ILI9341_BLACK);
	if (verboseMode) { Serial.println("Cleared Screen"); }
}

void drawOffButton() {
	OffButton.drawButton();
	/*tft.fillRect(OnOffButtonXPos, OnOffButtonYPos, OnOffButtonWidth, OnOffButtonHeight, ILI9341_RED);
	tft.setCursor((OnOffButtonWidth / 10), (OnOffButtonHeight / 3.5));
	tft.setTextColor(ILI9341_WHITE);
	tft.setTextSize(6);
	tft.println("OFF");*/
}

void drawOnButton() {
	OnButton.drawButton();
	/*tft.fillRect(OnOffButtonXPos, OnOffButtonYPos, OnOffButtonWidth, OnOffButtonHeight, ILI9341_GREEN);
	tft.setCursor((OnOffButtonWidth / 7), (OnOffButtonHeight / 5));
	tft.setTextColor(ILI9341_WHITE);
	tft.setTextSize(8);
	tft.println("ON");*/
}

void checkOperationStatus() {
	if (OnorOff == true)
	{
		drawOnButton();
		if (verboseMode) { Serial.println("Now ON"); }
		//delay(1000);
	}
	else if (OnorOff == false)
	{
		drawOffButton();
		if (verboseMode) { Serial.println("Now OFF"); }
		//delay(1000);
	}
}

void drawSerialSpeedButton() {
	serialSetSpeedButton.drawButton();
	/*tft.fillRect(SerialSpeedButtonXPos, SerialSpeedButtonYPos, SerialSpeedButtonWidth, SerialSpeedButtonHeight, ILI9341_LIGHTGREY);
	tft.setCursor(SerialSpeedButtonXPos + (SerialSpeedButtonWidth / 10), SerialSpeedButtonYPos + (SerialSpeedButtonHeight / 3.5));
	tft.setTextColor(ILI9341_WHITE);
	tft.setTextSize(3);
	//tft.setTextWrap(false);
	tft.print("Output");
	tft.setCursor(SerialSpeedButtonXPos + (SerialSpeedButtonWidth / 10), SerialSpeedButtonYPos + (SerialSpeedButtonHeight / 2));
	tft.print("Rate");*/
}

void drawNumberPad() {
	Number0.drawButton();
	if (verboseMode) { Serial.println("Drew Number 0 Button"); }
	Number1.drawButton();
	if (verboseMode) { Serial.println("Drew Number 1 Button"); }
	Number2.drawButton();
	if (verboseMode) { Serial.println("Drew Number 2 Button"); }
	Number3.drawButton();
	if (verboseMode) { Serial.println("Drew Number 3 Button"); }
	Number4.drawButton();
	if (verboseMode) { Serial.println("Drew Number 4 Button"); }
	Number5.drawButton();
	if (verboseMode) { Serial.println("Drew Number 5 Button"); }
	Number6.drawButton();
	if (verboseMode) { Serial.println("Drew Number 6 Button"); }
	Number7.drawButton();
	if (verboseMode) { Serial.println("Drew Number 7 Button"); }
	Number8.drawButton();
	if (verboseMode) { Serial.println("Drew Number 8 Button"); }
	Number9.drawButton();
	if (verboseMode) { Serial.println("Drew Number 9 Button"); }
	NumberDelete.drawButton();
	if (verboseMode) { Serial.println("Drew Number Delete Button"); }
}

void drawSpeedMeterLabel() {
	tft.setCursor(130, 5);
	tft.setTextColor(ILI9341_BLUE);
	tft.setTextSize(2);
	tft.print("Speed:");
}

void drawSpeedMeter() {
	tft.fillRect(120, 25, 80, 14, ILI9341_BLACK);
	tft.setCursor(120, 25);
	tft.setTextColor(ILI9341_BLUE);
	tft.setTextSize(2);
	tft.print(serialDelay);
	tft.print("ms");
}

void drawTempSpeedLabel() {
	tft.setCursor(130, 80);
	tft.setTextColor(ILI9341_ORANGE);
	tft.setTextSize(2);
	tft.print("Temp:");
}

void drawTempSpeed() {
	tft.fillRect(120, 100, 80, 14, ILI9341_BLACK);
	tft.setCursor(120, 100);
	tft.setTextColor(ILI9341_ORANGE);
	tft.setTextSize(2);
	tft.print(tempSerialDelay);
	tft.print("ms");
}

void setup() {
	Serial.begin(baudRate);

	tft.begin();

	if (!ts.begin(40)) {
		if (verboseMode) { Serial.println("Unable to start touchscreen."); }
	}
	else {
		if (verboseMode) { Serial.println("Touchscreen started."); }
	}
	//Initialize all buttons
	serialSetSpeedButton.initButtonUL(&tft, SerialSpeedButtonXPos, SerialSpeedButtonYPos, SerialSpeedButtonWidth, SerialSpeedButtonHeight, ILI9341_DARKCYAN, ILI9341_DARKCYAN, ILI9341_WHITE, "SET SPEED", 2);
	OnButton.initButtonUL(&tft, OnOffButtonXPos, OnOffButtonYPos, OnOffButtonWidth, OnOffButtonHeight, ILI9341_GREEN, ILI9341_GREEN, ILI9341_WHITE, "ON", 5);
	OffButton.initButtonUL(&tft, OnOffButtonXPos, OnOffButtonYPos, OnOffButtonWidth, OnOffButtonHeight, ILI9341_RED, ILI9341_RED, ILI9341_WHITE, "OFF", 5);
	//Number Pad Buttons
	Number0.initButtonUL(&tft, 0, 140, 40, 40, ILI9341_DARKGREY, ILI9341_DARKGREY, ILI9341_WHITE, "0", 3);
	Number1.initButtonUL(&tft, 50, 140, 40, 40, ILI9341_DARKGREY, ILI9341_DARKGREY, ILI9341_WHITE, "1", 3);
	Number2.initButtonUL(&tft, 100, 140, 40, 40, ILI9341_DARKGREY, ILI9341_DARKGREY, ILI9341_WHITE, "2", 3);
	Number3.initButtonUL(&tft, 150, 140, 40, 40, ILI9341_DARKGREY, ILI9341_DARKGREY, ILI9341_WHITE, "3", 3);
	Number4.initButtonUL(&tft, 200, 140, 40, 40, ILI9341_DARKGREY, ILI9341_DARKGREY, ILI9341_WHITE, "4", 3);
	Number5.initButtonUL(&tft, 250, 140, 40, 40, ILI9341_DARKGREY, ILI9341_DARKGREY, ILI9341_WHITE, "5", 3);
	Number6.initButtonUL(&tft, 0, 190, 40, 40, ILI9341_DARKGREY, ILI9341_DARKGREY, ILI9341_WHITE, "6", 3);
	Number7.initButtonUL(&tft, 50, 190, 40, 40, ILI9341_DARKGREY, ILI9341_DARKGREY, ILI9341_WHITE, "7", 3);
	Number8.initButtonUL(&tft, 100, 190, 40, 40, ILI9341_DARKGREY, ILI9341_DARKGREY, ILI9341_WHITE, "8", 3);
	Number9.initButtonUL(&tft, 150, 190, 40, 40, ILI9341_DARKGREY, ILI9341_DARKGREY, ILI9341_WHITE, "9", 3);
	NumberDelete.initButtonUL(&tft, 240, 200, 80, 40, ILI9341_MAROON, ILI9341_MAROON, ILI9341_WHITE, "Delete", 2);

	tft.setRotation(1);

	clearScreen();

	drawSerialSpeedButton();
	checkOperationStatus();
	drawSpeedMeterLabel();
	drawSpeedMeter();
	drawNumberPad();
	drawTempSpeedLabel();
	drawTempSpeed();
	if (verboseMode) { Serial.println("Done Drawing Setup Buttons"); }
}

void loop() {
	if (ts.touched()) {
		// Retrieve a point
		TS_Point p = ts.getPoint();
		TS_Point p2 = p;
		// rotate coordinate system
		// flip it around to match the screen.
		//p.y = map(p.y, 0, 320, 320, 0);
		p.x = p2.y;
		p.y = p2.x;
		p.x = map(p.x, 0, 320, 320, 0);
		/*if (verboseMode) {
			Serial.print("X: ");
			Serial.print(p.x);
			Serial.print(", ");
			Serial.print("Y: ");
			Serial.print(p.y);
			Serial.println();
		}*/
		if (OnButton.contains(p.x, p.y)) {
			OnButton.press(true);
		}
		else if (OffButton.contains(p.x, p.y)) {
			OffButton.press(true);
		}
		else if (serialSetSpeedButton.contains(p.x, p.y)) {
			serialSetSpeedButton.press(true);
			if (verboseMode) { Serial.println("Serial Speed Button Pressed"); }
		}
		else if (Number0.contains(p.x, p.y)) {
			Number0.press(true);
		}
		else if (Number1.contains(p.x, p.y)) {
			Number1.press(true);
		}
		else if (Number2.contains(p.x, p.y)) {
			Number2.press(true);
		}
		else if (Number3.contains(p.x, p.y)) {
			Number3.press(true);
		}
		else if (Number4.contains(p.x, p.y)) {
			Number4.press(true);
		}
		else if (Number5.contains(p.x, p.y)) {
			Number5.press(true);
		}
		else if (Number6.contains(p.x, p.y)) {
			Number6.press(true);
		}
		else if (Number7.contains(p.x, p.y)) {
			Number7.press(true);
		}
		else if (Number8.contains(p.x, p.y)) {
			Number8.press(true);
		}
		else if (Number9.contains(p.x, p.y)) {
			Number9.press(true);
		}
		else if (NumberDelete.contains(p.x, p.y)) {
			NumberDelete.press(true);
		}
	}
	else
	{
		OnButton.press(false);
		OffButton.press(false);
		serialSetSpeedButton.press(false);
		Number0.press(false);
		Number1.press(false);
		Number2.press(false);
		Number3.press(false);
		Number4.press(false);
		Number5.press(false);
		Number6.press(false);
		Number7.press(false);
		Number8.press(false);
		Number9.press(false);
		NumberDelete.press(false);
	}


	if (OnButton.justReleased() || OffButton.justReleased()) {
		if (verboseMode) { Serial.println("You touched the button"); }
		OnorOff = !OnorOff;
		checkOperationStatus();
		count = 0;
		CountingUp = true;
	}
	else if (OnorOff) {
		if (CountingUp) {
			Serial.println(count);
			count = count+1;
			if (count == 100) {
				CountingUp = false;
			}
			delay(serialDelay);
		}
		else if (CountingUp == false) {
			Serial.println(count);
			count = count-1;
			if (count == 0) {
				CountingUp = true;
			}
			delay(serialDelay);
		}
	}
	else if (serialSetSpeedButton.justPressed())
	{
		serialDelay = tempSerialDelay;
		drawSpeedMeter();
		if (verboseMode) { Serial.println("Set the speed to what user inputed"); }
	}
	else if (Number0.justPressed()) {
		tempSerialDelay = tempSerialDelay * 10 + 0;
		drawTempSpeed();
	}
	else if (Number1.justPressed()) {
		tempSerialDelay = tempSerialDelay * 10 + 1;
		drawTempSpeed();
	}
	else if (Number2.justPressed()) {
		tempSerialDelay = tempSerialDelay * 10 + 2;
		drawTempSpeed();
	}
	else if (Number3.justPressed()) {
		tempSerialDelay = tempSerialDelay * 10 + 3;
		drawTempSpeed();
	}
	else if (Number4.justPressed()) {
		tempSerialDelay = tempSerialDelay * 10 + 4;
		drawTempSpeed();
	}
	else if (Number5.justPressed()) {
		tempSerialDelay = tempSerialDelay * 10 + 5;
		drawTempSpeed();
	}
	else if (Number6.justPressed()) {
		tempSerialDelay = tempSerialDelay * 10 + 6;
		drawTempSpeed();
	}
	else if (Number7.justPressed()) {
		tempSerialDelay = tempSerialDelay * 10 + 7;
		drawTempSpeed();
	}
	else if (Number8.justPressed()) {
		tempSerialDelay = tempSerialDelay * 10 + 8;
		drawTempSpeed();
	}
	else if (Number9.justPressed()) {
		tempSerialDelay = tempSerialDelay * 10 + 9;
		drawTempSpeed();
	}
	else if (NumberDelete.justPressed()) {
		tempSerialDelay = 0;
		drawTempSpeed();
		if (verboseMode) { Serial.println("Deleted Temp Serial Speed, Reset to 0"); }
	}

	//This is the old button detection system I wrote before I learned the library already has button support.
	/*if (ts.touched()) {
		// Retrieve a point
		TS_Point p = ts.getPoint();
		// rotate coordinate system
		// flip it around to match the screen.
		//p.x = map(p.x, 0, 240, 240, 0);
		p.y = map(p.y, 0, 320, 320, 0);
		if (verboseMode) {
			Serial.print("X: ");
			Serial.print(p.x);
			Serial.print(", ");
			Serial.print("Y: ");
			Serial.print(p.y);
			Serial.println();
		}
		if (p.x <= 120 && p.y <= 120) {
			OnStatus = !OnStatus;
			checkOnStatus();
			OnStatusRepeat = false;
		}
		else if (p.x >= 0 && p.x <= 120 && p.y >= 200 && p.y <= 320) {
			SerialSpeedStatus = !SerialSpeedStatus;
			checkSerialSpeedStatus();
			SerialSpeedStatusRepeat = false;
		}
	}
	if (OnStatus) {
		int i = 0;
		for (i = 0; i < 100; i++) {
			Serial.println(i);
			delay(serialDelay);
		}
		for (i = 100; i > 0; i--) {
			Serial.println(i);
			delay(serialDelay);
		}
	}*/

}