#include <WiFiManager.h>

WiFiManager wm;

int count = 0;
int rssi;

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void _initMNwifi(String deviceToken){
  // WiFi.mode(WIFI_AP_STA); // Correct
  String host = "KU-" + deviceToken;
  wm.setAPCallback(configModeCallback);
  wm.setConfigPortalTimeout(300); // auto close configportal after n seconds
  wm.setAPClientCheck(true);     // avoid timeout if client connected to softap
  wm.setBreakAfterConfig(true);  // always exit configportal even if wifi save fails
  if (!wm.autoConnect(host.c_str()))
    {
      Serial.println("failed to connect and hit timeout");
      delay(1000);
    }
  else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        Serial.println("IP Address: " + WiFi.localIP().toString());
        Serial.println("Mode: " + WiFi.getMode());
        Serial.print("WiFi singnal Strenght: ");
        rssi = map(WiFi.RSSI(), -100, -50, 0, 100);
        Serial.println(constrain(rssi, 0, 100));
    }
}

void CheckWiFi(){
    if (WiFi.status() != WL_CONNECTED){
        while (WiFi.reconnect())
        {
            count++;
            if (count == 300){
                ESP.restart();
            }
        }
        count = 0;
        
    }
}