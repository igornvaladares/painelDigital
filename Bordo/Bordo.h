#include "Arduino.h"
#define VALOR_MIN_NIVEL_COMBUSTIVEL 0
#define VALOR_MAX_NIVEL_COMBUSTIVEL 100
#define ENDERECO_COMBUSTIVEL 2
class Bordo{ 

private:
	Util util;
	uint8_t PinNivelCombustivel;
	
	const uint8_t desvioCombustivel = 2;
	
	int nivelMemoria = 0; //Inicial
   public:

        Bordo(uint8_t pinNivelCombustivel){
		
		pinMode(pinNivelCombustivel, INPUT);
		PinNivelCombustivel = pinNivelCombustivel;

		obterNivelCombustivel();
		pinMode(2, INPUT);

	};

	int obterNivelCombustivel(){

		if (util.saidaTimer2()){

			util.reIniciaTimer2();	
			int sensorNivelCombustivel =0;
			
			sensorNivelCombustivel = analogRead(PinNivelCombustivel);

			int nivelAtual = map(sensorNivelCombustivel, 0, 1023, VALOR_MIN_NIVEL_COMBUSTIVEL, VALOR_MAX_NIVEL_COMBUSTIVEL);			
			
			if (nivelMemoria==0) nivelMemoria = nivelAtual;


			if (nivelAtual < (nivelMemoria - desvioCombustivel)){
				//Oscilando para baixo
				nivelMemoria--;
				
			}else{

				if (nivelAtual > (nivelMemoria + desvioCombustivel)){
					//Oscilando para cima
					nivelMemoria++;

				}
			}
			


		}
		
		return nivelMemoria;	


	}


	int obterSensoresDigitais(){

		  int digitalPins = 0;
		  int bitposition = 0;
					
		  for (int i=22; i<49; i++) {
		    
			if (digitalRead(i) == LOW) digitalPins |= (1 << bitposition);
			bitposition++;
		  }

		  return digitalPins;	

	}    



};

 


