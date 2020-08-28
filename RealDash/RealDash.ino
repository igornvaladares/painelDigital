//#include <PriUint64.h>
#include <avr/sleep.h>
#define TIMER_1 986 // Um segundo ( RPM, Velocidade,Odometro
#define TIMER_2 0 // 2 segundos ( nivel temperatura)
#define TIMER_3 0 // 5 segundos ( temperatura)
#include <Motor.h>
#include <Cambio.h>
#include <Bordo.h>
#include <RealDash.h>
//SERIAL
//Mega
    //Serial 0 = TX0 (D1) e RX0 (D0)
    //Serial 1 = TX1 (D18) e RX1 (D19)
    //Serial 2 = TX2 (D16) e RX2 (D17)
    //Serial 3 = TX3 (D14) e RX3 (D15)
// INTERRUPCAO
//Mega 2, 3, 18, 19, 20, 21    
                             
RealDash realDash(&Serial,230400); 
Cambio cambio(A0,A1,A2); //Porta Analogica de Seleção , Engate e pressao dualogic
Motor motor(digitalPinToInterrupt(2),digitalPinToInterrupt(3),A3);//uint8_t pinRotacao, (interrupção) uint8_t pinVelocidade, (interrupção) uint8_t pinTemperaturaAguaRadiador
Bordo bordo(A4);//uint8_t pinNivelCombustível
int pinPowerOffArduino=50  ; //  desligar o arduino

void setup(void)
{
 pinMode(pinPowerOffArduino, INPUT);
 
 realDash.iniciar();
  
 //Serial.begin(9600);
}
void loop()
{
     
 
 int rpm; 
 
 double km;
 int velocidade;
 int temperatura;
 int marcha;
 int nivelCombustivel;
 int pressaoDualogic;
 unsigned long long int sensores;
 
 //----------------------------<Digitais>----------------------------
 rpm = motor.obterRpm(); 
 
 //km = motor.obterOdometro();
  
 velocidade = motor.obterVelocidade();
 
 sensores = bordo.obterSensoresDigitais();
 //----------------------------</Digitais>----------------------------
 
 //----------------------------<Analogicos>----------------------------
 temperatura = motor.obterTemperaturaAguaRadiador();
 
 marcha = cambio.obterMarchaEngatada();

 pressaoDualogic = cambio.obterPressaoDualogic();
 
 nivelCombustivel = bordo.obterNivelCombustivel(); // reinicia o (TIMER2)
 //----------------------------</Analogicos>----------------------------
 
 
//Serial.print("COMBUSTIVEL:");
//Serial.println(nivelCombustivel); 

 enviarParaRealDash(rpm, velocidade, temperatura,nivelCombustivel, 
                    marcha,pressaoDualogic,sensores);

//Serial.print("Texto:");

//Serial.println(temperatura);
//Serial.println(PriUint64<DEC>(a));

 
 //powerOffArduino();
}



void enviarParaRealDash(unsigned int rpm, unsigned int velocidade,
                        unsigned int temperatura, unsigned int nivelCombustivel, 
                        signed char marchaEngatada,unsigned int pressaoDualogic,
                        unsigned long long int sensores){
  
  byte buf[8];

  
  memcpy(buf, &rpm, 2);
  memcpy(buf+ 2, &velocidade, 2);
  memcpy(buf+ 4, &temperatura, 2);
  memcpy(buf+ 6, &nivelCombustivel, 2);
  
  realDash.SendCANFrameToSerial(3200, buf);
  
  
  memcpy(buf, &sensores, 4);
  realDash.SendCANFrameToSerial(3201, buf);
 
  memcpy(buf, &marchaEngatada, 2);
  memcpy(buf + 2, &pressaoDualogic, 2);
  realDash.SendCANFrameToSerial(3202, buf);

 
  //tratarMensagens();


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

void powerOffArduino(){
   
    if ((digitalRead(pinPowerOffArduino)) == HIGH){

        Serial.println("Dormiu");
        sleep_enable();
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_cpu();

    }
 }
 
