#include "Arduino.h"
#define VALOR_MIN_NIVEL_COMBUSTIVEL 0
#define VALOR_MAX_NIVEL_COMBUSTIVEL 100
class Bordo{ 

private:
	uint8_t PinNivelCombustivel;


   public:

        Bordo(uint8_t pinNivelCombustivel){
		
		pinMode(pinNivelCombustivel, INPUT);
		PinNivelCombustivel = pinNivelCombustivel;

	};

	int obterNivelCombustivel(){

		int sensorNivelCombustivel = analogRead(PinNivelCombustivel);

	        int nivel  = map(sensorNivelCombustivel, 0, 1023, VALOR_MIN_NIVEL_COMBUSTIVEL, VALOR_MAX_NIVEL_COMBUSTIVEL);			

		return nivel;	
	}


	int obterSensores(){

		  int digitalPins = 0;

		  int bitposition = 0;

		  for (int i=4; i<14; i++) {
		    
			if (digitalRead(i) == HIGH) digitalPins |= (1 << bitposition);
			bitposition++;
		  }

		  return digitalPins;	

	}    

};

 


