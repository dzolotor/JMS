 /*********************************************************************************************************
 * Author: Daniel Zolotor
 * Last Modified: 6-18-17
 * JMS/EPT ECU
*********************************************************************************************************/

//ECU.cpp

#include "ECU.h"
#include <mcp_can.h>
#include <SPI.h>
#include <SD.h>

ECU::ECU()
{
	m_throttle0;
	m_throttle1;
	m_inverterPin = 7;
	m_brakePin = A0; 
	m_throttlePin1 = A1; //(throttle 1) 85~~575 at 3.3V 
	m_throttlePin0 = A2; //(throttle 0) 170~~880 at 5V

	// Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
	if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
	{
	Serial.println("MCP2515 Initialized Successfully!");
	} 
	else
	{
	Serial.println("Error Initializing MCP2515...");
	}
	CAN0.setMode(MCP_NORMAL); //Set operation mode to normal so the MCP2515 sends acks to received data.
	
	pinMode(CAN0_INT, INPUT); // Configuring pin for /INT input
	
}

void ECU::run()
{/*
    if((digitalRead(linearize(m_throttle0, m_throttle1) == true))){
      
          //change click to toggle button's pin
          if(((digitalRead(m_inverterPin) == HIGH) && (toggleInverter == false) && (getSensorValue(m_brakePin) > 115))){
              
             enableLockout(0);
             enableLockout(1);
             disableLockout(0);
             disableLockout(1);    
             Serial.println("Enabled/Disabled Lockout");
             rtdSound(1); //on
             delay(500);
             toggleInverter = true;
             
          }else if ((digitalRead(inverterPin) == HIGH) && (toggleInverter == true)){
          //}else if (digitalRead(inverterPin) == LOW && toggleInverter == true){  
             disableInverter(0);
             //flip direction
             
             disableInverter(1);
             Serial.println("Disabled inverter");
             //rtdSound(0); //off
             delay(500);
             toggleInverter = false;
          }

          
          //torque limit is 240 NM
          m_throttle0 = getSensorValue(m_throttlePin0);
		  
          //Serial.println(throttle0);
          torque = (double)(m_throttle0 - (170)) / (860-170) * 220.0 * 10;
          
          //never send negative speed
          if(torque < 0){
            torque = 0;
          }else if(torque > 1250){
            torque = 1250  ;
          }
      
          commandTorque(torque, 0);
          commandTorque(torque, 1);
          
          
    }else{
      digitalWrite(faultPin, HIGH);
      disableInverter(0);
      disableInverter(1);
      delay(500);
    }*/ 
}

void ECU::commandTorque(int torque, int m)
{
	int id;
    int dir; //ccw = 0
             //cw = 1
    if(m == 0)
	{
      id = 0x0C0;
      dir = 1;
    }
	else if(m == 1)
	{
      id = 0x1C0; 
      dir = 1;
    }

    unsigned char data3[8] = {(torque), 0, 244, 1, dir, 1, 0, 0};
    byte sndStat3 = CAN0.sendMsgBuf(id, 0, 8, data3); 
    //Serial.println(torque);
}

void ECU::disableInverter(int m)
{
	int id;
    int dir; //ccw = 0
             //cw = 1
    if(m == 0)
	{
      id = 0x0C0;
      dir = 0;
    }
	else if(m == 1)
	{
      id = 0x1C0;
      dir = 0; 
    }
  
    unsigned char data1[8] = {0, 0, 0, 0, 0, 1, 0, 0}; 
    byte sndStat1 = CAN0.sendMsgBuf(id, 0, 8, data1);
    //Serial.println("Inverter Disabled");
}

void ECU::disableLockout(int m)
{
	int id;

    if(m == 0)
	{
      id = 0x0C0;
    }
	else if(m == 1)
	{
      id = 0x1C0; 
    }
  
    unsigned char data2[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //Send out inverter disable command to release lockout.This command should have been set up to be transmitted at a rate of 500 ms or faster before the inverter is powered up.
    byte sndStat2 = CAN0.sendMsgBuf(id, 0, 8, data2);
    //Serial.println("Lockout Disabled");
}

void ECU::enableLockout(int m)
{
	int id;

    if(m == 0)
	{
      id = 0x0AA;
    }
	else if(m == 1)
	{
      id = 0x1AA; 
    }
    
    unsigned char data1[8] = {4, 0, 9, 0, 0, 0, 128, 0}; //Torque mode Is active. Lockout is enabled.
    byte sndStat1 = CAN0.sendMsgBuf(id, 0, 8, data1);
    //Serial.println("Lockout Enabled");
}

void ECU::recieveData()
{
    // Serial.println("recieving");
    if(!digitalRead(CAN0_INT)) // If CAN0_INT pin is low, read receive buffer
    {
    
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  
    Serial.print(msgString);
  
    if((rxId & 0x40000000) == 0x40000000)
	{    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else 
	{
      for(byte i = 0; i<len; i++)
	  {
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
    }
        
    Serial.println();
  }
}

int ECU::getSensorValue(int pin)
{
	m_sensorValue = analogRead(pin);
    return m_sensorValue;
}

bool ECU::linearize(int throttle0, int throttle1)
{
    double V5ratio = 5.0/880.0;
    double V3ratio = 3.3/575.0;

    //Serial.println(throttle0);
    double scaledThrottle0 = (throttle0 - 170) * V5ratio;
    //Serial.println(scaledThrottle0);
    double scaledThrottle1 = (throttle1 - 85) * V3ratio * (5/3.3);

    double dif = scaledThrottle0 - scaledThrottle1;
    //abs value
    if(dif < 0)
	{
      dif *= -1;
    }
    
    if(dif < (.5))
	{
      return(true);
    }
	else
	{
      return(false);
    }
}

double ECU::pedalMap(int throttle, int maxTrottle)
{
	//not final
	//getting more ideas from drivers
    double torque0 = throttle / 600.0 * 240.0;
    
    //acceleration- needs to be very sensitive which won't be a problem because of traction control
        //4x linear sensitivity
        /*
         * torque0 = torque0 * 4;
         * if(torque0 > maxThrottle)
         * {
         *    torque0 = maxThrottle;
         * }
         */
    //endurance- is linear but puts a cap on max speed to save high voltage
        //cap speed at enduranceMax
        /* enduranceMax = 400;
         * if(torque0 > enduranceMax)
         * {
         *    torque0 = enduranceMax;
         * }
         */

    //autocross and skid pad- linear ???
        /*
         * linear so do nothing now
         */
    
    return torque0;
}

double ECU::slip(int m)
{

    double w_avg = (w_FR + w_FL) / 2.0;
    double v = (w_avg * PI * r) / 30; //(m/s)

    double slip_R = ((w_RR * r) - v) / v;
    double slip_L = ((w_RL * r) - v) / v;
    
    double trq_R;
 
    return(0.0);
}

int ECU::writeSD(int torque)
{

  String dataString = "";

  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile)   
  {  
    int timeStamp = millis();
    //write to uSD card
    dataFile.print(timeStamp);
    dataFile.print(" ms");
    dataFile.print(", ");

    //write sensor data to uSD card
    dataFile.print(" Torque commanded");
    dataFile.print(" = ");
    dataFile.print(torque);
    //output also on Serial monitor for debugging
    Serial.print(" Torque commanded");
    Serial.print(" = ");
    Serial.print(torque);
    
    dataFile.println(); //create a new row to read data more clearly
    dataFile.close();   //close file
    Serial.println();   //print to the serial port too:

  }
  else
  { // if the file isn't open, pop up an error:
    Serial.println("error opening datalog.txt");
  } 
  
}
//getters
int ECU::getInverterPin()
{
	return(m_inverterPin);
}

int ECU::getFaultPin()
{
	return(m_faultPin);
}

int ECU::getBrakePin()
{
	return(m_brakePin);
}

int ECU::getThrottlePin0()
{
	return(m_throttlePin0);
}

int ECU::getThrottlePin1()
{
	return(m_throttlePin1);
}

int ECU::getThrottle0()
{
	return(m_throttle0);
}

int ECU::getThrottle1()
{
	return(m_throttle1);
}

bool ECU::getToggleInverter()
{
	return(m_toggleInverter);
}

int ECU::getTorque()
{
	return(m_torque);
}

//setters
void ECU::setToggleInverter(bool toggleInverter)
{
	m_toggleInverter = toggleInverter;
}

void ECU::setThrottle0(int throttle0)
{
	m_throttle0 = throttle0;
}

void ECU::setTorque(int torque)
{
	m_torque = torque;
}

