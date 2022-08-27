````
platformio -c clion init --ide clion

platformio -c clion run --target upload -e adafruit_feather_esp32_v2
````

Hardware
* Adafruit ESP32 Feather V2
* Adafruit 1.9" 320x170 Color IPS TFT Display - ST7789

Required drivers

* MacOS http://www.wch-ic.com/downloads/CH34XSER_MAC_ZIP.html
* Windows http://www.wch-ic.com/downloads/CH341SER_ZIP.html

### PlatformIO Dependencies

* adafruit/Adafruit NeoPixel@^1.10.5
* fdivitto/FabGL@^1.0.8

### Custom configs

* in TFTControllerGeneric.cpp:
```#define TFT_SPI_WRITE_FREQUENCY 27000000```

   
* in fabglconf.h:
```#define TFT_170x320 "\"TFT_172x320\" 172 320"```
and 
```m_rotOffsetY = 34;```