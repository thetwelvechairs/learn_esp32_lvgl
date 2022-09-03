````
platformio -c clion init --ide clion

platformio -c clion run --target upload -e adafruit_feather_esp32_v2
````

Hardware
* Adafruit ESP32-S3 TFT Feather

Required drivers

* MacOS http://www.wch-ic.com/downloads/CH34XSER_MAC_ZIP.html
* Windows http://www.wch-ic.com/downloads/CH341SER_ZIP.html

### PlatformIO Dependencies

* lvgl@^8.3.1
* bodmer/TFT_eSPI@^2.4.75
* adafruit/Adafruit NeoPixel@^1.10.5

### Custom configs

* lv_conf.h to be copied into root of your lvgl folder

   
* User_Setup.h to be copied into root of your TFT_eSPI folder