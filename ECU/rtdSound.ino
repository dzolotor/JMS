//Author: Daniel Zolotor

void rtdSound(int state){
// Set up a counter to pull from melody[] and beats[]
  if(state == 1){
    for (int i=0; i < rtd_MAX_COUNT; i++) {
    
      tone_ = rtdMelody[i];
      beat = rtdBeats[i];
  
      duration = beat * tempo; // Set up timing
  
      playTone(); 
      // A pause between notes...
      delayMicroseconds(pause);
  
      if(i == rtd_MAX_COUNT - 1){
        delay(2000);
      }
    }
  }else if(state == 0){
    for (int i=0; i<off_MAX_COUNT; i++) {
    
      tone_ = offMelody[i];
      beat = offBeats[i];
  
      duration = beat * tempo; // Set up timing
  
      playTone(); 
      // A pause between notes...
      delayMicroseconds(pause);
  
      if(i == off_MAX_COUNT - 1){
        delay(2000);
      }
    }
  }  
}
