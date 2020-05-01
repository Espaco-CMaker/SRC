WiFiManager wifiManager;
//===============================================================
// Autor: Fabio Bettio
// Data: 04/2020
// Descrição: Conecta Wifi
// Entradas: String nomedev
// Saídas:
// Bibliotecas:#include <WiFiManager.h> 
// https://github.com/tzapu/WiFiManager
//===============================================================
void conectaWifi()
{
  //wifiManager.resetSettings();
  if (DEBUG > 3)
    wifiManager.setDebugOutput(true);
  else
    wifiManager.setDebugOutput(false);

  if (!wifiManager.autoConnect(nomedev.c_str())) {
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
  while (!wifiManager.autoConnect("BettechESP")) {//3 - WL_CONNECTED
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
  if ( WiFi.localIP().toString() == "0.0.0.0")
  {
    imprimeInfoRede();
    bip(10, 500); // perdeu conexão wifi
    delay(3000);
    ESP.restart();
  }
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
