//joana.silva@jarezende.com.br
//5000-005-000003761-20122000905


class EventosExternos{

private:
   
	Util util;
	uint8_t PinSaidaDesligarAndroid;
	int rpmAnt;
	long tempoAnterior=0;
	int tempo_power = 300;
	

public:
	EventosExternos(uint8_t pinSaidaDesligarAndroid)
	{
	  pinMode(pinSaidaDesligarAndroid, OUTPUT);
	  PinSaidaDesligarAndroid = pinSaidaDesligarAndroid;
	  util.iniciaTimer5(TIMER_5);
	  //11 98870-4890
	}

	void gerenciarPower(int rpm){

		if (util.saidaTimer5()){ 
			util.reIniciaTimer5();


			if (rpm ==0) {
			          if (rpmAnt==0) {
					digitalWrite(PinSaidaDesligarAndroid, HIGH);
				        util.bloquear(tempo_power);
 					digitalWrite(PinSaidaDesligarAndroid, LOW);
											
				}
			}
			rpmAnt = rpm;


		}	   
	}
 };
 


