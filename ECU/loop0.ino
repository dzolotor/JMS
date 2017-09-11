//Author: Daniel Zolotor

/*********************************************************************************************************
  MAIN LOOP
*********************************************************************************************************/

void loop0()
{
    if((digitalRead(linearize(throttle0, throttle1) == true))){
      
          //change click to toggle button's pin
          if(((digitalRead(inverterPin) == HIGH) && (toggleInverter == false) && (getSensorValue(brakePin) > 115))){
              
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

          
          //torque limit 240 NM
          throttle0 = getSensorValue(potPin0);
          Serial.println(throttle0);
          torque = (double)(throttle0 - (170)) / (860-170) * 220.0 * 10;
          
          //never send negative speed
          if(torque < 0){
            torque = 0;
          }else if(torque > 1250){
            torque = 1250  ;
          }
      
          sendTorque(torque, 0);
          sendTorque(torque, 1);
          
          
    }else{
      digitalWrite(faultPin, HIGH);
      disableInverter(0);
      disableInverter(1);
      delay(500);
    }
    
}
