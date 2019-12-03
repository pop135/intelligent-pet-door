#include "RTClib.h"

RTC_DS3231 rtc;

void initRTC(){
	rtc.begin();
}