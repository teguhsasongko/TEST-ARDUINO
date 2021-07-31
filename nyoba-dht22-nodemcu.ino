String sentence;
String status_;
String suhu;
String kelembapan;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
  Serial.println("ERROR"); // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
//    Serial.write(Serial.read());
    sentence = Serial.readString();
//    Serial.println(status_);
//    Serial.println(sentence);
    suhu = String(getValue(sentence,' ',0));
    Serial.print("Suhu: ");
    Serial.println(suhu); 

    kelembapan = String(getValue(sentence,' ',1));
    Serial.print("kelembapan:");
    Serial.println(kelembapan); 
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
