#include "Pulso.h"
#include <Util.h>
#define VALOR_MIN_TEMP_RADIADOR 0
#define VALOR_MAX_TEMP_RADIADOR 125

#define TEMPO_TIMER1 1000
class Motor{ 

private:
	Pulso * pulso; 
	Util util;
	double rpm;
	double velocidade;
	uint8_t PinTemperaturaAguaRadiador;

   public:

        Motor(uint8_t pinRotacao,uint8_t pinVelocidade,uint8_t pinTemperaturaAguaRadiador){
	
		pinMode(pinRotacao, INPUT_PULLUP);
		pinMode(pinTemperaturaAguaRadiador, INPUT);
		
		PinTemperaturaAguaRadiador = pinTemperaturaAguaRadiador;

		//Punso de interrupção Rotação e Velocidade
		pulso = new Pulso(pinRotacao,pinVelocidade);
		util.iniciaTimer1(1000);
	
	};

       double obterRpm(){
				
		if (util.saidaTimer1()){
		
			long countPulso = pulso->getPulsoRpm();
			pulso->reiniciarRpm();
			util.reIniciaTimer1();	
			rpm = countPulso*60; // Segudo ---> Minuto

		}

		return rpm;

	}
    
	double obterVelocidade(){
				
		if (util.saidaTimer1()){

			double diametroRoda = 60; // cm
			double countPulso = pulso->getPulsoVelocidade();
			pulso->reiniciarVelocidade();
			util.reIniciaTimer1();
			double perimetro = diametroRoda * PI;// cm	
			double distanciaPercorrida = perimetro * countPulso; // cm
							// cm ---> m ---> km
			velocidade = (distanciaPercorrida/100/1000)*60*60;
								 	// s ---> min ---> h

		}

		return velocidade;

	}
    
	int obterTemperaturaAguaRadiador(){

		int sensorTemperaturaRadiador = analogRead(PinTemperaturaAguaRadiador);

	        int temperatura  = map(sensorTemperaturaRadiador, 0, 1023, VALOR_MIN_TEMP_RADIADOR, VALOR_MAX_TEMP_RADIADOR);			

		return temperatura;	
	}

};

 


