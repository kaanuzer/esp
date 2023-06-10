#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid     = "onemarin";
const char* password = "123456789";

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

int count_value =101;
int button1_State = 0;
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

IPAddress IP(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void action(AsyncWebServerRequest *request) {
  Serial.println("ACTION!");

  int params = request->params();
  for (int i = 0; i < params; i++) {
    AsyncWebParameter* p = request->getParam(i);
    Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
    
    if(p->value().c_str() == "up"){
       digitalWrite(4, HIGH);
        Serial.printf("%s\n",  p->value().c_str());
    }
    if(p->value().c_str() =="down"){
       digitalWrite(5, HIGH);
    }else{

    delay(1000);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    }
  }
  request->send(200);
}



void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(0, INPUT); 
  

         digitalWrite(4, HIGH);
                digitalWrite(5, HIGH);

  WiFi.softAP(ssid, password);
  delay(500);
  WiFi.softAPConfig(IP, gateway, subnet);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);


  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  server.on("/action", HTTP_POST, action);



  server.on("/chain", HTTP_GET, [](AsyncWebServerRequest * request) 
  {   
      String msg = "{chainlenght:"  + String(count_value) + "}";
    request->send(200,"application/json",msg);  
  });
     
  
  server.onNotFound(notFound);

  server.begin();

}

void loop() {

button1_State = digitalRead(0);
if (button1_State == HIGH) {
    count_value++;  
}

}