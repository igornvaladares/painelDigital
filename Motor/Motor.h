#include "Pulso.h"
#include "Util.h"
#define VALOR_MIN_TEMP_RADIADOR 20
#define VALOR_MAX_TEMP_RADIADOR 120


class Motor{ 

private:
	Pulso * pulso; 
	Util util;
	double wKMporH;
	float distanciaPercorrida;
	uint8_t PinTemperaturaAguaRadiador;
	int nivelMemoriaTemperatura=0;
	const byte PulsesPerRevolution = 1;  // Set how many pulses there are on each revolution. Default: 2.
	const unsigned long ZeroTimeout = 100000;  
	byte numReadings = 2; 
	volatile unsigned long LastTimeWeMeasured;  // Stores the last time we measured a pulse so we can calculate the period.
	volatile unsigned long PeriodBetweenPulses; // Stores the period between pulses in microseconds.
	volatile unsigned long PeriodAverage; // Stores the period between pulses in microseconds in total, if we are taking multiple pulses.
	unsigned long FrequencyRaw;  // Calculated frequency, based on the period. This has a lot of extra decimals without the decimal point.
	unsigned long RPM;  // Raw RPM without any processing.
	unsigned long LastTimeCycleMeasure = LastTimeWeMeasured;  // Stores the last time we measure a pulse in that cycle.
	unsigned long CurrentMicros = micros();  // Stores the micros in that cycle.
	unsigned int ZeroDebouncingExtra;  
	// Variables for smoothing tachometer:
	unsigned long readings[2];  // The input.
	unsigned long readIndex;  // The index of the current reading.
	unsigned long total;  // The running total.
	unsigned long average;  // The RPM value after applying the smoothing.

	int velocidadeAnt = 0;
	int velocidadeIntAnt = 0;

	bool isDesacelerandoPorDesvio(int velocidade, int desvio){

		if (velocidade >= velocidadeAnt-desvio){
			velocidadeAnt = velocidade;
			return false;
		}else{
			velocidadeAnt = 0;
			return true;
		}
  	}

	
	int doubleToInt(float velocidade){
		 return int ((velocidade) * 100);
	}


	public:
        Motor(uint8_t pinRotacao,uint8_t pinVelocidade,uint8_t pinTemperaturaAguaRadiador){
	
		pinMode(pinRotacao, INPUT);
		pinMode(pinTemperaturaAguaRadiador, INPUT);
		
		PinTemperaturaAguaRadiador = pinTemperaturaAguaRadiador;
		pulso = new Pulso(pinRotacao,pinVelocidade);

		util.iniciaTimer1(TIMER_1); 
		util.iniciaTimer2(TIMER_2); 
  		util.iniciaTimer3(TIMER_3); 
		
	};

	bool isEstabilizouVelocidade(double velocidade){
  	 
	   int velocidadeInt = doubleToInt(velocidade);
	
	   bool retorno =false;

		if (util.saidaTimer3()){
		  
		    util.reIniciaTimer3();
	        int deltaVelocidade = velocidadeInt - velocidadeIntAnt;
		    retorno =  ((deltaVelocidade) >= -33 && (deltaVelocidade) <= 0);
		    velocidadeIntAnt = velocidadeInt;


		}
		return retorno;
	}
	

	bool isDesacelerando(double velocidade){


	  if (util.saidaTimer2()){

	      util.reIniciaTimer2();

	      int velocidadeInt = doubleToInt(velocidade);
	      switch (velocidadeInt){
       	       case 6000 ... 6249:
	          return isDesacelerandoPorDesvio(velocidadeInt, 100);
	       case 6250 ... 6499:
        	  return isDesacelerandoPorDesvio(velocidadeInt, 50);
	       case 6500 ... 6999:
        	  return isDesacelerandoPorDesvio(velocidadeInt, 25);
	       case 7000 ... 7500:
        	  return isDesacelerandoPorDesvio(velocidadeInt, 10);
	       case 7501 ... 8000:
        	  return isDesacelerandoPorDesvio(velocidadeInt, 5);
	       case 8001 ... 10000:
        	  return isDesacelerandoPorDesvio(velocidadeInt, 1);
		   case 10001 ... 16000:
				return isDesacelerandoPorDesvio(velocidadeInt, 0);

	        defaut:
        	  return false;
	      }
        
 	 }

	  return false;

	}

       void interropePulso(){
            pulso->interrompePulso();
       }
       void iniciaPulso(){
		    pulso->iniciaPulso();
       }

       float obterDistanciaPercorrida(){
            return distanciaPercorrida;
       }      
       unsigned long obterRpm(){
	   volatile unsigned long * arrayPulso = pulso->getPulsoRpm();


		PeriodAverage = arrayPulso[0];
		PeriodBetweenPulses = arrayPulso[1];
		LastTimeWeMeasured = arrayPulso[2];

	  	LastTimeCycleMeasure = LastTimeWeMeasured;  // Store the LastTimeWeMeasured in a variable.
		CurrentMicros = micros();  // Store the micros() in a variable.

		if(CurrentMicros < LastTimeCycleMeasure){
    			LastTimeCycleMeasure = CurrentMicros;
		}

	 // Calculate the frequency:
	  	FrequencyRaw = 10000000000 / PeriodAverage;  
		 // If the pulses are too far apart that we reached the timeout for zero:
		if(PeriodBetweenPulses > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure > ZeroTimeout - ZeroDebouncingExtra){ 
    			FrequencyRaw = 0;  // Set frequency as 0.
	    		ZeroDebouncingExtra = 2000;  // Change the threshold a little so it doesn't bounce.
  		}
  		else{
    			ZeroDebouncingExtra = 0;  // Reset the threshold to the normal value so it doesn't bounce.
	  	}

		// Calculate the RPM:
  		RPM = FrequencyRaw / PulsesPerRevolution * 60;  // Frequency divided by amount of pulses per revolution multiply by
	  	RPM = RPM / 10000;  // Remove the decimals.
  		 // Smoothing RPM:
		total = total - readings[readIndex];  // Advance to the next position in the array.
  		readings[readIndex] = RPM;  // Takes the value that we are going to smooth.
  		total = total + readings[readIndex];  // Add the reading to the total.
  		readIndex = readIndex + 1;  // Advance to the next position in the array.
		if (readIndex >= numReadings)  // If we're at the end of the array:
  		{
    		readIndex = 0;  // Reset array index.
  		}
  		  // Calculate the average:
  		average = total / numReadings;  // The average value it's the smoothed result.


		return average;
	}
 
	double obterVelocidade(){
		if (util.saidaTimer1()){
			util.reIniciaTimer1();	
			unsigned int countPulso = pulso->getPulsoVelocidade();
			unsigned char PULSO_POR_VOLTA=15; 
			float DIAMETRO_RODA= 0.6573; 
			float FATOR_MS_KMH = 3.6;
			//distanciaPercorrida +=(countPulso/PULSO_POR_VOLTA)*PI;
			pulso->reiniciarVelocidade();
			float rpmV;
			float hz; 
			rpmV = (float)countPulso*60/PULSO_POR_VOLTA;
			hz = rpmV /60;
			wKMporH = (float)PI*hz*DIAMETRO_RODA*FATOR_MS_KMH; 
		
		}
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
			//	127un - 97 Graus
			//	_________
			// 	276 - 50 
			//	 X  - 1	
			// x=  5.5
			// 1024 / 5.62 = 186	
			//nivelAtual = 403;
			
			// Diferenca 65
			if (nivelAtual >0){
				nivelAtual = map(nivelAtual, 1023,0 ,0, 186)-65; 
				//nivelAtual = map(nivelAtual, 1023,0 ,0, 182)-75; 
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
		return nivelMemoriaTemperatura;	
	}


};

 


