/*
This sketch is demonstrating loading images from an array.
Use BMP24toILI565Array to generate .h files from your .bmp images.
Then include those .h files in your sketch and use the array name
(which is same as filename) in drawImage call.
*/

#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <ILI9341_due_config.h>
#include <ILI9341_due.h>
#include <elapsedMillis.h>
#include "images.h"
#include "screenProcs.h"
#include "sensorProcs.h"
#include "timeProcs.h"

#define LCD_RST 8   // Reset for LCD
#define LCD_DC 9    // Command/Data for LCD
#define LCD_CS 10   // Chip Select for LCD
#define SD_CS 11    // Chip Select for SD card
#define BUFFPIXELCOUNT 160  // size of the buffer in pixels
#define SD_SPI_SPEED SPI_HALF_SPEED // SD card SPI speed, try SPI_FULL_SPEED
SdFat sd; // set filesystem
SdFile bmpFile; // set filesystem

// Set up variables for timer functions
elapsedMillis timeElapsed1;
elapsedMillis timeElapsed2;
elapsedMillis timeElapsed3;
elapsedMillis screenExpired;
elapsedMillis idleExpired;

unsigned int screenCheck = 1000;
unsigned int sensorCheck = 10000;
unsigned int animateCheck = 500;
unsigned int screenImageBit = 1;
unsigned int idleImageBit = 0;
unsigned int screenTime = 10000;
unsigned int idleImgTime = 15000;

// Set up variables for pin inputs
int radioInputPin = 36;
int powerInputPin = 37;
int hornsInputPin = 38;
int startInputPin = 39;
int buttonSelectUp = 38;
int buttonSelectDwn = 40;
int buttonMenuSelect = 0;
boolean imgState = LOW;
boolean clearMenu = LOW;

// Set up method - initialize and configure
void setup() {
  Serial.begin(9600);
  Serial.print(F("Initializing Program Space...\n"));

  // SPI.setDataMode(SPI_MODE0); // both mode 1 & 3 should work 
  RTC_preinit();
  
  if (!sd.begin(SD_CS, SD_SPI_SPEED)){
    progmemPrintln(PSTR("failed!"));
    return;
  }
  progmemPrintln(PSTR("OK!"));
  
  // Initialize the input pins LOW CHANGE RISING FALLING HIGH
  pinMode(radioInputPin, INPUT);
  pinMode(powerInputPin, INPUT);
  pinMode(hornsInputPin, INPUT);
  pinMode(startInputPin, INPUT);
  pinMode(buttonSelectUp, INPUT);
  pinMode(buttonSelectDwn, INPUT);
  attachInterrupt(digitalPinToInterrupt(radioInputPin), radioInterrupt, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(powerInputPin), powerInterrupt, FALLING);
  // attachInterrupt(digitalPinToInterrupt(hornsInputPin), hornsInterrupt, RISING);
  // attachInterrupt(digitalPinToInterrupt(startInputPin), startInterrupt, CHANGE);

  // Initialize the TFT display
  screenInit();

  // Set the TFT image background and default image
  screenBackground();
  screenHappy(3000);
}

// Interrupt service routines
void radioInterrupt() {
  if (digitalRead(radioInputPin) == HIGH) {
    Serial.print(F("*** radioInterrupt *** HIGH ***\n"));
	  screenHappy(3000);
  } else {
    Serial.print(F("*** radioInterrupt *** LOW ***\n"));
	  screenSad(5000);
  }
}

void powerInterrupt() {
  screenUnhappy(3000);
}

void hornsInterrupt() {
  screenSurprised(3000);
}

void startInterrupt() {
  if (digitalRead(startInputPin) == HIGH) {
	  screenConfused(3000);
  } else {
	// screenUnhappy(3000);
	// Check for RPM bit: 1 running, 0 not running
	// 1: screenGoodNews(3000);
	// 0: screenSad(3000);
  }
}

// Check for image on screen expire
void checkScreenTimeout() {
  // Serial.print(F("--> checkScreenTimeout\n"));
  if (screenExpired > screenTime) {
    if (clearMenu) {
      screenInit();
      screenBackground();
      clearMenu = LOW;
    }
    screenIndifferent(0);
    screenExpired = 0;
  }
}

// If screen image is indifferent alternate thinking
void checkScreenIdle() {
  // Serial.print(F("--> checkScreenIdle\n"));
  if (idleExpired > idleImgTime) {
    // Serial.print(F("    --> checkScreenIdle --> idleExpired\n"));
    if (idleImageBit == 1) {
      if (imgState == 0) {
        screenThinkLeft(1000);
      } else {
        screenThinkRight(1000);
      }
    }
    imgState = !imgState;
    idleExpired = 0;
  }
  // Return
}

// resetScreenTimers
void resetScreenTimers(int imgBit, int otme, int ibit) {
  screenImageBit = imgBit;  // Set the image id bit
  screenTime = otme;        // Set the time-out for the image
  screenExpired = 0;        // Reset the timer
  idleImageBit = ibit;      // Set the bit for idle image
}

// Main loop for the program
void loop() {
  // Every second check screen items
  if (timeElapsed1 > screenCheck) {
	  if (screenImageBit == 0) {
	    checkScreenTimeout();
	  } else if (screenImageBit == 1) {
	    checkScreenIdle();
	  } else {
      //
    }
	  checkDigitalInputs(); // Each second
    timeElapsed1 = 0;     // reset the counter
  }

  // Every ten seconds check analog sensors
  if (timeElapsed2 > sensorCheck) {
	  if (screenImageBit == 1) {
		  checkAnalogInputs(); // Each 10 seconds
	  }
    timeElapsed2 = 0;
  }

  // Every half second animate sad image id set
  if (idleImageBit == 2) {
    if (timeElapsed3 > animateCheck) {
      screenAnimateSad();
      timeElapsed3 = 0;
    }
  }
  checkButtonPress();
  // Return
}

void checkButtonPress() {
  if (digitalRead(buttonSelectUp) == HIGH) {
    buttonMenuSelect ++;
    if (buttonMenuSelect > 6) {buttonMenuSelect = 1;}
    screenMenuSelect();
  }
  if (digitalRead(buttonSelectDwn) == HIGH) {
    buttonMenuSelect --;
    if (buttonMenuSelect < 1) {buttonMenuSelect = 6;}
    screenMenuSelect();
  }
}

void screenMenuSelect() {
  switch (buttonMenuSelect) {
    case 1:
      screenGaugeFuel(10000);
      break;
    case 2:
      screenGaugeOil(10000);
      break;
    case 3:
      screenGaugeTemp(10000);
      break;
    case 4:
      screenGaugeVolt(10000);
      break;
    case 5:
      screenOffRoad(15000);
      break;
    case 6:
      screenMulti(15000);
      break;
  }
  clearMenu = HIGH;
  delay(250);
}



