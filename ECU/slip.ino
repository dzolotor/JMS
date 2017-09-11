//Author: Daniel Zolotor

double slip(int m){

    double w_avg = (w_FR + w_FL) / 2.0;
    double v = (w_avg * PI * r) / 30; //(m/s)

    double slip_R = ((w_RR * r) - v) / v;
    double slip_L = ((w_RL * r) - v) / v;
    
    double trq_R;
 
    return(0.0);
}
