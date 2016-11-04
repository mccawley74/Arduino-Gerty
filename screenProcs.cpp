// screenProcs.cpp

#include <SdFat.h>
#include <SdFatUtil.h>
#include <ILI9341_due.h>
#include <ILI9341_due_config.h>

#include "roboto16.h"
#include "roboto32.h"
#include "roboto70.h"

#include "images.h"
#include "screenProcs.h"
#define LCD_RST 8   // Reset for LCD
#define LCD_DC 9    // Command/Data for LCD
#define LCD_CS 10   // Chip Select for LCD
#define SD_CS 11    // Chip Select for SD card
#define BUFFPIXELCOUNT 160  // size of the buffer in pixels
#define SD_SPI_SPEED SPI_FULL_SPEED // SD card SPI speed, try SPI_FULL_SPEED

ILI9341_due tft(LCD_CS, LCD_DC, LCD_RST);
#define error(s) sd.errorHalt_P(PSTR(s))

uint16_t colorBlueLight = tft.color565(99,179,192);
const uint16_t x = 159;
const uint16_t y = 149;
const int slaveSelect = 10;
boolean imgSwap = LOW;

void screenInit() {
  tft.begin();
  // Set screen to landscape orientation
  tft.setRotation(iliRotation270); 
  tft.fillScreen(colorBlueLight);
  // lower the frequency if the image is distorted
  // tft.setSPIClockDivider(2);
}

void screenAnimateSad() {
  Serial.print(F("--> checkScreenAnimation\n"));
  if (imgSwap == 0) {
    tft.drawImage(tear, 94, 106, tearWidth, tearHeight);
  } else {
    tft.fillRect(94, 106, 25, 35, ILI9341_YELLOW);
  }
  imgSwap = !imgSwap;
}

void screenBackground() {
  tft.fillArc(162, 122, 98, 4, 0, 360, ILI9341_BLACK);
  tft.fillCircle(162, 122, 93, ILI9341_YELLOW);
  tft.fillRect(280, 182, 11 ,38, ILI9341_BLACK);
  tft.fillRect(294, 182, 11 ,11, ILI9341_ORANGERED);
  tft.fillRect(294, 196, 11 ,11, ILI9341_ORANGERED);
  tft.fillRect(291, 209, 14, 11, ILI9341_BLACK);
  delay(100);
}

void screenBlank() {
  tft.fillCircle(162, 122, 93, ILI9341_YELLOW);
}

void screenHappy(int tout) {
  Serial.print(F("--> screenHappy\n"));
  screenBlank();
  FillEllipse(134, 98, 27, 45, ILI9341_BLACK); 
  FillEllipse(187, 98, 27, 45, ILI9341_BLACK);
  tft.fillArc(162, 87, 71, 5, 101, 259, ILI9341_BLACK);
  resetScreenTimers(0, tout, 0);
}

void screenExcited(int tout) {
  Serial.print(F("--> screenExcited\n"));
  screenBlank();
  FillEllipse(134, 98, 27, 45, ILI9341_BLACK); 
  FillEllipse(187, 98, 27, 45, ILI9341_BLACK);
  tft.fillArc(115, 140, 22, 22, 109, 320, ILI9341_RED);
  tft.fillArc(162, 87, 71, 5, 101, 259, ILI9341_BLACK);
  tft.fillArc(115, 140, 22, 5, 109, 320, ILI9341_BLACK);
  resetScreenTimers(0, tout, 0);
}

void screenUnhappy(int tout) {
  Serial.print(F("--> screenUnhappy\n"));
  screenBlank();
  FillEllipse(134, 98, 27, 45, ILI9341_BLACK); 
  FillEllipse(187, 98, 27, 45, ILI9341_BLACK);
  tft.fillArc(162, 198, 71, 5, 296, 63, ILI9341_BLACK);
  resetScreenTimers(0, tout, 0);
}

void screenSad(int tout) {
  Serial.print(F("--> screenSad\n"));
  screenBlank();
  tft.drawImage(tear, 94, 106, tearWidth, tearHeight);
  tft.drawImage(eyeleft, 111, 88, eyeleftWidth, eyeleftHeight);
  tft.drawImage(eyeright, 174, 88, eyerightWidth, eyerightHeight);
  tft.fillArc(162, 198, 71, 5, 296, 63, ILI9341_BLACK);
  resetScreenTimers(0, tout, 2);
}

void screenWinking(int tout) {
  Serial.print(F("--> screenWinking\n"));
  screenBlank();
  FillEllipse(134, 98, 27, 45, ILI9341_BLACK); 
  tft.drawImage(eyeleft, 167, 91, eyeleftWidth, eyeleftHeight);
  tft.fillArc(162, 87, 71, 5, 101, 259, ILI9341_BLACK);
  resetScreenTimers(0, tout, 0);
}

void screenSurprised(int tout) {
  Serial.print(F("--> screenSurprised\n"));
  screenBlank();
  FillEllipse(134, 98, 27, 45, ILI9341_BLACK); 
  FillEllipse(187, 98, 27, 45, ILI9341_BLACK);
  FillEllipse(161, 153, 27, 14, ILI9341_BLACK);
  tft.drawImage(leyebrow, 114, 55, leyebrowWidth, leyebrowHeight);
  tft.drawImage(reyebrow, 184, 55, reyebrowWidth, reyebrowHeight);
  resetScreenTimers(0, tout, 0);
}

void screenIndifferent(int tout) {
  Serial.print(F("--> screenIndifferent\n"));
  screenBlank();
  FillEllipse(134, 98, 27, 45, ILI9341_BLACK); 
  FillEllipse(187, 98, 27, 45, ILI9341_BLACK);
  tft.fillRoundRect(132, 145, 56, 7, 4, ILI9341_BLACK);
  resetScreenTimers(1, tout, 1);
}

void screenThinkLeft(int tout) {
  Serial.print(F("--> screenThinkLeft\n"));
  screenBlank();
  tft.drawImage(leyebrow, 100, 55, leyebrowWidth, leyebrowHeight);
  tft.drawImage(reyebrow, 163, 54, reyebrowWidth, reyebrowHeight);
  tft.drawImage(rmouth, 164, 133, rmouthWidth, rmouthHeight);
  FillEllipse(117, 96, 27, 45, ILI9341_BLACK); 
  FillEllipse(171, 96, 27, 45, ILI9341_BLACK);
  resetScreenTimers(0, tout, 0);
}

void screenThinkRight(int tout) {
  Serial.print(F("--> screenThinkRight\n"));
  screenBlank();
  tft.drawImage(leyebrow, 138, 54, leyebrowWidth, leyebrowHeight);
  tft.drawImage(reyebrow, 201, 56, reyebrowWidth, reyebrowHeight);
  tft.drawImage(lmouth, 127, 133, lmouthWidth, lmouthHeight);
  FillEllipse(155, 96, 27, 45, ILI9341_BLACK); 
  FillEllipse(209, 96, 27, 45, ILI9341_BLACK);
  resetScreenTimers(0, tout, 0);
}

void screenConfused(int tout) {
  Serial.print(F("--> screenConfused\n"));
  screenBlank();
  tft.drawImage(aeyebrow1, 118, 52, aeyebrow1Width, aeyebrow1Height);
  tft.drawImage(aeyebrow2, 177, 64, aeyebrow2Width, aeyebrow2Height);
  FillEllipse(134, 98, 27, 45, ILI9341_BLACK); 
  FillEllipse(187, 98, 27, 45, ILI9341_BLACK);
  tft.drawImage(smouth, 96, 128, smouthWidth, smouthHeight);
  resetScreenTimers(0, tout, 0);
}

void screenGoodNews(int tout) {
  Serial.print(F("--> screenGoodNews\n"));
  screenBlank();
  tft.drawImage(smile, 88, 118, smileWidth, smileHeight);
  FillEllipse(134, 98, 27, 45, ILI9341_BLACK); 
  FillEllipse(187, 98, 27, 45, ILI9341_BLACK);
  resetScreenTimers(0, tout, 0);
}

void screenGaugeFuel(int tout) {
  tft.setRotation(iliRotation270);
  tft.fillScreen(ILI9341_BLACK);
  bmpDraw("Fuel_Gauge.565", 0, 0);
  resetScreenTimers(0, tout, 0);
}
void screenGaugeOil(int tout) {
  tft.setRotation(iliRotation270);
  tft.fillScreen(ILI9341_BLACK);
  bmpDraw("Oil_Gauge.565", 0, 0);
  resetScreenTimers(0, tout, 0);
}
void screenGaugeTemp(int tout) {
  tft.setRotation(iliRotation270);
  tft.fillScreen(ILI9341_BLACK);
  bmpDraw("Temperature_Gauge.565", 0, 0);
  resetScreenTimers(0, tout, 0);
}
void screenGaugeVolt(int tout) {
  tft.setRotation(iliRotation270);
  tft.fillScreen(ILI9341_BLACK);
  bmpDraw("Volt_Gauge.565", 0, 0);
  tft.fillArc(160, 200, 140, 20, -75, 75, ILI9341_YELLOW);
  // tft.fillArc(s3x+radius,s3y+radius,radius,10,-3,3, tft.color565(127,103,6));
  resetScreenTimers(0, tout, 0);
}

void screenOffRoad(int tout) {
  tft.setRotation(iliRotation180);
  tft.fillScreen(ILI9341_BLACK);
  tft.fillRoundRect(22, 50, 190, 20, 4, ILI9341_WHITE);
  tft.fillRoundRect(22, 240, 190, 20, 4, ILI9341_WHITE);
  tft.fillRoundRect(110, 50, 20, 190, 4, ILI9341_WHITE);
  tft.fillRect(70, 120, 100, 75, ILI9341_WHITE);
  tft.fillRect(74, 124, 92, 67, ILI9341_BLACK);
  bmpDraw("Tire.565", 20, 10);
  bmpDraw("Tire.565", 175, 10);
  bmpDraw("Tire.565", 20, 200);
  bmpDraw("Tire.565", 175, 200);
  bmpDraw("Diff_Unlocked.565", 88, 30);
  bmpDraw("Diff_Unlocked.565", 88, 220);

  gTextArea titleArea{74, 124, 92, 67};
  tft.setFont(roboto32);
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  // tft.setTextScale(2);
  tft.setTextLetterSpacing(2);
  tft.setTextArea(titleArea);
  tft.printAligned("2WD", gTextAlignMiddleCenter);

  /*
  delay(1000);
  tft.clearTextArea();
  tft.printAligned("4FT", gTextAlignMiddleCenter);

  delay(1000);
  tft.clearTextArea();
  tft.setTextColor(ILI9341_ORANGERED, ILI9341_BLACK);
  tft.printAligned("4PT", gTextAlignMiddleCenter);

  delay(1000);
  tft.clearTextArea();
  tft.printAligned("4LO", gTextAlignMiddleCenter);
  

  delay(1000);
  bmpDraw("Diff_Locked.565", 88, 30);
  delay(1000);
  bmpDraw("Diff_Locked.565", 88, 220);
  resetScreenTimers(0, tout, 0);
  */
  resetScreenTimers(0, tout, 0);
}

void screenMulti(int tout) {
  tft.setRotation(iliRotation180);
  tft.fillScreen(ILI9341_BLACK);
  bmpDraw("Gauges.565", 0, 0);
  resetScreenTimers(0, tout, 0);
}

// draws a filled ellipse of given width & height
void FillEllipse(int xPos, int yPos, int width, int height, int color) {
  int a=width/2, b=height/2;        // get x & y radii
  int x1, x0 = a, y = 1, dx = 0;
  long a2 = a*a, b2 = b*b;          // need longs: big numbers!
  long a2b2 = a2 * b2;

  // draw centerline
  tft.drawLine(xPos-a, yPos, xPos+a, yPos, color);
  
  // draw horizontal lines...
  while (y<=b) {
    for (x1= x0-(dx-1); x1>0; x1--)
      if (b2*x1*x1 + a2*y*y <= a2b2)
        break;
        dx = x0-x1;
        x0 = x1;
        tft.drawLine(xPos-x0, yPos+y, xPos+x0, yPos+y, color);
        tft.drawLine(xPos-x0, yPos-y, xPos+x0, yPos-y, color);
        y += 1;
  }
}

void bmpDraw(char* filename, int x, int y) {
  SdFile   bmpFile;
  int      bmpWidth, bmpHeight; // W+H in pixels
  uint8_t  bmpDepth;            // Bit depth (currently must be 24)
  uint8_t  headerSize;
  uint32_t bmpImageoffset;      // Start of image data in file
  uint32_t rowSize;             // Not always = bmpWidth; may have padding
  uint32_t fileSize;
  boolean  goodBmp = false;     // Set to true on valid header parse
  boolean  flip = true;         // BMP is stored bottom-to-top
  uint16_t w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime;

  if ((x >= tft.width()) || (y >= tft.height())) return;

  // progmemPrint(PSTR("Loading image '"));
  // Serial.print(filename);
  // Serial.println('\'');
  startTime = millis();
  // Open requested file on SD card
  if (!bmpFile.open(filename, O_READ)) {
    // Serial.println("File open failed.");
    return;
  } else {
    //Serial.println("File opened.");
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42) {
    // BMP signature
    fileSize = read32(bmpFile);
    // progmemPrint(PSTR("File size: ")); Serial.println(fileSize);
    (void)read32(bmpFile);            // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    // progmemPrint(PSTR("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    
    // Read DIB header
    headerSize = read32(bmpFile);
    // progmemPrint(PSTR("Header size: ")); Serial.println(headerSize);
    bmpWidth = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    
    if (read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      // progmemPrint(PSTR("Bit Depth: ")); Serial.println(bmpDepth);
      if (read32(bmpFile) == 0) { // 0 = uncompressed
        //progmemPrint(PSTR("Image size: "));
        //Serial.print(bmpWidth);
        //Serial.print('x');
        //Serial.println(bmpHeight);
        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width())  w = tft.width() - x;
        if ((y + h - 1) >= tft.height()) h = tft.height() - y;
      
        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);

        if (bmpDepth == 16) { //565 format
          goodBmp = true; // Supported BMP format -- proceed!
          uint16_t buffer[BUFFPIXELCOUNT]; // pixel buffer
          bmpFile.seekSet(54);  //skip header
          uint32_t totalPixels = (uint32_t)bmpWidth*(uint32_t)bmpHeight;
          uint16_t numFullBufferRuns = totalPixels / BUFFPIXELCOUNT;
          for (uint32_t p = 0; p < numFullBufferRuns; p++) {
            // read pixels into the buffer
            bmpFile.read(buffer, 2 * BUFFPIXELCOUNT);
            // push them to the diplay
            tft.pushColors(buffer, 0, BUFFPIXELCOUNT);
          }

          // render any remaining pixels that did not fully fit the buffer
          uint32_t remainingPixels = totalPixels % BUFFPIXELCOUNT;
          if (remainingPixels > 0) {
            bmpFile.read(buffer, 2 * remainingPixels);
            tft.pushColors(buffer, 0, remainingPixels);
          }
        } else {
          progmemPrint(PSTR("Unsupported Bit Depth."));
        }

        if (goodBmp) {
          // progmemPrint(PSTR("Loaded in "));
          // Serial.print(millis() - startTime);
          // Serial.println(" ms");
        }
      }
    }
  }
  bmpFile.close();
  if (!goodBmp) progmemPrintln(PSTR("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(SdFile& f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(SdFile& f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

// Copy string from flash to serial port
// Source string MUST be inside a PSTR() declaration!
void progmemPrint(const char *str) {
  char c;
  while (c = pgm_read_byte(str++)) Serial.print(c);
}

// Same as above, with trailing newline
void progmemPrintln(const char *str) {
  progmemPrint(str);
  Serial.println();
}



