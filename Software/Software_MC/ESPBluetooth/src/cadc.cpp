#include "cadc.h"
 CADC::CADC(/* args */)
 {
 }
 
 CADC::~CADC()
 {
 }

 uint16_t CADC::GetVoltage()
 {
     return analogRead(pulsePIN);
 }

 uint16_t CADC::GetPulse()
 {
     return analogRead(bateryPIN);
 }
 