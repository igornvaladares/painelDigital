#include "Arduino.h"
#define VALOR_MIN_REFERENCIA_SENSOR 24
#define VALOR_MAX_REFERENCIA_SENSOR 478

#define VALOR_MIN_REFERENCIA_SENSOR_PRESSAO 0
#define VALOR_MAX_REFERENCIA_SENSOR_PRESSAO 100

class Cambio
{
    private:

        uint8_t PinSelecao;
        uint8_t PinEngate;
	uint8_t PinPressaoDualogic;

	signed char calcularMarcha(int valorSelecao, int valorEngate){
              
		signed char marcha= 1; // Neutro
		switch (valorSelecao) {
			 case 0 ... 199: // EM CIMA
				switch (valorEngate) {
					 case 0 ... 199: // DIREITA
					 	marcha= 6; //  5 - Marcha
					 break;
					 case 200 ... 299:
					 	marcha= 1;  // NEUTRO
					 break;
					 case 300 ... 500: //  ESQUEDA
					 	marcha= 0; // Ré
					 break;
				}
			 break;
			 case 200 ... 299: // NO MEIO
				switch (valorEngate) {
					 case 0 ... 199:// DIREITA
					 	marcha = 2; // 3 - Marcha
					 break;
					 case 200 ... 299:
					 	marcha = 1;  // NEUTRO
					 break;
					 case 300 ... 500:  //  ESQUEDA
					 	marcha = 3; // 4 - Marcha
					 break;
				}
			 break;
			 case 300 ... 500: //EM BAIXO
				switch (valorEngate) {
					 case 0 ... 199:// DIREITA
					 	marcha = 2; // 1 - Marcha
					 break;
					 case 200 ... 299:
					 	marcha = 1;  // NEUTRO
					 break;
					 case 300 ... 500:  //  ESQUEDA
					 	marcha = 3; // 2 - Marcha
					break;
					}
			 break;
		}
		
		return marcha;
	}

    public:

	Cambio(uint8_t pinSelecao,uint8_t pinEngate,uint8_t pinPressaoDualogic){

		pinMode(pinSelecao, INPUT);
		pinMode(pinEngate, INPUT);
		pinMode(PinPressaoDualogic, INPUT);

		PinSelecao = pinSelecao;
		PinEngate = pinEngate;
		PinPressaoDualogic = pinPressaoDualogic;

	}

	 signed char obterMarchaEngatada(){


		int valorSelecao; 
		int valorEngate; 

		valorSelecao = map(analogRead(PinSelecao), 0, 1023, VALOR_MIN_REFERENCIA_SENSOR, VALOR_MAX_REFERENCIA_SENSOR);			
		valorEngate  = map(analogRead(PinEngate), 0, 1023, VALOR_MIN_REFERENCIA_SENSOR, VALOR_MAX_REFERENCIA_SENSOR);			

		return calcularMarcha(valorSelecao,valorEngate);

	}


	 int obterPressaoDualogic(){

		float voltPorUnidade = 0.004887586;
         	return map(analogRead(PinPressaoDualogic), 0, 1023, VALOR_MIN_REFERENCIA_SENSOR_PRESSAO, VALOR_MAX_REFERENCIA_SENSOR_PRESSAO);			



	}

};


