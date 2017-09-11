//Author: Daniel Zolotor

int sendTorque(int torque, int m){
    
    int id;
    int dir; //ccw = 0
             //cw = 1

    if(m == 0){
      id = 0x0C0;
      dir = 1;
    }else if(m == 1){
      id = 0x1C0; 
      dir = 1;
    }

    unsigned char data3[8] = {(torque), 0, 244, 1, dir, 1, 0, 0};
    byte sndStat3 = CAN0.sendMsgBuf(id, 0, 8, data3); 
    Serial.println(torque);
}
