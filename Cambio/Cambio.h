#include "Arduino.h"
#define VALOR_MIN_REFERENCIA_SENSOR 24
#define VALOR_MAX_REFERENCIA_SENSOR 478
class Cambio
{
    private:

        uint8_t PinSelecao;
        uint8_t PinEngate;

        signed char calcularMarcha(int valorSelecao, int valorEngate){
              
		signed char marcha= 1; // Neutro
		switch (valorSelecao) {
			 case 0 ... 50: // EM CIMA
				switch (valorEngate) {
					 case 0 ... 50: // DIREITA
					 	marcha= 0; // Ré
					 break;
					 case 200 ... 250:
					 	marcha= 1;  // NEUTRO
					 break;
					 case 450 ... 500: //  ESQUEDA
					 	marcha= 6; // 5 - Marcha
					 break;
				}
			 break;
			 case 200 ... 250: // NO MEIO
				switch (valorEngate) {
					 case 0 ... 50:// DIREITA
					 	marcha = 2; // 3 - Marcha
					 break;
					 case 200 ... 250:
					 	marcha = 1;  // NEUTRO
					 break;
					 case 450 ... 500:  //  ESQUEDA
					 	marcha = 3; // 4 - Marcha
					 break;
				}
			 break;
			 case 450 ... 500: //EM BAIXO
				switch (valorEngate) {
					 case 0 ... 50:// DIREITA
					 	marcha = 2; // 1 - Marcha
					 break;
					 case 200 ... 250:
					 	marcha = 1;  // NEUTRO
					 break;
					 case 450 ... 500:  //  ESQUEDA
					 	marcha = 3; // 2 - Marcha
					break;
					}
			 break;
		}
		
		return marcha;
	}

    public:

	Cambio(uint8_t pinSelecao,uint8_t pinEngate){

		pinMode(pinSelecao, INPUT);
		pinMode(pinEngate, INPUT);

		PinSelecao = pinSelecao;
		PinEngate = pinEngate;

	}

	 signed char obterMarchaEngatada(){


		int valorSelecao; 
		int valorEngate; 

		valorSelecao = map(analogRead(PinSelecao), 0, 1023, VALOR_MIN_REFERENCIA_SENSOR, VALOR_MAX_REFERENCIA_SENSOR);			
		valorEngate  = map(analogRead(PinEngate), 0, 1023, VALOR_MIN_REFERENCIA_SENSOR, VALOR_MAX_REFERENCIA_SENSOR);			

		return calcularMarcha(valorSelecao,valorEngate);

	}
};


