#include "Pulso.h"
#include "EEPROM.h"
#include "Util.h"
#define VALOR_MIN_TEMP_RADIADOR 20
#define VALOR_MAX_TEMP_RADIADOR 120

class Motor{ 

private:
	Pulso * pulso; 
	Util util;
	double rpm;
	double velocidade;
	double KM=0;
	double KMPercorrida1Segundo;
	uint8_t PinTemperaturaAguaRadiador;
	int temperatura;

   public:

        Motor(uint8_t pinRotacao,uint8_t pinVelocidade,uint8_t pinTemperaturaAguaRadiador){
	
		pinMode(pinRotacao, INPUT_PULLUP);
		pinMode(pinTemperaturaAguaRadiador, INPUT);
		
		PinTemperaturaAguaRadiador = pinTemperaturaAguaRadiador;

		//Punso de interrupção Rotação e Velocidade
		pulso = new Pulso(pinRotacao,pinVelocidade);
		
		//byte h =  EEPROM.read(ENDERECO_ODOMETRO);
		//byte l =  EEPROM.read(ENDERECO_ODOMETRO+1);
		//KM = word(h,l);
		util.iniciaTimer1(TIMER_1); // Iniciar timer1 para controle de 'delay'
		util.iniciaTimer2(TIMER_2); // Iniciar timer1 para controle de 'delay'
	
	};

       double obterRpm(){
				
		if (util.saidaTimer1()){
			long countPulso = pulso->getPulsoRpm();
			pulso->reiniciarRpm();
			
			rpm = countPulso*60; // Segudo ---> Minuto

			
		}

		return rpm;

	}

	double obterOdometro(){

		if (util.saidaTimer1()){
			util.reIniciaTimer1();	
			double diametroRoda = 60; // cm
			long countPulso = pulso->getPulsoVelocidade();

			pulso->reiniciarVelocidade();
			
			double perimetro = diametroRoda * PI;// cm
	
			KMPercorrida1Segundo = (perimetro * countPulso/100/1000);
			KM += KMPercorrida1Segundo; 
			
			//byte h = highByte(KM);
			//byte l = lowByte(KM);

			//EEPROM.write(ENDERECO_ODOMETRO, h);
			//EEPROM.write(ENDERECO_ODOMETRO+1, l);



		}

		return KM;

	}


	int obterTemperaturaAguaRadiador(){

		if (util.saidaTimer2()){
			//OBS: Meio do Tanque a partir de 0,74 Volts e ventoinha liga a partir de 0.59v	

			 //2.82 > 26 graus (Refenrencia)			
		         //1.9 -> 45 graus 		
			 //0.94 -> 75 graus 

			///2.5 -> 30
			// 1.3 -> 60 graau
			// 0.6 -> 100 graus
			
			//3.75 -> 20 graus * 0.004887586 = 781
			//3.11 -> 24 graus =
			//0.48 -> 120 graus * 0.004887586 = 100

			//float voltPorUnidade = 0.004887586;
			temperatura  = map(analogRead(PinTemperaturaAguaRadiador), 781, 100, VALOR_MIN_TEMP_RADIADOR, VALOR_MAX_TEMP_RADIADOR);			
			//Serial.println("temperatura");		
			util.reIniciaTimer2();
		}

		return temperatura;	
	}




	double obterVelocidade(){

		return KMPercorrida1Segundo*60*60;


	}

/*
	double obterVelocidade(){

		if (util.saidaTimer1()){
			util.reIniciaTimer1();	// << VERIFICAR 
			double diametroRoda = 60; // cm
			long countPulso = pulso->getPulsoVelocidade();
			pulso->reiniciarVelocidade();
			double perimetro = diametroRoda * PI;// cm	
			double distanciaPercorrida = perimetro * countPulso; // cm
			
				// cm ---> m ---> km
			velocidade = (distanciaPercorrida/100/1000)*60*60;
								 	// s ---> min ---> h

		}

		return velocidade;

	}

*/
};

 


