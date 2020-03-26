#include <RelayWifiConfig.h>

/*
 * - Install sketch
 * - Connect to "Relay Wifi Configuration" Access Point
 * - Configure settings
 * - Save configuration
*/

void setup () {
}

bool _init = false;

void loop () {
    if (! _init) {
        relayConfig();
    }
    else {
        _init = true;
    }
