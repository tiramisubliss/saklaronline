#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid="ZST";
const char* password="aaaaaaaa";

const char* host = "http://yourhost/saklar.html";
String path = "http://yourhost.com/light.json";

const int lampu = 2;

void setup() {
  pinMode(lampu,OUTPUT);
  pinMode(lampu, HIGH);

  Serial.begin(115200);
  delay(20);
  Serial.print("Connecting to ");
  Serial.println("ssid");

  WiFi.begin(ssid,password);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP Address: " + WiFi.localIP());
}

void loop() {
  Serial.print("connecting to");
  Serial.print(host);
  WiFiClient client;
  const int httpPort = 80;

  if(!client.connect(host, httpPort)){
    Serial.println("connection failed");
    return;
  }

  client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                "Host : " + host + "\r\n" +
                "Connection: keep-alive\r\n\r\n");
   delay(500);

   String section = "header";
   while(client.available()){
    String line = client.readStringUntil('\r');

    if(section == "header"){
      Serial.print(".");
      if(line == "\n"){
        section = "json";
      }
    }
    else if (section == "json"){
      section="ignore";
      String result = line.substring(1);

      //parsing JSON
      int size = result.length() + 1;
      char json[size];
      result.toCharArray(json,size);

      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& json_parsed = jsonBuffer.parseObject(json);
      if(!json_parsed.success())
      {
        Serial.println("parseObject() failed");
        return;  
      }

      if(strcmp(json_parsed["light"], "on") == 0)
      {
        digitalWrite(lampu,HIGH);
        Serial.println("LED ON");
      
      }
      else {
        digitalWrite(lampu,LOW);
        Serial.println("LED OFF");
        
      }
    }
   }
    Serial.print("Closing Connection");
}
  
