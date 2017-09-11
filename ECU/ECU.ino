 /*********************************************************************************************************
 * Author: Daniel Zolotor
 * Last Modified: 6-18-17
 * JMS/EPT ECU
*********************************************************************************************************/

#include <SPI.h>
#include <mcp_can.h>
#include <SD.h>

// Chip Select pin is tied to pin 9 on the SparkFun CAN-Bus Shield
const int chipSelect = 9; 
 
// TONES  ========================================== 
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

//pot
int brakePin = A0; 
int potPin1 = A1; //(throttle 1) 85~~575 at 3.3V 
int potPin0 = A2; //(throttle 0) 170~~880 at 5V

const int inverterPin = 7; 
const int faultPin = 5;  
const int spkr = 8;    

int brake;
int throttle1;
int throttle0;

int sensorValue;
int torque;
bool toggleInverter = false;

int c = 0;
int d = 1;

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);     // Set CS to pin 10

//CONSTANTS
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

//READY TO DRIVE SOUND
//speaker on a PWM pin
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

/*******************************
   __  __          _____ _   _ 
 |  \/  |   /\   |_   _| \ | |
 | \  / |  /  \    | | |  \| |
 | |\/| | / /\ \   | | | . ` |
 | |  | |/ ____ \ _| |_| |\  |
 |_|  |_/_/    \_\_____|_| \_|
                                                          
*******************************/

void setup()
{
  setup0();  
}

void loop()
{
   loop0();
}

/********************************
  END FILE
********************************/
