#include "Arduino.h"
class Dualogic
{
    public:
	Dualogic(uint8_t pinSelecao,uint8_t pinEngate)
		{
		    pinMode(pinSelecao, INPUT);
		    pinMode(pinEngate, INPUT);

		    pinHSelecaoA0 = pinSelecao;
		    pinHEngateA1 = pinEngate;

		}
	 uint8_t obterMarchaEngatada(){

		  int sensorSelecao =  analogRead(pinHSelecaoA0);
		  int sensorEngate =  analogRead(pinHEngateA1);

	   return 1;	
	}
    private:
        uint8_t pinHSelecaoA0;
        uint8_t pinHEngateA1;
};


