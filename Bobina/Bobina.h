#include "MyInterrupts.h"
class Bobina : public Interrupt{
  private:
    long Pulso;
    
  public:
    Bobina(int pinRotacao){
      pinMode(pinRotacao, INPUT_PULLUP);
      attach(pinRotacao, FALLING);
    }

    long getPulso(){
	 return Pulso;
     }    

    long reiniciar(){
	 Pulso=0;
     }    
    // Função Executada na interrupcao
    void volatile execInterrupt(uint8_t interruptNum){
	Pulso++;
    }

    	
};

