//Author: Daniel Zolotor

int disableLockout(int m){

    int id;

    if(m == 0){
      id = 0x0C0;
    }else if(m == 1){
      id = 0x1C0; 
    }
  
    unsigned char data2[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //Send out inverter disable command to release lockout.This command should have been set up to be transmitted at a rate of 500 ms or faster before the inverter is powered up.
    byte sndStat2 = CAN0.sendMsgBuf(id, 0, 8, data2);
    Serial.println("Lockout Disabled");
}
