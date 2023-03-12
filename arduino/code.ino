#include <SoftwareSerial.h>
#include <DFRobot_DHT11.h>
#define RX 10
#define TX 11
#define dht_apin 9 
#define smokes 0

dht11 DHT;
String AP = "TP-Link_4E66";       // AP NAME
String PASS = "wormhole"; // AP PASSWORD
String API = "OEOBVWS4YRDPUZH9";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
float smokeSens;
  
SoftwareSerial esp8266(RX,TX); 
  
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  pinMode(dht_apin,INPUT);
  pinMode(smokes,INPUT);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  delay(5000);
}

void loop() {
  
 String getData = "GET /update?api_key="+ API +"&field1="+getTemperatureValue()+"&field2="+getHumidityValue();
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);
 delay(1500);
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}


String getTemperatureValue(){

   DHT.read(dht_apin);
   Serial.print(" Temperature(C)= ");
   int temp = DHT.temperature;
   Serial.println(temp); 
   delay(50);
   return String(temp); 
  
}
String getHumidityValue(){

   DHT.read(dht_apin);
   Serial.print(" Humidity in %= ");
   int humidity = DHT.humidity;
   Serial.println(humidity);
   delay(50);
   return String(humidity);

}

String getSmokeVal(){
  smokeSens = analogRead(smokes);
  Serial.print(" Concentration of smoke(in ppm)");
  Serial.println(smokeSens);
  delay(100);
  return String(smokeSens);
}

String getGasVal(){
  gasSens = analogRead(gas);
  Serial.print(" Concentration of smoke(in ppm)");
  Serial.println(gasSens);
  delay(100);
  return String(gasSens);
}


void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }

    countTimeCommand++;
  }

  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
 }
           
