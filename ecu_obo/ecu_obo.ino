/*********************************************************************************************************
 * Author: Daniel Zolotor
 * Last Modified: 6-18-17
 * JMS/EPT ECU
*********************************************************************************************************/

#include <mcp_can.h>
#include <ECU.h>

#include <SPI.h>
#include <SD.h>

//Chip Select pin is tied to pin 9 on the SparkFun CAN-Bus Shield
const int chipSelect = 9; 
 
//TONES  ========================================== 
//       note, period, &  frequency. 
#define  c3     7634    // 131 Hz 
#define  a3     4545    // 220 Hz 
#define  b3     4049    // 247 Hz 
#define  c4     3830    // 261 Hz 
#define  d4     3400    // 294 Hz 
#define  e4     3038    // 329 Hz 
#define  f4     2864    // 349 Hz 
#define  g4     2550    // 392 Hz 
#define  a4     2272    // 440 Hz 
#define  b4     2028    // 493 Hz 
#define  c5     1912    // 523 Hz 
#define  R      0       // rest

ECU ecu;

//READY TO DRIVE SOUND
//speaker is on a PWM pin
int speakerOut = 3;

int rtdMelody[] = {g4,  (g4+f4)/2.0,  (d4+e4)/2.0,  a3,  (a3 + b3)/2.0,   e4,  c5}; //Zelda Open Chest
int offMelody[] = {c3,  c4, a3,  a4, (a3 + b3)/2.0, (a4 + b4)/2.0}; //Mario Cave
int rtdBeats[]  = {15, 15, 15, 15, 15, 15, 15}; 
int offBeats[]  = {12, 20, 12, 20, 12, 20}; 
int rtd_MAX_COUNT = sizeof(rtdMelody) / 2; // Melody length, for looping.
int off_MAX_COUNT = sizeof(offMelody) / 2; // Melody length, for looping.

// set overall tempo
long tempo = 10000;
// set length of pause between notes
int pause = 1000;
// loop variable to increase rest length
int rest_count = 100;

// Initialize speaker variables
int tone_ = 0;
int beat = 0;
long duration  = 0;



void setup()
{
  
  Serial.begin(250000);

  

  //CAN0.setMode(MCP_NORMAL); //Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(ecu.getInverterPin(), INPUT);
  pinMode(ecu.getFaultPin(), OUTPUT);
  pinMode(speakerOut, OUTPUT);
  
  //Initialize serial terminal connection
  Serial.begin(250000);
  pinMode(chipSelect, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");

}

/*******************************
   __  __          _____ _   _ 
 |  \/  |   /\   |_   _| \ | |
 | \  / |  /  \    | | |  \| |
 | |\/| | / /\ \   | | | . ` |
 | |  | |/ ____ \ _| |_| |\  |
 |_|  |_/_/    \_\_____|_| \_|
                                                          
*******************************/

void loop()
{
   if((digitalRead(ecu.linearize(ecu.getThrottle1(), ecu.getThrottle0()) == true))){
      
          //change click to toggle button's pin
          if(((digitalRead(ecu.getInverterPin()) == HIGH) && (ecu.getToggleInverter() == false) && (ecu.getSensorValue(ecu.getBrakePin()) > 115))){
              
             ecu.enableLockout(0);
             ecu.enableLockout(1);
             ecu.disableLockout(0);
             ecu.disableLockout(1);    
             Serial.println("Enabled/Disabled Lockout");
             rtdSound(1); //on
             delay(500);
             ecu.setToggleInverter(true);
             
          }
          else if ((ecu.getInverterPin() == HIGH) && (ecu.getToggleInverter() == true))
          { 
             //disable inverters to avoid 'pop'
             //dont blow the motor controllers
             ecu.disableInverter(0);
             ecu.disableInverter(1);
             Serial.println("Disabled inverter");
             //rtdSound(0); //off
             delay(500);
             ecu.setToggleInverter(false);
          }

          
          //torque limit 240 NM
          ecu.setThrottle0(ecu.getSensorValue(ecu.getThrottlePin0()));
          Serial.println(ecu.getThrottle0());
          int tmp_torque = (double)(ecu.getThrottle0() - (170)) / (860-170) * 220.0 * 10;
          ecu.setTorque(tmp_torque);
          
          //never send negative speed
          if(ecu.getTorque() < 0){
            ecu.setTorque(0);
            }else if(ecu.getTorque() > 1250){
            ecu.setTorque(1250);
          }
      
          ecu.commandTorque(ecu.getTorque(), 0);
          ecu.commandTorque(ecu.getTorque(), 1);
          
    }else{
      digitalWrite(ecu.getFaultPin(), HIGH);
      ecu.disableInverter(0);
      ecu.disableInverter(1);
      delay(500);
    }
}

/********************************
  END FILE
********************************/
