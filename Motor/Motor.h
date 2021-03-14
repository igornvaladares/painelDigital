#include "Pulso.h"
#include "Util.h"
#define VALOR_MIN_TEMP_RADIADOR 20
#define VALOR_MAX_TEMP_RADIADOR 120

class Motor{ 

private:
	Pulso * pulso; 
	Util util;
	double wKMporH ;
	uint8_t PinTemperaturaAguaRadiador;
	int nivelMemoriaTemperatura=0;
	float contPulsoAnt;
	unsigned char contPulso;
	float contPulsoEstavel ;


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

       int obterRpm(){
		unsigned int rpm;		
		if (util.saidaTimer1()){
			contPulso = pulso->getPulsoRpm();
			if (abs(contPulsoAnt-contPulso)!=1)			
				contPulsoAnt = contPulso;

			pulso->reiniciarRpm();
		}
		if (!util.fequal(contPulsoAnt,contPulsoEstavel))
			if (contPulsoAnt > contPulsoEstavel)
				contPulsoEstavel+=0.10f;
			else 
				contPulsoEstavel-=0.10f;


		return (int)(contPulsoEstavel*120);

	}
 
	double obterVelocidade(){
		if (util.saidaTimer1()){
			util.reIniciaTimer1();	
			unsigned int countPulso = pulso->getPulsoVelocidade();
			unsigned char PULSO_POR_VOLTA=15; 
			float DIAMETRO_RODA= 0.6573; 
			float FATOR_MS_KMH = 3.6;

			pulso->reiniciarVelocidade();
			float rpmV;
			float hz; 
			rpmV = (float)countPulso*60/PULSO_POR_VOLTA;
			hz = rpmV /60;
			wKMporH = (float)PI*hz*DIAMETRO_RODA*FATOR_MS_KMH; 
			
		
		}
		
		//Serial.println(wKMporH);	
		return wKMporH;


	}


	int obterTemperaturaAguaRadiador(){

		if (util.saidaTimer2()){
			//OBS: Meio do Tanque a partir de 0,74 Volts e ventoinha liga a partir de 0.626v	(95 graus)

			//float voltPorUnidade = 0.004887586;
			int nivelAtual;


			nivelAtual = util.estabilizarEntrada(PinTemperaturaAguaRadiador);
			//Serial.println("pintemperatura");
			//Serial.print(temperatura);
			
			//	403un -> 47 Graus
			//	122un - 97 Graus
			//	_________
			// 	281 - 50 
			//	 X  - 1	
			// x=  5.62
			// 1024 / 5.62 = 182	
			//nivelAtual = 122;
			
			// Diferenca 65
			if (nivelAtual >0){
				nivelAtual = map(nivelAtual, 1023,0 ,0, 182)-70;
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

			
						
			}

			util.reIniciaTimer2();
		}
		//Serial.print("temperatura");								
		//Serial.println(nivelMemoriaTemperatura);		
		return nivelMemoriaTemperatura;	
	}


};

 


