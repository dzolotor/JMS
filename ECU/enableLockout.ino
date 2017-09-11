//Author: Daniel Zolotor

int enableLockout(int m){

    int id;

    if(m == 0){
      id = 0x0AA;
    }else if(m == 1){
      id = 0x1AA; 
    }
    
    unsigned char data1[8] = {4, 0, 9, 0, 0, 0, 128, 0}; //Torque mode Is active. Lockout is enabled.
    byte sndStat1 = CAN0.sendMsgBuf(id, 0, 8, data1);
    Serial.println("Lockout Enabled");
}
