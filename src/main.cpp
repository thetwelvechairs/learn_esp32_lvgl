#include <Arduino.h>
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"
#include "driver/i2s.h"
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>

static float bt_volume = 0.95;

Adafruit_NeoPixel pixels(1, 0, NEO_GRB + NEO_KHZ800);

// the callback (processes bluetooth data)
void bt_data_cb(const uint8_t *data, uint32_t len){
    if (len > 0){
        pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 0, 10));
        pixels.show();
    }
    // number of 16 bit samples
    int n = len / 2;

    // point to a 16bit sample
    int16_t* data16 = (int16_t*) data;

    // create a variable (potentially processed) that we'll pass via I2S
    int16_t fy;

    // Records number of bytes written via I2S
    size_t i2s_bytes_write = 0;

    for(int i = 0; i < n; i++){
        // put the current sample in fy
//        fy = *data16;
        fy = (int16_t)((*data16) * bt_volume);

        //making this value larger will decrease the volume(Very simple DSP!).
        fy /= 1;

        // write data to I2S buffer
        i2s_write(I2S_NUM_0, &fy, 2, &i2s_bytes_write, 10);

        //move to next memory address housing 16-bit data
        data16++;
    }
    pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 0, 0));
    pixels.show();
}


void printWifiStatus() {
    // // print the SSID of the network you're attached to:
    // Serial.print("SSID: ");
    // Serial.println(WiFi.SSID());

    // // print your board's IP address:
    // IPAddress ip = WiFi.localIP();
    // Serial.print("IP Address: ");
    // Serial.println(ip);

    // // print the received signal strength:
    // long rssi = WiFi.RSSI();
    // Serial.print("signal strength (RSSI): ");
    // Serial.print(rssi);
    // Serial.println(" dBm");
}


void initWiFi() {
//     WiFi.begin(ssid, password);
//     Serial.print("Connecting to WiFi...");
//     while (WiFi.status() != WL_CONNECTED) {
//         printWifiStatus();
//         delay(1000);
//     }
    // Serial.println(WiFi.localIP());
    // server.begin();
}


void initBluetooth(){
    // i2s configuration
    static const i2s_config_t i2s_config = {
            .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX),
            .sample_rate = 44100,
            .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
            .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
            .communication_format = I2S_COMM_FORMAT_STAND_I2S,
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // default interrupt priority
            .dma_buf_count = 8,
            .dma_buf_len = 1000,
            .use_apll = false,
            .tx_desc_auto_clear = true
    };

    // i2s pinout
    static const i2s_pin_config_t pin_config = {
            .bck_io_num = 32, //26
            .ws_io_num = 27,
            .data_out_num = 33, //
            .data_in_num = I2S_PIN_NO_CHANGE
    };

    // now configure i2s with constructed pinout and config
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, nullptr);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO);
    i2s_set_sample_rates(I2S_NUM_0, 44100);

    // set up bluetooth classic via bluedroid
    btStart();
    esp_bluedroid_init();
    esp_bluedroid_enable();

    // set up device name
    const char *dev_name = "ESP-EAKER";
    esp_bt_dev_set_device_name(dev_name);

    // initialize A2DP sink and set the data callback(A2DP is bluetooth audio)
    esp_a2d_sink_register_data_callback(bt_data_cb);
    esp_a2d_sink_init();

    // set discoverable and connectable mode, wait to be connected
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
}


void setupPixel(){
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    pixels.begin();
}


void setup(){
    setupPixel();
    initWiFi();
    initBluetooth();
}


void loop(){
    try {
        // WiFiClient client = server.available();   // Listen for incoming clients
        ESP_LOGI(TAG, "Disabling Bluetooth");
        pixels.clear();
        pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 0, 0));
        pixels.show();
    }
    catch (std::exception const &e){
        Serial.println(e.what());
    }
}



int main(){
    try {
        setup();
    }
    catch (std::exception const &e){
        Serial.println(e.what());
    }

    while (true){
        try {
            loop();
        }
        catch (std::exception const &e){
            Serial.println(e.what());
        }
    }
}