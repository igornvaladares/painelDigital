#include <Motor.h>
#include <Dualogic.h>
#include <Painel.h>
// Porta Analogica de Seleção e Engate
Dualogic dualogic(A0,A1);
// Porta de Interrupção(RPM), 
Motor motor(0,4,5,7);

Painel painel(8,9);

void setup(void)
{
Serial.begin(2000000);
}
void loop(void)
{
 
 int marchaEngatada = dualogic.obterMarchaEngatada();

 double rpm = motor.obterRpm();
  
  if (painel.setaEsqueda()){
      Serial.print("Seta Esquerda");
  }
 Serial.print("RPM:");
 Serial.println(rpm);

 
}
