#include <Motor.h>
#include <Cambio.h>
#include <Bordo.h>
#include <RealDash.h>


// Porta Analogica de Seleção e Engate
Cambio cambio(A0,A1);
// Porta de Comunicação
RealDash realDash(11,12); // RX, TX
/*

Portas:
uint8_t pinRotacao, (interrupção)
uint8_t pinVelocidade, (interrupção)
uint8_t pinTemperaturaAguaRadiador
*/
Motor motor(2,3,A2);
Bordo bordo(A3);

unsigned int nivelCombustivel;
unsigned int pressaoDualogic=0; 
unsigned int pressaoMotor=0; 
unsigned int sensores = 0;
unsigned int rpm1 = 0;

String command = "";

int analogPins[7] = {0};

void setup(void)
{
  Serial.begin(9600);  
  //MinhaSerial.begin(115200);  
  
}
void loop()
{

  pressaoDualogic = 15;
  
  pressaoMotor = 4;

  
 enviarParaRealDash(motor.obterRpm(),
                    motor.obterVelocidade(), 
                    motor.obterTemperaturaAguaRadiador(), 
                    nivelCombustivel, 
                    cambio.obterMarchaEngatada(),
                    bordo.obterSensores());
 
 
}



void enviarParaRealDash(unsigned int rpm, unsigned int velocidade,
                        unsigned int tempMotor, unsigned int nivelCombustivel, 
                        char marchaEngatada,
                        unsigned int sensores){
  
  byte buf[8];


  if (rpm1++ > 8000)
  {
    rpm1 = 1000;
  }

  rpm = rpm1;
  
  velocidade=90;
  nivelCombustivel=20;
  tempMotor = 90;
  memcpy(buf, &rpm, 2);
  memcpy(buf+ 2, &velocidade, 2);
  memcpy(buf+ 4, &tempMotor, 2);
  memcpy(buf+ 6, &nivelCombustivel, 2);
  
  realDash.SendCANFrameToSerial(3200, buf);
  
  
  memcpy(buf, &sensores, 2);
  realDash.SendCANFrameToSerial(3201, buf);
  
  Serial.print("Marcha:");
  marchaEngatada = 'F';
  
  Serial.println(&marchaEngatada);

  


  //Marcha Engatada
  realDash.SendTextExtensionFrameToSerial(3202, &marchaEngatada);

  
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
