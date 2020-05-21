// ========================================
// Rotinas de Entrada e Saida
// ========================================
// void bip(int num, int tempo);
// void piscaLed();
// void ligaLed(int pin);
// void desligaLed(int pin);
// void rotorCima();
// void rotorBaixo();
// void acionaMotor();
// void calibraServo();

void inicializaIOs()
{
  pinMode(pinReset, INPUT_PULLUP);
  //pinMode(pinServo, OUTPUT);//servo
  pinMode(led_pwr, OUTPUT);//led
  pinMode(pinBuzzer, OUTPUT);//led

}

void atualizaPortas()
{
  int val=0;
  //d0 = digitalRead(pinD0);
  //d1 = digitalRead(pinD1);
  //d2 = digitalRead(pinD2);
  val = analogRead(pinA0);
  // trata sensor invertido
  if (LOGICAINVERTIDASENSOR)
    a0 = 1023 - val;
  else
    a0 = val;

  if (a0 < corte_infra)
    bip(1, 200);

}
int leSensor()
{
  int val;
  
  if (val < corte_infra)
    bip(1, 200);
  return val;
}
void bip(int num, int tempo)
{
  if (dado.bip)
  {
    for (int i = 0; i < num; i++)
    {
      digitalWrite(pinBuzzer, HIGH);
      delay(tempo);
      digitalWrite(pinBuzzer, LOW);
      delay(tempo);
    }
  }
}
void bipForce(int num, int tempo)
{
 
    for (int i = 0; i < num; i++)
    {
      digitalWrite(pinBuzzer, HIGH);
      delay(tempo);
      digitalWrite(pinBuzzer, LOW);
      delay(tempo);
    }
 
}

void piscaLed(int pinLed)
{
  int t = 450;
  ligaLed(pinLed);
  delay(t);
  desligaLed(pinLed);

}

void ligaLed(int pin)
{
  digitalWrite(pin, HIGH);
}
void desligaLed(int pin)
{
  digitalWrite(pin, LOW);
}
void rotorCima()// dentro da interrupt
{
  //Serial.println("Rotor em cima");
  meuservo.write(angulo_ini);
  delay(temp_aciona); // dentro da interrupt

}
void rotorBaixo()// dentro da interrupt
{

  //Serial.println("Rotor embaixo");
  meuservo.write(angulo_fim);
  delay(temp_aciona); // dentro da interrupt

}


//===============================================================
// Autor: Fabio Bettio
// Data: 04/2020
// Descrição: Aciona servo para acionamento do rotor
// Entradas:
// Saidas:
//===============================================================

void acionaMotor()// dentro da interrupt
{
  //imprimeLog("Acionamento");
  bip(2, 100);
  for (int i = 0; i < dado.disparo[acionaRotor][3] / PORCAO; i++)
  {
    Serial.printf("valor:%d/%d rotor:%d\n", i, dado.disparo[acionaRotor][3], acionaRotor);
    rotorBaixo();
    rotorCima();
    
    delay(2000);
    dado.racao_mes += PORCAO; // contabiliza errado se estiver sem comida
  }
  sprintf(txt, "Alimentador Acionado %dx (descarregado %d gramas)\n", dado.disparo[acionaRotor][3] / PORCAO, dado.disparo[acionaRotor][3]);
  imprimeLog(txt);

  atualizaEEPROM();
}

//===============================================================
// Autor: Fabio Bettio
// Data: 04/2020
// Descrição: Calibração do servo, para ajustes de montagem
// Entradas:
// Saidas:
//===============================================================

void testeServo()
{
  // Calibração do Rotor
  meuservo.write(angulo_ini);
  Serial.println("Posição 0 do Servo, aguarda 1s");
  delay(temp_aciona); // dentro da interrupt
  meuservo.write(angulo_fim);
  Serial.println("Posição 180 do Servo, aguarda 1s");
  delay(temp_aciona); // dentro da interrupt
  //////////////////

}
