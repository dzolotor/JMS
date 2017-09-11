//Author: Daniel Zolotor

int disableInverter(int m){

    int id;
    int dir; //ccw = 0
             //cw = 1

    if(m == 0){
      id = 0x0C0;
      dir = 0;
    }else if(m == 1){
      id = 0x1C0;
      dir = 0; 
    }
  
    unsigned char data1[8] = {0, 0, 0, 0, 0, 1, 0, 0}; //Torque mode Is active. Lockout is enabled.
    byte sndStat1 = CAN0.sendMsgBuf(id, 0, 8, data1);
    Serial.println("Inverter Disabled");
}
