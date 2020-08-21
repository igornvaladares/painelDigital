#define TIMER_1 986 // Um segundo ( RPM, Velocidade,Odometro
#define TIMER_2 0 // 2 segundos ( nivel temperatura)
#define TIMER_3 0 // 5 segundos ( temperatura)
#include <Motor.h>
#include <Cambio.h>
#include <Bordo.h>
#include <RealDash.h>

RealDash realDash(11,12); // RX, TX
Cambio cambio(A0,A1, A3); //Porta Analogica de Seleção , Engate e pressao dualogic
Motor motor(digitalPinToInterrupt(2),digitalPinToInterrupt(3),A4);//uint8_t pinRotacao, (interrupção) uint8_t pinVelocidade, (interrupção) uint8_t pinTemperaturaAguaRadiador
Bordo bordo(A5);//uint8_t pinNivelCombustível


void setup(void)
{
     
  Serial.begin(9600);
  
}
void loop()
{
     
  
 int rpm; 
 int km;
 int velocidade;
 int temperatura;
 int marcha;
 int nivelCombustivel;
 int pressaoDualogic;
 unsigned long sensores;
 
 //----------------------------<Digitais>----------------------------
 rpm = motor.obterRpm(); 
 
 km = motor.obterOdometro();
  
 velocidade = motor.obterVelocidade();
 
 sensores = bordo.obterSensoresDigitais();
 //----------------------------</Digitais>----------------------------
 
 //----------------------------<Analogicos>----------------------------
 temperatura = motor.obterTemperaturaAguaRadiador();
 
 marcha = cambio.obterMarchaEngatada();

 pressaoDualogic = cambio.obterPressaoDualogic();
 
 nivelCombustivel = bordo.obterNivelCombustivel(); // reinicia o (TIMER2)
 //----------------------------</Analogicos>----------------------------
 
  //rpm = 3000;
//Serial.print("COMBUSTIVEL:");
//Serial.println(nivelCombustivel); 

 enviarParaRealDash(rpm, velocidade, temperatura,nivelCombustivel, 
                    marcha,pressaoDualogic,sensores);
 
 
}



void enviarParaRealDash(unsigned int rpm, unsigned int velocidade,
                        unsigned int tempMotor, unsigned int nivelCombustivel, 
                        signed char marchaEngatada,unsigned int pressaoDualogic,
                        unsigned long sensores){
  
  byte buf[8];

  
  memcpy(buf, &rpm, 2);
  memcpy(buf+ 2, &velocidade, 2);
  memcpy(buf+ 4, &tempMotor, 2);
  memcpy(buf+ 6, &nivelCombustivel, 2);
  
  realDash.SendCANFrameToSerial(3200, buf);
  
  
  memcpy(buf, &sensores, 2);
  realDash.SendCANFrameToSerial(3201, buf);
 
  memcpy(buf, &marchaEngatada, 2);
  memcpy(buf + 2, &pressaoDualogic, 2);
  realDash.SendCANFrameToSerial(3202, buf);

  
  //Serial.print("sensores:");
  //Serial.println(sensores);
  
  tratarMensagens();
  
}


void tratarMensagens(){
  
   const char* msgComplementar1 = "ATIVADO";
   const char* msgComplementar2 = "DESATIVADO";
   const char* msg1 =  "Atenção: Pouco Combustível";
   const char* msg2 =  "Atenção: Falha na Injeção Eletrónica";
   const char* msg3 =  "Atenção: Pressão Dualogic Baixa";
   const char* msg4 =  "Atenção: Verificar Bateria";
   const char* msg5 =  "Atenção: Porta Aberta";
   
   const char* msg10 =  "Perigo: Temperatura Muito Alta";
   const char* msg11 =  "Perigo: Anomalia";
   
   const char* msg20 =  "Cruise Control";

   //realDash.SendTextExtensionFrameToSerial(3202, msg1);

   
   

   
} 
