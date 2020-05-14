/**
 * @todo blink if too many errors arise
 */
enum class log_level { DEBUG, WARNING, ERROR };
class Log {
public:
    void init(log_level lvl, String filename = "log.dat", bool serial_enable = false)
    {
        _log_lvl = lvl;
        _logfile = filename;
        _serial = serial_enable;
        
        if (_serial)
        {
            Serial.begin(115200);
            Serial.setTimeout(2000);
        }
    }
    
    void setLevel(log_level lvl)
    {
        _log_lvl = lvl;
    }
    
    void info(String str = "\n")
    {
        if (_log_lvl != log_level::DEBUG)
            return;
        
        if (_serial) Serial.println(str);
        write(str);
    }
    
    void warn(String str = "\n")
    {
        if (_log_lvl != log_level::DEBUG && _log_lvl != log_level::WARNING)
            return;
        
        if (_serial) Serial.println(str);
        write(str);
    }
    
    void err(String str = "\n")
    {
        if (_serial) Serial.println(str);
        write(str);
    }
    
    
    void upload()
    {
        File file = SPIFFS.open(_logfile.c_str(), "r"); // Open it
        if (!file) {
            if (_log_lvl == log_level::DEBUG) Serial.println("Error opening file for reading");
            return;
        }
        
        HTTPClient http;
        String tmp;
        
        if (_log_lvl == log_level::DEBUG) Serial.println("upload start");
                
        String logData;
        
        while (file.available()){
            logData += char(file.read());
        }
        file.close();
        
        if (_log_lvl == log_level::DEBUG) {
            Serial.println("=====================================");
            Serial.println(logData);
            Serial.println("=====================================");
        }
        
        if (_log_lvl == log_level::DEBUG) Serial.println("upload start");
        
        http.begin("http://requestbin.net/r/rus025ru");
        http.addHeader("Content-Type", "text/plain");  //Specify content-type header
        int httpCode = http.POST("data=" + logData);
        //    String response = http.getString();
        //    response.trim();
        http.writeToStream(&Serial);
        http.end();
        
        if (httpCode == 200){
            SPIFFS.remove(_logfile.c_str());
        } else {
            if (_log_lvl == log_level::DEBUG) Serial.println("Submission unsuccessful - appending to existing log...");
        }
        
        if (_log_lvl == log_level::DEBUG) Serial.println("upload end");
    }
    
private:
    void write(String str)
    {
        DateTime now = rtc.now();
        String txt = "[";
        txt += now.day();
        txt += "/";
        txt += now.month();
        txt += " ";
        txt += now.hour();
        txt += ":";
        txt += now.minute();
        txt += ":";
        txt += now.second();
        txt += "] ";
        txt += str;
        
        File file = SPIFFS.open(_logfile.c_str(), "a+"); // Append or create it
        if (!file) {
            if (_log_lvl == log_level::DEBUG) Serial.println("Error opening file for writing");
            return;
        }
        
        int bytesWritten = file.println(txt);
        
        if (bytesWritten <= 0) {
            if (_log_lvl == log_level::DEBUG) Serial.println("File write failed");
        }
        
        file.close();
    }
    
    log_level _log_lvl;
    String _logfile;
    bool _serial;
} Logger;
