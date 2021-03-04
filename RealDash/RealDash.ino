//#include <PriUint64.h>
#include "EEPROM.h"
#define ENDERECO_ODOMETRO 0
#define ENDERECO_MODOAUTOMATICO 2
#define ESTADO_PAINEL_LIGADO 1

#define TIMER_1 500 // Um segundo ( RPM, Velocidade,Odometro (1000 > 500 > 250 > 125)
#define TIMER_2 10000 // leitura de 5 em 5 segundos ( temperatura)
#define TIMER_3 10000 // leitura de 5 em 5 segundos ( nivel combustivel)
#define TIMER_4 500 // se demorar 1 segundo, alternar modo automativo < - >  Manual
#define TIMER_5 60000 // se demorar 1m com RPM = 0 , desligar android
#include <Motor.h>
#include <Cambio.h>
#include <Bordo.h>
#include <RealDash.h>
#include <EventosExternos.h>
//SERIAL
//Mega
    //Serial 0 = TX0 (D1) e RX0 (D0)
    //Serial 1 = TX1 (D18) e RX1 (D19)
    //Serial 2 = TX2 (D16) e RX2 (D17)
    //Serial 3 = TX3 (D14) e RX3 (D15)
// INTERRUPCAO
//Mega 2, 3, 18, 19, 20, 21    
                             
//sRealDash realDash(&Serial,230400); 
RealDash realDash(&Serial,115200); 
EventosExternos evento(52); // Porta de saida para desligar o Android

Cambio cambio(A0,A1,A2,A3,A4); //Porta Analogica de Seleção , Engate, pressao dualogic, pin1Joystick , pin2Joystick 
Motor motor(digitalPinToInterrupt(2),digitalPinToInterrupt(3),A5);//uint8_t pinRotacao, (interrupção) uint8_t pinVelocidade, (interrupção) uint8_t pinTemperaturaAguaRadiador
Bordo bordo(A14,A15);//uint8_t pinNivelCombustível1, uint8_t pinNivelCombustível21
//int cont=0;
void setup(void)
{

realDash.iniciar();
  
//Serial.begin(115200);
}
void loop()
{
     
 
 int rpm;
 double velocidade;
 int temperatura;
 int marcha;
 int nivelCombustivel;
 int pressaoDualogic;
 bool modoAutomatico;
 unsigned long int sensores;
 
 //----------------------------<Digitais>----------------------------
 rpm = motor.obterRpm(); 
 evento.gerenciarPower(rpm);
   
 velocidade = motor.obterVelocidade();
 
 sensores = bordo.obterSensoresDigitais();

 
 //----------------------------</Digitais>----------------------------
 
 //----------------------------<Analogicos>----------------------------
 
 
 temperatura = motor.obterTemperaturaAguaRadiador();
 
 marcha = cambio.obterMarchaEngatada();

 //pressaoDualogic = cambio.obterPressaoDualogic();

//Serial.print("marcha:");
//Serial.println(marcha);
 
 // usa o binario dos sensores para adicionar mais um bit caso estivre selecionado o modo automático
 //ensores = cambio.obterModoAutomatico(sensores);
 
 //Serial.print("Sensores:");
 //Serial.println(PriUint64<DEC>(sensores));
 
 ///Serial.println(sensores);
 nivelCombustivel = bordo.obterNivelCombustivel(); // reinicia o (TIMER2)
 //----------------------------</Analogicos>----------------------------
 //Serial.print("combustivel:");
 //Serial.println(nivelCombustivel);
 
 //Serial.print("temperatura :");
 //Serial.println(temperatura);

 //Serial.print("Marcha:");
 //Serial.println(marcha);

 //cont+=30;
//if (cont>8000) cont =0;
//Serial.print("cont :");
//Serial.println(cont);

 //Serial.print("rpm :");
 //Serial.println(rpm);
 //Serial.print("- Velocidade:");
//Serial.print(velocidade);

//pressaoDualogic=55;
//velocidade = 120;
//if (rpm >8000)
//rpm =0;
//rpm = rpm +10;
 enviarParaRealDash(rpm, velocidade, temperatura,nivelCombustivel, 
                   marcha,pressaoDualogic,sensores);

}



void enviarParaRealDash(unsigned int rpm, unsigned int velocidade,
                        unsigned int temperatura, unsigned int nivelCombustivel, 
                        signed char marchaEngatada,unsigned int pressaoDualogic,
                        unsigned long int sensores){
  
  byte buf[8];

  
  memcpy(buf, &rpm, 2);
  memcpy(buf+ 2, &velocidade, 2);
  memcpy(buf+ 4, &temperatura, 2);
  memcpy(buf+ 6, &nivelCombustivel, 2);
  realDash.SendCANFrameToSerial(3200, buf);
    
  memcpy(buf, &sensores, 4);
  memcpy(buf+ 4, &marchaEngatada, 2);
  memcpy(buf + 6, &pressaoDualogic, 2);

  realDash.SendCANFrameToSerial(3201, buf);
  realDash.SendCANFrameToSerial(3202, buf);


   
} 
 