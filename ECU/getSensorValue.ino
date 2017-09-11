//Author: Daniel Zolotor

int getSensorValue(int potPin){
    sensorValue = analogRead(potPin);
    return sensorValue;
}
