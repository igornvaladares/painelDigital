//joana.silva@jarezende.com.br
//5000-005-000003761-20122000905
class Util{

private:
   unsigned long Tempo1 = 0;
   unsigned long Delay1 = 0;
public:
	Util()
	{
	  
	}

	unsigned long getTempoEsperaTimer1() {

	    return Delay1;
	
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
   
   
};
 
 


