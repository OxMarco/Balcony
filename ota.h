/**
 * @brief Checks if a new firmware version is avaliable and performs an OTA update
 * @link https://www.bakke.online/index.php/2017/06/02/self-updating-ota-firmware-for-esp8266/
 */
String getMAC()
{
    uint8_t mac[6];
    char result[14];
    
    snprintf( result, sizeof( result ), "%02x%02x%02x%02x%02x%02x", mac[ 0 ], mac[ 1 ], mac[ 2 ], mac[ 3 ], mac[ 4 ], mac[ 5 ] );
    
    return String( result );
}

void checkForUpdates() {
    String mac = getMAC();
    String fwURL = String( fwUrlBase );
    fwURL.concat( mac );
    String fwVersionURL = fwURL;
    fwVersionURL.concat( ".version" );
    
    Logger.info( "Checking for firmware updates..." );
    Logger.info( "Firmware version URL: " );
    Logger.info( fwVersionURL );
    
    HTTPClient httpClient;
    httpClient.begin( fwVersionURL );
    int httpCode = httpClient.GET();
    if( httpCode == 200 ) {
        String newFWVersion = httpClient.getString();
        
    Logger.info( "Current firmware version: " );
    Logger.info( FW_VERSION );
    Logger.info( "Available firmware version: " );
    Logger.info( newFWVersion );
        
    int newVersion = newFWVersion.toInt();
        
    if( newVersion > FW_VERSION ) {
        Logger.info( "Preparing to update..." );
            
        String fwImageURL = fwURL;
        fwImageURL.concat( ".bin" );
        t_httpUpdate_return ret = ESPhttpUpdate.update( fwImageURL );
            
        switch(ret) {
                case HTTP_UPDATE_FAILED:
                    String tmp;
                    snprintf(tmp, sizeof(tmp), "HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                    Logger.err(tmp);
                break;
                    
                case HTTP_UPDATE_NO_UPDATES:
                    Logger.info("HTTP_UPDATE_NO_UPDATES");
                break;
            }
        }
        else {
            Logger.info( "Already on latest version" );
        }
    } else {
        Logger.err( "Firmware version check failed, got HTTP response code " );
        Logger.err( httpCode );
    }
    httpClient.end();
}
