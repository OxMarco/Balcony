void test()
{
    // check if voltage is ok
    ESP.getVcc();
    
    // check why it was reset
    ESP.getResetReason(); // returns a String containing the last reset reason in human readable format.

    // check if enough free heap space
    ESP.getFreeHeap(); // returns the free heap size.
    
    // check if enough free space
    ESP.getSketchSize(); // returns the size of the current sketch as an unsigned 32-bit integer.

    ESP.getFreeSketchSpace(); // returns the free sketch space as an unsigned 32-bit integer.

    // check if sketch MD5 is equal to the one on the website
    ESP.getSketchMD5(); // returns a lowercase String containing the MD5 of the current sketch.
    
    uint32_t realSize = ESP.getFlashChipRealSize();
    uint32_t ideSize = ESP.getFlashChipSize();
    
    if (ideSize != realSize) {
      Logger.err("Flash Chip configuration wrong!\n");
    } else {
      Logger.info("Flash Chip configuration ok.\n");
    }

}

void info()
{
    ESP.getChipId(); // returns the ESP8266 chip ID as a 32-bit integer.
    ESP.getFlashChipId(); // returns the flash chip ID as a 32-bit integer.
    ESP.getFlashChipSpeed(); // returns the flash chip frequency, in Hz.
    ESP.getCoreVersion(); // returns a String containing the core version.
    ESP.getSdkVersion(); // returns the SDK version as a char.
    ESP.getCpuFreqMHz(); // returns the CPU frequency in MHz as an unsigned 8-bit integer.

}


void heap_status(const char* what) {
  // we could use getFreeHeap() getMaxFreeBlockSize() and getHeapFragmentation()
  // or all at once:
  uint32_t free;
  uint16_t max;
  uint8_t frag;
  ESP.getHeapStats(&free, &max, &frag);

  Serial.printf("free: %5d - max: %5d - frag: %3d%% <- ", free, max, frag);
  // %s requires a malloc that could fail, using println instead:
  Serial.println(what);
}

void corruption_test() {
    Serial.printf("CRC check: %s\n", ESP.checkFlashCRC() ? "OK" : "ERROR");
}

void readVoltage() { // read internal VCC
  float volts = ESP.getVcc();
  Serial.printf("The internal VCC reads %1.2f volts\n", volts / 1000);
}

void resetReason() {
    String resetCause = ESP.getResetReason();
    Serial.println(resetCause);
    resetCount = 0;
    if ((resetCause == "External System") || (resetCause == "Power on")) {
      Serial.println(F("I'm awake and starting the Low Power tests"));
    }
}
