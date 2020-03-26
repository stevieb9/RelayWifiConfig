#ifndef RELAYWIFICONFIG_H
#define RELAYWIFICONFIG_H

#include <Arduino.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>

#define CONFIG_PIN 0

void relayConfig (uint8_t manualConfig);
void saveSettingsCallback ();

#endif
