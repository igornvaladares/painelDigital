#include <Motor.h>
#include <Dualogic.h>
// Porta Analogica de Seleção e Engate
Dualogic dualogic(A0,A1);
// Porta de Interrupção(RPM), 
Motor motor(0,A4,A5,A6);

void setup(void)
{
Serial.begin(2000000);
}
void loop(void)
{
 
 int marchaEngatada = dualogic.obterMarchaEngatada();

 double rpm = motor.obterRpm();

 Serial.print("RPM:");
 Serial.println(rpm);

 
}
