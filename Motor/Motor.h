#include "Pulso.h"
#include "Util.h"
#define VALOR_MIN_TEMP_RADIADOR 20
#define VALOR_MAX_TEMP_RADIADOR 120

class Motor{ 

private:
	Pulso * pulso; 
	Util util;
	double rpmEstavel;
	double wKMporH ;
	uint8_t PinTemperaturaAguaRadiador;
	int nivelMemoriaTemperatura=0;
	unsigned int countPulsoAnt;

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
		unsigned int rpm;		
		if (util.saidaTimer1()){
			unsigned int countPulso = pulso->getPulsoRpm();
			if (abs(countPulsoAnt-countPulso)!=1)			
				countPulsoAnt = countPulso;

			pulso->reiniciarRpm();
			rpm = countPulsoAnt*120; // Segudo
		
			//Serial.println("Pulso RPM:");							
			//Serial.println(countPulso);			
		}

		if (rpm > rpmEstavel+15)
			rpmEstavel=rpmEstavel+30;
		else if (rpm < rpmEstavel-15)
			rpmEstavel=rpmEstavel-30;
	
		return rpmEstavel;

	}
 
	double obterVelocidade(){

		if (util.saidaTimer1()){
			util.reIniciaTimer1();	
			long countPulso = pulso->getPulsoVelocidade();
			pulso->reiniciarVelocidade();

			double diametroRoda = 65.73/100; 
			double fatorMsParaKmh = 3.6;
			int pulsoPorVolta=30; 
			int rpmV;
			double hz; 
			rpmV = countPulso*120/pulsoPorVolta;
			hz = rpmV /60;
			wKMporH = PI*hz*diametroRoda*fatorMsParaKmh; 
			
		
		}
		
		return wKMporH;


	}


	int obterTemperaturaAguaRadiador(){

		if (util.saidaTimer2()){
			//OBS: Meio do Tanque a partir de 0,74 Volts e ventoinha liga a partir de 0.60v	(95 graus)

			//float voltPorUnidade = 0.004887586;
			int nivelAtual;


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
			if (nivelAtual >0){
				nivelAtual = map(nivelAtual, 1023,0 ,0, 185)-74;
				if (nivelMemoriaTemperatura ==0) nivelMemoriaTemperatura = nivelAtual;
				if (nivelAtual < nivelMemoriaTemperatura){
					//Oscilando para baixo
					nivelMemoriaTemperatura --;
				}else{
					if (nivelAtual > nivelMemoriaTemperatura){
					//Oscilando para cima
						nivelMemoriaTemperatura++;
					}
				}

			
				//Serial.print("-");								
				//Serial.println(temperatura);				
			}
			util.reIniciaTimer2();
		}

		return nivelMemoriaTemperatura;	
	}


};

 


