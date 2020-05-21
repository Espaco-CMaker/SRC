WiFiManager wifiManager;
//===============================================================
// Autor: Fabio Bettio
// Data: 04/2020
// Descrição: Conecta Wifi
// Entradas: String nomedev
// Saídas:
// Bibliotecas:#include <WiFiManager.h>
// https://github.com/tzapu/WiFiManager
//  WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
//  WL_IDLE_STATUS      = 0,
//  WL_NO_SSID_AVAIL    = 1,
//  WL_SCAN_COMPLETED   = 2,
//  WL_CONNECTED        = 3,
//  WL_CONNECT_FAILED   = 4,
//  WL_CONNECTION_LOST  = 5,
//  WL_DISCONNECTED     = 6
//===============================================================
void conectaWifi()
{
  //wifiManager.resetSettings();
  if (DEBUG > 3)
    wifiManager.setDebugOutput(true);
  else
    wifiManager.setDebugOutput(false);

  if (!wifiManager.autoConnect("CMaker")) {
    Serial.println("#### Falhou para conectar. Resetando...");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  imprimeInfoRede();

}
void testaConexao2()
{
  int i = 0;
  //Serial.println(WiFi.status());
  //Serial.println(WL_CONNECTED);
  while (!wifiManager.autoConnect(nomedev.c_str())) {//3 - WL_CONNECTED
    delay(500);
    // Serial.print(".");
    snprintf(txt, MAXBUF, "(%d) Tentando conexão com wifi... ", i);
    Serial.println(txt);
    i++;
    if (i >= 20) // Não conseguiu se logar no wifi, tentando reset...
    {
      Serial.println("Resetando...");
      ESP.restart();
    }
  }
}
void testaConexao()
{
  // ### Perdeu conexão Wifi
  if (( WiFi.localIP().toString() == "0.0.0.0") || WiFi.status() != WL_CONNECTED)
  {
    imprimeInfoRede();
    bip(10, 500); // perdeu conexão wifi
    delay(3000);
    ESP.restart();
  }
}
void restaTudo()
{
  Serial.println("Hard Reset");
  zeraEEPROM();
  wifiManager.resetSettings();
  delay(3000);
  ESP.restart();
}
/*
   Reset do Wifi no caso de perda de IP
  int i = 1;
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  while (WiFi.status() != WL_CONNECTED) {//
    delay(500);
    // Serial.print(".");
    snprintf(temp, MAXBUF, "(%d) Tentando conexão com wifi... (%s)", i, ssid);
    Serial.println(temp);
    i++;
    if (i >= 20) // Não conseguiu se logar no wifi, tentando reset...
    {
      Serial.println("Resetando...");
      ESP.restart();
    }
  }
*/
