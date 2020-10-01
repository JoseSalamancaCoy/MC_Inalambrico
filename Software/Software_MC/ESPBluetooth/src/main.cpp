#include <Arduino.h>
#include "cadc.h"
#include "user.h"
//#include <SPI.h>
#include <BluetoothSerial.h>

#define BUFFER_SIZE 10000

TaskHandle_t Bluetooth;   
TaskHandle_t TaskAdc;

xSemaphoreHandle hdl_send;

bool event_Voltaje;
bool event_send;


dataI_t bufferADC[BUFFER_SIZE];
dataI_t voltaje;
uint16_t ind_adc;
uint16_t ind_bluetooth;



//uint16data_t value;

//BlynkTransportEsp32_BT connBluetooth;
BluetoothSerial SerialBT;
  
void setup() {
  Serial.begin(115200);
  event_send=false;
  ind_adc=0;
  ind_bluetooth=0;
  hdl_send= xSemaphoreCreateBinary(); //Inicialicia el semaforo
   
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    TaskBluetooth,   /* Task function. */
                    "Bluetooth",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Bluetooth,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */                  
  delay(500); 

  xTaskCreatePinnedToCore( TaskADC, "TaskAdc", 10000, NULL, 1, &TaskAdc,0);                 
  delay(500); 
}

void loop() 
{
     
 
}

void TaskBluetooth( void * pvP )
{
   SerialBT.begin("MC_Bt");
   Serial.println("Task Bluetooth Init");

   for(;;)
   {
        
      if(SerialBT.available()>=3)
      {
               
      }
      
      if(!event_send)continue;
      event_send=false;
      if(ind_bluetooth>=ind_adc)
      {
         ind_bluetooth=0;
         ind_adc=0;
      };

      if(++ind_bluetooth==BUFFER_SIZE)
      {
         ind_bluetooth=0;
         ind_adc=0;
      }
         
      SerialBT.write(bufferADC[ind_bluetooth].values,sizeof(dataI_t));
         
                                               
   }

}

void TaskADC( void * pvP )
{
   CADC adc_esp;
   uint16_t coutTickVoltage=0;
   event_Voltaje=false;
   uint32_t indX=0;
   Serial.println("Task ADC Init");
  // xSemaphoreGive(hdl_send); //Libera el semaforo para poder usarlo

   portTickType xLastWakeTime;
   for(;;)
   {
     
      
         xLastWakeTime = xTaskGetTickCount();
         indX++;
         
         
         bufferADC[ind_adc].valY=adc_esp.GetPulse();
         bufferADC[ind_adc].valX=indX;
         
         //
        
         
         if(++coutTickVoltage>2000)
         {
            coutTickVoltage=0;
            bufferADC[ind_adc].valY=0x8000+adc_esp.GetVoltage();
            bufferADC[ind_adc].valX+=3;       
         
         }

         if(++ind_adc>BUFFER_SIZE)
         {
            ind_adc=0;
            ind_bluetooth=0;
         }
         if((ind_adc %1000)==0)
         {
            Serial.print("IndADC: "); Serial.println(ind_adc); 
            Serial.print("IndBlu: "); Serial.println(ind_bluetooth);
         }
         event_send=true;
         delay(3);

   }
      
}