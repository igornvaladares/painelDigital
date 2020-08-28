#include "MyInterrupts.h"

class Pulso : public Interrupt{
  private:

    uint8_t PinRotacao;
    uint8_t PinVelocidade;

    long countPulsoRpm;
    long countPulsoVelocidade;
  public:
      Pulso(uint8_t pinRotacao, uint8_t pinVelocidade){

      PinRotacao = pinRotacao;
      PinVelocidade = pinVelocidade;

      pinMode(pinRotacao, INPUT_PULLUP);
      pinMode(pinVelocidade, INPUT_PULLUP);

      attach(pinRotacao, FALLING);

      attach(pinVelocidade, FALLING);

    }

    long getPulsoRpm(){
	 return countPulsoRpm;
     }    

    long getPulsoVelocidade(){
	 return countPulsoVelocidade;
     }    

    long reiniciarRpm(){
	 countPulsoRpm=0;
     }    
    long reiniciarVelocidade(){
	 countPulsoVelocidade=0;
     }    

    // Função Executada na interrupcao
    void volatile execInterrupt(uint8_t interruptNum){
	
	if (interruptNum==PinRotacao){
		countPulsoRpm++;
	}

	if (interruptNum==PinVelocidade){
		countPulsoVelocidade++;
	}

    }

    	
};

