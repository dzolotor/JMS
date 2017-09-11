//Author: Daniel Zolotor

int writeSD(int torque){

  String dataString = "";

  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile)   {  
    
    int timeStamp = millis();
    //write to uSD card
    dataFile.print(timeStamp);
    dataFile.print(" ms");
    dataFile.print(", ");

    //write sensor data to uSD card
    dataFile.print(" Torque commanded");
    dataFile.print(" = ");
    dataFile.print(torque);
    //output also on Serial monitor for debugging
    Serial.print(" Torque commanded");
    Serial.print(" = ");
    Serial.print(torque);
    
    dataFile.println(); //create a new row to read data more clearly
    dataFile.close();   //close file
    Serial.println();   //print to the serial port too:

  } else{ // if the file isn't open, pop up an error:
    Serial.println("error opening datalog.txt");
  } 
  
}
