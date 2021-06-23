#include <avr/sleep.h>
#include <EEPROM.h>
const byte entradaTrava= 2;
const byte entradaDestrava= 3;
const byte entradaPosChave = 9;
const byte entradaSensorPorta= 8;


const byte saidaPainelDigital = 11;
const byte saidaBeep= 10;
const byte saidaTrava= 7;
const byte saidaBuzina= 6;
const byte saidaDestravaMotorista= 5;
const byte saidaDestravaTudo= 4;


bool modoDestrava=false; // TRUE - ABRE TUDO; FALSE- ABRE SÒ MOTORISTA
bool podeBuzinar=true; 
bool buzinarProximoClick = true;
signed char clicks=0;
signed char clicksAnt=0;
long tempoAnterior=0;


signed char estadoAtual=0;


#define MODO_DEBUG 1
#define ESTADO_PAINEL_LIGADO 1
#define ESTADO_MOTORISTA_ABERTA -1
#define ESTADO_TUDO_ABERTA -2
#define ESTADO_TUDO_FECHADA 1
#define ESTADO_SUBINDO_VIDRO 3
#define ESTADO_DESCENDO_VIDRO -3


#define QTD_BUZINA_PORTA_ABERTA 3 
#define QTD_BUZINA_TRAVA 1 
#define QTD_BEEP_TRAVA 1 
#define QTD_BEEP_DESTRAVA 2
#define QTD_BEEP_MODODESTRAVA 5
#define QTD_BEEP_NAO_BUZINAR 1

#define TEMPO_BUZINA_PORTA_ABERTA 50
#define TEMPO_BUZINA_TRAVA 10

#define TEMPO_BEEP_DESTRAVA_TUDO 100
#define TEMPO_BEEP_DESTRAVA_MOTORISTA 50
#define TEMPO_BEEP_TRAVA 50
#define TEMPO_BEEP_MODODESTRAVA 50
#define TEMPO_BEEP_NAO_BUZINAR 500
#define TEMPO_POWER_PAINEL 300

#define TRAVAR 1
#define DESTRAVAR 2

#define TEMPO_DESTRAVA 150
#define TEMPO_TRAVA 100
#define TEMPO_SUBIR_VIDROS 15000
#define TEMPO_DESCER_VIDROS 15000

#define ENDERECO_DESTRAVA 0
#define DESTRAVA_TUDO 1
#define DESTRAVA_MOTORISTA 0
volatile byte pulsoGND = LOW;
volatile byte pulsoPositivo = HIGH;

signed char estadoPainelDigital= ESTADO_PAINEL_LIGADO;


void habilitaInterrupcao(){
 
  attachInterrupt(digitalPinToInterrupt(entradaTrava), clickTrava, LOW);
 
  attachInterrupt(digitalPinToInterrupt(entradaDestrava), clickDestrava, LOW);
 
}
  
void desabilitaInterrupcao(){
   
   detachInterrupt(digitalPinToInterrupt(entradaTrava));    
    
   detachInterrupt(digitalPinToInterrupt(entradaDestrava));    
 
 }
  
void setup() {


  Serial.begin(2000000);

  modoDestrava = EEPROM.read(ENDERECO_DESTRAVA);

  ativarPortas();

  habilitaInterrupcao();
 
}

void clickTrava() {
  
    if (!isPosChave()){ // Carro desligado
    
        desabilitaInterrupcao();
        
        if (lerEntrada(entradaTrava)){
     
        
          if (clicks<0){
    
             reiniciaTravas();
    
          }
         clicks++;
    
         }
          
       habilitaInterrupcao();
    
    }
}

void clickDestrava() {

    desabilitaInterrupcao();
    
    if (lerEntrada(entradaDestrava)){
 
        if (clicks>0){
          reiniciaTravas();
        }
       
        clicks--;
      
    }
      
   habilitaInterrupcao();
}

void loop(){
    
  gerenciarTravas();

  if (MODO_DEBUG){
    Serial.print("ESTADO ATUAL:");
    Serial.println(estadoAtual);
    Serial.print("clicks:");
    Serial.println(clicks);

  }
  
  dormir();

}
 
void gerenciarTravas(){

     if (isTravou()){
        gerenciarTrava();
     }
     else{ 
          if (isDestravou()){
              gerenciarDestrava();
           }
     }

 }
     
void gerenciarTrava(){

    if (!isPosChave()){ // Carro desligado
      
       switch(clicks)
        {
          case 1:
              gerenciarTrava1Click();
              break;
          case 2:
              gerenciarTrava2Click();
              break;
          case 3:
              gerenciarTrava3Click();
              break;
    
          }
       // COMENTADO PARA TEST   
       //desligarPainelDigital(); 

    }
}
  
void gerenciarDestrava(){

   switch(clicks)
   
    {
      case -1:
          gerenciarDestrava1Click();
      break;
      case -2:
          gerenciarDestrava2Click();
       break;
      case -3:
          gerenciarDestrava3Click();
       break;
      case -4:
          gerenciarDestrava4Click();
       break;

      }
    if (!isPosChave())
        ligarPainelDigital();

}

void gerenciarTrava1Click(){
 
    if (estadoAtual != ESTADO_TUDO_FECHADA){
         travar();
         beep(QTD_BEEP_TRAVA, TEMPO_BEEP_TRAVA);
         if (isBuzinarProximoClick()){
               verificarTravaParaBuzinar();
          } else buzinarProximoClick = true;
     }

}

void gerenciarTrava2Click(){
       
      subirVidros();
      
}

void gerenciarTrava3Click(){
  
   if (isPortaAberta()){
       modoDestravar();
       beep(QTD_BEEP_MODODESTRAVA, TEMPO_BEEP_MODODESTRAVA);
   }
   
   reiniciaTravas();
    
 }

void gerenciarDestrava1Click(){

  if (modoDestrava == DESTRAVA_MOTORISTA){ // DEstrava apenas motorista
          if (estadoAtual != ESTADO_MOTORISTA_ABERTA){
                destravarMotorista();
                beep(QTD_BEEP_DESTRAVA,TEMPO_BEEP_DESTRAVA_MOTORISTA);     
              }
           
           }
    else{ if (modoDestrava == DESTRAVA_TUDO){ // DEstrava 4 portas
             if (estadoAtual != ESTADO_TUDO_ABERTA) {
                    destravarTudo();
                    beep(QTD_BEEP_DESTRAVA,TEMPO_BEEP_DESTRAVA_TUDO);
                    clicks--; 
                }
                
            }
       }
}

void gerenciarDestrava2Click(){

   if (estadoAtual != ESTADO_TUDO_ABERTA) {
          destravarTudo();
          beep(QTD_BEEP_DESTRAVA,TEMPO_BEEP_DESTRAVA_TUDO);     
     }
          
}

void gerenciarDestrava3Click(){
  
    if (isPortaAberta()){
          buzinarProximoClick = false;
          beep(QTD_BEEP_NAO_BUZINAR,TEMPO_BEEP_NAO_BUZINAR);     
     }
  
  }

void gerenciarDestrava4Click(){

   descerVidros();

   reiniciaTravas();

 }  

void buzinar(int qtd, int duracao){

    if (isPodeBuzinar()){

      if (MODO_DEBUG){
        Serial.print("BUZINA:");
        Serial.println(qtd);
      }

      acionarBuzina(saidaBuzina, qtd, duracao);
            
    }

  }
  
void beep(int qtd, int duracao){
   
    if (MODO_DEBUG){
      Serial.print("BEEPS:");
      Serial.println(qtd);
    }
 
    acionarBeep(saidaBeep,qtd,duracao);
  
 }

void travar(){
  
     if (MODO_DEBUG){
       Serial.println("TRAVAR");
     }

     digitalWrite(saidaTrava,pulsoGND);
     bloquear(TEMPO_TRAVA);

     if (!isPortaAberta()){
        estadoAtual = ESTADO_TUDO_FECHADA;
     }
   

 }
void destravarMotorista(){
 
  if (MODO_DEBUG){

   Serial.println("DESTRAVA MOTORISTA");
  
  }

  digitalWrite(saidaDestravaMotorista,pulsoGND);
  bloquear(TEMPO_DESTRAVA);
  estadoAtual = ESTADO_MOTORISTA_ABERTA;

}

void destravarTudo(){
  if (MODO_DEBUG){
    Serial.println("DESTRAVA TUDO");
  }

  digitalWrite(saidaDestravaTudo,pulsoGND);
  bloquear(TEMPO_DESTRAVA);
  
   if (isPortaAberta()){
        estadoAtual = ESTADO_TUDO_ABERTA;
   }
}
// Alternar entre abrir porta do motorista ou abrir tudo
void modoDestravar(){
   
    modoDestrava = !modoDestrava;
    if (MODO_DEBUG){
      Serial.print("MODO DESTRAVA:");
      Serial.println(modoDestrava);
    }
    EEPROM.write(ENDERECO_DESTRAVA, modoDestrava);
    reiniciaTravas();
   }

void subirVidros(){
 
  if (MODO_DEBUG){
    Serial.println("SUBIR");
  }
  estadoAtual = ESTADO_SUBINDO_VIDRO;
  verificarTravaParaBuzinar();
  digitalWrite(saidaTrava,pulsoGND);
  aguardar(TEMPO_SUBIR_VIDROS);
  
 }
void descerVidros(){
   
    if (MODO_DEBUG){
      Serial.println("DESCER");
    }
    estadoAtual = ESTADO_DESCENDO_VIDRO;

    digitalWrite(saidaDestravaTudo,pulsoGND);
    aguardar(TEMPO_DESCER_VIDROS);
   
 }
 
 
void ligarPainelDigital(){
  
   // if (estadoPainelDigital!= ESTADO_PAINEL_LIGADO){
        estadoPainelDigital = ESTADO_PAINEL_LIGADO;
         digitalWrite(saidaPainelDigital, pulsoPositivo);
         bloquear(TEMPO_POWER_PAINEL);
         Serial.println("LIGAR PAINEL");

   //   }
    
 }
 
void desligarPainelDigital(){

    if (estadoPainelDigital == ESTADO_PAINEL_LIGADO){
     
         estadoPainelDigital =!ESTADO_PAINEL_LIGADO;
         digitalWrite(saidaPainelDigital, pulsoPositivo);
         bloquear(TEMPO_POWER_PAINEL);
         Serial.println("DESLIGAR PAINEL");

      }
    
 }

void dormir(){
 
  desativaSaidas();
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_cpu();
 
}

void aguardar(unsigned long tempoEspera){

    long tempoAtual = millis();

    clicksAnt = clicks;
   
    while((tempoAtual-tempoAnterior)<=tempoEspera && (clicksAnt==clicks)){
   
          tempoAtual = millis();
        
          Serial.print("clicks:");
          Serial.println(clicks);
    
    }
      
    tempoAnterior = millis();
    
    if (clicks != clicksAnt){;
     
        reiniciaTravas();
    
    }    
    

  }

 void bloquear(unsigned long tempoEspera){
 
   long tempoAtual = millis();
 
   while((tempoAtual-tempoAnterior)<=tempoEspera){
 
        tempoAtual = millis();
  
    }
    
   tempoAnterior = millis();

   desativaSaidas();

 }

int lerEntrada(int pin_entrada) {
 
  int estado;

      if (digitalRead(pin_entrada) == LOW)
        {
          estado = digitalRead(pin_entrada);
          if (estado == LOW)
          {
            while (estado == LOW)
              estado = digitalRead(pin_entrada);
            return 1;
          }  
        }
        
        return 0;
            
 }

void verificarTravaParaBuzinar(){

   if (isPortaAberta()){
      buzinar(QTD_BUZINA_PORTA_ABERTA,TEMPO_BUZINA_PORTA_ABERTA);
      reiniciaTravas();
  } else{
       buzinar(QTD_BUZINA_TRAVA, TEMPO_BUZINA_TRAVA);
  }
 
}

void reiniciaTravas(){
 
    clicks=0;
    
    estadoAtual=0;
 
 }

void acionarBuzina(int pin_buzina, int vezes,long duracao){

 for(int x=0;x<vezes;x++){

      digitalWrite(pin_buzina,pulsoGND);
    
      bloquear(duracao);
    
      digitalWrite(pin_buzina,pulsoPositivo);
    
      bloquear(duracao);
 }

}

void acionarBeep(int pin_beep, int vezes,long duracao){
  
 for(int x=0;x<vezes;x++){
   
      digitalWrite(pin_beep,pulsoPositivo);
    
      bloquear(duracao);
    
      digitalWrite(pin_beep,pulsoGND);
    
      bloquear(duracao);
  }

}

bool isBuzinarProximoClick(){
  
  return buzinarProximoClick;
  
 }
  
bool isPortaAberta(){
 
  return (digitalRead(entradaSensorPorta)==pulsoGND);
  

}
  
bool isPosChave(){
 
  return digitalRead(entradaPosChave)==pulsoPositivo;
  
  
 }

bool isTravou(){

    return clicks>0;

}

bool isDestravou(){

    return clicks<0;

}


 bool isPodeBuzinar(){
 
  return podeBuzinar;
  
 }

 void desativaSaidas(){
     
     digitalWrite(saidaDestravaMotorista,pulsoPositivo);// Relê 1
     digitalWrite(saidaDestravaTudo,pulsoPositivo);// Relê 2
     digitalWrite(saidaTrava,pulsoPositivo);// Relê 3
     digitalWrite(saidaBuzina,pulsoPositivo);// Relê 4
     
     digitalWrite(saidaPainelDigital,pulsoGND);// FotoAcoplador


  }

 void ativarPortas(){
  
  
  pinMode(saidaTrava, OUTPUT);
  pinMode(saidaDestravaMotorista, OUTPUT);
  pinMode(saidaDestravaTudo, OUTPUT);
  pinMode(saidaBuzina, OUTPUT);
  pinMode(saidaBeep, OUTPUT);
  pinMode(saidaPainelDigital,OUTPUT);
 
  
  pinMode(entradaTrava, INPUT_PULLUP);
  pinMode(entradaDestrava, INPUT_PULLUP);
  pinMode(entradaSensorPorta, INPUT_PULLUP);
  pinMode(entradaPosChave, INPUT);

  
  
  }
