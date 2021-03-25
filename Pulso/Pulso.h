#include "MyInterrupts.h"
class Pulso : public Interrupt{
  private:

    uint8_t PinRotacao;
    uint8_t PinVelocidade;
    volatile unsigned long arrayPulso[3]={0};
    long countPulsoVelocidade;
	const unsigned long ZeroTimeout = 100000;  // For high response time, a good value would be 100000.
	volatile unsigned long LastTimeWeMeasured;  // Stores the last time we measured a pulse so we can calculate the period.
	volatile unsigned long PeriodBetweenPulses = ZeroTimeout+1000;  // Stores the period between pulses in microseconds.
	volatile unsigned long PeriodAverage = ZeroTimeout+1000;  // Stores the period between pulses in microseconds in total, if we are taking multiple pulses.
	unsigned int PulseCounter = 1;  // Counts the amount of pulse readings we took so we can average multiple pulses before calculating the period.
	unsigned long PeriodSum; // Stores the summation of all the periods to do the average.
	unsigned long LastTimeCycleMeasure = LastTimeWeMeasured;  // Stores the last time we measure a pulse in that cycle.
	unsigned int AmountOfReadings = 1;

  public:
      Pulso(uint8_t pinRotacao, uint8_t pinVelocidade){

      PinRotacao = pinRotacao;
      PinVelocidade = pinVelocidade;

      pinMode(pinRotacao, INPUT_PULLUP);
      pinMode(pinVelocidade, INPUT);
     
      attach(pinRotacao, FALLING);

      attach(pinVelocidade, RISING);

  
    }

     volatile unsigned long * getPulsoRpm(){
	 return arrayPulso;
     }    
    long getPulsoVelocidade(){
	 return countPulsoVelocidade;
     }    

    long reiniciarVelocidade(){
	 countPulsoVelocidade=0;
     }    

    // Função Executada na interrupcao
    void volatile execInterrupt(uint8_t interruptNum){
	
	if (interruptNum==PinRotacao){
		PeriodBetweenPulses = micros() - LastTimeWeMeasured;  // Current "micros" minus the old "micros" when the last pulse happens.
                                                        // This will result with the period (microseconds) between both pulses.
                                                        // The way is made, the overflow of the "micros" is not going to cause any issue.

		LastTimeWeMeasured = micros();  // Stores the current micros so the next time we have a pulse we would have something to compare with.
		if(PulseCounter >= AmountOfReadings)  // If counter for amount of readings reach the set limit:
  		{
    			PeriodAverage = PeriodSum / AmountOfReadings;  // Calculate the final period dividing the sum of all readings by the
                                                   // amount of readings to get the average.
    			PulseCounter = 1;  // Reset the counter to start over. The reset value is 1 because its the minimum setting allowed (1 reading).
    			PeriodSum = PeriodBetweenPulses;  // Reset PeriodSum to start a new averaging operation.
			int RemapedAmountOfReadings = map(PeriodBetweenPulses, 40000, 5000, 1, 10);  // Remap the period range to the reading range.
    			RemapedAmountOfReadings = constrain(RemapedAmountOfReadings, 1, 10);  // Constrain the value so it doesn't go below or above the limits.
    			AmountOfReadings = RemapedAmountOfReadings;  // Set amount of readings as the remaped value.
  		}
  		else
  		{
    			PulseCounter++;  // Increase the counter for amount of readings by 1.
    			PeriodSum = PeriodSum + PeriodBetweenPulses;  // Add the periods so later we can average.
  		}

		arrayPulso[0]=PeriodAverage;
		arrayPulso[1]=PeriodBetweenPulses;
		arrayPulso[2]=LastTimeWeMeasured;


	}else if (interruptNum==PinVelocidade){
		countPulsoVelocidade++;
		//Serial.print("VELOCIDADE");
	}

    }

    	
};

