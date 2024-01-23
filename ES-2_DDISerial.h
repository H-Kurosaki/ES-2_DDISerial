

/*
METER(Former Company Name:Decagon) 
ES-2 Electrical Conductivity Sensor
DDI Serial Libraly for Arduino
Supported platforms:
  Arduino UNO R3(Not R4)
  Arduino Mega 2560 R3 etc.
Only Arduino with AVR microcontroller driven by other 5V 16MHz is supported.
その他の5V 16MHzで駆動するAVRマイコンを搭載したArduinoのみ対応します。
*/

#ifndef _ES2DDISerial_H_
#define _ES2DDISerial_H_

#include "Arduino.h"
#include "String.h"

#define MAX_BITS  240
#define MAX_STR_BUFFER  25
#define POWER_LOWACTIVE LOW
#define POWER_HIGHACTIVE HUGH


class ES2 {
  public:
void begin(int datapin,int powerpin,bool powermode);
double GetEC_uScM(void);
double GetEC_dSM(void);
double GetWaterTemp(void);
bool Read(void);
	
  private:
double LastEC_uScm;
double LastWaterTemp;
bool TestChecksum(char *response);
int dpin;
int ppin;
bool pmode;
};


#endif
