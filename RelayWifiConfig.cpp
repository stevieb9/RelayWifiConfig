#include <RelayWifiConfig.h>

void relayConfig () {

    WiFiManager wifiManager;

    wifiManager.setTimeout(30);

    if (!wifiManager.startConfigPortal("Relay WiFi Configuration")) {
        delay(3000);
        ESP.reset();
        delay(5000);
    }
}
