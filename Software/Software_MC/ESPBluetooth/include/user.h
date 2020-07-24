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
        uint8_t low;
        uint8_t hight;
    };
    uint16_t value16;
    char stream[2];
    
}uint16data_t;



typedef union 
{
  struct 
  {
      uint16_t valY;
      uint32_t valX;      
  };
  uint8_t values[8];
  
}dataI_t;
extern  dataI_t dataI;

typedef union 
{
  struct 
  {
      float valY;
      float valX;
  };
  uint8_t values[8];
  
}dataF_t;
extern  dataF_t dataF;

void TaskBluetooth( void * pvP );
void TaskADC( void * pvP );

#endif