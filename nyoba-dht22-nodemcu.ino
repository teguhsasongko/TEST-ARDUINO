#include <ESP8266WiFi.h>
#include <WiFiClient.h>

String sentence;
String suhu;
String kelembapan;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

const char* ssid = "localhost"; 
const char* password = "";
const char* host = "192.168.1.9";
const int port = 80;

WiFiClient client;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {; // wait for serial port to connect. Needed for native USB port only
  }
  // Networking
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connect(host, port)) {
    Serial.println("Connection failed");
    return;
  }
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    
//    Serial.write(Serial.read());
    sentence = Serial.readString();
    suhu = String(getValue(sentence,' ',0));
    Serial.print("Suhu: ");
    Serial.println(suhu); 

    kelembapan = String(getValue(sentence,' ',1));
    Serial.print("kelembapan:");
    Serial.println(kelembapan); 
    }
    kirim();
    delay(5000);
}
void kirim()   //CONNECTING WITH MYSQL
 { 
String apiUrl = "/lomba/kirim.php?";
  apiUrl += "data4="+String(suhu);
  apiUrl += "&data5="+String(kelembapan);
  
  // Set header Request
  client.print(String("POST ") + apiUrl + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  // Pastikan tidak berlarut-larut
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 1000) {
      Serial.println(">>> Client Timeout !");
      Serial.println(">>> Operation failed !");
      client.stop();
      return;
    }
  }
  // Baca hasil balasan dari PHP
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.println(line);
  }             
} 
 
String getValue(String data, char separator, int index){
  int found = 0;
  int strIndex[] = {0,-1};
  int maxIndex = data.length()-1;

  for(int i =0; i<=maxIndex && found<=index; i++ ){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]): "";
}
