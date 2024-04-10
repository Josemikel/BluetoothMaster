#include <Arduino.h>
#include "DHT.h"
#include "BluetoothSerial.h" // We will include the Serial Bluetooth header
#include <TFT_eSPI.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif

#define DHTPIN 27 // pin 27 del ttgo
#define DHTTYPE DHT11



BluetoothSerial BT; // Bluetooth Object
DHT dht(DHTPIN, DHTTYPE);
TFT_eSPI tft = TFT_eSPI();

String clientName = "ESP32_LED_Control";
bool connected;
float th;
void setup() {
  Serial.begin(115200); // Initializing serial connection for debugging
  dht.begin();
  BT.begin("ESP32_client", true); // Name of your Bluetooth Device and in master mode
  Serial.println("Bluetooth Device is in master mode. Connecting to the host...");
  connected = BT.connect(clientName);
  if(connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while(!BT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  }
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Temperatura", 10, 18, 2);
  tft.drawString("Humedad", 10, 88, 2);
  tft.drawString("`C", 63, 40, 4);
  tft.drawString("%", 63, 110, 4);
}
void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString(String(t, 1), 10, 40, 4);
  tft.drawString(String(h, 1), 10, 110, 4);

  //BT.write(arr, 10); 
  
  BT.print(String(t,1));
  BT.println(String(h,1));
  delay(100);
  BT.flush();

  

  //BT.write(49); // Send 1 in ASCII
  //delay(500);
  //BT.write(48);
}

