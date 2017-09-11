//Author: Daniel Zolotor

int requestTest(){
    
    unsigned char data3[8] = {0, 142, 0, 0, 0, 1, 0, 0}; 
    byte sndStat3 = CAN0.sendMsgBuf(0x0C1, 0, 8, data3); 
    //Serial.println("requesting");
}


