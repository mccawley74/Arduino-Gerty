//screenProcs.h

#ifndef HEADER_SCREENPROCS
  #define HEADER_SCREENPROCS
  
void screenInit(void);
void screenBackground(void);
void screenBlank(void);
void screenHappy(int tout);
void screenExcited(int tout);
void screenUnhappy(int tout);
void screenSad(int tout);
void screenWinking(int tout);
void screenSurprised(int tout);
void screenIndifferent(int tout);
void screenThinkLeft(int tout);
void screenThinkRight(int tout);
void screenConfused(int tout);
void screenGoodNews(int tout);
void screenAnimateSad(void);
void screenGaugeFuel(int tout);
void screenGaugeOil(int tout);
void screenGaugeTemp(int tout);
void screenGaugeVolt(int tout);
void screenOffRoad(int tout);
void screenMulti(int tout);
void FillEllipse(int xPos, int yPos, int width, int height, int color);
void resetScreenTimers(int itme, int otme, int ibit);
void bmpDraw(char* filename, int x, int y);
void progmemPrint(const char *str);
void progmemPrintln(const char *str);

uint16_t read16(SdFile& f);
uint32_t read32(SdFile& f);

#endif
