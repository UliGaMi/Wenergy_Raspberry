#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2);

#include <PZEM004Tv30.h>
#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#endif
#if !defined(PZEM_SERIAL)
#define PZEM_SERIAL Serial2
#endif
#if defined(ESP32)
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);
#elif defined(ESP8266)
#else
PZEM004Tv30 pzem(PZEM_SERIAL);
#endif

const char* ssid = "";
const char* password = "";

int state = 1;

int relePin = 15; 


void setup() {
  Serial.begin(115200);

  SerialBT.begin("ESP32_BT");
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  pinMode(relePin, OUTPUT);
  digitalWrite(15, HIGH);

  lcd.begin();
  lcd.backlight();
}

void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    
    HTTPClient http;
    http.begin(""); 
    int httpCode = http.GET();
  
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
      StaticJsonDocument<200> doc;
      deserializeJson(doc, payload);
      int deviceStatus = doc["deviceStatus"];
      if((state != deviceStatus) && !isnan(deviceStatus)){
        state = deviceStatus;
        if(state == 0){
          digitalWrite(relePin, LOW);
        }
        else{
          digitalWrite(relePin, HIGH);
        }
      }

      
      
    } else {
      Serial.println("Error en la solicitud GET HTTP");
    }
    http.end();
    lcd.clear();
    if(state==1){
      float voltage = pzem.voltage();
      float current = pzem.current();
      float power = pzem.power();
      if( !isnan(voltage) && !isnan(current) && !isnan(power)){
          lcd.clear();
          lcd.print("V:" + String(voltage,1));
          lcd.setCursor(0,2);
          lcd.print("A:" + String(current,1));
          lcd.setCursor(9,2);
          lcd.print("W:" + String(power,1));
        
          Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
          Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
          Serial.print("Power: ");        Serial.print(power);        Serial.println("W");

          if(SerialBT.connected()){
                SerialBT.println(String(voltage)+","+String(current)+","+String(power));
          }
      } else {
        Serial.println("Error al leer el voltage");
      }   
    }
    
    delay(1000);

    
  }
}