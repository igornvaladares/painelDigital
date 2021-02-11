#include "Pulso.h"
#include "Util.h"
#define VALOR_MIN_TEMP_RADIADOR 20
#define VALOR_MAX_TEMP_RADIADOR 120

class Motor{ 

private:
	Pulso * pulso; 
	Util util;
	double rpm;
	double velocidade;
	double KM=0;
	double KMPercorridaMeioSegundo;
	uint8_t PinTemperaturaAguaRadiador;
	int nivelMemoriaTemperatura=0;
	double ref=0;
	int countPulsoAnt;

   public:

        Motor(uint8_t pinRotacao,uint8_t pinVelocidade,uint8_t pinTemperaturaAguaRadiador){
	
		pinMode(pinRotacao, INPUT_PULLUP);
		pinMode(pinTemperaturaAguaRadiador, INPUT);
		
		PinTemperaturaAguaRadiador = pinTemperaturaAguaRadiador;

		//Punso de interrupção Rotação e Velocidade
		pulso = new Pulso(pinRotacao,pinVelocidade);
		
		//byte h =  EEPROM.read(ENDERECO_ODOMETRO);
		//byte l =  EEPROM.read(ENDERECO_ODOMETRO+1);
		//KM = word(h,l);
		util.iniciaTimer1(TIMER_1); // Iniciar timer1 para controle de 'delay'
		util.iniciaTimer2(TIMER_2); // Iniciar timer1 para controle de 'delay'
		
	};

       double obterRpm(){
				
		if (util.saidaTimer1()){
			int countPulso = pulso->getPulsoRpm();
			if (abs(countPulsoAnt-countPulso)!=1)			
				countPulsoAnt = countPulso;

			pulso->reiniciarRpm();
			//1s    1/2s   1/4s   1/8s
			//60 -> 120 -> 240 -> 480
			rpm = countPulsoAnt*120; // Segudo
			//rpm++;
			//Serial.println("Pulso RPM:");							
			//Serial.println(countPulso);			
		}

				
		return rpm;

	}

	double obterOdometro(){

		if (util.saidaTimer1()){
			util.reIniciaTimer1();	
			double diametroRoda = 60; // cm
			//14 pulsos = uma volta completa
			double pulsoPorVolta = 16;			

			long countPulso = pulso->getPulsoVelocidade();
			pulso->reiniciarVelocidade();
			
			double perimetro = diametroRoda * PI;// cm
	

			KMPercorridaMeioSegundo = ((perimetro * (countPulso / pulsoPorVolta))/100/1000);
			KM += KMPercorridaMeioSegundo; 

			//byte h = highByte(KM);
			//byte l = lowByte(KM);

			//EEPROM.write(ENDERECO_ODOMETRO, h);
			//EEPROM.write(ENDERECO_ODOMETRO+1, l);



		
		}
		
		return KM;

	}


	int obterTemperaturaAguaRadiador(){

		if (util.saidaTimer2()){
			//OBS: Meio do Tanque a partir de 0,74 Volts e ventoinha liga a partir de 0.60v	(95 graus)

			//float voltPorUnidade = 0.004887586;
			int nivelAtual=0;


			nivelAtual = util.estabilizarEntrada(PinTemperaturaAguaRadiador);
			//Serial.println("pintemperatura");
			//Serial.print(temperatura);
			
			//	440un -> 40 Graus
			//	125un - 97 Graus
			//	_________
			// 	315 - 57 
			//	 X  - 1	
			// x=  5.52
			// 1024 / 5.52 = 185

			
			// Diferenca 65
			nivelAtual = map(nivelAtual, 1023,0 ,0, 185)-75;

			if (nivelMemoriaTemperatura==0) nivelMemoriaTemperatura = nivelAtual;

			if (nivelAtual < nivelMemoriaTemperatura){
				//Oscilando para baixo
				nivelMemoriaTemperatura--;
			}else{
				if (nivelAtual > nivelMemoriaTemperatura){
				//Oscilando para cima
					nivelMemoriaTemperatura++;
				}
			}

			
			//Serial.print("-");								
			//Serial.println(temperatura);				

			util.reIniciaTimer2();
		}

		return nivelMemoriaTemperatura;	
	}




	double obterVelocidade(){

		return KMPercorridaMeioSegundo*60*60*2;



	}

/*
	double obterVelocidade(){

		if (util.saidaTimer1()){
			util.reIniciaTimer1();	// << VERIFICAR 
			double diametroRoda = 60; // cm
			long countPulso = pulso->getPulsoVelocidade();
			pulso->reiniciarVelocidade();
			double perimetro = diametroRoda * PI;// cm	
			double distanciaPercorrida = perimetro * countPulso; // cm
			
				// cm ---> m ---> km
			velocidade = (distanciaPercorrida/100/1000)*60*60;
								 	// s ---> min ---> h

		}

		return velocidade;

	}

*/
};

 


