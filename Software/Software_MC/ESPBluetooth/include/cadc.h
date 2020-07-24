#ifndef __CADC_H
#define __CADC_H
#include <Arduino.h>

#define pulsePIN 34
#define bateryPIN 35

 class CADC
 {
 
 public:
     CADC(/* args */);
     uint16_t GetVoltage();
     uint16_t GetPulse();
     ~CADC();

 private:
     /* data */
 };
 
#endif