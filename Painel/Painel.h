#include "Arduino.h"
class Painel{ 

private:
        bool PinSetaEsquerda;
        bool PinSetaDireita;


   public:

        Painel(uint8_t pinSetaEsquerda,uint8_t pinSetaDireita){
		
		pinMode(pinSetaEsquerda, INPUT);
		pinMode(pinSetaDireita, INPUT);
		
		PinSetaDireita = pinSetaDireita;
		PinSetaEsquerda = pinSetaEsquerda;

	};

       bool setaEsqueda(){
	
	}
       bool setaDireita(){
	
	}
    

};

 


