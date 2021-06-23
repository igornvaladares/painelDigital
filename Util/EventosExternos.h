//joana.silva@jarezende.com.br
//5000-005-000003761-20122000905

class EventosExternos{

private:
   
	Util util;
	uint8_t PinSaidaDesligarAndroid;
	unsigned long int sensoresAnt;
	long tempoAnterior=0;
	int tempo_power = 300;
	void desligarPainel(){

		digitalWrite(PinSaidaDesligarAndroid, HIGH);
	        util.bloquear(tempo_power*2);
 		digitalWrite(PinSaidaDesligarAndroid, LOW);
		standByPainel();		
	}
	void standByPainel(){
		digitalWrite(PinSaidaDesligarAndroid, HIGH);
	        util.bloquear(tempo_power);
 		digitalWrite(PinSaidaDesligarAndroid, LOW);
	}
	

public:
	EventosExternos(uint8_t pinSaidaDesligarAndroid)
	{
	  pinMode(pinSaidaDesligarAndroid, OUTPUT);
	  PinSaidaDesligarAndroid = pinSaidaDesligarAndroid;
	  util.iniciaTimer5(TIMER_5);
	  //11 98870-4890
	}

	void gerenciarPower(unsigned long int sensores, int rpm){

		if (util.saidaTimer5()){ 
			util.reIniciaTimer5();
			if ((sensores ==0) && (sensoresAnt==0) && (rpm==0)) {
				standByPainel();
				
			}

			sensoresAnt = sensores;


		}	   
	}
	
 };