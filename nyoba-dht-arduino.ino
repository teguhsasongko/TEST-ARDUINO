#include <DHT.h> // sensor library using lib from https://www.ardumotive.com/how-to-use-dht-22-sensor-en.html
#include <LiquidCrystal_I2C.h> // LCD library using from  https://www.ardumotive.com/i2clcden.html for the i2c LCD library 
#include <Wire.h> 
#include <SoftwareSerial.h>

#define DHTPIN 13     // DHT PIN 2
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

SoftwareSerial espSerial(5, 6);
DHT dht(DHTPIN, DHTTYPE); //Sensor initiation
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 after finding it from serial monitor (see comment above) for a 16 chars and 2 line display

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
}

void loop()
{
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
  lcd.setCursor(0,0);
  lcd.print("Temperature: ");
  lcd.print(t);
  lcd.print(" *C\t");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.println(" %");

//SEND DATA
Serial.print("Kelembapan Udara: ");
Serial.print(h);
Serial.println("% ");

Serial.print(" Suhu: ");
Serial.print(t);
Serial.println("C");


espSerial.println(String(h)+" "+String(t)+" ");
delay(1000);
}
