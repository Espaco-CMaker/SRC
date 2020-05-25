/*
 * Este sketch não usa conexão criptografada. Suas credenciais poderão ser interceptadas
 * Use uma conta de email descartável. E não use para nada que seja sigiloso.
 * Jefferson Ryan - automalabs.com.br - julho 2017
 *  
 */

 
#include <Ticker.h>
#include <ESP8266WiFi.h>

// itens a preencher ================================================
// Configure o resto em AlarmeON() e AlarmeOFF() 
const char* SSID = "";
const char* PASS = "";
//$mail->SMTPAuth = true; 
//$mail->SMTPSecure = "ssl"; 
//$mail->Host = "smtp.gmail.com";
//$mail->Port  = 465;

char smtp_server[] = "smtp.gmail.com";
//esta tem que ser a porta do seu provedor que aceita conexões sem criptografia
const int smtp_port = 587; //pode ser 25, 2525 587, 465, etc. Veja com seu provedor de email.
const String mail_username="fgbettio@gmail.com";
const String mail_password="****";
const String mail_from="<fgbettio@gmail.com>";
const String mail_to="<fgbettio@yahoo.com.br>";
String mail_subject="Alimentador Ativo"; //Preencha em AlarmeON() e AlarmeOFF()
String mail_line1="Ativo"; //Preencha em AlarmeON() e AlarmeOFF()
String mail_line2=""; //Preencha em AlarmeON() e AlarmeOFF()
String mail_line3=""; //Preencha em AlarmeON() e AlarmeOFF()

const int sensorPin = 0; //0=GPIO zero
const int intervalo=15; //em minutos
//=====================================================================


const char* _b64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

WiFiClient client;

void a3_to_a4(unsigned char * a4, unsigned char * a3) {
  a4[0] = (a3[0] & 0xfc) >> 2;
  a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
  a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
  a4[3] = (a3[2] & 0x3f);
}

int base64_encode(char *output, const char *input, int inputLen) {
  int i = 0, j = 0;
  int encLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];
  while(inputLen--) {
    a3[i++] = *(input++);
    if(i == 3) {
      a3_to_a4(a4, a3);
      for(i = 0; i < 4; i++) {
        output[encLen++] = _b64_alphabet[a4[i]];
      }
      i = 0;
    }
  }
  if(i) {
    for(j = i; j < 3; j++) {
      a3[j] = '\0';
    }
    a3_to_a4(a4, a3);
    for(j = 0; j < i + 1; j++) {
      output[encLen++] = _b64_alphabet[a4[j]];
    }
    while((i++ < 3)) {
      output[encLen++] = '=';
    }
  }
  output[encLen] = '\0';
  return encLen;
}

void setup1()
{
  Serial.begin(115200);
  delay(10);
  Serial.println("");
  Serial.println("");
  Serial.print("Connecting To ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  byte ret = sendEmail();
}

void loop1()
{
}

byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;

  if (client.connect(smtp_server, smtp_port) == 1) {
    Serial.println("connected");
  } else {
    Serial.println("connection failed");
    return 0;
  }
  if (!eRcv()) return 0;

    char* buf = (char*)malloc(100);

  Serial.println("Sending EHLO");
  client.print("EHLO ");
  client.println(smtp_server); 
  if (!eRcv()) return 0;
  Serial.println("Sending auth login");
  client.println("auth login");
  if (!eRcv()) return 0;
  Serial.println("Sending User");
  base64_encode(buf, mail_username.c_str(), mail_username.length());
  client.println(buf);  
  if (!eRcv()) return 0;
  
  Serial.println("Sending Password");
  base64_encode(buf, mail_password.c_str(), mail_password.length());
  client.println(buf);   
  if (!eRcv()) return 0;
  
  Serial.println("Sending From");
  // change to your email address (sender)
  client.print("MAIL From: ");
  client.println(mail_from);
  if (!eRcv()) return 0;
  // change to recipient address
  Serial.println("Sending To");
  client.print("RCPT To: ");
  client.println(mail_to);
  if (!eRcv()) return 0;
  Serial.println("Sending DATA");
  client.println("DATA");
  if (!eRcv()) return 0;
  Serial.println("Sending email");
  // change to recipient address
  client.print("To: ");
  client.println(mail_to);
  // change to your address
  client.print("From: "); 
  client.println(mail_from); //Este é o "nome para exibição"
  client.print("Subject: ");
  client.print(mail_subject);
  client.println("\r\n");    
  client.println(mail_line1);
  client.println(mail_line2);
  client.println(mail_line3);
  client.println("."); //O email tem que terminar assim
  if (!eRcv()) return 0;
  Serial.println("Sending QUIT");
  client.println("QUIT");
  if (!eRcv()) return 0;
  client.stop();
  Serial.println("disconnected");
  return 1;
}

byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;

  while (!client.available()) {
    delay(1);
    loopCount++;
    // if nothing received for 10 seconds, timeout
    if (loopCount > 10000) {
      client.stop();
      Serial.println("\r\nTimeout");
      return 0;
    }
  }

  respCode = client.peek();
  while (client.available())
  {
    thisByte = client.read();
    Serial.write(thisByte);
  }

  if (respCode >= '4')
  {
    //  efail();
    return 0;
  }
  return 1;
}
