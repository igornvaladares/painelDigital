#include "Rpm.h"

class Motor{ 
	Rpm * rpm; 
   public:
        Motor(uint8_t pinRotacao,uint8_t pinNivelAguaRadiador,uint8_t 	pinStatusVentiladorRadiador,uint8_t pinTemperaturaAguaRadiador){

	pinMode(pinRotacao, INPUT_PULLUP);
    	
	pinMode(pinNivelAguaRadiador, INPUT);
	pinMode(pinStatusVentiladorRadiador, INPUT);
	pinMode(pinTemperaturaAguaRadiador, INPUT);
	


	pinAgua = pinNivelAguaRadiador;
	pinVentilador = pinStatusVentiladorRadiador;
	pinTemperatura = pinTemperaturaAguaRadiador;
	
	rpm = new Rpm(pinRotacao);


	};
       double obterRpm(){

     		delay(1000); // Tempo que faz a leitura dos pulsos.
		long pulso = rpm->getPulso();
		rpm->reiniciar();
		return pulso*60; // Segudo ---> Minuto

	}
    

	int obterNivelAguaRadiador(){

	    analogWrite(pinAgua, 0);

	   return 1;	
	}
	uint8_t obterStatusVentiladorRadiador(){

	    analogWrite(pinVentilador, 0);

	   return 1;	
	}

	int obterTemperaturaAguaRadiador(){

	    analogWrite(pinTemperatura, 0);

	   return 1;	
	}

  private:
        uint8_t pinRpm;
        uint8_t pinAgua;
        uint8_t pinVentilador;
        uint8_t pinTemperatura;


};

 


