#include <iostream>
#include <lvgl.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include <WiFi.h>

#include <Adafruit_NeoPixel.h>

//#define TFT_WIDTH       170
//#define TFT_HEIGHT      320
//TFT_MOSI 35
//TFT_SCLK 36
//TFT_CS   7
//TFT_DC   39
//TFT_RST  40

static const uint16_t screenWidth  = TFT_WIDTH;
static const uint16_t screenHeight = TFT_HEIGHT;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

const char ssid[] = "VOKAMISLINK";
const char password[] = "RUDRAFTDODGER911";

WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliary variables to store the current output state
String output26State = "off";
String output27State = "off";

static long strength = 0;

static lv_obj_t *wifi = nullptr;
static lv_obj_t *label = nullptr;
static lv_obj_t *arc = nullptr;

static lv_style_t style_arc;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p){
    uint32_t width = (area->x2 - area->x1 + 1);
    uint32_t height = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, width, height);
    tft.pushColors((uint16_t *) &color_p->full, width * height, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

void setupScreen(){
    wifi = lv_label_create(lv_scr_act());
    label = lv_label_create(lv_scr_act());
    arc = lv_arc_create(lv_scr_act());

    lv_arc_set_end_angle(arc, 0);
    lv_obj_set_size(arc, 80, 80);
    lv_arc_set_range(arc, -90, -20);
    lv_obj_remove_style(arc, nullptr, LV_PART_KNOB);
    lv_style_init(&style_arc);

    static lv_style_t styleText;
    lv_style_init(&styleText);
    lv_style_set_text_font(&styleText, &lv_font_montserrat_20);
    lv_obj_add_style(wifi, &styleText, 0);
    lv_obj_add_style(label, &styleText, 0);

    lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 50);
    lv_obj_align(wifi, LV_ALIGN_CENTER, 0, -54);
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI): ");
    Serial.print(rssi);
    Serial.println(" dBm");
}

void initWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        printWifiStatus();
        delay(1000);
    }
    Serial.println(WiFi.localIP());
    server.begin();
}

void setup(){
    pinMode(0, INPUT);

//    pinMode(TFT_I2C_POWER, OUTPUT);
//    digitalWrite(TFT_I2C_POWER, HIGH);
//    pinMode(TFT_BACKLITE, OUTPUT);
//    digitalWrite(TFT_BACKLITE, LOW);
//    delay(10);
    // initialize TFT
    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(1);
//    digitalWrite(TFT_BACKLITE, HIGH);

    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, nullptr, screenWidth * 10);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    setupScreen();

    initWiFi();
    lv_label_set_text(wifi, WiFi.SSID().c_str());
}

void loop(){
    //-30 dBm	Amazing
    //-67 dBm	Very Good
    //-70 dBm	Okay
    //-80 dBm	Not Good
    //-90 dBm	Unusable
    auto rssi = WiFi.RSSI();
    if (strength != rssi){
        strength = rssi;
        String myString = String(strength) + " dBm";
        lv_arc_set_value(arc, strength);
        lv_label_set_text(label, myString.c_str());
        if (rssi > -40){
            lv_style_set_arc_color(&style_arc, lv_color_make(0, 255, 0));
        }
        if (rssi < -40 && rssi > -60){
            lv_style_set_arc_color(&style_arc, lv_color_make(64, 255, 0));
        }
        if (rssi < -60 && rssi > -70){
            lv_style_set_arc_color(&style_arc, lv_color_make(191, 255, 0));
        }
        if (rssi < -70 && rssi > -80){
            lv_style_set_arc_color(&style_arc, lv_color_make(255, 255, 0));
        }
        if (rssi < -80){
            lv_style_set_arc_color(&style_arc, lv_color_make(255, 0, 0));
        }
//        lv_obj_add_style(arc, &style_arc, LV_PART_INDICATOR);
    }

    try {
        WiFiClient client = server.available();   // Listen for incoming clients

        if (client) {                             // If a new client connects,
            Serial.println("New Client.");          // print a message out in the serial port
            String currentLine = "";                // make a String to hold incoming data from the client
            while (client.connected()) {            // loop while the client's connected
                if (client.available()) {             // if there's bytes to read from the client,
                    char c = client.read();             // read a byte, then
                    Serial.write(c);                    // print it out the serial monitor
                    header += c;
                    if (c == '\n') {                    // if the byte is a newline character
                        // if the current line is blank, you got two newline characters in a row.
                        // that's the end of the client HTTP request, so send a response:
                        if (currentLine.length() == 0) {
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println("Connection: close");
                            client.println();

                            // turns the GPIOs on and off
                            if (header.indexOf("GET /26/on") >= 0) {
                                Serial.println("GPIO 26 on");
                                neopixelWrite(PIN_NEOPIXEL, 0, 0, 100);
                            } else if (header.indexOf("GET /26/off") >= 0) {
                                Serial.println("GPIO 26 off");
                                neopixelWrite(PIN_NEOPIXEL, 0, 0, 1);
                            } else if (header.indexOf("GET /27/on") >= 0) {
                                Serial.println("GPIO 27 on");
                                neopixelWrite(PIN_NEOPIXEL, 100, 0, 0);
                            } else if (header.indexOf("GET /27/off") >= 0) {
                                Serial.println("GPIO 27 off");
                                neopixelWrite(PIN_NEOPIXEL, 1, 0, 0);
                            }
                            // Display the HTML web page
                            client.println("<!DOCTYPE html><html>");
                            client.println(
                                    "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                            client.println("<link rel=\"icon\" href=\"data:,\">");
                            // CSS to style the on/off buttons
                            // Feel free to change the background-color and font-size attributes to fit your preferences
                            client.println(
                                    "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                            client.println(
                                    ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                            client.println(".button2 {background-color: #555555;}</style></head>");

                            // Web Page Heading
                            client.println("<body><h1>ESP32 Web Server</h1>");

                            // Display current state, and ON/OFF buttons for GPIO 26
                            client.println("<p>GPIO 26 - State " + output26State + "</p>");
                            // If the output26State is off, it displays the ON button
                            if (output26State == "off") {
                                client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
                            } else {
                                client.println(
                                        "<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
                            }

                            // Display current state, and ON/OFF buttons for GPIO 27
                            client.println("<p>GPIO 27 - State " + output27State + "</p>");
                            // If the output27State is off, it displays the ON button
                            if (output27State == "off") {
                                client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
                            } else {
                                client.println(
                                        "<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
                            }
                            client.println("</body></html>");

                            // The HTTP response ends with another blank line
                            client.println();
                            // Break out of the while loop
                            break;
                        } else { // if you got a newline, then clear currentLine
                            currentLine = "";
                        }
                    } else if (c != '\r') {  // if you got anything else but a carriage return character,
                        currentLine += c;      // add it to the end of the currentLine
                    }
                }
            }
            // Clear the header variable
            header = "";

            // Close the connection
            client.stop();
            Serial.println("Client disconnected.");
            Serial.println("");
        }
    }
    catch (std::exception const &e){
        Serial.println(e.what());
    }

    lv_timer_handler(); /* let the GUI do its work */
    delay(5);
}

int main(){
    try {
        setup();
    }
    catch (std::exception const &e){
        Serial.println(e.what());
    }

    while (1){
        try {
            loop();
        }
        catch (std::exception const &e){
            Serial.println(e.what());
        }
    }
}