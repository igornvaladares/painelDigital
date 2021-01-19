#include "Arduino.h"
#define VALOR_MIN_NIVEL_COMBUSTIVEL 0
#define VALOR_MAX_NIVEL_COMBUSTIVEL 100
#define VOLT_TANQUE_VAZIO 5.60
#define VOLT_TANQUE_CHEIO 0.72
//Mega
#define PIN_INI 22  
//PIN 28 "NAO USAR POIS é apenas de indicação no .xml para o RealDash( AlgoAberto) "
#define PIN_FIM 50
#define PIN_PORTAS PIN_INI + 6

class Bordo{ 

private:
	Util util;
	uint8_t PinNivelCombustivel;
	
	const uint8_t desvioCombustivel = 0;
	
	int nivelMemoria = 0; //Inicial
   public:

        Bordo(uint8_t pinNivelCombustivel){
		
		pinMode(pinNivelCombustivel, INPUT);
		PinNivelCombustivel = pinNivelCombustivel;

		obterNivelCombustivel();
		util.iniciaTimer3(TIMER_3); // Iniciar timer3 para controle de 'delay'    

		for (int i=PIN_INI; i<=PIN_FIM; i++) {

			pinMode(i,INPUT_PULLUP);
	
		}

	};

	int obterNivelCombustivel(){

		/*
		COMBUSTIVEL (VERIFICAR)
		Cheio: 33 ± 4 ohms | 0.723 Volts
		Vazio: 313 ± 4 ohms | 5.6 volts
		233 -> 5.1 Volts
		*/

		if (util.saidaTimer3()){

			float R1 = 30000;
			float R2 = 7500;
			float voltPorUnidade = 0.004887586;
			int nivelAtual=0;			
			float sensorNivelCombustivel=0; 
		
			float sensorNivelCombustivel_Aux =0;
			//(0-5)			
			sensorNivelCombustivel_Aux = analogRead(PinNivelCombustivel);
			//(0-25)
			if (sensorNivelCombustivel_Aux>0){
				sensorNivelCombustivel =  sensorNivelCombustivel_Aux / (R2/(R1+R2));
				nivelAtual = map((sensorNivelCombustivel*voltPorUnidade),VOLT_TANQUE_VAZIO ,VOLT_TANQUE_CHEIO , 						            VALOR_MIN_NIVEL_COMBUSTIVEL,VALOR_MAX_NIVEL_COMBUSTIVEL);			
			}else nivelAtual = 0;

			if (nivelMemoria==0 && nivelAtual!=100) nivelMemoria = nivelAtual;



			if (nivelAtual < (nivelMemoria - desvioCombustivel)){
				//Oscilando para baixo
				nivelMemoria--;
				
			}else{

				if (nivelAtual > (nivelMemoria + desvioCombustivel)){
					//Oscilando para cima
					nivelMemoria++;

				}
			}
			//	Serial.println(PIN_PORTAS);

			util.reIniciaTimer3();	
		}


		return nivelMemoria;	


	}

	//Retorna o valor numerico correspondente ao binario das portas ON OFF
	// Ex. 00100 = 4 ( Porta 2)
 	// Ex. 00110 = 5 ( Portas 1 e 2)
	
	//11111 11111 11111 11111 11111 111 
	//22   				  49
	unsigned long int obterSensoresDigitais(){

		  unsigned long long int digitalPins = 0;

		  int bitposition = 0;
		  bool algoAberto = false; //4 Portas, Porta-Mala ou Capoo (bits[0 - 5] do .xml ) 
	          
		  //2^0,2^1,2^2,2^3,2^4 ... ATÉ 2^50  (.XML)
			//	22    â	 50	
		  for (int i=PIN_INI; i<=PIN_FIM; i++) {
		    
			if (digitalRead(i) == LOW){
				digitalPins |= (1UL << bitposition);

				if (i<PIN_PORTAS){
				   algoAberto = true; 
				}
			}
			bitposition++;
		  
		

		  }
		  //2^6 
		  if (algoAberto) digitalPins |= (1 << (PIN_PORTAS - PIN_INI));

		  
		  return digitalPins;

	}    



};

 


