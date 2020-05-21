//imprimeLog("Setup");


void imprimeLog(char *txt)
{
  char b[500];

  atualizaHora();
  // texto += " > ";
  //nomedev
  Serial.println("============================================");
  sprintf(b, "%2.2d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d %s ", a->tm_mday, a->tm_mon + 1, a->tm_year + 1900, a->tm_hour, a->tm_min, a->tm_sec, txt );
  Serial.print(b);
  Serial.println(WiFi.localIP());
  FiFoLog(b);// usado no HTML
  //Serial.println("=============================");
  if (a->tm_year + 1900 < 2000)
  {
    Serial.println("#### Data inválida!");
    //while(1);
  }

}
void imprimeLogSerial(char *txt)
{
  char b[1000];

  atualizaHora();
  // texto += " > ";
  //nomedev
  Serial.println("============================================");
  sprintf(b, "%2.2d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d %s ", a->tm_mday, a->tm_mon + 1, a->tm_year + 1900, a->tm_hour, a->tm_min, a->tm_sec, txt );
  Serial.print(b);
  Serial.println(WiFi.localIP());
  //FiFoLog(b);
  //Serial.println("=============================");
  if (a->tm_year + 1900 < 2000)
  {
    Serial.println("#### Data inválida!");
    //while(1);
  }

}
//imprimeDebug("Setup",__FILE__,__FUNCTION__,__LINE__);
void imprimeDebug(char *txt, String file, String func, int line)
{
  String texto;
  char b[500];

  atualizaHora();
  // texto += " > ";
  //nomedev
  texto += nomedev;
  texto += "\nArq:";
  texto += file;
  texto += " ";
  texto += __DATE__;
  texto += " ";
  texto += __TIME__;
  texto += "\nFunc:";
  texto += func;
  //texto += "\nFunc2:";
  //texto += __PRETTY_FUNCTION__;// Carsh!!!!!!!!
  //texto += " (L:";
  //texto += line;
  // texto += ")";
  //DateTime.sync(0); // start the clock
  Serial.println("\n=============================");
  sprintf(b, "%s\n%2.2d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d \n%s", texto.c_str(), a->tm_mday, a->tm_mon + 1, a->tm_year + 1900, a->tm_hour, a->tm_min, a->tm_sec, txt );
  Serial.println(b);
  Serial.println(WiFi.localIP());
  Serial.println("=============================\n");
  if (a->tm_year + 1900 < 2000)
  {
    Serial.println("##############################\nData errada!");
    //while(1);
  }

}
void imprimeStatus()
{
  char ltxt[MAXBUF];
  //char ltxt2[MAXBUF];
  atualizaHora();
  //sprintf(ltxt, "\nStatus Serial: Aciona.:%d \nInter.:%d \nQuant.:%d \nQuan.Mes:%d\nBoot:%d\nBip:%d\nNivel Ração:%d\n", dado.acionamentos, dado.intervalos, dado.quantidade, dado.racao_mes,  dado.primeiroboot,dado.bip,a0);
  sprintf(ltxt, "\nStatus: Boot:%d\nBip:%d\nNivel Ração:%d\n",  dado.primeiroboot,dado.bip,a0);
  //imprimeDebug("Setup",__FILE__,__FUNCTION__,__LINE__);
  imprimeLog(ltxt);
  //Serial.println(ltxt);
}
void imprimeStatusSerial()
{
  char ltxt[MAXBUF];
  //char ltxt2[MAXBUF];
  //char ltxt3[MAXBUF];
  atualizaHora();
  atualizaPortas();
  //sprintf(ltxt, "\nStatus Serial: Boot:%d\nBip:%d\nInversão:%d\nNivel Ração:%d\n",  dado.primeiroboot,dado.bip,dado.logicaInvertidaSensor,a0);
  sprintf(ltxt, "\nStatus Serial: Aciona.:%d \nInter.:%d \nQuant.:%d \nQuan.Mes:%d\nBoot:%d\nBip:%d\nNivel Ração:%d\n", dado.acionamentos, dado.intervalos, dado.quantidade, dado.racao_mes, dado.primeiroboot,dado.bip,a0);
  //imprimeDebug("Setup",__FILE__,__FUNCTION__,__LINE__);
  for (int l = 0; l < MAXPROG; l++)
    sprintf(ltxt, "%s %2.2d:%2.2d (%d)\n",ltxt, dado.disparo[l][0], dado.disparo[l][1], dado.disparo[l][2]);
  //sprintf(ltxt3, "%s\n%s\n", ltxt,ltxt2);
  //Serial.println(ltxt);
  imprimeLogSerial(ltxt);

}
void imprimeHora()
{
  atualizaHora();
  sprintf(txt, "%2.2d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d\n", a->tm_mday, a->tm_mon + 1, a->tm_year + 1900, a->tm_hour, a->tm_min, a->tm_sec);
  Serial.println(txt);
  //sprintf(txt, "[%d] %2.2d:%2.2d:%2. 2d", t.d, t.h, t.m, t.s);
  //Serial.println(txt);
}
void imprimeTempoLong(long t)
{
  char ltxt_old[MAXBUF];
  unsigned int segundo = (t / 1000) % 60;
  unsigned int minuto = (t / (1000 * 60)) % 60;
  unsigned int hora =  (t / (1000 * 60 * 60)) % 24;
  unsigned int dia =  (t / (1000 * 60 * 60)) / 24;
  strcpy(ltxt_old, txt);
  sprintf(txt, "[%d] %2.2d:%2.2d:%2.2d", dia, hora, minuto, segundo);
  if (strcmp(ltxt_old, txt) != 0)
    Serial.println(txt);
}

/*
  agora.s = (int) (millis() / 1000) % 60 ;
  agora.m = (int) ((millis() / (1000 * 60)) % 60);
  agora.h   = (int) ((millis() / (1000 * 60 * 60)) % 24);

*/
void imprimeTempoAtivo()
{
  char txt_old[MAXBUF];
  //incrementaTempo(t);
  strcpy(txt_old, txt);

  //if (strcmp(txt_old, txt) != 0)
  //sprintf(txt, "[%d] %2.2d:%2.2d:%2.2d", ativo.d, ativo.h, ativo.m, ativo.s);

  if (strcmp(txt, txt_old) >= 1 )
    Serial.println(txt);

  //t.s_old = t.s;
  //strncpy(txt_old, txt, MAXBUF);
}

void imprimePortas()
{

  sprintf(txt, "D0:%d D1:%d D2:%d A0:%d", d0, d1, d2, a0);
  Serial.println(txt);
}
void imprimeInfoRede()
{
  
  byte numSsid = WiFi.scanNetworks();

  // print the list of networks seen:
  Serial.println("============================================");
  Serial.print("Lista de Wi-fi disponíveis:");
  Serial.println(numSsid);
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") Redes: ");
    Serial.printf("%s (%d dBm)\n", WiFi.SSID(thisNet).c_str(), WiFi.RSSI());
  }
  WiFi.hostname("teste");
  MDNS.begin("teste");
  Serial.println("----------------------");
  //Serial.println(wifiManager.getConfigPortalSSID());
  Serial.print("IP:");
  Serial.println(WiFi.localIP());
  Serial.println(imprimeMAC());
  //Serial.printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
  Serial.printf("Hostname: %s\n", WiFi.hostname().c_str());
  Serial.printf("SSID: [%s]%s (%d dBm) Status:%d\n", WiFi.SSID().c_str(), WiFi.BSSIDstr().c_str(), WiFi.RSSI(), WiFi.status());
  //WiFi.psk();
  Serial.println("============================================");
  //WiFi.hostname("Station_Tester_02");// Muda no nome do Host

}
String imprimeMAC()
{
  char txt[100];
  uint8_t macAddr[6];
  WiFi.macAddress(macAddr);
  sprintf(txt, "%02X:%02X:%02X:%02X:%02X:%02X", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
  //Serial.println(txt);
  return ctostr(txt);
}
String imprimeLogHTML()
{
  String retorno = "";
  //ultimosLogs[MAXLOG]
  for (int i = 0; i < MAXLOG; i++)
  {
    retorno = retorno + "[" + i + "] " + ultimosLogs[i] + "<br>";
  }
  return retorno;

}
