//Author: Daniel Zolotor

//not final
//getting more ideas from our drivers

double pedalMap(int throttle, int maxTrottle){

    double torque0 = throttle / 600.0 * 240.0;
    
    //acceleration- needs to be very sensitive which won't be a problem because of traction control
        //4x linear sensitivity
        /*
         * torque0 = torque0 * 4;
         * if(torque0 > maxThrottle)
         * {
         *    torque0 = maxThrottle;
         * }
         */
    //endurance- is linear but puts a cap on max speed to save high voltage
        //cap speed at enduranceMax
        /* enduranceMax = 400;
         * if(torque0 > enduranceMax)
         * {
         *    torque0 = enduranceMax;
         * }
         */

    //autocross and skid pad- linear ???
        /*
         * linear so do nothing now
         */
    
    return torque0;
    
}
