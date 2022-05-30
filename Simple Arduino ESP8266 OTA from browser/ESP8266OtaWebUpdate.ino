// I use for my toys simple OTA through a browser.
// When using port mapping on router, it works from anywhere.
// Here is the entire code that is necessary for that.
// The uploaded file must, of course, be in binary form as *.bin
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
const char* ssid =      "SSID";     // Set your router SSID
const char* password =  "PASS"; // Set your router password
#define OTAUSER         "admin"    // Set OTA user
#define OTAPASSWORD     "admin"    // Set OTA password
#define OTAPATH         "/firmware"// Set path for update
#define SERVERPORT      80         // Server port
ESP8266WebServer HttpServer(SERVERPORT);
ESP8266HTTPUpdateServer httpUpdater;
//-----------------------------------------------------------------
void setup(void) {
  Serial.begin(115200);
  WiFi.hostname("ESP");
  WiFi.begin(ssid, password);
  /* wait for WiFi connect */
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  String hostname = WiFi.hostname();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP Mac Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("The hostname is ");
  Serial.println(hostname);  
  
  httpUpdater.setup(&HttpServer, OTAPATH, OTAUSER, OTAPASSWORD);
  HttpServer.onNotFound(handleNotFound);
  HttpServer.begin();
}
void loop(void) {
  HttpServer.handleClient();       // Listen for HTTP requests from clients
  // ...
  // ...
}
/* Send HTTP status 404 Not Found */
void handleNotFound() {
  HttpServer.send(404, "text/plain", "404: Not found");
}