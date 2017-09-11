//Author: Daniel Zolotor

void setup0(){
  
  Serial.begin(250000);

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK){
    Serial.println("MCP2515 Initialized Successfully!");
  } else{
    Serial.println("Error Initializing MCP2515...");
  }

  CAN0.setMode(MCP_NORMAL); //Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT); // Configuring pin for /INT input
  pinMode(inverterPin, INPUT);
  pinMode(faultPin, OUTPUT);
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

