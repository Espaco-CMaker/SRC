void checaHora()
{
  atualizaHora();
  //Serial.printf("Agora:%d:%d:%d - ROT:%d\nAgendamentos\n", a->tm_hour,  a->tm_min,  a->tm_sec, acionaRotor);
  for (int l = 0; l < MAXPROG; l++)
  {
    // Faxia de 5min entre cada acionamento
    if ((dado.disparo[l][0] == a->tm_hour) && (dado.disparo[l][1] == a->tm_min) && (acionaRotor == -1) && (dado.disparo[l][2] == AGENDADO)) //, a->tm_min;
    {
      //#define AGENDADO 0
      //#define EXECUTADO 1
      //#define SEMCOMIDA 2
      if (a0 > corte_infra)// Com comida
        dado.disparo[l][2] = EXECUTADO;// EXECUTADO
      else
      {
        dado.disparo[l][2] = SEMCOMIDA;// SEMCOMIDA
        imprimeLog("##### SEM COMIDA #####");
      }
      acionaRotor =  l;
      Serial.printf("%d:%d - %d acionaRotor=%d\n", dado.disparo[l][0], dado.disparo[l][1], dado.disparo[l][2],acionaRotor);
      //acionamentoArmado = false;
    }
    //Serial.printf("%d:%d - %d\n", dado.disparo[l][0], dado.disparo[l][1], dado.disparo[l][2]);
  }
  if (a->tm_hour == 0 && a->tm_min == 0)// meia noite tudo vira AGENDADO
    zeraAcionamentos();
  if (a->tm_mday == 1 && a->tm_hour == 0 && a->tm_min == 0 && dado.racao_mes != 0) // Inicio do Mês
    zeraRacaoMes();

}
void zeraRacaoMes()
{
  dado.acionamentos_anterior = dado.racao_mes;
  dado.racao_mes = 0;
  atualizaEEPROM();
}
void zeraAcionamentos()
{
  bool latualizaEEPROM = false; // bloqueia a multiplas execuções em uma memso minuto, não permite gravar se não teve mudança
  for (int l = 0; l < 5; l++)
  {
    if (dado.disparo[l][2] == EXECUTADO || dado.disparo[l][2] == SEMCOMIDA)
    {
      dado.disparo[l][2] = AGENDADO;
      latualizaEEPROM = true;
    }
  }
  if (latualizaEEPROM == true)
    atualizaEEPROM();
}
void testeTempo()
{
  char buff[20];
  struct tm *sTm;
  //time_t now;

  time(&agora_t);

  //sprintf(buff, "%s", ctime(&agora_t)); // use ctime to format time to a string.
  //Serial.println(buff);

  //Serial.println("==============>1");
  agora_t = time (0);
  //gmtime_s (&now, &sTm);
  //Serial.println("==============>2");
  sTm = gmtime (&agora_t);
  //Serial.println("==============>3");
  Serial.println(agora_t);
  Serial.println("==============>4");
  strftime (buff, sizeof(buff), ">>>%Y-%m-%d %H:%M:%S", sTm);
  //sprintf(buff, "%s %s\n",buff , "Event occurred now");
  Serial.println(buff);
}


WiFiUDP udp;//Cria um objeto "UDP".
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);//Cria um objeto "NTP" com as configurações.

void atualizaHora()
{
  //ntp.update();
  unsigned long epochTime = ntp.getEpochTime();
  //update() or forceupdate()
  //ntp.forceUpdate();//Força o Update.
  a = gmtime ((time_t *)&epochTime);
}
void ntpInicio()
{
  char hora_ntp[100];
  uint32_t now = millis();
  //pool.ntp.org
  //time.windows.com
  //a.st1.ntp.br
  //a.ntp.br
  do
  {
    ntp.begin();//Inicia o NTP.
    ntp.forceUpdate();//Força o Update.
    unsigned long epochTime = ntp.getEpochTime();
    a = gmtime ((time_t *)&epochTime);;
  } while (a->tm_year + 1900 < 2000);
}
