#ifndef USER_H
#define USER_H
#include <stdint.h>
#include <Arduino.h>
//#include "RTClib.h"

typedef union 
{
    struct 
    {
        uint8_t dummy;
        unsigned dummy2:7;
        unsigned DN:1;
    };
    uint16_t Preset;
}SENSAS_TIMER;


typedef union 
{
  struct 
  {
      float valY;
      float valX;
  };
  uint8_t values[16];
  
}user_data;
extern  user_data data;


/*
extern SENSAS_TIMER tmr_test;
extern SENSAS_TIMER tmr_serial;
extern SENSAS_TIMER tmr_rtc;
*/
//extern RTC_Micros rtc;
extern String DateTimeWiFi;
#endif