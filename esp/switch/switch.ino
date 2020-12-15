
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>


// SOFTAP WIFI
const char *ssid = "IOT-TechShare";
const char *password = "12345678";
const char *localDomain = "techshare"; // ie: http://techshare.local



ESP8266WebServer server(80);

void setup() {
  // set baud rate 
  Serial.begin(115200);
  Serial.println();
  Serial.print("WIFI Access point : ");
  
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
  delay(5);
  MDNS.update();
}


//--------------------------------------------
void startWebServer(){

 
  // setup request handlers
  server.on("/", handleRoot);
  server.on("/setpin", handleSetPin);   
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
  { 
      if( server.hasArg("pin") && server.hasArg("state")){
        int pin = server.arg("pin").toInt();
        int state = server.arg("state").toInt();
        
        pinMode(pin,OUTPUT);
        digitalWrite(pin, state );
        
        server.send(200, "application/json", "{\"state\":"+String(state)+"}");        
      }
      else{      
        server.send(400, "text/plain", "bad request!");
      }
  };

}
//--------------------------------------------
void handleRoot() {
  if(!handleFileRead(server.uri())){
      server.send(404, "text/plain", "404: Not Found");
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
