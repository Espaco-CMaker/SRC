void handleRoot() {
  //digitalWrite(led, 1);
  char temp[800];

  if ( server.hasArg("txt") ) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }
}

void handleNotFound() {
  //digitalWrite(led, 1);
  String message = "Arquivo não encontrado\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
  digitalWrite(led, 0);
}
void serveInit()
{
  imprimeLog("Servidor Web UP");
  server.on("/", handleRoot);
  //server.on("/test.svg", drawGraph);
  server.on("/inline", []() {
    server.send(200, "text/plain", "Operacional");
  });
  server.onNotFound(handleNotFound);
  server.begin();
}
String hrHTML()
{
  // não testado
  //lpage += "<tr class='active'><td>";
  //lpage += "<hr>";
  //lpage += "</td><td>";
}
String linHTML(String lin1, String lin2, String lin3, String lin4 )
{
  String lpage = "";
  lpage += "<tr class='active'><td>";
  lpage += lin1;
  lpage += "</td><td>";
  lpage += lin2;
  lpage += "</td><td>";
  lpage += lin3;
  lpage += "</td><td>";
  lpage += lin4;
  lpage += "</td></tr>";
  return lpage;
}
String abretabelaHTML(String titulo1, String titulo2, String titulo3, String titulo4 )
{
  String lpage = "";
  lpage +=       "<table class='table'>";  // Tableau des relevés
  lpage +=         "<thead><tr><th>";
  lpage +=            titulo1;
  lpage +=          "</th><th>";
  lpage +=            titulo2;
  lpage +=          "</th><th>";
  lpage +=            titulo3;
  lpage +=          "</th><th>";
  lpage +=            titulo4;
  lpage +=          "</th></tr></thead>"; //Entête
  lpage +=         "<tbody>";  //
  return lpage;
}
String fechatabelaHTML()
{
  String lpage = "";
  lpage +=       "</tbody></table>";
  return lpage;
}
//--------------------------------------
String getPage() {
  String txt_h, txt_m, txt_check, ltxt;
  String ltmp;
  int lquant_dia = 0;
  atualizaPortas();
  atualizaHora();

  for (int l = 0; l <  MAXPROG; l++)
  {
    lquant_dia += dado.disparo[l][3];
  }

  sprintf(txt, "%2.2d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d", a->tm_mday, a->tm_mon + 1, a->tm_year + 1900, a->tm_hour, a->tm_min, a->tm_sec);
  String lpage = "<html lang='pt-br'><head><meta http-equiv='refresh' content='90' name='viewport' content='width=device-width, initial-scale=1'/><meta http-equiv='Content-Type' content='text/html;charset=utf-8'>";
  lpage += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'><script src='https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js'></script><script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
  lpage += "<title>";
  lpage += "Alimentador Automático";
  lpage += "</title>";
  lpage += "</head><body>";
  //lpage += "<div class='container-fluid'>";
  //lpage +=   "<div class='row'>";
  //lpage +=     "<div class='col-md-12'>";
  lpage +=       "<h1>Alimentador Automático</h1>";
  lpage += abretabelaHTML("Dado", "Valor", "Observação", "");
  lpage += linHTML("Nome do Dispositivo", nomedev, "Dados e versão do dispositivo", "");
  ltmp = WiFi.SSID().c_str();
  ltmp += " - MAC ";
  ltmp += WiFi.BSSIDstr().c_str();
  ltmp += " (";
  ltmp += itostr(WiFi.RSSI());
  ltmp += " dBm)";
  lpage += linHTML("Rede", ltmp , "Dados da rede Wi-fi", "");
  //lpage += linHTML("RSSI dBm",  , "", "");
  //lpage += linHTML("BSSIDstr", WiFi.BSSIDstr().c_str() , "", "");
  lpage += linHTML("IP/MACAdress", WiFi.localIP().toString() , imprimeMAC(), "");
  //lpage += linHTML("Hostname", WiFi.hostname().c_str() , "", "");
  lpage += linHTML("Hora do Dispositivo", txt, "", "");
  // Serial.printf("SSID: [%s]%s (%d dBm) Status:%d\n", WiFi.SSID().c_str(),WiFi.BSSIDstr().c_str(),WiFi.RSSI(),WiFi.status());

  if (a0 > corte_infra)
    ltmp = "<font color='green'><b>COM COMIDA</b></font>";
  else
    ltmp = "<font color='red'><b>SEM COMIDA</b></font>";
  lpage += linHTML("Reservatório", ltmp, "De acordo com o sensor pode assumir dois valores COM COMIDA ou SEM COMIDA", "");
  lpage += linHTML("Ração mês atual", itostr(dado.racao_mes) + "g", "Quantidade de ração por mês", "");
  lpage += linHTML("Ração mês anterior", itostr(dado.acionamentos_anterior) + "g", "Quantidade de ração no mês passado", "");
  lpage += linHTML("Quantidade por acionamento", itostr(PORCAO) + " g", "Quantidade de ração por acionamento. Depende do modelo do equipamento", "");
  lpage += linHTML("Total dia", itostr(lquant_dia) + " g", "Quantidade total de ração por dia de acordo com o agendamento", "");
  lpage += fechatabelaHTML();

  lpage += "<form action='/' method='POST' id='form'>";
  lpage += "<div class='form-group'>";


  lpage +=        abretabelaHTML("Parâmetros", "Dados", "Observações", " ");
  //lpage = lpage + linHTML("Quantidade", "<INPUT type='text' size='4' name='quantidade' value='" + itostr(dado.quantidade) + "'> gramas", "Quantidade de ração por acionamento", "");
  if (dado.bip == 1)
    ltmp = " <input type='Checkbox' id='chkbip' name='chkbip' value='ativo' class='form-check-input' checked> Som Ativado";
  else
    ltmp = " <input type='Checkbox' id='chkbip' name='chkbip' value='desativo' class='form-check-input' > Som Ativado";
  lpage = lpage + linHTML("Alerta Sonoro ", ltmp , "Para ativar o som deixe essa opção marcada.", "");
  
  ltmp = " <input type='Checkbox' id='zeratudo' name='zeratudo' value='zeratudo' class='form-check-input' > Zera Tudo";
  lpage = lpage + linHTML("Zera Valores ", ltmp , "Esta opção zera todos os valores.", "");

  /*
    if (dado.logicaInvertidaSensor == true)
    ltmp = " <input type='Checkbox' id='chklogica' name='chklogica' value='ativo' class='form-check-input' checked> Lógica Invertida";
    else
    ltmp = " <input type='Checkbox' id='chklogica' name='chklogica' value='desativo' class='form-check-input' > Lógica Invertida";
    lpage = lpage + linHTML("Lógica Invertida", ltmp , "Ao ativar este recurso o sensor de nível trabalhará invertido.", "");
  */

  lpage +=        fechatabelaHTML();

  // ################## Tabela de Horários
  lpage +=        abretabelaHTML("Acionamentos - Hora:Minuto ", "", "", " ");

  for (int l = 0; l <  MAXPROG; l++)
  {
    txt_h = "<INPUT type = 'text' size = '3' name = 'hora" + itostr(l) + "' value = '" + itostr(dado.disparo[l][0]) + "'> ";
    txt_m = "<INPUT type = 'text' size = '3' name = 'min"  + itostr(l) + "' value = '" + itostr(dado.disparo[l][1]) + "'> ";
    //txt_check ="";
    txt_check = "<select  id='quant' name = 'quant" + itostr(l) + "'>"; // value='" + itostr(l) + "'>";
    //txt_check += "<option>DESATIVADO</option>";
    for (int i = 0; i < 10; i++)// quantidade de opções de pesos
    {
      if (dado.disparo[l][3] == i * PORCAO)
        txt_check += "<option selected>" + itostr(i * PORCAO) + " gramas</option>";
      else
        txt_check += "<option>" + itostr(i * PORCAO) + " gramas </option>";
    }
    txt_check += "</select>";

    switch (dado.disparo[l][2])
    {
      case -1:// DESATIVADO
        txt_check += " - <button type = 'button submit' name = 'estado' value = 'desativado' class = 'btn btn-secondary'>DESATIVADO </button>";
        break;//checked
      case 0://AGENDADO
        txt_check += " - <button type = 'button submit' name = 'estado' value = 'agendado'  class = 'btn btn-warning'>AGENDADO </button> ";
        break;
      case 1://EXECUTADO
        txt_check += " - <button type = 'button submit' name = 'estado' value = 'success' class = 'btn btn-success'>EXECUTADO </button> ";
        break;
      case 2://SEMCOMIDA
        txt_check += " - <button type = 'button submit' name = 'estado' value = 'semcomida' class = 'btn btn-danger'>EXECUTADO SEM COMIDA </button> ";
        break;
    }
    lpage +=      linHTML("Hora Programada " + itostr(l + 1) + " - " + txt_h + ": " + txt_m + " - " + txt_check, "", "", "" );
  }
  lpage +=        fechatabelaHTML();
  // ################## Tabela de Horários FECHA

  lpage +=        abretabelaHTML("Campo de Observações (" + itostr(MAXBUF) + " caracteres)", "", "", "");
  lpage +=        linHTML("<textarea maxlength = '" + itostr(MAXBUF) + "' form = 'form' class = 'form-control rounded-0' type = 'text' id = 'txt' name = 'txt' width = '100%' rows = '4' wrap = 'hard'>" + convertToString(dado.texto) + " </textarea> ", "" , "", "");
  lpage +=        linHTML("<INPUT type = 'submit' class = 'btn btn-primary' value = 'GRAVAR ALTERAÇÕES'>", "", "", "");
  lpage +=        fechatabelaHTML();
  lpage +=        " </div>";
  lpage +=        " </form>";

  // imprimeLogHTML()
  lpage +=        abretabelaHTML("Log de Eventos", "", "", "");
  lpage +=        linHTML(imprimeLogHTML(), "", "", "");
  lpage +=        fechatabelaHTML();
  //lpage +=        "</div>";
  lpage +=        "</body></html>";

  return lpage;
}

void handleSubmit()
{
  struct dataPROMConfig temp;
  bool mudou = false;
  String txt_h;
  String txt_m;
  String txt_q;

  //temp.acionamentos = server.arg("acionamentos").toInt();
  //temp.quantidade = server.arg("quantidade").toInt();

  // ### SOM
  if (server.arg("chkbip") == "desativo")
    temp.bip = 1;
  if (server.arg("chkbip") == "ativo")
    temp.bip = 0;
  if ((temp.bip != dado.bip))
  {
    dado.bip = temp.bip;
    mudou = true;
  }
  if (server.arg("zeratudo") == "zeratudo")
    zeraVars();



    // ### Inversão de Nível
    /*
      if(server.arg("chklogica")=="desativo")
      temp.logicaInvertidaSensor=1;
      if(server.arg("chklogica")=="ativo")
      temp.logicaInvertidaSensor=0;
      if ((temp.logicaInvertidaSensor != dado.logicaInvertidaSensor))
      {
      dado.logicaInvertidaSensor = temp.logicaInvertidaSensor;
      mudou = true;
      }
    */



    //Serial.printf("BIP:%d [%s]\n", temp.bip, server.arg("chkbip").c_str());
    //temp.intervalos = erver.arg("intervalo").toInt();

    // ### Text Area
    strcpy(temp.texto, server.arg("txt").c_str());// Area de Texto
  if ((temp.texto[0] != 0) && (strcmp(temp.texto, dado.texto) != 0))
  {
    strncpy(dado.texto, temp.texto, MAXBUF);
    //Serial.println("4");
    mudou = true;
  }

  // Tabela de Horarios e dosagens
  for (int l = 0; l < MAXPROG; l++)
  {
    txt_h = "hora" + itostr(l);
    txt_m = "min" + itostr(l);
    txt_q = "quant" + itostr(l);

    temp.disparo[l][0] = server.arg(txt_h).toInt();
    temp.disparo[l][1] = server.arg(txt_m).toInt();
    temp.disparo[l][3] = server.arg(txt_q).toInt();

    if (temp.disparo[l][3] == 0)
      temp.disparo[l][2] = DESATIVADO;
    if (temp.disparo[l][3] >= 1)
      temp.disparo[l][2] = AGENDADO;

    //Serial.println(temp.texto);

    //Serial.printf("Temp:[%d]%d:%d status:%d aciona:%d\n", l, temp.disparo[l][0], temp.disparo[l][1], temp.disparo[l][2], temp.disparo[l][3]);
    //Serial.printf("Dado:[%d]%d:%d status:%d aciona:%d\n", l, dado.disparo[l][0], dado.disparo[l][1], dado.disparo[l][2], dado.disparo[l][3]);
    // Não é vazio e houve mudança
    if ((server.arg(txt_h).c_str() != "") && (temp.disparo[l][0] != dado.disparo[l][0]) || (server.arg(txt_m).c_str() != "") && (temp.disparo[l][1] != dado.disparo[l][1]) || (temp.disparo[l][3] != dado.disparo[l][3]))
    {
      Serial.printf("Alteração detectada\n");
      //#define DESATIVADO -1
      //#define AGENDADO 0
      //#define EXECUTADO 1
      //#define SEMCOMIDA 2

      // se for inserido -1 desativa

      //else // efetua o agendamento

      dado.disparo[l][0] = temp.disparo[l][0];
      dado.disparo[l][1] = temp.disparo[l][1];
      dado.disparo[l][2] = temp.disparo[l][2];
      dado.disparo[l][3] = temp.disparo[l][3];
      mudou = true;
    }
  }
  if (mudou)
    atualizaEEPROM();
  server.send ( 200, "text / html", getPage() );
}
