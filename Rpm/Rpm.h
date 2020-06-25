#include "MyInterrupts.h"
class Rpm : public Interrupt{
  private:
    long pulso;
    
  public:
    Rpm(int pinRotacao){
      pinMode(pinRotacao, INPUT_PULLUP);
      attach(pinRotacao, FALLING);
    }

    long getPulso(){
	 return pulso;
     }    

    long reiniciar(){
	 pulso=0;
     }    
    // Função Executada na interrupcao
    void volatile execInterrupt(uint8_t interruptNum){
	pulso++;
    }

    	
};

