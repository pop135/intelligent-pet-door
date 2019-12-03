#include <EEPROM.h>

#define MAX_SCHEDULE 	( 100 )
int nschedule=0;


void initEEPROM(){
	for(int i=2;i<(MAX_SCHEDULE*2);i=i+2){
		if((EEPROM.read(i)&0x3F) != 0x3F){
			nschedule++;
		}
	}
}