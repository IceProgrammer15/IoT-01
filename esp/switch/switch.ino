
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// SOFTAP WIFI
const char *ssid = "IOT-TechShare";
const char *password = "12345678";
const String localDomain = "techshare"; // ie: http://techshare.local


const int inputPin = 2;
int currentValue = 0;

ESP8266WebServer server(80);

void setup() {
  // set baud rate 
  Serial.begin(115200);
  Serial.println();

  // read current state of the input pin (0 or 1)
  pinMode(inputPin, INPUT);
  currentValue = digitalRead(inputPin);
  
  Serial.print("WIFI Access point : ");
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  
  bool wifiAP = WiFi.softAP(ssid, password);      
  
  Serial.println(wifiAP ? "Ready" : "Failed!");
  
  //Start/mount filesystem
  LittleFS.begin();

  // Optional: setup DNS 
  if (MDNS.begin(localDomain)) {
    Serial.println("DNS started...");
  }

  startWebServer();

}


//Main Loop
void loop() {
  server.handleClient();  
  MDNS.update();
  delay(5);
  handleInputs();
}

//--------------------------------------------
void handleInputs(){
  int pinValue = digitalRead(inputPin);
  if(pinValue != currentValue){
    currentValue = pinValue;
      Serial.println("Input value changed to: "+String(pinValue));
      sendHttpRequest();
  }
  
}
//--------------------------------------------
void sendHttpRequest(){
  
// std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
 //client->setFingerprint(fingerprint);



WiFiClientSecure client;
  client.setInsecure();


  
  HTTPClient http;
  http.begin(client, "https://hooks.slack.com/services/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"); //HTTP
  http.addHeader("Content-Type", "application/json");
  
  String msg = currentValue == 1 ? "Someone snatched your coaster! :mag:" : "False alarm! the coaster is back!";

  String payload = "{\"username\": \"IOT-Device!\", \"text\": \"" + msg + "\", \"icon_emoji\": \":ghost:\"}";
  
  int httpCode = http.POST(payload);
  
  Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  Serial.println(httpCode);

  char sslErrorMsg[80];
  int sslError = client.getLastSSLError(sslErrorMsg, sizeof(sslErrorMsg));
  if (sslError) {
    Serial.printf("SSL error: %d: %s\r\n", sslError, sslErrorMsg);
  }
  
  http.end();
}
//--------------------------------------------
void startWebServer(){

 
  // setup request handlers
  server.on("/", handleRoot);
  server.on("/setpin", handleSetPin);   
  server.on("/scan", handleScan);
  server.on("/connect", handleConnect);
  server.onNotFound(handleRoot); // 404


  // start the sewrver
  server.begin();

  // Get IP addresss of the access point
  IPAddress ip = WiFi.softAPIP();
  Serial.print("HTTP server started at :");
  Serial.println(ip);
  
}
//--------------------------------------------
void handleSetPin(){
 
      if( server.hasArg("pin") && server.hasArg("state")){
        int pin = server.arg("pin").toInt();
        int state = server.arg("state").toInt();
        
        pinMode(pin,OUTPUT);
        analogWrite(pin, state );
        
        server.send(200, "application/json", "{\"state\":"+String(state)+"}");        
      }
      else{      
        server.send(400, "text/plain", "bad request!");
      }
}
//--------------------------------------------
void handleRoot() {
  if(!handleFileRead(server.uri())){
      server.send(404, "text/plain", "404: Not Found");
  } 
}
//--------------------------------------------
void handleScan() {
  int n = WiFi.scanNetworks();
  
  String json = "";
  for (int i = 0; i < n; i++)
  {
    Serial.println(WiFi.SSID(i));
    json = json + "\""+WiFi.SSID(i)+"\"";
    if(i<n-1){
      json = json + ",";
    }
  }
  
  json = "["+json +"]";
  server.send(200, "application/json", json);
}
//--------------------------------------------
void handleConnect(){


   if( server.hasArg("ssid") && server.hasArg("password")){
        String ssid = server.arg("ssid");
        String password = server.arg("password");

        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED)
        {
          delay(500);         
        }
                
        server.send(200, "application/json", "{\"ip\":\""+WiFi.localIP().toString()+"\"}");        
      }
      else{      
        server.send(400, "text/plain", "bad request!");
      }
}
//--------------------------------------------
//Read file from LittleFS and send to the client/browser

bool handleFileRead(String path) {  
  
  if (path.endsWith("/")) path += "index.html";
  
  
  String contentType = getContentType(path); // MIME type

  // use gzip version of files
  if(path.endsWith(".js") || path.endsWith(".css") || path.endsWith(".html")){
    path = path + ".gz";
  }
  
  if (LittleFS.exists(path)) {                          
      
    File file = LittleFS.open(path, "r");                 
    size_t sent = server.streamFile(file, contentType); 
    file.close();
    
    return true;
  }  
  return false; 
}

//--------------------------------------------
//returns MIME type to serve static files
String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}
