#include "ibus.h"

//Taxa de atualização da comunicação em milisegundos(Não mexer)
#define UPDATE_INTERVAL 10 

//Pino digital do relé que aciona led da ponta do blaster
#define STUD_LED 2

//Pino digital do relé que aciona motor de vibrar a empunhadura
#define GUN_RECOIL 3

//Numero de entradas analógicas,cada analógico tem 2 potenciometros a serem ligados,totalizando 2 potenciometros a serem lidos
#define ANALOG_INPUTS_COUNT 2 

//Portas analógicas do Arduino para conectar os fios do analógico
byte analogPins[] = {A0, A1};

//Numero de entradas digitais,que são o numero de botões liga/desliga a serem usados no controle
#define DIGITAL_INPUTS_COUNT 4 

//Portas digitais onde serão conectados os fios dos botões feitos
byte digitalPins[] = { 4, 5, 6, 7};

//Taxa de comunicação serial,esse valor é especifico para o progama do controle virutal
#define BAUD_RATE 115200 

//Os tipos de entradas pro controle são somados 
#define NUM_CHANNELS ( (ANALOG_INPUTS_COUNT)+ (DIGITAL_INPUTS_COUNT) )

//E aqui esse valor informa quantos canais vão receber os dados dos botões e analógicos por voce controlados
IBus ibus(NUM_CHANNELS);

void setup()
{
  
 //Relé
 pinMode(2,OUTPUT);
 pinMode(3,OUTPUT);
 //BOTAUM
 pinMode(4,INPUT_PULLUP);
 pinMode(5,INPUT_PULLUP);  
 pinMode(6,INPUT_PULLUP);
 pinMode(7,INPUT_PULLUP);


//é aqui que se inicia a comunicação dos dados do Arduino com o computador e as aplicações de Joystic Virtual
  Serial.begin(BAUD_RATE);

}


void loop()

{
//talvez seja interessante usar uma função de interrupção aqui...
if(digitalRead(digitalPins[0]) ==LOW)//posição 0 desse vetor é o pino 2,que sera o gatilho da arma de lego
{
digitalWrite(STUD_LED,LOW);//ativa led do lego
digitalWrite(GUN_RECOIL,LOW);//ativa vibração
}else if(digitalRead(digitalPins[0]) ==HIGH)
{
digitalWrite(STUD_LED,HIGH);
digitalWrite(GUN_RECOIL,HIGH);
}

//Aqui se inicia a comunicação dos estados dos botões e analógicos com o PC
int i, bm_ch = 0;
  unsigned long time = millis();


       ibus.begin();
  
//Aqui os estados dos botões e do analógico são lidos e enviados para a comunicação serial
      for(i=0; i < ANALOG_INPUTS_COUNT; i++)
    ibus.write(analogRead(analogPins[i]));

    for(i=0; i < DIGITAL_INPUTS_COUNT; i++)
    ibus.write(digitalRead(digitalPins[i]) == HIGH ? 1023 : 0);
 
     ibus.end();

    time = millis() - time; // time elapsed in reading the inputs
    if(time < UPDATE_INTERVAL)
    // sleep till it is time for the next update
    delay(UPDATE_INTERVAL  - time);

}
  
