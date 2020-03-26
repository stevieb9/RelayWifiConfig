#include <RelayWifiConfig.h>

#include <FS.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>

char mqtt_un[41] = "";
char mqtt_pw[41] = "";
char mqtt_id[41] = "";

bool shouldSaveConfig = false;

void saveConfigCallback () {
    Serial.println("Should save config");
    shouldSaveConfig = true;
}

void relayConfig (uint8_t manualConfig) {

    WiFiManager wifiManager;
    //SPIFFS.format();

    if (SPIFFS.begin()) {
        if (SPIFFS.exists("/config.json")) {
            File configFile = SPIFFS.open("/config.json", "r");
            if (configFile) {
                size_t size = configFile.size();
                std::unique_ptr<char[]> buf(new char[size]);

                configFile.readBytes(buf.get(), size);
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.parseObject(buf.get());
                json.printTo(Serial);
                if (json.success()) {
                    strcpy(mqtt_un, json["mqtt_un"]);
                    strcpy(mqtt_pw, json["mqtt_pw"]);
                    strcpy(mqtt_id, json["mqtt_id"]);

                } 
                
                configFile.close();
            }
        }
    }


    WiFiManagerParameter custom_mqtt_un("mqtt_un", "MQTT Username", mqtt_un, 41);
    WiFiManagerParameter custom_mqtt_pw("mqtt_pw", "MQTT Password", mqtt_pw, 41);
    WiFiManagerParameter custom_mqtt_id("mqtt_id", "MQTT Client ID", mqtt_id, 41);

    wifiManager.setSaveConfigCallback(saveConfigCallback);
    wifiManager.setTimeout(30);
    //wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    wifiManager.addParameter(&custom_mqtt_un);
    wifiManager.addParameter(&custom_mqtt_pw);
    wifiManager.addParameter(&custom_mqtt_id);

    if (manualConfig) {
        if (!wifiManager.startConfigPortal("RelayConfigAP", "password")) {
            Serial.println(F("Failed to configure RelayConfigAP"));
            delay(3000);
            ESP.reset();
            delay(5000);
        }
    }
    else {
        if (!wifiManager.autoConnect("RelayConfigAP", "password")) {
            Serial.println(F("Failed to connect to configured AP"));
            delay(3000);
            ESP.reset();
            delay(5000);
        }
    }

    strcpy(mqtt_un, custom_mqtt_un.getValue());
    strcpy(mqtt_pw, custom_mqtt_pw.getValue());
    strcpy(mqtt_id, custom_mqtt_id.getValue());

    if (shouldSaveConfig) {
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.createObject();
        json["mqtt_un"] = mqtt_un;
        json["mqtt_pw"] = mqtt_pw;
        json["mqtt_id"] = mqtt_id;

        File configFile = SPIFFS.open("/config.json", "w");
        
        if (!configFile) {
            Serial.println("Failed to open config file for writing");
        }

        json.printTo(Serial);
        json.printTo(configFile);
        configFile.close();
    }
    
    Serial.println(WiFi.localIP());
}
