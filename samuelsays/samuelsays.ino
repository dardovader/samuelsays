#include <Button.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define SOFT_SERIAL_TX 2
#define SOFT_SERIAL_RX 3
#define PIN_BOTON_1 4
#define PIN_BOTON_2 5
#define PIN_BOTON_3 6
#define PIN_BOTON_4 7
#define PIN_BOTON_5 8
#define PIN_LED_1   9
#define PIN_LED_2   10
#define PIN_LED_3   11
#define PIN_LED_4   12
#define PIN_LED_5   13

Button b1(PIN_BOTON_1);
Button b2(PIN_BOTON_2);
Button b3(PIN_BOTON_3);
Button b4(PIN_BOTON_4);
Button b5(PIN_BOTON_5);

#define SONIDO_START_JUEGO      0
#define SONIDO_NUM_JUGADORES_1  1
#define SONIDO_NUM_JUGADORES_2  2
#define SONIDO_NUM_JUGADORES_3  3
#define SONIDO_NUM_JUGADORES_4  4
#define SONIDO_ELIJA_NUM_JUG    5
#define SONIDO_LO_HAS_PETADO    6
#define SONIDO_PULSAR_B1        7
#define SONIDO_PULSAR_B2        8
#define SONIDO_PULSAR_B3        9
#define SONIDO_PULSAR_B4        10
#define SONIDO_PULSAR_B5        11
#define SONIDO_ERES_UN_PAQUETE  12

const byte MAX_SECUENCIA=255;

byte secuencia[255];
byte modo=0;
byte num_jugadores = 0;
byte jugador_turno=0;
byte fase = 0;
int velocidad = 1000;
int espera = 1000;

SoftwareSerial soft_serial(SOFT_SERIAL_RX, SOFT_SERIAL_TX); // RX, TX
DFRobotDFPlayerMini mp3;

void setup()
{
  b1.begin();
  b2.begin();
  b3.begin();
  b4.begin();
  b5.begin();
  pinMode(PIN_LED_1,OUTPUT);
  pinMode(PIN_LED_2,OUTPUT);
  pinMode(PIN_LED_3,OUTPUT);
  pinMode(PIN_LED_4,OUTPUT);
  pinMode(PIN_LED_5,OUTPUT);
  mp3.begin(soft_serial);
}

void loop()
{   
  switch (fase)
  {
    case 0:
      prejuego();
      break;
    case 1: //juego
      juego();
      break;
    case 2: // fin
      fin();
      break;
  }
}


void prejuego()
{
  apagarLeds();
  if (b1.released())  // Un jugador
  {
    num_jugadores = 1;
    apagarLeds();
    digitalWrite(PIN_LED_1,HIGH);
    mp3.play(SONIDO_NUM_JUGADORES_1);
  }
  if (b2.released()) // dos jugadores
  {
    num_jugadores = 2;
    apagarLeds();
    digitalWrite(PIN_LED_2,HIGH);
    mp3.play(SONIDO_NUM_JUGADORES_2);
  }
    if (b3.released()) // 3 jugadores
  {
    num_jugadores = 3;
    apagarLeds();
    digitalWrite(PIN_LED_3,HIGH);
    mp3.play(SONIDO_NUM_JUGADORES_3);
  }
  if (b4.released()) // 4 jugadores
  {
    num_jugadores = 4;
    apagarLeds();
    digitalWrite(PIN_LED_4,HIGH);
    mp3.play(SONIDO_NUM_JUGADORES_4);
  }
  if (b5.released()) // Start
  {
    if (num_jugadores != 0) {
      fase = 1;
      //tocarSonido Start 
      mp3.play(SONIDO_START_JUEGO);
    }
    else
    {
      mp3.play(SONIDO_ELIJA_NUM_JUG);
    }
  }
  
}

void apagarLeds()
{
  digitalWrite(PIN_LED_1,LOW);
  digitalWrite(PIN_LED_2,LOW);
  digitalWrite(PIN_LED_3,LOW);
  digitalWrite(PIN_LED_4,LOW);
  digitalWrite(PIN_LED_5,LOW);
}

void juego()
{
  static byte i= 0;
  secuencia[i] = random(1,6);
  // tocar secuencia
  for (byte j=0; j<=i; j++)
  { 
    apagarLeds();
    switch (secuencia[j])
    {
      case 1:
        digitalWrite(PIN_LED_1,HIGH);
        mp3.play(SONIDO_PULSAR_B1);
        break;
      case 2:
        digitalWrite(PIN_LED_2,HIGH);
        mp3.play(SONIDO_PULSAR_B2);
        break;
      case 3:
        digitalWrite(PIN_LED_3,HIGH);
        mp3.play(SONIDO_PULSAR_B3);
        break;
      case 4:
        digitalWrite(PIN_LED_4,HIGH);
        mp3.play(SONIDO_PULSAR_B4);
        break;
      case 5:
        digitalWrite(PIN_LED_5,HIGH);
        mp3.play(SONIDO_PULSAR_B4);
        break;
    }
    delay(velocidad);
  }
  //esperar jugador
  static byte boton=0;
  for (byte k=0; k<=i; k++)
  {
    while (b1.toggled() || b2.toggled() || b3.toggled() || b4.toggled() || b5.toggled())
    {
      if (b1.released())
      {
        boton=1;
        digitalWrite(PIN_LED_1,HIGH);
        mp3.play(SONIDO_PULSAR_B1);
      }
      else 
      {
        if (b2.released())
        {
          boton=2;
          digitalWrite(PIN_LED_2,HIGH);
          mp3.play(SONIDO_PULSAR_B2);
        }
        else
        {
          if (b3.released())
          {
            boton=3;
            digitalWrite(PIN_LED_3,HIGH);
            mp3.play(SONIDO_PULSAR_B3);
          }
          else
          {
            if (b4.released())
            {
              boton=4;
              digitalWrite(PIN_LED_4,HIGH);
              mp3.play(SONIDO_PULSAR_B4);
            }
            else
            {
              boton=5;
              digitalWrite(PIN_LED_5,HIGH);
              mp3.play(SONIDO_PULSAR_B5);
            }
          }
        }
      }   
    }
    if (boton != secuencia[k])
    {
      modo=2;
      break;
    }
  }
}

void fin()
{
  mp3.play(SONIDO_ERES_UN_PAQUETE);
  delay(300);
}

