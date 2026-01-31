#include <Arduino.h>
// #include <WiFi.h>
#include <Wire.h>
#include <PZEM004Tv30.h>

// #include "MyLED.h"
// #include "MySensor.h"
#include "WiFiSetUp.h"
#include "mqttSetUp.h"
#include "dataStc.h"

#define PZEM_RX_PIN 42
#define PZEM_TX_PIN 41
#define PZEM_TX02_PIN 39

#define PZEM_SERIAL Serial2

PZEM004Tv30 pz(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);
  
PZEM004Tv30 pzem[2] = {
  PZEM004Tv30(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN, 0x10),
  PZEM004Tv30(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN, 0x11)
};


unsigned long previousMillis, teleMillis = 0;

String deviceToken;

const char* serverName = "";
String apiKey = "";

float voltage, current, power, energy, frequency, pf;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600 * 7;
String timeS = "";

String current_version = "0.0.1";

void getMac()
{
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("Get Mac");
  Serial.println("OK");
  Serial.print("+deviceToken: ");
  Serial.println(WiFi.macAddress());
  // SerialBT.print("Get Mac");
  // SerialBT.println("OK");
  // SerialBT.print("+deviceToken: ");
  // SerialBT.println(WiFi.macAddress());
  for (int i = 0; i < 6; i++) {
    if (mac[i] < 0x10) {
      deviceToken += "0"; // Add leading zero if needed
    }
    deviceToken += String(mac[i], HEX); // Convert byte to hex
  }
  deviceToken.toUpperCase();
  // ESPUI.updateLabel(GUI.idLabel, String(deviceToken));

  // // --- NEW CODE TO EXTRACT DIGITS ---
  // for (int i = 0; i < deviceToken.length(); i++) {
  //   // Check if the character at the current position is a digit
  //   if (isDigit(deviceToken.charAt(i))) {
  //     // If it is a digit, add it to our new string
  //     digitsOnlyToken += deviceToken.charAt(i);
  //   }
  // }

}

void _intitPz(){
  for (int i = 0; i < 2; i++){
    pzem[i] = PZEM004Tv30(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN, 0x10 + i);
    delayMicroseconds(2000000);
  }

}

void getPz(){
  // Print out the measured values from each PZEM module
    for(int i = 0; i < 2; i++){
        // Print the Address of the PZEM
        Serial.print("PZEM ");
        Serial.print(i);
        Serial.print(" - Address:");
        Serial.println(pzem[i].getAddress(), HEX);
        Serial.println(pzem[i].readAddress(), HEX);
        Serial.println("===================");


        // Read the data from the sensor
        data[i].voltage = pzem[i].voltage();
        data[i].current = pzem[i].current();
        data[i].power = pzem[i].power();
        data[i].energy = pzem[i].energy();
        data[i].frequency = pzem[i].frequency();
        data[i].pf = pzem[i].pf();


        // Check if the data is valid
        if(isnan(data[i].voltage)){
            Serial.println("Error reading voltage");
        } else if (isnan(data[i].current)) {
            Serial.println("Error reading current");
        } else if (isnan(data[i].power)) {
            Serial.println("Error reading power");
        } else if (isnan(data[i].energy)) {
            Serial.println("Error reading energy");
        } else if (isnan(data[i].frequency)) {
            Serial.println("Error reading frequency");
        } else if (isnan(data[i].pf)) {
            Serial.println("Error reading power factor");
        } else {
            // Print the values to the Serial console
            Serial.print("Voltage: ");      Serial.print(data[i].voltage);      Serial.println("V");
            Serial.print("Current: ");      Serial.print(data[i].current);      Serial.println("A");
            Serial.print("Power: ");        Serial.print(data[i].power);        Serial.println("W");
            Serial.print("Energy: ");       Serial.print(data[i].energy,3);     Serial.println("kWh");
            Serial.print("Frequency: ");    Serial.print(data[i].frequency, 1); Serial.println("Hz");
            Serial.print("PF: ");           Serial.println(data[i].pf);

        }

        Serial.println("-------------------");
        Serial.println();
    }
    

}

void setAddr(){

  static uint8_t addr = 0x11; // use address 0x10 and 0x11 respectively

    // Print out current custom address
    Serial.print("Previous address:   0x");
    Serial.println(pz.readAddress(), HEX);

    // Set the custom address
    Serial.print("Setting address to: 0x");
    Serial.println(addr, HEX);
    if(!pz.setAddress(addr))
    {
      // Setting custom address failed. Probably no PZEM connected
      Serial.println("Error setting address.");
    } else {
      // Print out the new custom address
      Serial.print("Current address:    0x");
      Serial.println(pz.readAddress(), HEX);
      Serial.println();
    }

  // testing section after set new address
  Serial.println("Strat read pz01");
  Serial.println("PZEM ");
  // Serial.print(0);
  Serial.print(" - GET Address:");
  Serial.println(pz.getAddress(), HEX);
  Serial.print(" - READ Address:");
  Serial.println(pz.readAddress(), HEX);
  Serial.println("===================");
  Serial.print("Voltage01: ");
  Serial.println(pz.voltage());
  Serial.println();

}

String a0(int n) {
  return (n < 10) ? "0" + String(n) : String(n);
}

String getTS(){
  timeS = "";
  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    timeS = "Failed to obtain time";
  }else{
    timeS = a0(timeinfo.tm_mday) + "/" + a0(timeinfo.tm_mon + 1) + "/" + String(timeinfo.tm_year + 1900) + "  " + a0(timeinfo.tm_hour) + ":" + a0(timeinfo.tm_min) + ":" + a0(timeinfo.tm_sec) + ":" + a0(timeinfo.tm_sec / 1000) + "";
  }

  return timeS;

}

void setup() {

    // pinMode(5, OUTPUT);

    // Debugging Serial port
    Serial.begin(115200);
    
    delay(2000);
    
    // _intitPz();

    // _initRGB();
    // delay(2000);
    getMac();
    // delay(2000);
    _initMNwifi(deviceToken);
    // delay(2000);
    _initMqtt();

    configTime(3600 * 7, 0, ntpServer);
    Serial.println("TimeStamp:"+ getTS());

    Serial.println("Version: " + current_version);

    // pz.begin(9600,SERIAL_8N1, PZEM_RX_PIN, PZEM_TX_PIN);

}

void loop() {
  unsigned long currentMillis = millis();

  

  // CheckWiFi();

  // if (currentMillis - previousMillis >= 60000){
  //   previousMillis += 60000;
  //   Serial.print("WiFi singnal Strenght: ");
  //   rssi = map(WiFi.RSSI(), -100, -50, 0, 100);
  //   Serial.println(constrain(rssi, 0, 100));
    
    
  // }

  String json;

  if (currentMillis - teleMillis >= 1000){
    teleMillis += 1000;

    // setAddr();

    getPz();
    
    json = "";
    json.concat("{\"TS\":\"");
    json.concat(getTS());
    json.concat("\",\"DeviceTk\":\"");
    json.concat(deviceToken);
    json.concat("\",\"Version\":\"");
    json.concat(current_version);
    json.concat("\",\"RSSI\":");
    json.concat(map(WiFi.RSSI(), -100, -50, 0, 100));
    json.concat("\",\"M01_vol\":");
    json.concat(data[0].voltage);
    json.concat(",\"M01_curr\":");
    json.concat(data[0].current);
    json.concat(",\"M01_power\":");
    json.concat(data[0].power);
    json.concat(",\"M01_energy\":");
    json.concat(data[0].energy);
    json.concat(",\"M01_frequency\":");
    json.concat(data[0].frequency);
    json.concat(",\"M01_powfac\":");
    json.concat(data[0].pf);
    json.concat(",\"M02_vol\":");
    json.concat(data[1].voltage);
    json.concat(",\"M02_curr\":");
    json.concat(data[1].current);
    json.concat(",\"M02_power\":");
    json.concat(data[1].power);
    json.concat(",\"M02_energy\":");
    json.concat(data[1].energy);
    json.concat(",\"M02_frequency\":");
    json.concat(data[1].frequency);
    json.concat(",\"M02_powfac\":");
    json.concat(data[1].pf);
    json.concat("}");

    Serial.println(json);

    Serial.println();

    // Length (with one extra character for the null terminator)
    int str_len = json.length() + 1;
    // Prepare the character array (the buffer)
    char char_array[str_len];
    // Copy it over
    json.toCharArray(char_array, str_len);
    String topic = deviceToken + "/silo01";
    uint16_t packetIdPub1 = mqttClient.publish(topic.c_str(), 0, true, char_array);
    if (packetIdPub1){
      Serial.println("Send Msg success!");
    }
    Serial.printf("Publishing on topic %s at QoS 0", topic.c_str());
    Serial.printf(" Message: %.2f \n", json);
  }
  


}
