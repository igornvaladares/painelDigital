#include "Arduino.h"
#define VOLT_MIN_REFERENCIA 0
#define VOLT_MAX_REFERENCIA 500
#define DURACAO_ACAO 300
#define VOLTAGEM_ALTA 3
#define VOLTAGEM_BAIXA 2
#define AGUARDAR_PARA_MUDAR_MARCHA 500
#define AGUARDAR_PARA_MUDAR_AUTO_MANUAL 500
#define D 2
#define N 1

class Cambio
{
    private:
	Util util;
        uint8_t PinSelecao;
        uint8_t PinEngate;
	uint8_t PinPressaoDualogic;

	//Saida
        uint8_t Pin4;
        uint8_t Pin5;
		uint8_t Pin6;
        uint8_t Pin7;
	//Entrada
        uint8_t PinAn4;
        uint8_t PinAn5;
        uint8_t PinAn6;
        uint8_t PinAn7;

	signed char marcha= 1; // Neutro
	bool modoAutomatico = true;
	
	

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
	void liberarReles(){


	    digitalWrite(Pin4,HIGH);
	    digitalWrite(Pin5,HIGH);
	    digitalWrite(Pin6,HIGH);
	    digitalWrite(Pin7,HIGH);

	}
	bool sinalAlavancaAcionadaParaAutoOuManual(){

	return  (util.estabilizarEntrada(PinAn4)< VOLTAGEM_BAIXA  &&
        	util.estabilizarEntrada(PinAn5)< VOLTAGEM_BAIXA  &&
	        util.estabilizarEntrada(PinAn6)> VOLTAGEM_ALTA &&
        	util.estabilizarEntrada(PinAn7)> VOLTAGEM_ALTA);
			

	}
    public:
	
	bool sinalAlavancaAcionadaParaCima(){

	return  (util.estabilizarEntrada(PinAn4)< VOLTAGEM_BAIXA  &&
        	util.estabilizarEntrada(PinAn5)> VOLTAGEM_ALTA  &&
	        util.estabilizarEntrada(PinAn6)< VOLTAGEM_BAIXA &&
        	util.estabilizarEntrada(PinAn7)> VOLTAGEM_ALTA);


	}


	bool alavancaAcionadaParaCima(){

			bool estado;

			if (sinalAlavancaAcionadaParaCima())
			{
				long tempo = millis();
				estado = sinalAlavancaAcionadaParaCima();
				if (estado)
				{
					while (estado)
						estado = sinalAlavancaAcionadaParaCima();
					
					return true;
				
				}  
			}
			return false;


	}
	bool alavancaAcionadaParaAutoOuManual(){

			bool estado;

			if (sinalAlavancaAcionadaParaAutoOuManual())
			{
				long tempo = millis();
				estado = sinalAlavancaAcionadaParaAutoOuManual();
				if (estado)
				{
					while (estado)
						estado = sinalAlavancaAcionadaParaAutoOuManual();
					
					if ((millis()-tempo)>= DURACAO_ACAO) return true; else false;
				
				}  
			}
			return false;


	}
	void atualizarModoAutoManual(){
		
		modoAutomatico = !modoAutomatico;

	}
	bool isModoAutomatico(){

	  return modoAutomatico;
  
	}
	bool isModoManual(){

	  return !modoAutomatico;

	}

	void mudarParaAutomatico(int apartirDo){
		
		if  (apartirDo != N)
  			mudarParaManualOuAutomaticoApartirDoD();

	}
	void mudarParaManual(int apartirDo){
	 
	 	if  (apartirDo != N)
  			mudarParaManualOuAutomaticoApartirDoD();

	}

	void mudarParaManualOuAutomaticoApartirDoD(){
		
  	  	util.bloquear(AGUARDAR_PARA_MUDAR_AUTO_MANUAL);
		
        digitalWrite(Pin4,LOW); // pin 5 jostick
    	digitalWrite(Pin6,LOW);// pin 3 jostick
        util.bloquear(DURACAO_ACAO);
	  	liberarReles();
		atualizarModoAutoManual();
  	  	util.bloquear(AGUARDAR_PARA_MUDAR_AUTO_MANUAL);


	}

	void avancarUmaMarcha(){
	    util.bloquear(AGUARDAR_PARA_MUDAR_MARCHA);
	    digitalWrite(Pin4,LOW); // pin 5 jostick
	    digitalWrite(Pin5,LOW); // pin 4 jostick
	    digitalWrite(Pin6,LOW); // pin 3 jostick
	    digitalWrite(Pin7,LOW); // pin 2 jostick
	    util.bloquear(DURACAO_ACAO);
        liberarReles();
	    util.bloquear(AGUARDAR_PARA_MUDAR_MARCHA);

	}


	Cambio(uint8_t pinSelecao,uint8_t pinEngate,uint8_t pinPressaoDualogic,
	       uint8_t pin4,		uint8_t pin5,		uint8_t pin6,		uint8_t pin7,
	       uint8_t pinAn4,		uint8_t pinAn5,		uint8_t pinAn6,		uint8_t pinAn7
		
		){

		PinSelecao = pinSelecao;
		PinEngate = pinEngate;
		PinPressaoDualogic = pinPressaoDualogic;

		PinAn4 = pinAn4;//pin 5 jostick
		PinAn5 = pinAn5;//pin 4 jostick
		PinAn6 = pinAn6;//pin 3 jostick
		PinAn7 = pinAn7;//pin 2 jostick

		pinMode(pin4, OUTPUT);// pin 5 jostick
		pinMode(pin5, OUTPUT);// pin 4 jostick
		pinMode(pin6, OUTPUT);// pin 3 jostick
		pinMode(pin7, OUTPUT);// pin 2 jostick
		Pin4 = pin4;
		Pin5 = pin5;
		Pin6 = pin6;
		Pin7 = pin7;
		liberarReles();	
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


