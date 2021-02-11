#include "Arduino.h"
//Mega
#define PIN_INI 22  
//PIN 28 "NAO USAR POIS é apenas de indicação no .xml para o RealDash( AlgoAberto) "
#define PIN_FIM 50
#define PIN_CAPO 27
#define PIN_PORTAS PIN_INI + 6

class Bordo{ 

private:
	Util util;
	uint8_t PinNivelCombustivel1;
	uint8_t PinNivelCombustivel2;	
	int nivelEstavel = 0; //Inicial
   public:

        Bordo(uint8_t pinNivelCombustivel1, uint8_t pinNivelCombustivel2){
		
		pinMode(pinNivelCombustivel1, INPUT);
		pinMode(pinNivelCombustivel2, INPUT);

		PinNivelCombustivel1 = pinNivelCombustivel1;
		PinNivelCombustivel2 = pinNivelCombustivel2;

		obterNivelCombustivel();
		util.iniciaTimer3(TIMER_3); // Iniciar timer3 para controle de 'delay'    

		for (int i=PIN_INI; i<=PIN_FIM; i++) {

			pinMode(i,INPUT_PULLUP);
	
		}

	};

	int obterNivelCombustivel(){

		/*

		Vazio - 	          6.66(ligado) e 5.9(pos chave)
		inicio de reserva 280ohm  6.25(ligado) e 5.43(pos chave)


		COMBUSTIVEL (VERIFICAR)
		<Tensao     	 >Tensao
		Vazio    Meio    Cheio		
		 285-315 155-165 38-42
		 308-318 162-168 36-40 

		5.5 volts > vazio (Calculado)

		6.5 volts > 1/4 de Tanque (ligado)
		7.3 volts > 1/2 de Tanque (ligado)

		8.1 volts > 3/4 de Tanque (calculado)
		9.0 volts > 4/4(cheio) de Tanque (calculado)



		*/

		if (util.saidaTimer3()){

			//float R1 = 30000;
			//float R2 = 7500;
			float voltPorUnidade = 0.004887586;
			int nivelAtual=0;			
			float sensorNivelCombustivel=0; 
		
			float sensorNivelCombustivel_Aux =0;
			float sensorNivelCombustivel_Aux1 =0;
			float sensorNivelCombustivel_Aux2 =0;
			//(0-5)			
			sensorNivelCombustivel_Aux1 = util.estabilizarEntrada(PinNivelCombustivel1);
			sensorNivelCombustivel_Aux2 = util.estabilizarEntrada(PinNivelCombustivel2);

			sensorNivelCombustivel_Aux = abs(sensorNivelCombustivel_Aux2 - sensorNivelCombustivel_Aux1);

			//Serial.print("-");
			//Serial.println(sensorNivelCombustivel_Aux2);

			//(0-25)
			if (sensorNivelCombustivel_Aux>0){

				//sensorNivelCombustivel =  sensorNivelCombustivel_Aux * 5; // = sensorNivelCombustivel_Aux / (R2/(R1+R2));

				nivelAtual=map(sensorNivelCombustivel_Aux,272,164,0,100);			
				//Serial.print("volt:");
				//Serial.println(sensorNivelCombustivel*voltPorUnidade);


			}else nivelAtual = 0;


			//Serial.println("nivelAtual:");
			//Serial.print(nivelAtual);

			if (nivelAtual==0) nivelEstavel = nivelAtual;



			if (nivelAtual < nivelEstavel){
				//Oscilando para baixo
				nivelEstavel--;
				
			}else{

				if (nivelAtual > nivelEstavel){
					//Oscilando para cima
					nivelEstavel++;

				}
			}
			//	Serial.println(PIN_PORTAS);

			///if (nivelMemoria<0)  nivelMemoria=0;

			util.reIniciaTimer3();	
		}


		return nivelEstavel;	


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
		    
			if (i !=PIN_CAPO) { // CAPO tem a logica inversa. Low é fechado, hight aberto
			
				if (digitalRead(i) == LOW){
					digitalPins |= (1UL << bitposition);

					if (i<PIN_PORTAS){
					   algoAberto = true; 
					}

				}
			}else
				if (digitalRead(i)){
					digitalPins |= (1UL << bitposition);
					algoAberto = true; 
				}
			bitposition++;
		  
		

		  }
		  //2^6 
		  if (algoAberto) digitalPins |= (1 << (PIN_PORTAS - PIN_INI));

		  
		  return digitalPins;

	}    



};

 


