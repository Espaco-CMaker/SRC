// Versão 0.8 Estável, analógico ok.


#include <PS2X_lib.h>
#include <Servo.h>
PS2X ps2x;

Servo direcao;
int direcaoPos = 95;

int error = 0;
int type = 0;
byte vibrate = 0;

#define PS2_DAT        12  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        13  //17

int MDa = 3;
int MDb = 5;
int MEa = 6;
int MEb = 9;

int vel;
int d;

int vel_bal = 0;
int d_bal = 0;

void setup()
{
  Serial.begin(9600);//artiva serial;
  // error = ps2x.config_gamepad(13, 11,10 , 12, true, true);
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
  ps2x.enableRumble();
  ps2x.enablePressures();
  pinMode(MDa, OUTPUT);
  pinMode(MDb, OUTPUT);
  pinMode(MEa, OUTPUT);
  pinMode(MEb, OUTPUT);
  //direcao.attach(9); // Pino do servo motor
  Serial.println("Estou vivo!!!!");
  //frenteV(0,0);

  //vel_bal = ps2x.Analog(PSS_RX) - 127;
  //d_bal = ps2x.Analog(PSS_LY) - 128;
  //delay(10000);
}
char tmp[100];
void loop()
{

  error = 0;
  type = 1;
  ps2x.read_gamepad(false, vibrate);
  d = map(ps2x.Analog(PSS_RX), 0, 255, -100, 100);
  vel = map(ps2x.Analog(PSS_LY), 0, 255, 100, -100);
  if (d >= 0)
  {
    if (vel == 0)
      //acionarMs(d * (100 + d) / 100, d * (100 - d) / 100);
      acionarMs(d,0);
    else
      acionarMs(vel, vel * (100 - d) / 100);
    //sprintf(tmp, "(VEL:%d,D:%d) Motores (ME:%d | MD:%d)", vel, d, vel, vel * (100 - d) / 100);

  }
  if (d < 0)
  {
    if (vel == 0)
      //acionarMs(d * (100 - d) / 100, d * (100 - d) / 100);
      acionarMs(0,-d );
    else
      acionarMs(vel * (100 + d) / 100, vel);
    //sprintf(tmp, "(VEL:%d,D:%d) Motores (ME:%d | MD:%d)", vel, d, vel * (100 + d) / 100, vel);

  }
  Serial.println(tmp); // imprimir na serial

}

void acionarMs(int esq, int dir)
{
  //sprintf(tmp, "(1E:%d,D:%d)", esq, dir);
  //Serial.println(tmp);
  esq = map(esq, -100, 100, -255, 255);
  dir = map(dir, -100, 100, -255, 255);
  sprintf(tmp, "(2E:%d,D:%d)", esq, dir);
  Serial.println(tmp);

  if (dir < 0 || esq < 0)
  {
    analogWrite(MDb, 0);
    analogWrite(MDa, 256 - dir);
    analogWrite(MEa, 0);
    analogWrite(MEb, 256 - esq);
  }
  else
  {
    analogWrite(MDb, +dir);
    analogWrite(MDa, 0);
    analogWrite(MEa, esq);
    analogWrite(MEb, 0);
  }

}

void ME(int vel, int d)
{
  //M1(vel+d);
}

void MD(int vel, int d)
{
  // M2(vel-d);
}
