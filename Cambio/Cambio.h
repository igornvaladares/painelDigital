#include "Arduino.h"
#define VOLT_MIN_REFERENCIA 0
#define VOLT_MAX_REFERENCIA 500

#define VALOR_MIN_REFERENCIA_SENSOR_PRESSAO 0
#define VALOR_MAX_REFERENCIA_SENSOR_PRESSAO 100
class Cambio
{
    private:
	Util util;
        uint8_t PinSelecao;
        uint8_t PinEngate;
	uint8_t PinPressaoDualogic;
	uint8_t Pin1Joystick;
	uint8_t Pin2Joystick;

	bool modoAutomatico;

	signed char calcularMarcha(int voltSelecao, int voltEngate){
              
		signed char marcha= 1; // Neutro
		switch (voltSelecao) {
			 case 0 ... 199: // EM CIMA
				switch (voltEngate) {
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
				switch (voltEngate) {
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
				switch (voltEngate) {
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

	Cambio(uint8_t pinSelecao,uint8_t pinEngate,uint8_t pinPressaoDualogic, uint8_t pin1Joystick, uint8_t pin2Joystick){

		pinMode(pinSelecao, INPUT);
		pinMode(pinEngate, INPUT);
		pinMode(PinPressaoDualogic, INPUT);
		
		pinMode(pin1Joystick, INPUT);
		pinMode(pin2Joystick, INPUT);

		PinSelecao = pinSelecao;
		PinEngate = pinEngate;
		PinPressaoDualogic = pinPressaoDualogic;
		Pin1Joystick = pin1Joystick;
		Pin2Joystick = pin2Joystick;

		modoAutomatico= EEPROM.read(ENDERECO_MODOAUTOMATICO);
		util.iniciaTimer4(TIMER_4); // Iniciar timer4 para controle de 'delay'    

	}

	 signed char obterMarchaEngatada(){


		int voltSelecao; 
		int voltEngate; 

		voltSelecao = map(analogRead(PinSelecao), 0, 1023, VOLT_MIN_REFERENCIA, VOLT_MAX_REFERENCIA);			
		voltEngate  = map(analogRead(PinEngate), 0, 1023, VOLT_MIN_REFERENCIA, VOLT_MAX_REFERENCIA);			

		return calcularMarcha(voltSelecao,voltEngate);

	}


	 int obterPressaoDualogic(){


         	return map(analogRead(PinPressaoDualogic), 0, 1023, VALOR_MIN_REFERENCIA_SENSOR_PRESSAO, VALOR_MAX_REFERENCIA_SENSOR_PRESSAO);			



	}

	long int obterModoAutomatico(long int sensores){



		int volt1 = map(analogRead(Pin1Joystick), 0, 1023, VOLT_MIN_REFERENCIA, VOLT_MAX_REFERENCIA); //Fio2			
		int volt2  = map(analogRead(Pin2Joystick), 0, 1023, VOLT_MIN_REFERENCIA, VOLT_MAX_REFERENCIA); //Fio3			


		switch (volt1) {
			case 7 ... 210:
			switch (volt2) {
				case 7 ... 210: 
					if (util.saidaTimer4()){ // se a ação demorar "TIMER4" segundos
						
						modoAutomatico =! modoAutomatico;
						EEPROM.write(modoAutomatico,ENDERECO_MODOAUTOMATICO);
						util.reIniciaTimer4();
					}
					break;	
				default: 
					util.reIniciaTimer4();
			}
			break;
		default:
			 util.reIniciaTimer4();		
		
		}

		if (modoAutomatico) // 28 = ultimo bit para sensor, portanto o bit 29 vai dizer ao realdash a indicção do modo automativo
		    sensores |= (1UL << 29);


         	return sensores;



	}

};


