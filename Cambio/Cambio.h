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
	uint8_t Pin1Joystick;
	uint8_t Pin2Joystick;
	signed char marcha= 1; // Neutro
	bool modoAutomatico;

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

	unsigned long int obterModoAutomatico(unsigned long int sensores){



		int volt1 = map(util.estabilizarEntrada(Pin1Joystick), 0, 1023, VOLT_MIN_REFERENCIA, VOLT_MAX_REFERENCIA); //Fio2			
		int volt2  = map(util.estabilizarEntrada(Pin2Joystick), 0, 1023, VOLT_MIN_REFERENCIA, VOLT_MAX_REFERENCIA); //Fio3			

		

		switch (volt1) {
			case 70 ... 210:
			switch (volt2) {
				case 70 ... 210: 
					if (util.saidaTimer4()){ // se a ação demorar "TIMER4" segundos
						
						modoAutomatico =! modoAutomatico;
						EEPROM.write(modoAutomatico,ENDERECO_MODOAUTOMATICO);
						util.reIniciaTimer4();
						//Serial.print("Sensores:");
						//		Serial.print(volt1);
						//		Serial.print("-");
						//		Serial.println(volt2);
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


