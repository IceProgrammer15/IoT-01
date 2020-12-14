
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>

ESP8266WiFiMulti WiFiMulti;

const char *ssid = "IOT-TechShare";
const char *password = "12345678";
const int PIN = 2;


ESP8266WebServer server(80);

void handleRoot() {
  if(!handleFileRead(server.uri())){
      server.send(404, "text/plain", "404: Not Found");
  } 
}


void setup() {

  Serial.begin(115200);
  Serial.println();

  pinMode(PIN,OUTPUT);

//   WiFi.mode(WIFI_AP_STA);
//   WiFiMulti.addAP("ICE Network", "pass");
//   while(WiFiMulti.run() != WL_CONNECTED) {
//         Serial.println('waiting...');
//         delay(500);
//   }
  
  Serial.print("Setup server ... ");
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");

  
  LittleFS.begin();   
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  if (MDNS.begin("techshare")) {
    Serial.println("DNS started.");
  }
  
  
  server.on("/", handleRoot);
  server.on("/setpin", handleSetPin); 
  //Handle 404
  server.onNotFound(handleRoot);


  //Start  Server
  server.begin();
  Serial.println("HTTP server started");

}

void handleSetPin(){
  {
      Serial.println("SetPin");    
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


//Main Loop
void loop() {
  server.handleClient();
}


//To serve static files
String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

//Read file from LittleFS and return back
bool handleFileRead(String path) { // send the right file to the client (if it exists)  
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  Serial.println("handleFileRead: " + path);
  String contentType = getContentType(path);            // Get the MIME type
  if (LittleFS.exists(path)) {                            // If the file exists
    File file = LittleFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}
