#include <LiquidCrystal_I2C.h>

int gatilho = 10;
int echo = 9;
float tempo;
float distancia_cm;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
// SCL = Pino A5 do Arduino
// SDA = Pino A4 do Arduino

void setup() {
  Serial.begin(115200);
  pinMode(gatilho, OUTPUT);
  pinMode(echo, INPUT);
  lcd.begin (16, 2);
}

void loop()
{

  // Limpa o trigger
  digitalWrite(gatilho, LOW);
  delayMicroseconds(5);

  //Dispara
  digitalWrite(gatilho, HIGH);
  delayMicroseconds(10);
  digitalWrite(gatilho, LOW);
  
  tempo = pulseIn(echo, HIGH);// microsegundos.
  
  //----------------------------//
  // Velocidade do som 343m/s
  // 343m - 1s
  // dist - tempo
  // dist x 1s = 343m x tempo
  // dist = 343 x tempo (segundos)

  distancia_cm = tempo/1000000 * 343 / 2*100;


  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print (distancia_cm);
  lcd.print("cm   ");

  Serial.println(distancia_cm);
  delay(10);
}
