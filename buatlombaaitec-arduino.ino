#include <Wire.h> 
#include <SoftwareSerial.h>
SoftwareSerial espSerial(5, 6);

//Humidity Library
#include <DHT.h> // sensor library using lib from https://www.ardumotive.com/how-to-use-dht-22-sensor-en.html
#define DHTPIN 13     // DHT PIN 2
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE); //Sensor initiation

//LCD Library
#include <LiquidCrystal_I2C.h> // LCD library using from  https://www.ardumotive.com/i2clcden.html for the i2c LCD library 
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 after finding it from serial monitor (see comment above) for a 16 chars and 2 line display

//PH Tanah Init
#define analogInPin A1 //Sensor Kelembapan 1
#define analogInPin A2 //Sensor Kelembapan 2
int sensorValue = 0;        
float outputValue = 0.0; 

//Kelembaban Tanah  
int sensorPin = A1;
int sensor2Pin = A2;
int nilai;
int nilai2;

// Pengukuran Air
const int trigPin = 12;
const int echoPin = 11;
const int buzzer = 4;
long duration;
int distance;

//pengukuran Pupuk
const int trig2Pin = 10;
const int echo2Pin = 9;
const int buzzer2 = 5;
long duration2;
int distance2;

//RELAY
int relay = 3; //relay pompa air
int relay2 = 4; //relay pompa pupuk

String Info;
String airRelay;
String pupukRelay;
String levelAir;
String kondisiAir;
String levelPupuk;
String kondisiPupuk;

void setup()
{
Serial.begin(115200);
espSerial.begin(115200);
Serial.println("Program started");
lcd.init(); // initialize the lcd 
dht.begin();
  // Print a message to the LCD.
lcd.backlight();
lcd.setBacklight(HIGH);
pinMode(sensorPin, INPUT); // Kelembaban 1
  pinMode(sensor2Pin, INPUT); // Kelembaban 2
  pinMode(relay, OUTPUT); // Pompa Kelembaban
  pinMode(relay2, OUTPUT); // Pompa pH
  pinMode(trigPin, OUTPUT); //Pengukuran Air
  pinMode(echoPin, INPUT);// Pengukuran Air
  pinMode(trig2Pin, OUTPUT); //Pengukuran Air
  pinMode(echo2Pin, INPUT);// Pengukuran Air
  pinMode(buzzer, OUTPUT); // buzzer Air
  pinMode(buzzer2, OUTPUT); // buzzer pupuk
}

void loop()
{
// Sensor PH
  sensorValue = analogRead(analogInPin);
  outputValue = ((sensorValue*(0.01388))-(-4.1388))+0.3;
  if(outputValue < 6.6)
   {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("adc = ");
   lcd.print(sensorValue);
   lcd.setCursor(0,1);
   lcd.print("pH = ");
   lcd.print(outputValue);
   delay(1000);
   pupukRelay = "ON";
   digitalWrite(relay2,HIGH);
   }
  
  else if(outputValue >=6.6)
   {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("adc = ");
   lcd.print(sensorValue);
   lcd.setCursor(0,1);
   lcd.print("PH = ");
   lcd.print(outputValue);
   delay(1000);
   pupukRelay = "ON";
   digitalWrite(relay2,LOW);
   }
   
//DHT22
  float h = dht.readHumidity(); //measure humidity
  String myString = "";
  myString.concat(h);
  
  float t = dht.readTemperature(); //measure temperature
  String myStringe = "";
  myStringe.concat(t);
  
  if (isnan(h) || isnan(t)) { //Check. If the reading fails, then "Read error" is displayed and the program exits
    Serial.println("Reading error");
    return;
    }
   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temperature: ");
  lcd.print(t);
  lcd.print(" *C\t");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.println(" %");
  
  // Sensor Ultrasonik Air
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); 
  distance = duration*0.034/2;

  // Sensor Ultrasonik Pupuk
  digitalWrite(trig2Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2Pin, LOW);
  duration2 = pulseIn(echo2Pin, HIGH); 
  distance2 = duration2*0.034/2;

  // Pengukuran Kelembaban Tanah
  int nilai = analogRead(sensorPin); 
  if(nilai > 700)
  {
   lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pohon 1 = ");
    lcd.print(nilai);
    lcd.setCursor(0, 1);
    lcd.print("Tanah Kering");
    delay(1000);
    airRelay = "ON";
    digitalWrite(relay, LOW);
  }
  else if(nilai >= 350 && nilai <= 700)
  {
   lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pohon 1 = ");
    lcd.print(nilai);
    lcd.setCursor(0, 1);
    lcd.print("Tanah Normal");
    delay(1000);
    airRelay = "OFF";
    digitalWrite(relay, HIGH);
  }
  else if(nilai < 350){
   lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pohon 1 = ");
    lcd.print(nilai);
    lcd.setCursor(0, 1);
    lcd.print("Tanah Lembab");
    delay(1000);
    airRelay = "OFF";
    digitalWrite(relay, HIGH);
  }
  
// Pengukuran Kelembaban Tanah 2
  int nilai2 = analogRead(sensor2Pin); 
  if(nilai2 > 700)
  {
   lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pohon 2 = ");
    lcd.print(nilai2);
    lcd.setCursor(0, 1);
    lcd.print("Tanah Kering");
    delay(1000);
    airRelay = "ON";
    digitalWrite(relay, LOW);
  }
  else if(nilai2 >= 350 && nilai2 <= 700)
  {
   lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pohon 2 = ");
    lcd.print(nilai2);
    lcd.setCursor(0, 1);
    lcd.print("Tanah Normal");
    delay(1000);
    airRelay = "OFF";
    digitalWrite(relay, HIGH);
  }
  else if(nilai2 < 350){
   lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pohon 2 = ");
    lcd.print(nilai2);
    lcd.setCursor(0, 1);
    lcd.print("Tanah Lembab");
    delay(1000);
    airRelay = "OFF";
    digitalWrite(relay, HIGH);
  }

// Pengukuran Air
   if (distance > 16)
    {
   lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sisa Air Kurang");
    lcd.setCursor(0,1);
    lcd.print("Dari 6 cm");
    levelAir = "15%";
    kondisiAir = "Sisa Air Kurang";
    digitalWrite(buzzer, HIGH);
    delay (1000);
}
    else if(distance >= 6 && distance <= 16)
{
   lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sisa Air");
    lcd.setCursor(0,1);
    lcd.print("Masih Cukup");
    levelAir = "50%";
    kondisiAir = "Sisa Air Cukup";
    digitalWrite(buzzer, LOW);
    delay (1000);
}
    else if(distance < 6 )
{
   lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Air");
    lcd.setCursor(0,1);
    lcd.print("Masih Penuh");
    levelAir = "90%";
    kondisiAir = "Air Masih Penuh";
    digitalWrite(buzzer, LOW);
    delay (1000);
   }

// Pengukuran Pupuk
   if (distance2 > 16)
    {
   lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sisa Pupuk Kurang");
    lcd.setCursor(0,1);
    lcd.print("Dari 6 cm");
    digitalWrite(buzzer2, HIGH);
    levelPupuk = "15%";
    kondisiPupuk = "Sisa Pupuk Kurang";
    delay (1000);
}
    else if(distance2 >= 6 && distance2 <= 16)
{
   lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sisa Pupuk");
    lcd.setCursor(0,1);
    lcd.print("Masih Cukup");
    digitalWrite(buzzer2, LOW);
    levelPupuk = "50%";
    kondisiPupuk = "Pupuk Masih Cukup";
    delay (1000);
}
    else if(distance2 < 6 )
{
   lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Pupuk");
    lcd.setCursor(0,1);
    lcd.print("Masih Penuh");
    levelPupuk = "90%";
    kondisiPupuk = "Pupuk Masih Penuh";
    digitalWrite(buzzer2, LOW);
    delay (1000);
   }
   
//SEND DATA
Serial.print("Kelembapan Udara: ");
Serial.print(h);
Serial.println("% ");

Serial.print(" Suhu: ");
Serial.print(t);
Serial.println("C");


espSerial.println(String(nilai)+" "+String(nilai2)+" "+String(outputValue)+" "+String(levelAir)+" "+String(kondisiAir)+" "+String(airRelay)+" "+String(levelPupuk)+" "+String(kondisiPupuk)+" "+String(pupukRelay)+" "+String(h)+" "+String(t));
delay(1000);
}
