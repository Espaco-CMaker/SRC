
//lpage += "<div class='container-fluid'>";
//lpage +=   "<div class='row'>";
//lpage +=     "<div class='col-md-12'>";
/*
lpage +=        abretabelaHTML("Parâmetros", "Dados", "Observações", "                       ");

  ltmp = "        <select  id='quant' name='quantidade' value='" + itostr(dado.quantidade) + "'>";
  for (int val = PORCAO; val <= 1000; val += PORCAO)
  {
    if (dado.quantidade == val)
      ltmp = ltmp + " <option selected>" + val + "</option>";
    else
      ltmp = ltmp + " <option>" + val + "</option>";
  }
  ltmp += "       </select>";

  lpage +=        linHTML("Quantidade diária", ltmp , "Quantidade de ração por dia, este número é multiplo da 'Quantidade por acionamento'", "");
  lpage +=        fechatabelaHTML();

*/


void incrementaTempo()
{
  //agora.s=millis();
  //agora.s_old = agora.s;// usado para imprimir só quando houver mudanças;
  //agora.s = (int) (millis() / 1000) % 60 ;
  //agora.m = (int) ((millis() / (1000 * 60)) % 60);
  //agora.h   = (int) ((millis() / (1000 * 60 * 60)) % 24);


  // while(!(millis()%1000));//pausa 1 segundo
  //agora.s ++;
  /*
    if (agora.s >= 60)
    {
    agora.s = 0;
    agora.m++;
    }
    if (agora.m >= 60)
    {
    agora.m = 0;
    agora.s = 0;
    agora.h++;
    }
  */
  ///  if (agora.h >= 24)
  //  {
  //    agora.s = 0;
  //    agora.m = 0;
  //    agora.h = 0;
  //   agora.d++;
  //  }
  //Serial.println(agora.s);
  //  if (agora.s_old != agora.s)
  //  imprimeHora();
}
void OLDconectaWifi()
{

  WiFiManager wifiManager;
  wifiManager.autoConnect("BettechESP");
  Serial.println("Conectado! :)");
  Serial.println(WiFi.localIP());
  imprimeLog("Wi-fi conectado");
}
/*  nomedev += " > ";
   nomedev += __FILE__;
   nomedev += " diz: Estou vivo!";
   //DateTime.sync(0); // start the clock
   Serial.println();
   for (int i = 0; i <= nomedev.length(); i++)
   Serial.print("=");
   Serial.println();
   Serial.println(nomedev);
   //Serial.println();
*/
//testeTempo();

//imprimePortas();
//leEEPROM();
//imprimeStatus();

//imprimeHora();

//imprimeDebug("loop",__FILE__,__FUNCTION__,__LINE__);
//delay(1000);
//imprimeTempoLong(millis());
//imprimeStatus();
//if (a0 > 500)
//  acionaMotor();
//  incrementaTempo();
// atualizaTempo(ativo);
//imprimeTempoAtivo();
//imprimeTempo(ini);
// if (testaDisparo(ini, 1000))
// imprimeTempo(ini);
//ligaLed(pinLed);
//delay(500);
//desligaLed(pinLed);
//delay(500);


//__FILE__, __FUNCTION__, __LINE__.

void temp()
{
  //attachInterrupt(&piscaLed, 1000);

  // Portas
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

}

void drawGraph() {
  String out = "";
  int y = 0;
  int y2;
  int x_max = 390;

  char temp[100];
  out += " < svg xmlns = \"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  //int y = rand() % 130;

  if (d0 == 1)//https://www.w3schools.com/graphics/svg_circle.asp
    out += " <circle cx=\"50\" cy=\"50\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" />";
  else
    out += " <circle cx=\"50\" cy=\"50\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"blue\" />";
  out += temp;
  for (int x = 10; x < x_max; x += 10) {
    //int y2 = rand() % 130;
    y2 = (d0 * 100);
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);

    out += temp;
    y = y2;
  }

  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}
