//Author: Daniel Zolotor

bool linearize(int throttle0, int throttle1){

    double V5ratio = 5.0/880.0;
    double V3ratio = 3.3/575.0;


    //Serial.println(throttle0);
    double scaledThrottle0 = (throttle0 - 170) * V5ratio;
  
    //Serial.println(scaledThrottle0);
    double scaledThrottle1 = (throttle1 - 85) * V3ratio * (5/3.3);

    double dif = scaledThrottle0 - scaledThrottle1;
    //abs value
    if(dif < 0){
      dif *= -1;
    }

    //Serial.print(throttle0);
    //Serial.print(", ");
    //Serial.print(throttle1);
    //Serial.print(", ");
    //Serial.println(dif);
    
    if(dif < (.5)){
      return(true);
    }else{
      return(false);
    }
    
}
