
void reset()
{
  //Serial.println("Resetando 8s...");
  wdt_enable (WDTO_1S);
  Serial.println("Resetando...");
  wdt_reset ();
  Serial.println("Resetando agora vai...");
  while (1);
}
// Pass oneWire reference to DallasTemperature library
DallasTemperature  temperatureSensors(&oneWire);

// Create an array for DS18B20 sensor addresses
DeviceAddress  DS18B20[3];

// https://dragaosemchama.com/2019/02/musicas-para-arduino/
void musica()
{
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(pinBuzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(pinBuzzer);
  }
}


/*-----------------------------------------------------------------------------------------------
   Function: INIT_DS18B20
   Description: Initialize DS18B20 Temperature sensor(s)
   Ins: Integer value for sensor precision.  Can be 9, 10, 11 or 12 bits
   Outs: none
   ----------------------------------------------------------------------------------------------*/

void INIT_DS18B20(int precision)
{
  temperatureSensors.begin();

  int available = temperatureSensors.getDeviceCount();
  sprintf(txt, "Encontrado:%d Sensores\n", available);
  Serial.print(txt);
  for (int x = 0; x != available; x++)
  {

    if (temperatureSensors.getAddress(DS18B20[x], x))
    {
      //temperatureSensors.setResolution(DS18B20[x], precision);
    }

    //Serial.println(temperatureSensors.getAddress(DS18B20[x], x), HEX);
  }
  mostra_endereco_sensor(DS18B20[0]);
  mostra_endereco_sensor(DS18B20[1]);
}

/*-----------------------------------------------------------------------------------------------
   Function: DS18B20_CELSIUS
   Description: Get celsius reading from DS18B20 Temperature sensor
   Ins: Integer value for sensor address
   Outs: Returns celsius reading
   ----------------------------------------------------------------------------------------------*/

float DS18B20_CELSIUS(int address)
{
  if (temperatureSensors.getAddress(DS18B20[address], address))
  {
    temperatureSensors.requestTemperatures();
    return temperatureSensors.getTempC(DS18B20[address]);
  }
  else
    return 0;
}




//=================================
void abertura()
{
  lcd.setCursor(0, 0);
  lcd.print(ver);
  lcd.setCursor(0, 1);
  lcd.print("#Total:");
  lcd.print(printHora(hora_gravada));
  lcd.setCursor(16, 1);
  lcd.print("#");
  //musica();
  for (int j = 0; j < 2; j++)
    for (int i = 5; i <= 8; i++)
    {
      digitalWrite(i, 1);
      bip();
      digitalWrite(i, 0);
    }

  delay(2000);
}

void LEDAZ(bool liga)
{
  digitalWrite(pinLEDAZ, liga);
}
void LEDVD(bool liga)
{
  digitalWrite(pinLEDVD, liga);
}
void LEDAM(bool liga)
{
  digitalWrite(pinLEDAM, liga);
}
void LEDVM(bool liga)
{
  digitalWrite(pinLEDVM, liga);
}

void zeraHora()
{
  hora_gravada = 0;
  EEPROMWritelong(0, 0);
}
char* printTime(long val)
{
  //Serial.println(val,BIN);
  uint8_t days = val / MILISECONDS_IN_DAY ;
  uint8_t hours = (val % MILISECONDS_IN_DAY) / MILISECONDS_IN_HOUR;
  uint8_t minutes = ((val % MILISECONDS_IN_DAY) % MILISECONDS_IN_HOUR) / MILISECONDS_IN_MINUTE ;
  uint8_t seconds = (((val % MILISECONDS_IN_DAY) % MILISECONDS_IN_HOUR) % MILISECONDS_IN_MINUTE) / MILISECONDS_IN_SECOND;

  // digital clock display of current time
  sprintf(txt, "%2.2d:%2.2d:%2.2d    ", hours, minutes, seconds);

  //Serial.println(txt);
  return txt;
}
char* printHora(long val)
{
  long days = val / MILISECONDS_IN_DAY ;
  uint8_t hours = (val % MILISECONDS_IN_DAY) / MILISECONDS_IN_HOUR;
  //uint8_t hours = (MILISECONDS_IN_DAY) / MILISECONDS_IN_HOUR;
  uint8_t minutes = ((val % MILISECONDS_IN_DAY) % MILISECONDS_IN_HOUR) / MILISECONDS_IN_MINUTE ;
  uint8_t seconds = (((val % MILISECONDS_IN_DAY) % MILISECONDS_IN_HOUR) % MILISECONDS_IN_MINUTE) / MILISECONDS_IN_SECOND;
  // digital clock display of current time

  sprintf(txt, "%5.5ldH%2.2dM", hours + (days * 24), minutes);
  //Serial.println(txt);
  //while (1);

  return txt;
}
void bip()
{
  Serial.print("tdelay:");
  Serial.println(tdelay);
  //digitalWrite(pinBuzzer, HIGH);
  tone(pinBuzzer, tdelay * 5);
  delay(tdelay);
  noTone(pinBuzzer);
  delay(tdelay);
}
void bipseno()
{
  for (float a = 0; a < PI * 4; a += 0.1)
  {
    tone(pinBuzzer, 3000 + 500 * sin(a));
    //Serial.println(4000 * sin(a));
    delay(10);
  }

  noTone(pinBuzzer);
  //delay(100);
}

long EEPROMReadlong(long address) {
  long resp = 0;
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  resp = ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);

  Serial.print("Lendo EEPROM > ");
  Serial.println(resp);


  return (resp);
}

void EEPROMWritelong(int address, long value) {
  Serial.print("Gravando EEPROM > ");
  Serial.println(value);
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}
void mostra_endereco_sensor(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
  Serial.println();
}
int ast = 0;
void printAsterisco(int lin, int col)
{
  lcd.setCursor(col + 1, lin);
  lcd.print(" ");
  lcd.setCursor(col, lin);

  switch (ast)// asterisco
  {
    case 0: lcd.print("|"); break;
    case 1: lcd.print("/"); break;
    case 2: lcd.print("-"); break;
    case 3: lcd.write(1); ast = -1; break;
  }
  ast++;

}

void imprimeHoraTeste()
{
  Serial.println(hora_gravada);
  Serial.println(printHora(hora_gravada));
  //Serial.println(printTime(hora_gravada));
  Serial.println(printTime(hora_agora_gravada));
  Serial.println(">====================");
  /*
    if (ast == 0)
    {
      lcd.setCursor(8, 0);
      lcd.print(printTime(hora_agora));
    }
  */
}
void imprimeFluxo()
{
  switch (ast) //Asterisco
  {
    case 0:
      lcd.setCursor(8, 0);
      lcd.print(printTime(hora_agora));
      delay(tdelay);
      break;
    case 1:
      lcd.setCursor(8, 0);
      lcd.print(printHora(hora_gravada));
      delay(tdelay);

      break;
    case 2:
      lcdFluxo();
      break;
    case 3:
      lcdFluxo();
      break;
  }


}


void aciona_rele(int porta)
{
  Serial.println("Liga Rele");
  digitalWrite(porta, LOW);
}

void desliga_rele(int porta)
{
  Serial.println("Desliga Rele");
  digitalWrite(porta, HIGH);
}
bool checa_temp(float temp1, float temp2, float lim_min, float lim_max)
{
  //#define TEMPMIN 19.0 // Fabricante: 18
  //#define TEMPMAX 24.0 // Fabricante: 25
  //#define TEMPCRIT 25 // Fabricante: 25

  /*
    // ###### CONTROLE RELE
    if ( temp2 >= TEMPMIN )//Temp da água > 19
    {
      aciona_rele(pinReleFrio);
    }
    // ## ALARME BAIXA TEMP (<18)
    if ( temp2 <= TEMPMIN - 1) //Temp da água < 18
    {
      desliga_rele(pinReleFrio);
    }
  */

  // ####### ALARME BAIXA TEMP (<18)
  if (temp1 < TEMPMIN || temp2 < TEMPMIN) // Alerta sonoro BAIXA temperatura
  {
    Status(-1);
  }

  // ######  NORMAL
  if (( temp1 >= TEMPMIN && temp1 <= TEMPMAX) && (temp2 >= TEMPMIN && temp2 <= TEMPMAX))// Normal
  {
    Status(0);// Nomal

  }
  // Alta Temp
  if ( ((temp1 >= TEMPMAX )  ||  (temp2 >= TEMPMAX)) && ((temp1 < TEMPCRIT) && (temp2 < TEMPCRIT)) ) // Alerta sonoro ALTA temperatura
    //if ( (temp1 >= TEMPMAX && temp1 < TEMPCRIT)  ||  (temp2 >= TEMPMAX && temp2 < TEMPCRIT) ) // Alerta sonoro ALTA temperatura
  {
    Serial.println("ALARME TEMP!!!");
    //alarmeTempAlta(2);// ALTA TEMP
    Status(2);//Alta

    // Lerta sonoro extra
    for (int x = 0; x < (temp1 - 24) * 9; x++)// O número de Bips deve bater com o valor da temperatura decimal. Ex. oito bitps a temp está em 24.8 graus.
    {
      tone(pinBuzzer, 200 * ((temp1 - 24) * 10));
      delay((2 * tdelay));
      noTone(pinBuzzer);
      delay((1 * tdelay));
    }

  }

  // ###### ALARME TEMP CRITICA
  if (temp1 >= TEMPCRIT || temp2 >= TEMPCRIT) // SOA BIP CONTÍNUO SE ACIMA DE 25 GRAUS (não ok)
  {
    //ControleLEDs(2);
    Status(4);//Temp Alta Crítico

  }
  if (temp2 >= TEMPMAX - 1.0) // Se temperatura do reservatório estiver em 23 graus, resfriar
  {
    //
    Status(5);// Reservatorio muito quente
  }

}
// ################## INTERRUPÇÕES ################
void inctempo()
{
  Serial.print(".");
  //printAsterisco(1, 10);
  if (laser == LOW)
  {
    Serial.println("Incrementa Tempo");
    hora_agora += 1.0;
    hora_agora_gravada += 1.0;
    //lcd.setCursor(12, 1);
    //lcd.print("LON ");
  }
}
void lcdFluxo()
{
  lcd.setCursor(8, 0);
  lcd.print(fluxo);
  lcd.print("L/m ");
  delay(tdelay);
}
void medeFluxo()
{
  //executa a contagem de pulsos uma vez por segundo
  if ((millis() - tempo_antes) > 1000) {

    //desabilita a interrupcao para realizar a conversao do valor de pulsos
    detachInterrupt(INTERRUPCAO_SENSOR);

    //conversao do valor de pulsos para L/min
    fluxo = ((1000.0 / (millis() - tempo_antes)) * contador) / fator_calibracao;

    //exibicao do valor de fluxo

    Serial.print("Fluxo de: ");
    Serial.print(fluxo);
    Serial.println(" L/min");

    lcdFluxo();

    //calculo do volume em L passado pelo sensor
    volume = fluxo / 60;

    //armazenamento do volume
    volume_total += volume;

    //exibicao do valor de volume
    /*
      Serial.print("Volume: ");
      Serial.print(volume_total);
      Serial.println(" L");
      Serial.println();
    */
    Serial.println(millis());
    if (fluxo > 10)
    {
      printAsterisco(1, 10);
      noTone(pinBuzzer);
    }
    else
    {
      if (millis() > 10000)// evita alarme falso ao ligar equipamento, só aciona alarme após 20s
      {
        //imprimeFluxo();
        //Status(4);//Temp Alta Critico
        alarmeFluxo();
      }
    }
    //reinicializacao do contador de pulsos
    contador = 0;

    //atualizacao da variavel tempo_antes
    tempo_antes = millis();

    //contagem de pulsos do sensor
    attachInterrupt(INTERRUPCAO_SENSOR, contador_pulso, FALLING);

  }
  /*
    //logica para zerar a variavel volume_total
    if (digitalRead(PINO_BOTAO) == HIGH) {
      delay(30);
      if (digitalRead(PINO_BOTAO) == HIGH) {
        volume_total = 0;
      }
    }
  */
}
//funcao chamada pela interrupcao para contagem de pulsos
void contador_pulso() {
  contador++;
  //Serial.print(".");
}
void alarmeFluxo()
{
  Status(3);
}

void Status(int val)
{
  lcd.setCursor(8, 1);
  lcd.print("        ");
  lcd.setCursor(8, 1);

  switch (val)
  {
    case -1://BAIXA
      tdelay = 100;
      Serial.println("##### Alarme de Baixa Temperatura");
      lcd.print("  T.BAIXO");
      digitalWrite(pinReleQuente, HIGH);// desliga laser
      LEDAZ(1);
      LEDVD(0);
      LEDAM(0);
      LEDVM(0);
      bip();
      delay(tdelay);
      break;
    case 0://NORMAL
      digitalWrite(pinReleQuente, LOW);// libera laser
      tdelay = 100;
      LEDAZ(0);
      LEDVD(1);
      LEDAM(0);
      LEDVM(0);
      digitalWrite(pinBuzzer, LOW);
      noTone(pinBuzzer);
      delay(tdelay * 5);
      break;
    case 1://Erro Leitura
      Serial.println("##### Erro de Leitura Sensor de Temperatura");
      lcd.print("  ErrTemp");
      tone(pinBuzzer, 5000);
      delay(tdelay);
      LEDAZ(0);
      LEDVD(0);
      LEDAM(0);
      LEDVM(1);
      break;
    case 2://ALTO
      tdelay = 50;
      Serial.println("##### Temperatura ALTA");
      lcd.print("   T.ALTA");
      digitalWrite(pinReleQuente, LOW);// libera laser
      LEDAZ(0);
      LEDVD(0);
      LEDAM(1);
      LEDVM(0);
      tone(pinBuzzer, 4000);
      delay(2 * tdelay);
      break;
    case 3://Fluxo Baixo
      Serial.println("##### Fluxo BAIXO");
      //ast = 3; //força impressão do fluxo

      lcd.print("  F.BAIXO");
      digitalWrite(pinReleQuente, HIGH);// desliga laser
      lcdFluxo();
      delay(2000);
      LEDAZ(0);
      LEDVD(0);
      LEDAM(0);
      LEDVM(1);
      bipseno();
      //delay(500);
      break;
    case 4://Temp. Crítico
      Serial.println("##### CRITICO");
      lcd.print("  CRITICO");
      digitalWrite(pinReleQuente, HIGH);// desliga laser
      tdelay = 1;
      LEDAZ(0);
      LEDVD(0);
      LEDAM(0);
      LEDVM(1);
      bipseno();
      //delay(3000);
      break;
    case 5://Resfriar reservatorio
      Serial.println("##### RESERVATORIO QUENTE");
      lcd.print("<< GELO  ");
      digitalWrite(pinReleQuente, LOW);// liga laser
      tdelay = 1;
      LEDAZ(0);
      LEDVD(0);
      LEDAM(1);
      LEDVM(0);
      tone(pinBuzzer, 4000);
      delay(10 * tdelay);
      delay(1000);
      break;

  }
}
