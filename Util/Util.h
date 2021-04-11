//joana.silva@jarezende.com.br
//5000-005-000003761-20122000905
class Util{

private:
   unsigned long Tempo1 = 0;
   unsigned long Delay1 = 0;

   unsigned long Tempo2 = 0;
   unsigned long Delay2 = 0;

   unsigned long Tempo3 = 0;
   unsigned long Delay3 = 0;

   unsigned long Tempo4 = 0;
   unsigned long Delay4 = 0;

   long Tempo5 = 0;
   unsigned long Delay5 = 0;
   unsigned long Tempo6 = 0;
   unsigned long Delay6 = 0;

public:
	Util()
	{
	  
	}

	void iniciaTimer1(unsigned long delay) {

		Delay1 = delay;
	
	}

	void reIniciaTimer1() {

	    Tempo1 = millis();   
		
	}

	bool saidaTimer1() {

	    if ((millis() - Delay1) > Tempo1) { 
		
			return true;

		}else return false;
	}      
   
   
	void iniciaTimer2(unsigned long delay) {

		Delay2 = delay;
	
	}

	void reIniciaTimer2() {

	    Tempo2 = millis();   
		
	}

	bool saidaTimer2() {

	    if ((millis() - Delay2) > Tempo2) { 
		
			return true;

		}else return false;
	}      
   
	void iniciaTimer3(unsigned long delay) {

		Delay3 = delay;
	
	}

	void reIniciaTimer3() {

	    Tempo3 = millis();   
		
	}

	bool saidaTimer3() {

	    if (((millis() - Delay3) > Tempo3)&&(millis()>Delay3)) { 

		return true;

		}else return false;
	}      
   

	void iniciaTimer4(unsigned long delay) {

		Delay4 = delay;
	    
	}

	void reIniciaTimer4() {

	    Tempo4 = millis();   
		
	}

	
	bool saidaTimer4() {

	    if ((millis() - Delay4) > Tempo4) { 
		
			return true;

		}else return false;
	}      
   
	void iniciaTimer5(unsigned long delay) {

		Delay5 = delay;

	}

	void reIniciaTimer5() {

	    Tempo5 = millis();   

	}

	bool saidaTimer5() {
	    if (((millis() - Delay5) > Tempo5)&&(millis()>Delay5)) { 
			return true;

		}else return false;
	}   
   
	double estabilizarEntrada(uint8_t pinAnalogico){
	
		double mediaAnaLogico=0;

		for (int i=1;i<=20;i++){

			mediaAnaLogico += analogRead(pinAnalogico);
		}

		mediaAnaLogico = mediaAnaLogico / 20;

		return mediaAnaLogico;

	
   	 }


	 void bloquear(unsigned long tempoEspera){
	 
	   long tempoAtual = millis();
	   long tempoAnterior = millis();
	   while((tempoAtual-tempoAnterior)<=tempoEspera){
	 
		tempoAtual = millis();
	  
	    }
	    
	   tempoAnterior = millis();

	 
 	}

	
	bool fequal(float a, float b)
	{
		float precision = 0.01;
 		return fabs(a-b) < precision;
	}

};
 
 


