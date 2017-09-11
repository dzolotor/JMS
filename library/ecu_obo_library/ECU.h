 /*********************************************************************************************************
 * Author: Daniel Zolotor
 * Last Modified: 6-18-17
 * JMS/EPT ECU
*********************************************************************************************************/

//ECU.h

#include <mcp_can.h>
#include <SPI.h>
#include <SD.h>

#ifndef ECU_H
#define ECU_H

class ECU
{
	private:
				
		#define CAN0_INT 2   // Set INT to pin 2
		MCP_CAN CAN0{2};     // Set CS to pin 10
		
		int m_throttle0;
		int m_throttle1;
		int m_inverterPin;		
		int m_brakePin; 
		int m_throttlePin0; //(throttle 0) 170~~880 at 5V
		int m_throttlePin1; //(throttle 1) 85~~575 at 3.3V 	
		int m_faultPin = 5;  

		int m_sensorValue;
		int m_torque;
		bool m_toggleInverter = false;

		int c = 0;
		int d = 1;
		
		//CAN
		long unsigned int rxId;
		unsigned char len = 0;
		unsigned char rxBuf[8];
		char msgString[128];                        // Array to store serial string
							
		//CONSTANTS for tvtc
		const double r = 0.26035; //wheel radius in meters
		const double L = 1.6002;  //wheelbase in meters 

		//tvtc vars
		double swa;  //steering wheel angle
		double w_FR; //angular velocity front right (rad/s)
		double w_FL; //angular velocity front left  (rad/s)
		double w_RR; //angular velocity rear right  (rad/s)
		double w_RL; //angular velocity rear left   (rad/s)
		double w;    //YAW rate (rad/s)

		double r_FR; //(m) turning radius of front right weel
		double r_FL;   //(m) turning radius of front right weel
		double r_RR;   //(m) turning radius of front right weel
		double r_RL;   //(m) turning radius of front right weel
		double r_REF;  //(m) turning radius of outermost wheel
		double t_preR; //pre-slip left torque
		double t_preL; //pre-slip right torque

		
	public:
		ECU();
		//voids
		void run();
		void commandTorque(int torque, int m);
		void disableInverter(int m);
		void disableLockout(int m);
		void enableLockout(int m);
		void recieveData();
		void toggleInverter(bool state);
		//returns
		int getSensorValue(int pin);
		bool linearize(int throttle0, int throttle1);
		double pedalMap(int throttle, int maxTrottle);
		double slip(int m);
		int writeSD(int torque);
		//getters
		int getInverterPin();
		int getFaultPin();
		int getBrakePin();
		int getThrottlePin0();
		int getThrottlePin1();
		int getThrottle0();
		int getThrottle1();
		int getTorque();
		bool getToggleInverter();
		//setters
		void setToggleInverter(bool toggleInverter);
		void setThrottle0(int throttle0);
		void setTorque(int torque);
					
};

#endif