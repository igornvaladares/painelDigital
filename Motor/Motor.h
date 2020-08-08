#include "Pulso.h"
#include "EEPROM.h"
#include "Util.h"
#define VALOR_MIN_TEMP_RADIADOR 0
#define VALOR_MAX_TEMP_RADIADOR 125
#define ENDERECO_ODOMETRO 0
#define TEMPO_TIMER1 1000
class Motor{ 

private:
	Pulso * pulso; 
	Util util;
	double rpm;
	double velocidade;
	long KM;
	long KMPercorrida1Segundo;
	uint8_t PinTemperaturaAguaRadiador;
	int temperatura;

   public:

        Motor(uint8_t pinRotacao,uint8_t pinVelocidade,uint8_t pinTemperaturaAguaRadiador){
	
		pinMode(pinRotacao, INPUT_PULLUP);
		pinMode(pinTemperaturaAguaRadiador, INPUT);
		
		PinTemperaturaAguaRadiador = pinTemperaturaAguaRadiador;

		//Punso de interrupção Rotação e Velocidade
		pulso = new Pulso(pinRotacao,pinVelocidade);
		
		byte h =  EEPROM.read(ENDERECO_ODOMETRO);
		byte l =  EEPROM.read(ENDERECO_ODOMETRO+1);
		KM = word(h,l);
		util.iniciaTimer1(TIMER_1); // Iniciar timer1 para controle de 'delay'
		util.iniciaTimer2(TIMER_2); // Iniciar timer2 para controle de 'delay'    
	
	};

       double obterRpm(){
				
		if (util.saidaTimer1()){
			long countPulso = pulso->getPulsoRpm();
			pulso->reiniciarRpm();
			
			rpm = countPulso*60; // Segudo ---> Minuto

			
		}

		return rpm;

	}

	long obterOdometro(){

		if (util.saidaTimer1()){
			util.reIniciaTimer1();	
			double diametroRoda = 60; // cm
			long countPulso = pulso->getPulsoVelocidade();

			pulso->reiniciarVelocidade();
			
			double perimetro = diametroRoda * PI;// cm
	
			KMPercorrida1Segundo = (perimetro * countPulso/100/1000);

			KM=+KMPercorrida1Segundo; 
			
			byte h = highByte(KM);
			byte l = lowByte(KM);

			EEPROM.write(ENDERECO_ODOMETRO, h);
			EEPROM.write(ENDERECO_ODOMETRO+1, l);


		}

		return KM;

	}


	int obterTemperaturaAguaRadiador(){

		if (util.saidaTimer2()){

			temperatura  = map(analogRead(PinTemperaturaAguaRadiador), 0, 1023, VALOR_MIN_TEMP_RADIADOR, VALOR_MAX_TEMP_RADIADOR);			
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

 


