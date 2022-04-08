#include <ArduinoJson.h>

#include<DHT.h>
#include <WiFi.h>
#include<HTTPClient.h>

#define DHTTYPE DHT11
#define TemperatureSensor 26

int WaterSensor = 34;
int LED = 13;
int LightSensor = 35;

//HUAWEI-B315-D551
const char* ssid ="HUAWEI-B315-D551";
//Mercan2016
const char* password = "Mercan2016";
int HTTP_PORT =  3000; //change this to relevant port
String HOST_NAME = "http://192.168.8.102:3000/postTest"; //url
String PATH_NAME = "/";//specific

DynamicJsonDocument doc(1024);
DHT dht(TemperatureSensor,DHTTYPE);


void setup() {
  // put your setup code here, to run once:
  
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  networkStartup();
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  nonsense();
  String Values[6] = {String(waterLevelFlowSensor()),Temperature(),Humidity(),Lumination()};
  httpPost(Values);
  nonsense();
  delay(30000);
  
}
void nonsense(){
  
  if (digitalRead(LED)==1){
    digitalWrite(LED,LOW);
    }
  else{
    digitalWrite(LED, HIGH);
    }
  }
  
void httpPost(String Data [6] ){
  Serial.print(WiFi.status());
  if (WiFi.status()!=WL_CONNECTED){
    Serial.print("conn to newtork failed");
    return;
    }
    HTTPClient http;
    WiFiClient client; 
    //http://192.168.8.102:3000/HydroData
    http.begin("https://api.hydro.jbgaard.xyz/HydroData");
    http.addHeader("Content-Type","application/json");
    http.addHeader("x-api-key","HydroProject2022");
      
    Serial.println(http.POST(SerializeData(Data)));
    Serial.println(http.getString());
    Serial.println("reee");
    http.end();  
  }
  
  String SerializeData(String datas[6]){
    doc["LiquidLV"]=datas[0];
    doc["Temperature"]=datas[1];
    doc["Humidity"]=datas[2];
    doc["Lumination"]=datas[3];
    doc["DeviceID"]=WiFi.macAddress();
      String reee;
    serializeJson(doc,reee);
    return reee;
    }
    
double waterLevelFlowSensor() {
  double val = analogRead(WaterSensor); 
  Serial.print("Liquid level: ");
  Serial.print(val/21.5);
  Serial.print("%");
  Serial.print("\n");
  
  Serial.print("\n");
  return val/21.5;
  }
  
  String Temperature(){
  String val = String(dht.readTemperature());
  Serial.println("temperature"+ val);
  return val;
  }
  
  String Humidity(){
     String val =String(dht.readHumidity());
     Serial.println("humidity"+val);
     return val;
  }
  String Lumination(){
  String val = String(analogRead(LightSensor));
  return val;
  }
  
void networkStartup(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wifi ..");
  int turntry = 0;
  while (WiFi.status()!= WL_CONNECTED){
    Serial.print("\n");
    Serial.print("Retrying to connect .. ");
    Serial.print(+WiFi.status());
    delay(1500);
    }
    Serial.println(WiFi.localIP());
  }
