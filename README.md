# Balcony Project
![logo](https://cdn.dribbble.com/users/1797520/screenshots/5983584/thatsamole_thumbnail.jpg)

## Abstract
The n-th project about multiple redundant power-independent sensor boxes to monitor environmental parameters of a balcony garden.

What makes it different from other projects is the modular and easily extensible structure and the extensive debug logging. The final goal is to create a robust IoT device able to run for several years without requiring maintenance.

## Description
The design consists of two separate elements, multiple sensor boxes and a single hub. 
Sensor boxes are autonomous thanks to a solar panel while the hub is connected to a wall adapter and to the Internet via WiFi through the MQTT IoT protocol. A remote server acts as a MQTT broker and performs data analysis.

The sensor boxes perform environmental data collection and stream it via MQTT to the hub at a configurable frequency. They support OTA updates from a server and can receive new congurations on a separate channel. Errors are streamed via MQTT to the hub that logs and uploads them to the remote server. In case of need, sensor boxes can save errors locally on SPIFFS to stream them later when reconnecting to the hub.

The hub acts as a gateway forwarding MQTT connections to a remote server and, in case of power issues, logging sensor data to be streamed when the connection is re-established.

Using sensor data, the remote server chooses watering timing and sends a message to the hub to open the main water valve. Manual flow control valves can be used to reduce the water flow if needed. In the future, sensor boxes could independently open secondary valves in order to selectively irrigate according to the crop needs. Plants like rosemary need very little watering while basil or parsley prefers a wet ground.

Sensor boxes perform light, temperature, humidity, pressure and soil moisture measurements while the hub only checks the internal temperature and humidity to identify possible moisture problems.
Thus, sensors need special enclosures to allow enough airflow to prevent moisture without altering the environmental parameters. My choice is 3mm plywood with white varnish to reflect sunlight.
The main hub, instead, can be put in a classical enclosed plastic box with silica gel.

## Long-Term Reliability
Most IoT solutions suffer several issues in the long run, some dependent on the hardware choice others on the programming.

### Problems
1. Insufficient power supply coming from the solar panel
2. Connection issues
3. Eccessive insulation or moisture damage
4. Components degradation

### Possible Solutions
1. Leverage deep sleep functions of ESP SoC and use a higher wattage solar panel
2. WiFi Mesh and WiFi repeaters
3. Stevenson's screen boxes and silica gel (except when performing weather monitoring)
4. Reduce file write and sensor reading


## Bill of Materials

### Hub-specific
* Esp8266 Wemos D1 mini pro + external antenna
* RTC logger shield + 2GB SD card
* 5V Relay board
* 12V 1/2" solenoid N/C water valve
* 12V wall adapter
* DHT11 temperature and humidity sensor

### Box-specific
* Esp8266 Wemos D1 mini
* 18650 Li-ion battery
* Wemos LiPo charger and 5V booster circuit
* 1.25W 5V polycrystalline solar panel
* BME280 environmental sensor (temperature, humidity and pressure )
* MAX44009 ambient light sensor
* Sealed soil moisture sensor

## Working Diagram
![diagram](https://github.com/grcasanova/Balcony/blob/master/mqtt_diagram.png?raw=true =400x400)




## Credits
* logo - Fabrizio Garda (https://dribbble.com/fgarda)
* OTA (https://www.bakke.online/index.php/2017/06/02/self-updating-ota-firmware-for-esp8266/)