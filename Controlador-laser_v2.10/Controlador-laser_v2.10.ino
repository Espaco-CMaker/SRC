// Hardware: Arduino UNO
// Placa: Arduino UNO
// Chip: ATmega328
// Data: 10/2021
// Autor: Bettio
// Informações:
// Fontes
// Sensor de Fluxo: https://www.usinainfo.com.br/blog/sensor-de-fluxo-de-agua-arduino-como-sensor-de-vazao-para-projetos/
// https://www.robocore.net/tutoriais/usando-sensor-fluxo-de-agua
//
//
//#########################################
// Valor máximo 60.000 Horas
// Decomentar a linha abaixo para zerar os dados da EEPROM
// EEPROMWritelong(0, 0);
// hora_gravada = (220.0 * 60.0 * 60.0);// seta valor forçado de horas
// EEPROMWritelong(0, hora_gravada);
// delay(200);
// INICIO DA GRAVAÇAO DA EEPROM 26/05/2021
// Troca do Tubo de 50W por 60W 09/10/2021
// V2.9 (27/12/2023) - Inserdido bloqueio de laser 
//#########################################

char ver[20] = "CTRL Laser v2.10";


#include <avr/wdt.h>

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define PAUSE 0
#define REST      0

#include <Wire.h>
#include <EEPROM.h>

//Necessário incluir as libs abaixo
#include <LiquidCrystal_I2C.h>// Se der probelma deletar todas as libs com LCD de C:\Users\Maker\Documents\Arduino\libraries e instalar NewliquidCrystal_ok.zip   
#include <OneWire.h>//https://www.arduinolibraries.info/libraries/one-wire
#include <DallasTemperature.h>//https://github.com/milesburton/Arduino-Temperature-Control-Library
#include "TimerOne.h"

void inctempo(void);
void imprimeStatus(int err);
void medeFluxo();
void alarmeTempAlta(int err);


#define TEMPMIN 18 // Fabricante: 18
#define TEMPMAX 24 // Fabricante: 25
#define TEMPCRIT 25 // Fabricante: 25

// #### PINAGEM
#define ONE_WIRE_BUS1 4// sensor de temperatura
int pinBuzzer = 9;
int pinLaser = 3; // INTERRUPÇÃO 2
int pinBot = 10;
int pinFluxo = 2;// INTERRUPÇÃO 1
int pinReleFrio = 12;
int pinReleQuente = 11;

// LEDs

int pinLEDAZ = 8;
int pinLEDVD = 7;
int pinLEDAM = 6;
int pinLEDVM = 5;


int laser = HIGH; // HUGH = DESLIGADO

#define  MILISECONDS_IN_DAY  86400UL // 86400000 milliseconds in a day
#define  MILISECONDS_IN_HOUR  3600UL // 3600000 milliseconds in an hour
#define  MILISECONDS_IN_MINUTE  60UL // 60000 milliseconds in a minute
#define  MILISECONDS_IN_SECOND   1UL // 1000 milliseconds in a second

//===================================
// Sensor de Fluxo
//definicao do pino do sensor e de interrupcao
const int INTERRUPCAO_SENSOR = 0; //interrupt = 0 equivale ao pino digital 2


//definicao da variavel de contagem de voltas
unsigned long contador = 0;

//definicao do fator de calibracao para conversao do valor lido
const float fator_calibracao = 4.5;

//definicao das variaveis de fluxo e volume
float fluxo = 0;
float volume = 0;
float volume_total = 0;

//definicao da variavel de intervalo de tempo
unsigned long tempo_antes = 0;

//===================================

int estado_btnplay;
int estado_btnstop;

unsigned long tempo;
unsigned long tempo_play = 0;
unsigned long tempo_stop;
unsigned long tempo_segundo;


OneWire oneWire(ONE_WIRE_BUS1);

OneWire oneWire1(ONE_WIRE_BUS1);
OneWire oneWire2(ONE_WIRE_BUS1);//usa o mesmo BUS
float tempMin = 999;
float tempMax = 0;

int flag_primerio_acesso = 321; //O sitema busca pelo código 321 no end 1 da EEPROM, se não encontrar assume que é a prieira carga e zera a hora;

long hora_ini = 0.0;//UL
long hora_agora = 0.0;
long hora_gravada =  0.0;
long hora_agora_gravada =  0.0;

char txt[100];
int tdelay = 50;

DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);

DeviceAddress sensor1;
DeviceAddress sensor2;

LiquidCrystal_I2C lcd(0x27, 16 , 2);
//LiquidCrystal_I2C lcd(0x27,20,4);
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, POSITIVE);

byte barra[] = {
  B00000,
  B10000,
  B01000,
  B00100,
  B00010,
  B00001,
  B00000,
  B00000
};

// Musica
int tempom = 144;

// change this to whichever pin you want to use
//int buzzer = 11;//pinBuzzer

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {


  REST, 2, REST, 4, REST, 8, NOTE_DS4, 8,
  NOTE_E4, -4, REST, 8, NOTE_FS4, 8, NOTE_G4, -4, REST, 8, NOTE_DS4, 8,
  NOTE_E4, -8, NOTE_FS4, 8,  NOTE_G4, -8, NOTE_C5, 8, NOTE_B4, -8, NOTE_E4, 8, NOTE_G4, -8, NOTE_B4, 8,
  NOTE_AS4, 2, NOTE_A4, -16, NOTE_G4, -16, NOTE_E4, -16, NOTE_D4, -16,
  NOTE_E4, 2, REST, 4, REST, 8, NOTE_DS4, 4,

};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / 2 / 2;

// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = (60000 * 4) / tempom;

int divider = 0, noteDuration = 0;

// Fim musica

void setup(void) {
  Serial.begin(115200);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinReleFrio, OUTPUT);
  pinMode(pinReleQuente, OUTPUT);
  pinMode(pinBot, INPUT_PULLUP);
  digitalWrite(pinReleFrio, HIGH);
  digitalWrite(pinReleQuente, LOW); // LOW = OK (Ativo) Permite que o laser funcione.
  pinMode(pinFluxo, INPUT_PULLUP);//Fluxo
  pinMode(pinLaser, INPUT);// em teste PULLUP ou INPUT
  //Serial.println("Resetando...");
  //wdt_reset ();

  //wdt_enable (WDTO_8S);

  // PinMode LEDs
  pinMode(pinLEDAZ, OUTPUT);//
  pinMode(pinLEDVD, OUTPUT);//
  pinMode(pinLEDAM, OUTPUT);//
  pinMode(pinLEDVM, OUTPUT);//



  //sensors1.begin();

  INIT_DS18B20(12);

  Timer1.initialize(1000000);  //1seg
  Timer1.attachInterrupt(inctempo);

  //lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, barra);
  //delay(1000);

  //bipseno();


  //imprimeHoraTeste();
  flag_primerio_acesso = 0;
  flag_primerio_acesso = EEPROMReadlong(5);

  // Testa primeiro acesso
  if (flag_primerio_acesso != 321)
  {
    Serial.print("#### PRIMEIRO ACESSO ZERANDO HORA ####");
    zeraHora();
    EEPROMWritelong(5, 321);
    EEPROMWritelong(0, 0);
    flag_primerio_acesso = EEPROMReadlong(5);
  }
  hora_gravada = EEPROMReadlong(0);
  //imprimeHoraTeste();

  abertura();
  tdelay = 150;
  lcd.clear();
  //hora_ini = 120*1000;//seta valor inicial de hora
  hora_ini = 0.0;

  Serial.print("Hora Inicial:");
  Serial.println(hora_ini);

}
void loop() {

  int bot = 0;
  int i = 0;
  float tempC1;
  float tempC2;


  laser = digitalRead(pinLaser);
  Serial.println(laser);
  lcd.setCursor(8, 1);
  lcd.print("        ");
  
  lcd.setCursor(11, 1);
  if (laser == LOW)
    lcd.print(" LON ");
  else
    lcd.print(" LOFF");


  imprimeHoraTeste();
  imprimeFluxo();
  bot = digitalRead(pinBot);
  delay(tdelay);
  if (bot == LOW)//Botão press
  {
    Serial.println("Botão PRESS");
    bip();
    //if (hora_agora > 0)
    {
      //hora_gravada = (EEPROMReadlong(0));
      //EEPROMWritelong(0, hora_agora + hora_gravada);
      lcd.setCursor(0, 1);
      lcd.print("#Total:");
      hora_gravada = (EEPROMReadlong(0));
      lcd.print(printHora(hora_gravada));
      lcd.setCursor(16, 1);
      lcd.print("#");
      hora_agora = 0.0;
      delay(5000);
      lcd.clear();
    }
    reset();
  }

  if (hora_agora_gravada > 60 * 1)// a cada 60s
  {
    Serial.println("Hora Agora Gravada > 0");
    hora_gravada = (EEPROMReadlong(0));
    imprimeHoraTeste();
    Serial.println(hora_agora_gravada);

    EEPROMWritelong(0, hora_agora_gravada + hora_gravada);
    imprimeHoraTeste();
    //imprimeFluxo();

    lcd.setCursor(0, 1);
    lcd.print("#Total:");
    hora_gravada = EEPROMReadlong(0);
    lcd.print(printHora(hora_gravada));
    lcd.setCursor(16, 1);
    lcd.print("#");
    delay(2 * tdelay);
    lcd.clear();
    hora_agora_gravada = 0.0;
  }

  sensors1.requestTemperatures();

  do
  {
    //tempC1 = sensors1.getTempC(sensor1) + 1; // +1 Calibragem, sensor marcando 1 grau a menos
    //tempC1 = sensors1.getTempC(sensor1);
    tempC1 = DS18B20_CELSIUS(0);
    Serial.print("T1 ");
    Serial.print(tempC1);
    Serial.println(" ºC");
    i++;
    if (i > 50)
    {
      Status(1);// ERRO
      break;
    }
  } while (tempC1 < -100 || tempC1 == 0);
  //sensors2.requestTemperatures();

  do
  {
    //tempC2 = tempC2 = sensors1.getTempC(sensor2);
    tempC2 = DS18B20_CELSIUS(1);
    Serial.print("T2 ");
    Serial.print(tempC2);
    Serial.println(" ºC");
    i++;
    if (i > 50)
    {
      Status(1);// ERRO
      break;
    }
  } while (tempC2 < -50 || tempC2 == 0);
  //Serial.println(tempC1);
  /*
    if (tempC1 < tempMin)
    {-
      if (tempC1 != -127)
        tempMin = tempC1;
    }
    if (tempC1 > tempMax)
    {
      tempMax = tempC1;
    }
  */
  lcd.setCursor(0, 0);
  //lcd.print("1:");
  lcd.print("#");
  lcd.print(tempC1, 1);
  lcd.write(223);
  lcd.print("C ");
  lcd.setCursor(0, 1);
  //lcd.print("2:");
  lcd.print("H");
  lcd.print(tempC2, 1);
  lcd.write(223);
  lcd.print("C  ");

  /*lcd.print(printHora(hora_agora));
    lcd.print("   ");
    lcd.setCursor(10, 0);
    lcd.print("H:");
    lcd.print(tempMax, 1);
    lcd.print(" ");
  */

  checa_temp(tempC1, tempC2, TEMPMIN, TEMPMAX);
  medeFluxo();

}
