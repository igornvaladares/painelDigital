#include "Pulso.h"
#include "Util.h"
#define VALOR_MIN_TEMP_RADIADOR 20
#define VALOR_MAX_TEMP_RADIADOR 120
#define  VALOR_ACELERADOR_MIN_REF 70 //0.77V
#define  VALOR_ACELERADOR_MAX_REF 120 //1.2v
#define  VALOR_ACELERADOR_MAX 400 //4.0v

class Motor
{

private:
	Pulso *pulso;
	Util util;
	float wKMporH;
	float distanciaPercorrida;
	uint8_t PinTemperaturaAguaRadiador;
	uint8_t PinAcelerador;
	uint8_t QUINTA =6;

	int nivelMemoriaTemperatura = 0;
	const byte PulsesPerRevolution = 1; // Set how many pulses there are on each revolution. Default: 2.
	const unsigned long ZeroTimeout = 100000;
	byte numReadings = 2;
	volatile unsigned long LastTimeWeMeasured;				 // Stores the last time we measured a pulse so we can calculate the period.
	volatile unsigned long PeriodBetweenPulses;				 // Stores the period between pulses in microseconds.
	volatile unsigned long PeriodAverage;					 // Stores the period between pulses in microseconds in total, if we are taking multiple pulses.
	unsigned long FrequencyRaw;								 // Calculated frequency, based on the period. This has a lot of extra decimals without the decimal point.
	unsigned long RPM;										 // Raw RPM without any processing.
	unsigned long LastTimeCycleMeasure = LastTimeWeMeasured; // Stores the last time we measure a pulse in that cycle.
	unsigned long CurrentMicros = micros();					 // Stores the micros in that cycle.
	unsigned int ZeroDebouncingExtra;
	// Variables for smoothing tachometer:
	unsigned long readings[2]; // The input.
	unsigned long readIndex;   // The index of the current reading.
	unsigned long total;	   // The running total.
	unsigned long average;	   // The RPM value after applying the smoothing.

	int velocidadeAnt = 0;
	double velocidade100Ant = 0;
	bool soltouAcelerador=false;

	bool isDesacelerandoPorDesvio(int velocidade, int desvio)
	{

		bool retorno = false;
		if (velocidade < velocidadeAnt - desvio)
		{
			retorno = !retorno;
		}
		velocidadeAnt = velocidade;
		return retorno;
	}
	bool isAcelerandoPorDesvio(int velocidade, int desvio)
	{

		bool retorno = false;
		if (velocidade > velocidadeAnt + desvio)
		{
			retorno = !retorno;
		}
		velocidadeAnt = velocidade;
		return retorno;
	}

public:
	Motor(uint8_t pinRotacao, uint8_t pinVelocidade, uint8_t pinTemperaturaAguaRadiador,uint8_t pinAcelerador)
	{

		pinMode(pinRotacao, INPUT);
		pinMode(pinTemperaturaAguaRadiador, INPUT);
		//pinMode(pinVelocidade, INPUT_PULLUP);

		PinTemperaturaAguaRadiador = pinTemperaturaAguaRadiador;
		PinAcelerador = pinAcelerador;

		pulso = new Pulso(pinRotacao, pinVelocidade);

		util.iniciaTimer1(TIMER_1);
		util.iniciaTimer2(TIMER_2);
		util.iniciaTimer3(TIMER_3);
		util.iniciaTimer5(TIMER_2);
		util.iniciaTimer6(TIMER_6);

	};

	bool isAlvoAceleradorParaTrocaDeMarcha(){

		bool retorno = false;
		//Serial.print(" S-");

		if (util.saidaTimer6())	{
                   soltouAcelerador = false;
		}
 
		if (util.saidaTimer2() && !soltouAcelerador)	{
                   // Serial.println("Le acelerador :");

		   util.reIniciaTimer2(); 
	       int nivelAcelerador = util.estabilizarEntrada(PinAcelerador);
	  	
		   int voltAc = map(nivelAcelerador, 0, 1023, 0, 500);			

		   //Serial.print("Acele:");
  		   //Serial.print(voltAc);	
		  if (voltAc< VALOR_ACELERADOR_MIN_REF ){
			soltouAcelerador =true;
			util.reIniciaTimer6(); 

		  }
		
	  	  return (voltAc>= VALOR_ACELERADOR_MIN_REF && voltAc<= VALOR_ACELERADOR_MAX_REF);

		}
		return retorno;
	}
	bool isPisandoAcelerador(){

	    int nivelAcelerador = util.estabilizarEntrada(PinAcelerador);
		  int voltAc = map(nivelAcelerador, 0, 1023, 0, 500);			
	  	return (voltAc>=VALOR_ACELERADOR_MIN_REF);

	}
	bool isPisouFundo(){

		bool retorno = false;
		//Serial.print(" S-");


		if (util.saidaTimer5())	{
        //  Serial.println("Le acelerador :");

		   util.reIniciaTimer5(); 
	     int nivelAcelerador = util.estabilizarEntrada(PinAcelerador);
	  	 int voltAc = map(nivelAcelerador, 0, 1023, 0, 500);			
       return (voltAc>=VALOR_ACELERADOR_MAX);

		}
		return retorno;
	}
	bool isEstabilizouVelocidade(double velocidade)
	{

		double velocidade100 = velocidade;
		bool retorno = false;
		if (util.saidaTimer3()){

			util.reIniciaTimer3();
			double aceleracao =  ((velocidade100Ant - velocidade100) * 2); // 0.5 segundos = Timer3
			velocidade100Ant = velocidade100;
			return fabs(aceleracao) < 1.2; // 1.2km/h
		
		}
		return retorno;
	}


	void interropePulso()
	{
		pulso->interrompePulso();
	}
	void iniciaPulso()
	{
		pulso->iniciaPulso();
	}
	unsigned long obterRpm()
	{
		volatile unsigned long *arrayPulso = pulso->getPulsoRpm();

		PeriodAverage = arrayPulso[0];
		PeriodBetweenPulses = arrayPulso[1];
		LastTimeWeMeasured = arrayPulso[2];

		LastTimeCycleMeasure = LastTimeWeMeasured; // Store the LastTimeWeMeasured in a variable.
		CurrentMicros = micros();				   // Store the micros() in a variable.

		if (CurrentMicros < LastTimeCycleMeasure)
		{
			LastTimeCycleMeasure = CurrentMicros;
		}

		// Calculate the frequency:
		FrequencyRaw = 10000000000 / PeriodAverage;
		// If the pulses are too far apart that we reached the timeout for zero:
		if (PeriodBetweenPulses > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure > ZeroTimeout - ZeroDebouncingExtra)
		{
			FrequencyRaw = 0;			// Set frequency as 0.
			ZeroDebouncingExtra = 2000; // Change the threshold a little so it doesn't bounce.
		}
		else
		{
			ZeroDebouncingExtra = 0; // Reset the threshold to the normal value so it doesn't bounce.
		}

		// Calculate the RPM:
		RPM = FrequencyRaw / PulsesPerRevolution * 60; // Frequency divided by amount of pulses per revolution multiply by
		RPM = RPM / 10000;							   // Remove the decimals.
													   // Smoothing RPM:
		total = total - readings[readIndex];		   // Advance to the next position in the array.
		readings[readIndex] = RPM;					   // Takes the value that we are going to smooth.
		total = total + readings[readIndex];		   // Add the reading to the total.
		readIndex = readIndex + 1;					   // Advance to the next position in the array.
		if (readIndex >= numReadings)				   // If we're at the end of the array:
		{
			readIndex = 0; // Reset array index.
		}
		// Calculate the average:
		average = total / numReadings; // The average value it's the smoothed result.

		return average;
	}

	float obterVelocidade(uint8_t marcha)
	{
		if (util.saidaTimer1())
		{
			float FATOR_CORRECAO_MARCHA = (marcha == QUINTA) ? 1.276 : 1;
			util.reIniciaTimer1();
			unsigned int countPulso = pulso->getPulsoVelocidade();
			int PULSO_POR_VOLTA = 29; //16
			float DIAMETRO_RODA = 0.6573;
			float FATOR_MS_KMH = 3.6;
			// distanciaPercorrida +=(countPulso/PULSO_POR_VOLTA)*PI;
			pulso->reiniciarVelocidade();
			// * 2 TRANFOMAA 500ms em hz 
			wKMporH = (countPulso * 2) * (PI * DIAMETRO_RODA * FATOR_MS_KMH)/ PULSO_POR_VOLTA;
			
			wKMporH = wKMporH * FATOR_CORRECAO_MARCHA;
			if (wKMporH < 3.0){
			   wKMporH=0;
			}
			
		}
		//wKMporH++;

		return wKMporH;
	}

};
