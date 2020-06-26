#include "Bobina.h"
#include <Util.h>
#define TEMPO_TIMER1 1000
class Motor{ 

private:
	Bobina * bobina; 
	Util util;
	double rpm;
        uint8_t PinNivelAguaRadiador;
        uint8_t PinStatusVentiladorRadiador;
	uint8_t PinTemperaturaAguaRadiador;

   public:

        Motor(uint8_t pinRotacao,uint8_t pinNivelAguaRadiador,uint8_t 	pinStatusVentiladorRadiador,uint8_t pinTemperaturaAguaRadiador){

		pinMode(pinRotacao, INPUT_PULLUP);
		pinMode(pinNivelAguaRadiador, INPUT);
		pinMode(pinStatusVentiladorRadiador, INPUT);
		pinMode(pinTemperaturaAguaRadiador, INPUT);
		
		PinNivelAguaRadiador = pinNivelAguaRadiador;
		PinStatusVentiladorRadiador = pinStatusVentiladorRadiador;
		PinTemperaturaAguaRadiador = pinTemperaturaAguaRadiador;
		
		bobina = new Bobina(pinRotacao);
		
		util.iniciaTimer1(1000);
	
	};

       double obterRpm(){
				
		if (util.saidaTimer1()){
		
			float multiplicador = 1000/util.getTempoEsperaTimer1();
			long pulso = bobina->getPulso();
			bobina->reiniciar();
			util.reIniciaTimer1();	
			rpm = pulso*60; // Segudo ---> Minuto

		}

		return rpm;

	}
    

	int obterNivelAguaRadiador(){

		analogWrite(PinNivelAguaRadiador, 0);

		return 1;	
	}
	uint8_t obterStatusVentiladorRadiador(){

		analogWrite(PinStatusVentiladorRadiador, 0);

		return 1;	
	}
	int obterTemperaturaAguaRadiador(){

		analogWrite(PinTemperaturaAguaRadiador, 0);

		return 1;	
	}

};

 


