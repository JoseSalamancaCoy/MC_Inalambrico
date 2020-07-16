#include <Arduino.h>
//#include <SPI.h>


#include "user_bluetooth.h"

  
void setup() {
   Serial.begin(115200);   
   xTaskCreate(Task_bluetooth,"Bluetooth Task",10000,NULL,1,NULL); 
   
}

void loop() 
{

 
}