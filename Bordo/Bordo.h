#include "Arduino.h"
//Mega
//#define PIN_INI 22
#define PIN_INI 28
//PIN 28 "NAO USAR POIS é apenas de indicação no .xml para o RealDash( AlgoAberto) "
#define PIN_FIM 50
#define PIN_CAPO 28
//#define PIN_PORTAS PIN_INI + 6
#define BIT_ALGOABERTO 6
#define DESVIO_LEITURA_COMBS 1
class Bordo{ 

private:
	Util util;
	uint8_t PinNivelCombustivel1;
	uint8_t PinNivelCombustivel2;	
	int nivelEstavel = 0; //Inicial
	int incrementarCombs = false; //Inicial
	int decrementarCombs = false; //Inicial

	int nivelEstavelAnt = 0; //Inicial
	int deltaNivelEstavel =0;
	float distanciaAnt =0;
	float deltaDistancia=0;

	float voltPorUnidade = 0.004887586;
	bool ajustarCombstivel=false;
	bool isPortaAberta(uint8_t analogPin){
		// se maior que 0.5V
		//Serial.println(util.estabilizarEntrada(analogPin) * voltPorUnidade);
		if ((util.estabilizarEntrada(analogPin) * voltPorUnidade)>0.5){

			return false;

		}
		return true;

	}
       public:

        Bordo(uint8_t pinNivelCombustivel1, uint8_t pinNivelCombustivel2){
		
		pinMode(pinNivelCombustivel1, INPUT);
		pinMode(pinNivelCombustivel2, INPUT);

		PinNivelCombustivel1 = pinNivelCombustivel1;
		PinNivelCombustivel2 = pinNivelCombustivel2;

		
		util.iniciaTimer3(TIMER_3); // Iniciar timer3 para controle de 'delay'    
		nivelEstavel = obterNivelCombustivelMemoria();
		for (int i=PIN_INI; i<=PIN_FIM; i++) {

			pinMode(i,INPUT_PULLUP);
	
		}

	};
        float obterConsumo(float distancia){
		
				
		if (util.saidaTimer3()){
			deltaDistancia = distancia-distanciaAnt;
			deltaNivelEstavel = nivelEstavel -nivelEstavelAnt;
	
			distanciaAnt=distancia;
			nivelEstavelAnt=nivelEstavel;
		}
		return deltaDistancia / (deltaNivelEstavel*0.6) ;
	
	}
	int obterNivelCombustivel(){

		/*

		Vazio - 	          6.66(ligado) e 5.9(pos chave)
		inicio de reserva 280ohm  6.25(ligado) e 5.43(pos chave)
		3/4 	- 	          3.00(ligado) e 2.8(pos chave)
		4/4 	- 	          2.1(Calculado)

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

			
			int nivelAtual=0;			
			float sensorNivelCombustivel=0; 
		
			float sensorNivelCombustivel_Aux =0;
			float sensorNivelCombustivel_Aux1 =0;
			float sensorNivelCombustivel_Aux2 =0;
			sensorNivelCombustivel_Aux1 = util.estabilizarEntrada(PinNivelCombustivel1);
			sensorNivelCombustivel_Aux2 = util.estabilizarEntrada(PinNivelCombustivel2);
			sensorNivelCombustivel_Aux = abs(sensorNivelCombustivel_Aux2 - sensorNivelCombustivel_Aux1);
			//(0-25)
			if (sensorNivelCombustivel_Aux>0){
				// 6.53 % 5 % 0.004887586 = 268
				// 2.1 % 5 % 0.004887586 = 86	
				// 1.49 % 5 % 0.004887586 = 61	
			
				//nivelAtual=map(sensorNivelCombustivel_Aux,280,61,0,100);
				nivelAtual=map(sensorNivelCombustivel_Aux,269,61,0,100);
				if (abs(nivelAtual-nivelEstavel)>=20){
					if (ajustarCombstivel){
						nivelEstavel = nivelAtual;
					}

					ajustarCombstivel = true;
				}else ajustarCombstivel = false;

			
				//Serial.print("volt:");
				//Serial.println(sensorNivelCombustivel*voltPorUnidade);


			}//else nivelAtual = 0;


			if (nivelEstavel==0) nivelEstavel = nivelAtual;

			if (nivelAtual < (nivelEstavel - DESVIO_LEITURA_COMBS)){
				//Oscilando para baixo
				if (decrementarCombs){
					nivelEstavel-= DESVIO_LEITURA_COMBS;
					salvarNivelCombustivel(nivelEstavel);
				}
				decrementarCombs=!decrementarCombs; 		
			}else{
				if (nivelAtual > (nivelEstavel + DESVIO_LEITURA_COMBS+1)){
					//Oscilando para baixo
					if (incrementarCombs)
						nivelEstavel+=DESVIO_LEITURA_COMBS;
					incrementarCombs=!incrementarCombs;
				}
			}
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
		  int pin_analogico_ini = 8;//A8
		  int pin_analogico_fim = 12;//A12
		 
	          bool algoAberto = false; //4 Portas, Porta-Mala ou Capoo (bits[0 - 5] do .xml ) (4 Portas + Porta Malas)Portas Analogicas

		  int bitposition = 0; 
		for (int i=pin_analogico_ini; i<=pin_analogico_fim; i++) {  

			if (isPortaAberta(i)){

				//digitalPins |= (1UL << bitposition);
				//algoAberto = true; 

			}
			//bitposition++;

   	 	  }

		for (int i=PIN_INI; i<=PIN_FIM; i++) {

		    
			if (i !=PIN_CAPO ) { // CAPO tem a logica inversa. Low é fechado, hight aberto
				// Lendo sinais do painel
				if (digitalRead(i) == LOW){
					digitalPins |= (1UL << bitposition);

				}
			}else{
				if (digitalRead(i)){ //Capô
					digitalPins |= (1UL << bitposition);
	
				}
			}
			bitposition++;
		  }
		  //2^6 
		
		  return digitalPins;
	} 

void salvarNivelCombustivel(int nivel){
    byte h = highByte(nivel);
    byte l = lowByte(nivel);

    EEPROM.write(ENDERECO_NIVEL, h);
    EEPROM.write(ENDERECO_NIVEL+1, l);

}
int obterNivelCombustivelMemoria(){
	
      byte h =  EEPROM.read(ENDERECO_NIVEL);
      byte l =  EEPROM.read(ENDERECO_NIVEL+1);
      return word(h,l);

}

};

 


