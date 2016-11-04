// timeProcs.h

#ifndef HEADER_TIMEPROCS
#define HEADER_TIMEPROCS

int RTC_preinit();
int RTC_init();
int SetTimeDate(int d, int mo, int y, int h, int mi, int s);
String ReadTimeDate();


#endif
