#include "Arduino.h"
#define VOLT_MIN_REFERENCIA 0
#define VOLT_MAX_REFERENCIA 500
class Cambio
{
    private:
	Util util;
        uint8_t PinSelecao;
        uint8_t PinEngate;
	uint8_t PinPressaoDualogic;
	signed char marcha= 1; // Neutro

	signed char calcularMarcha(int voltSelecao, int voltEngate){
            
		switch (voltSelecao) {
			  case 0 ... 199: // EM CIMA
				switch (voltEngate) {
					 case 0 ... 199: // DIREITA
					 	marcha= 6; //  5 - Marcha
					 break;
					 case 200 ... 269:
				//	 	marcha= 1;  // NEUTRO
					 break;
					 case 270 ... 500: //  ESQUEDA
					 	marcha= 0; // Ré
					 break;
				}
			 break;
			 case 200 ... 269: // NO MEIO
				switch (voltEngate) {
					 case 0 ... 170:// DIREITA
					 	marcha = 4; // 3 - Marcha
					 break;
					 case 171 ... 269:
					 	marcha = 1;  // NEUTRO
					 break;
					 case 270 ... 500:  //  ESQUEDA
					 	marcha = 5; // 4 - Marcha
					 break;
				}
			 break;
			 case 270 ... 500: //EM BAIXO
				switch (voltEngate) {
					 case 0 ... 199:// DIREITA
					 	marcha = 2; // 1 - Marcha
					 break;
					 case 200 ... 269:
				//	 	marcha = 1;  // NEUTRO
					 break;
					 case 270 ... 500:  //  ESQUEDA
					 	marcha = 3; // 2 - Marcha
					 break;
					}
			 break;
	}	
		
	return marcha;

    }

    public:

	Cambio(uint8_t pinSelecao,uint8_t pinEngate,uint8_t pinPressaoDualogic, uint8_t pin1Joystick, uint8_t pin2Joystick){

		pinMode(pinSelecao, INPUT);
		pinMode(pinEngate, INPUT);
		pinMode(PinPressaoDualogic, INPUT);
		
	
		PinSelecao = pinSelecao;
		PinEngate = pinEngate;
		PinPressaoDualogic = pinPressaoDualogic;
	
		util.iniciaTimer4(TIMER_4); // Iniciar timer4 para controle de 'delay'    

	}

	 signed char obterMarchaEngatada(){


		if (util.saidaTimer4()){ 
			int voltSelecao, entradaSelecao; 
			int voltEngate, entradaEngate; 
			entradaSelecao = util.estabilizarEntrada(PinSelecao);
			entradaEngate = util.estabilizarEntrada(PinEngate);
			if (entradaEngate>50 && entradaSelecao>50){
				voltSelecao = map(entradaSelecao, 0, 1023, VOLT_MIN_REFERENCIA, VOLT_MAX_REFERENCIA);			
				voltEngate  = map(entradaEngate, 0, 1023, VOLT_MIN_REFERENCIA, VOLT_MAX_REFERENCIA);			
				marcha = calcularMarcha(voltSelecao,voltEngate);

				//Serial.print("entradaSelecao:");
				//Serial.print(voltSelecao);

				//Serial.print("entradaEngate:");
				//Serial.print(voltEngate);
 
			}
			util.reIniciaTimer4();
		}
		return marcha;

	}


	 int obterPressaoDualogic(){

		int anaLogico =0;
         	anaLogico = util.estabilizarEntrada(PinPressaoDualogic);	
		//576un > 36bar
		//481un > 46bar
		//____________________
		// 95  - 10
		// x      1
 
		// x = 9.5
		// 1024	/ 9.5 = 108	

		//Serial.print("Pressao:");
		//Serial.print(anaLogico);
		
		//Serial.print("-");
		//Serial.println(map(anaLogico, 0, 1023, 0, 108)-14);
		
		return map(anaLogico, 0, 1023, 0, 108)-7;

	}


};


