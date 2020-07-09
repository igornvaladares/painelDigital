#include "Arduino.h"
#define VALOR_MIN_REFERENCIA_SENSOR 24
#define VALOR_MAX_REFERENCIA_SENSOR 478
class Cambio
{
    private:
        uint8_t PinSelecao;
        uint8_t PinEngate;

        char calcularMarcha(int valorSelecao, int valorEngate){
                char marcha=' '; 
		switch (valorSelecao) {
			 case 0 ... 50: // EM CIMA
				switch (valorEngate) {
					 case 0 ... 50: // DIREITA
					 	marcha = 'R'; // Ré
					 break;
					 case 200 ... 250:
					 	marcha = 'N';  // NEUTRO
					 break;
					 case 450 ... 500: //  ESQUEDA
					 	marcha = '5'; // 5 - Marcha
					 break;
				}
			 break;
			 case 200 ... 250: // NO MEIO
				switch (valorEngate) {
					 case 0 ... 50:// DIREITA
					 	marcha = '3'; // 3 - Marcha
					 break;
					 case 200 ... 250:
					 	marcha = 'N';  // NEUTRO
					 break;
					 case 450 ... 500:  //  ESQUEDA
					 	marcha = '4'; // 4 - Marcha
					 break;
				}
			 break;
			 case 450 ... 500: //EM BAIXO
				switch (valorEngate) {
					 case 0 ... 50:// DIREITA
					 	marcha = '1'; // 1 - Marcha
					 break;
					 case 200 ... 250:
					 	marcha = '0';  // NEUTRO
					 break;
					 case 450 ... 500:  //  ESQUEDA
					 	marcha = '2'; // 2 - Marcha
					break;
					}
			 break;
		}
		
		return marcha;
	};

    public:

	Cambio(uint8_t pinSelecao,uint8_t pinEngate){

		    pinMode(pinSelecao, INPUT);
		    pinMode(pinEngate, INPUT);

		    PinSelecao = pinSelecao;
		    PinEngate = pinEngate;

	}

	 char obterMarchaEngatada(){

		  int sensorSelecao =  analogRead(PinSelecao);
		  int sensorEngate =  analogRead(PinEngate);

		  int valorSelecao; 
		  int valorEngate; 

		  valorSelecao = map(sensorSelecao, 0, 1023, VALOR_MIN_REFERENCIA_SENSOR, VALOR_MAX_REFERENCIA_SENSOR);			
		  valorEngate  = map(sensorEngate, 0, 1023, VALOR_MIN_REFERENCIA_SENSOR, VALOR_MAX_REFERENCIA_SENSOR);			
	   
		 return calcularMarcha(valorSelecao,valorEngate);
		 // return 'N';

	}
};


