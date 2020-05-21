// ========================================
// Rotinas de ativação dos perifericos (porta, alarme e portão)
// ========================================
//String ctostr(char *a);
//String convertToString(char* a)
//String itostr(int x);
//void atualizaStatus();
//===============================================================
// Autor: Fabio Bettio
// Data: 07/2019
// Descrição: le Temperatura LM35
// Entradas:
// Saidas:
//===============================================================

// Variáveis Globais
unsigned int seg_old;


void configFabrica()
{
  int hwreset = false;
  while (digitalRead(pinReset) == LOW)
  {
    Serial.printf("EM RESET (%d)\n", digitalRead(pinReset));
    //ESP.wdtFeed();
    hwreset = true;
    delay(1);
  }
  if (hwreset == true)
    zeraEEPROM();
}

void dispara()// dentro da interrupt
{
  Serial.println("Estou vivo!!! Interrupção");
  atualizaHora();
  imprimeStatusSerial();
}

void zeraVars()
{
  /*
    bool primeiroboot = true;
    int64_t racao_mes;// = -1;
    int64_t acionamentos;// = -1; // numero de acionamentos;
    int64_t acionamentos_anterior;// = -1; // numero de acionamentos mês anterior;
    long intervalos;// = -1; // intervalo entre acionamento (segundos)
    int quantidade;// = -1;
    int disparo[MAXPROG][MAXCOL];
    char texto[MAXBUF];
    int antipombo = 0;
    //Colunas... [0] Hora [1] Minuto [2] Status [3] numero da acionamentos
    //#define AGENDADO 0
    //#define EXECUTADO 1
    //#define SEMCOMIDA 2
    int bip = 0;
    int logicaInvertidaSensor = true;
  */
  dado.primeiroboot = false;
  dado.racao_mes = 0;
  dado.acionamentos = 0;
  dado.acionamentos_anterior = 0;
  dado.intervalos = 0;
  dado.quantidade = 0;

  for (int l = 0; l < MAXPROG; l++)
    for (int c = 0; c <= MAXCOL ; c++)
      dado.disparo[l][c] = -1;

  memset(dado.texto, 0, MAXBUF);
  dado.antipombo = 0;
  dado.bip = 0;
  //dado.logicaInvertidaSensor = 0;
  memset(&dado, 0,sizeof(dado));
  imprimeLog("\n***********************************\nPrimeiro BOOT, variáveis zeradas, sistema pronto\n***********************************\n");
}
void nivelComida()
{

}

String itostr(int x)
{
  char laux[50];
  itoa(x, laux, 10);
  return laux;
}
String ctostr(char *a)
{
  int i;
  String s = "";
  int a_size = 0;
  while (*a != '\0')
  {
    s = s + a[i];
    a_size++;
    if (a_size > 500)
    {
      a_size = -1;
      Serial.print("#### Erro - não encontrado caracter fim de string!!");
      break;
    }
    a++;
  }

  //Serial.print("#### a_size:");
  //Serial.println(a_size);
  /*
    for (i = 0; i < a_size; i++) {

    }
  */
  return s;
}

String convertToString(char* a)
{
  String s = a;
  return s;
}
void FiFoLog(String l)
{
  //ultimosLogs[MAXLOG]
  for (int i = MAXLOG - 1; i >= 0; i--)
  {
    ultimosLogs[i] = ultimosLogs[i - 1];
  }
  ultimosLogs[0] = l;
}
