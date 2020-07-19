#include "Pulso.h"
#include "EEPROM.h"
#include <Util.h>
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
	long distanciaPercorrida1Segundo;
	uint8_t PinTemperaturaAguaRadiador;

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

		util.iniciaTimer1(1000);
	
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
			double diametroRoda = 60; // cm
			long countPulso = pulso->getPulsoVelocidade();

			util.reIniciaTimer1();	// << VERIFICAR 
			pulso->reiniciarVelocidade();
			
			double perimetro = diametroRoda * PI;// cm	
			KM=+(perimetro * countPulso/100/1000); 
			
			distanciaPercorrida1Segundo = (perimetro * countPulso/100/1000);

			byte h = highByte(KM);
			byte l = lowByte(KM);

			EEPROM.write(ENDERECO_ODOMETRO, h);
			EEPROM.write(ENDERECO_ODOMETRO+1, l);


		}

		return KM;

	}


	int obterTemperaturaAguaRadiador(){

		int sensorTemperaturaRadiador = analogRead(PinTemperaturaAguaRadiador);

	        int temperatura  = map(sensorTemperaturaRadiador, 0, 1023, VALOR_MIN_TEMP_RADIADOR, VALOR_MAX_TEMP_RADIADOR);			

		return temperatura;	
	}




	double obterVelocidade(){

		return distanciaPercorrida1Segundo*60*60;


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

 


