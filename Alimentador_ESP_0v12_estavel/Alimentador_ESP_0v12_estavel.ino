// Hardware: ES8266
// Placa: NodeMCU- 0.9 (ESP12) > NodeMCU- 1.0 (ESP-12E Module)
// Chip: ESP8266EX
// Data: 11/2019
// Autor: Bettio
// Informações:
// Fontes
// Servo: http://blog.baudaeletronica.com.br/nodemcu-esp8266-com-micro-servo-9g/
// Graficos: https://www.w3schools.com/graphics/svg_circle.asp
// Wifi-Manager:  //https://github.com/tzapu/WiFiManager
// Instalação ESP8266 >
// http://arduino.esp8266.com/stable/package_esp8266com_index.json
// Instalar: esp8266 by ESP8266 Community
//
#include <Ticker.h>//arduino_esp32_master.zip
#include <locale.h> //necessário para usar setlocale
#include <EEPROM.h>
#include <Servo.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
//#include <DateTime.h>

#define MAXBUF 300 // Tamanho do buffer de texto
#define MAXLOG 20  // Numero máximo de registros de LOG
#define MAXPROG 5 // Numero maximo de dispoaros por dia
#define MAXCOL 4  // Número de colunas nos acionamentos Hora, Minuto e Status 
#define LOGICAINVERTIDASENSOR true // Se true trabalha com logica invertida na leuitura do sensor IV.

#define DESATIVADO -1
#define AGENDADO 0
#define EXECUTADO 1
#define SEMCOMIDA 2

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#else
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>

#endif
//#include <SPIFFS.h>
#include <NTPClient.h>//Biblioteca do NTP. NTPClient-master.zip - http://zetcode.com/articles/cdatetime/
Ticker ticker;
char txt[MAXBUF];

int DEBUG = 4;
//const char* nomedev = "NodeMCU-32S-v0.01.01";
//ESP8266-NodeMCU0.9-ESP-12-
String nomedev = "CMaker-";


// Vairiaveis de statut e configuração
//char estado = 't';
int corte_infra = 500;

//=========== TEMPO ===========//
long tempo = 0;
//Struct com os dados do dia e hora
struct tm *a;
time_t agora_t;

//=========== ROTOR ===========//
int PORCAO = 100; // com tubo chanfrado e servo motor - quantidade por acionamento (50 reverso / 100 ligado) com extrator de PCV em pas de borracha
// Comedor pago grande 45g
// Rotor Baixo - Comedor pequeno - milho - 1 acionamento 38g 2 acionamento 73g (36g)
// Rotor Cima  - Comedor pequeno - milho - 1 acionamento 50g 2 acionamento 74g (36g)



int acionaRotor = -1;
int angulo_fim = 0;
int angulo_ini = 180;
int temp_aciona = 1000; // tempo de acionamento servo
Servo meuservo;  // instância chamada meuservo

// Variáveis permanentes
//EEPROMReadInt(add1_EEPROM)
//EEPROMWriteInt(add1_EEPROM, quant_total)

//=========== VARIÁVEIS WEB ===========//
// int acionamentos = 3; // numero de acionamentos;
// long intervalos = 30;// intervalo entre acionamento (segundos)
// Estrutura Principal
struct dataPROMConfig
{
  int primeiroboot = 1;
  int64_t racao_mes;// = -1;
  int64_t acionamentos;// = -1; // numero de acionamentos;
  int64_t acionamentos_anterior;// = -1; // numero de acionamentos mês anterior;
  long intervalos;// = -1; // intervalo entre acionamento (segundos)
  int quantidade;// = -1;
  int disparo[MAXPROG][MAXCOL];
  char texto[MAXBUF];
  int antipombo = 0;
  //Colunas... [0] Hora [1] Minuto [2] Status [3] numero da acionamentos
  //#define AGENDADO 0
  //#define EXECUTADO 1
  //#define SEMCOMIDA 2
  int bip = 0;
  //int logicaInvertidaSensor = 0;
} dado;

String ultimosLogs[MAXLOG] = {"", "", "", "", "", "", "", "", "", ""};
ESP8266WebServer server(15001);

// Variáveis Globais
//=========== IOS ===========//
// Pinagem
int led_pwr =   2;
int pinA0 =     A0;// Sensor comida
int pinReset =  D2;// GND = Reset
int pinServo =  D6;// servo
int pinBuzzer = D3;

//int pos = 0;    // variável de posição iniciada em zero
int d0 = 0, d1 = 0, d2 = 0, a0 = 0;
const int led = 13;

//=========== SETUP ===========//
void setup() {
  //#########################################
  //restaTudo();
  //#########################################

  nomedev += __FILE__;
  Serial.begin(9600);
  setlocale(LC_ALL, "Portuguese");
  inicializaIOs();

  EEPROM.begin(sizeof(dataPROMConfig));

  // #### JUMPER PINO D0 <-> 5V reinicia D0 <-> GND modo reset
  configFabrica();

  leEEPROM();
  imprimeStatusSerial();

  imprimeLog(__FILE__);

  ligaLed(led_pwr);
  conectaWifi();
  ntpInicio();

  ticker.attach(1000 * 60, dispara);
  serveInit();
  meuservo.attach(pinServo);  // anexando o pino D4 do NodeMcu ao meuservo
  rotorCima();
  imprimeLog("#### SISTEMA ATIVO ####");
  bipForce(2, 100);
  byte ret = sendEmail();
}
//MDNSResponder mdns;
//const char* mDNSname     = "ESP-AEF433";
void loop() {
  piscaLed(led_pwr);
  //MDNS.update();
  testaConexao();
  server.handleClient();
  atualizaPortas();
  //imprimeStatusSerial();
  imprimeInfoRede();
  delay(1000);
  checaHora();// função chama atualizahora que chama NTP que trava o fluxo...
  //imprimeStatusSerial();
  //delay(100);
  if (acionaRotor >= 0 )
  {
    //ticker.detach();
    acionaMotor();
    acionaRotor = -1;
    //delay(1000*60*1);// melhorar isso
    //imprimeStatus();
    //ticker.attach(dado.intervalos*3600, dispara);
  }
  //testeServo();
  //MDNS.update(); 
  //delay(10000);
}
