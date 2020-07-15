#include <Motor.h>
#include <Cambio.h>
#include <Bordo.h>
#include <RealDash.h>


RealDash realDash(11,12); // RX, TX
// Porta Analogica de Seleção e Engate
Cambio cambio(A0,A1);
// Porta de Comunicação

/*

Portas:
uint8_t pinRotacao, (interrupção)
uint8_t pinVelocidade, (interrupção)
uint8_t pinTemperaturaAguaRadiador
*/
Motor motor(0,1,A2);
Bordo bordo(A3);
Util util;



void setup(void)
{
  Serial.begin(2000000);
  
}
void loop()
{
  
 int rpm; 
 int velocidade;
 int temperatura;
 int marcha;
 int nivelCombustivel;
 int tps;
 int sensores;
  
 rpm = motor.obterRpm();
 velocidade = motor.obterVelocidade();
 temperatura = motor.obterTemperaturaAguaRadiador();

 //Serial.println(rpm); 
 //Serial.println(velocidade); 
 
 marcha = cambio.obterMarchaEngatada();
 sensores = bordo.obterSensores();
 //velocidade=90;
 nivelCombustivel=40;// %
 temperatura = 90; // Graus
 tps = 25;// %
 
 enviarParaRealDash(rpm, velocidade, temperatura,nivelCombustivel, 
                    marcha,tps,
                    sensores);
 
 
}



void enviarParaRealDash(unsigned int rpm, unsigned int velocidade,
                        unsigned int tempMotor, unsigned int nivelCombustivel, 
                        signed char marchaEngatada,unsigned int tps,
                        unsigned int sensores){
  
  byte buf[8];

  
  memcpy(buf, &rpm, 2);
  memcpy(buf+ 2, &velocidade, 2);
  memcpy(buf+ 4, &tempMotor, 2);
  memcpy(buf+ 6, &nivelCombustivel, 2);
  
  realDash.SendCANFrameToSerial(3200, buf);
  
  
  memcpy(buf, &sensores, 2);
  realDash.SendCANFrameToSerial(3201, buf);
 
  memcpy(buf, &marchaEngatada, 2);
  memcpy(buf + 2, &tps, 2);
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

   realDash.SendTextExtensionFrameToSerial(3202, msg1);
   

   
} 
/*

 void SendCANFrameToSerial(unsigned long canFrameId, const byte* frameData)
{
  // the 4 byte identifier at the beginning of each CAN frame
  // this is required for RealDash to 'catch-up' on ongoing stream of CAN frames
  const byte serialBlockTag[4] = { 0x44, 0x33, 0x22, 0x11 };
  MinhaSerial.write(serialBlockTag, 4);

  // the CAN frame id number (as 32bit little endian value)
  MinhaSerial.write((const byte*)&canFrameId, 4);

  // CAN frame payload
  MinhaSerial.write(frameData, 8);
}


void SendTextExtensionFrameToSerial(unsigned long canFrameId, const char* text)
{
  if (text)
  {
    // the 4 byte identifier at the beginning of each CAN frame
    // this is required for RealDash to 'catch-up' on ongoing stream of CAN frames
    const byte textExtensionBlockTag[4] = { 0x55, 0x33, 0x22, 0x11 };
    MinhaSerial.write(textExtensionBlockTag, 4);

    // the CAN frame id number (as 32bit little endian value)
    MinhaSerial.write((const byte*)&canFrameId, 4);

    // text payload
    MinhaSerial.write(text, strlen(text) + 1);
  }
}
*/
