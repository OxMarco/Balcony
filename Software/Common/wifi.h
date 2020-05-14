// This structure is stored in RTC memory to save the WiFi state and reset count (number of Deep Sleeps),
// and it reconnects twice as fast as the first connection; it's used several places in this demo
struct nv_s {
  WiFiState wss; // core's WiFi save state

  struct {
    uint32_t crc32;
    uint32_t rstCount;  // stores the Deep Sleep reset count
    // you can add anything else here that you want to save, must be 4-byte aligned
  } rtcData;
};
static nv_s* nv = (nv_s*)RTC_USER_MEM; // user RTC RAM area
uint32_t resetCount = 0;  // keeps track of the number of Deep Sleep tests / resets
uint32_t timeout = 30E3;  // 30 second timeout on the WiFi connection

/**
 * @brief Updates the reset count CRC
 */
void updateRTCcrc() {
    // Read previous resets (Deep Sleeps) from RTC memory, if any
    uint32_t crcOfData = crc32((uint8_t*) &nv->rtcData.rstCount, sizeof(nv->rtcData.rstCount));
    if ((crcOfData = nv->rtcData.crc32)) {
      resetCount = nv->rtcData.rstCount;  // read the previous reset count
      resetCount++;
    }
    nv->rtcData.rstCount = resetCount; // update the reset count & CRC
    nv->rtcData.crc32 = crc32((uint8_t*) &nv->rtcData.rstCount, sizeof(nv->rtcData.rstCount));
}

/**
 * @brief Reinitializes WiFi using previously saved status
 */
void initWiFi() {
    uint32_t wifiBegin = millis();  // how long does it take to connect
    if ((crc32((uint8_t*) &nv->rtcData.rstCount + 1, sizeof(nv->wss)) && !WiFi.shutdownValidCRC(&nv->wss))) {
        // if good copy of wss, overwrite invalid (primary) copy
        memcpy((uint32_t*) &nv->wss, (uint32_t*) &nv->rtcData.rstCount + 1, sizeof(nv->wss));
    }
    if (WiFi.shutdownValidCRC(&nv->wss)) {  // if we have a valid WiFi saved state
        memcpy((uint32_t*) &nv->rtcData.rstCount + 1, (uint32_t*) &nv->wss, sizeof(nv->wss)); // save a copy of it
        Logger.info(F("resuming WiFi"));
    }
    if (!(WiFi.mode(WIFI_RESUME, &nv->wss))) {  // couldn't resume, or no valid saved WiFi state yet
        /* Explicitly set the ESP8266 as a WiFi-client (STAtion mode), otherwise by default it
         would try to act as both a client and an access-point and could cause network issues
         with other WiFi devices on your network. */
        WiFi.persistent(false);  // don't store the connection each time to save wear on the flash
        WiFi.mode(WIFI_STA);
        WiFi.setOutputPower(10);  // reduce RF output power, increase if it won't connect
        // WiFi.config(staticIP, gateway, subnet);
        WiFi.begin(ssid, password);
        Logger.info(F("connecting to WiFi"));
    }
    wifiTimeout.reset(timeout);
    while (((!WiFi.localIP()) || (WiFi.status() != WL_CONNECTED)) && (!wifiTimeout)) {
        yield();
    }
    if ((WiFi.status() == WL_CONNECTED) && WiFi.localIP()) {
        Logger.info(F("WiFi connected"));
        Logger.info(F("WiFi connect time = "));
        float reConn = (millis() - wifiBegin);
        Logger.info("%1.2f seconds\n", reConn / 1000);
        Logger.info(F("IP address: "));
        Logger.info(WiFi.localIP());
    } else {
        Logger.err(F("WiFi timed out and didn't connect"));
    }
    WiFi.setAutoReconnect(true);
}

void sleep()
{
    WiFi.mode(WIFI_SHUTDOWN, &nv->wss);
}
