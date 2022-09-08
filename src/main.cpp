#include <iostream>
#include <lvgl.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include <WiFi.h>

#include <Adafruit_NeoPixel.h>

#define TFT_WIDTH       320
#define TFT_HEIGHT      240
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

static lv_style_t style_btn;
static lv_style_t style_btn_pressed;

const char ssid[] = "VOKAMISLINK";
const char password[] = "RUDRAFTDODGER911";

//-30 dBm	Amazing
//-67 dBm	Very Good
//-70 dBm	Okay
//-80 dBm	Not Good
//-90 dBm	Unusable

WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

static long strength = 0;
static char *strengthString;


void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p){
    uint32_t width = (area->x2 - area->x1 + 1);
    uint32_t height = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, width, height);
    tft.pushColors((uint16_t *) &color_p->full, width * height, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

void lv_example_calendar_1(){
    lv_obj_t  * calendar = lv_calendar_create(lv_scr_act());
    lv_obj_set_size(calendar, 185, 185);
    lv_obj_align(calendar, LV_ALIGN_CENTER, 0, 27);
//    lv_obj_add_event_cb(calendar, event_handler, LV_EVENT_ALL, NULL);

    lv_calendar_set_today_date(calendar, 2021, 02, 23);
    lv_calendar_set_showed_date(calendar, 2021, 02);

    /*Highlight a few days*/
    static lv_calendar_date_t highlighted_days[3];       /*Only its pointer will be saved so should be static*/
    highlighted_days[0].year = 2021;
    highlighted_days[0].month = 02;
    highlighted_days[0].day = 6;

    highlighted_days[1].year = 2021;
    highlighted_days[1].month = 02;
    highlighted_days[1].day = 11;

    highlighted_days[2].year = 2022;
    highlighted_days[2].month = 02;
    highlighted_days[2].day = 22;

    lv_calendar_set_highlighted_dates(calendar, highlighted_days, 3);

#if LV_USE_CALENDAR_HEADER_DROPDOWN
    lv_calendar_header_dropdown_create(calendar);
#elif LV_USE_CALENDAR_HEADER_ARROW
    lv_calendar_header_arrow_create(calendar);
#endif
    lv_calendar_set_showed_date(calendar, 2021, 10);
}

void lv_example_checkbox_1(){
    lv_obj_set_flex_flow(lv_scr_act(), LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(lv_scr_act(), LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * cb;
    cb = lv_checkbox_create(lv_scr_act());
    lv_checkbox_set_text(cb, "Apple");
//    lv_obj_add_event_cb(cb, event_handler, LV_EVENT_ALL, NULL);

    cb = lv_checkbox_create(lv_scr_act());
    lv_checkbox_set_text(cb, "Banana");
    lv_obj_add_state(cb, LV_STATE_CHECKED);
//    lv_obj_add_event_cb(cb, event_handler, LV_EVENT_ALL, NULL);

    cb = lv_checkbox_create(lv_scr_act());
    lv_checkbox_set_text(cb, "Lemon");
    lv_obj_add_state(cb, LV_STATE_DISABLED);
//    lv_obj_add_event_cb(cb, event_handler, LV_EVENT_ALL, NULL);

    cb = lv_checkbox_create(lv_scr_act());
    lv_obj_add_state(cb, LV_STATE_CHECKED | LV_STATE_DISABLED);
    lv_checkbox_set_text(cb, "Melon\nand a new line");
//    lv_obj_add_event_cb(cb, event_handler, LV_EVENT_ALL, NULL);

    lv_obj_update_layout(cb);
}

void lv_example_line_1(){
    /*Create an array for the points of the line*/
    static lv_point_t line_points[] = { {5, 5}, {70, 70}, {120, 10}, {180, 60}, {240, 10} };

    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 8);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_line_rounded(&style_line, true);

    /*Create a line and apply the new style*/
    lv_obj_t * line1;
    line1 = lv_line_create(lv_scr_act());
    lv_line_set_points(line1, line_points, 5);     /*Set the points*/
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_center(line1);
}

//static lv_obj_t *label;
//void basic(){
//    /* Create simple label */
//    label = lv_label_create(lv_scr_act());
//    lv_label_set_text(label, "");
//    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
//}

static lv_obj_t *label = nullptr;
static lv_obj_t *bar1 = nullptr;
void lv_example_bar_1(){
    label = lv_label_create(lv_scr_act());
    bar1 = lv_bar_create(lv_scr_act());

    lv_obj_set_size(bar1, 200, 20);
    lv_obj_center(bar1);

    lv_label_set_text(label, "");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
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
//    pinMode(TFT_BACKLITE, OUTPUT);
//    digitalWrite(TFT_BACKLITE, LOW);
//    pinMode(TFT_I2C_POWER, OUTPUT);
//    digitalWrite(TFT_I2C_POWER, LOW);

    lv_init();

    // initialize TFT
    tft.init();
    tft.setRotation(1);
    tft.fillRect(20, 20, screenWidth, screenHeight, TFT_BLACK);

    neopixelWrite(PIN_NEOPIXEL, 0, 1, 0);

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


//    lv_example_bar_3();
    lv_example_bar_1();
//    basic();
//    lv_example_checkbox_1();

    // Turn on TFT backlight
    pinMode(TFT_BACKLITE, OUTPUT);
    digitalWrite(TFT_BACKLITE, HIGH);
    // Turn on TFT I2C power supply
    pinMode(TFT_I2C_POWER, OUTPUT);
    digitalWrite(TFT_I2C_POWER, HIGH);

    initWiFi();
}

void loop(){
    if (strength != WiFi.RSSI()){
        strength = WiFi.RSSI();
        String myString = String(strength) + " dBm";
        lv_bar_set_value(bar1, strength, LV_ANIM_ON);
        lv_label_set_text(label, myString.c_str());
        Serial.println(strength);
        Serial.println(myString);
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