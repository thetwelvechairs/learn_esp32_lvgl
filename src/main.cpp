#include <iostream>
#include <lvgl.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Adafruit_NeoPixel.h>

#define TFT_WIDTH       135
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

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p){
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *) &color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

void setup(){
    // initialize TFT
    tft.init();
//    tft.setRotation(1);
    neopixelWrite(PIN_NEOPIXEL, 0, 1, 0);

    lv_init();

    lv_disp_draw_buf_init(&draw_buf, buf, nullptr, screenWidth * 10);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /* Create simple label */
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "LVGL");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    /*Create a button and use the new styles*/
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_set_pos(btn, 10, 10);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_add_style(btn, &style_btn, 0);
    lv_obj_add_style(btn, &style_btn_pressed, LV_STATE_PRESSED);

    /*Add a label to the button*/
    lv_obj_t *label2 = lv_label_create(btn);
    lv_label_set_text(label2, "Button");
    lv_obj_center(label2);

    // Turn on TFT backlight
    pinMode(TFT_BACKLITE, OUTPUT);
    digitalWrite(TFT_BACKLITE, HIGH);

    // Turn on TFT I2C power supply
    pinMode(TFT_I2C_POWER, OUTPUT);
    digitalWrite(TFT_I2C_POWER, HIGH);
}

void loop(){
    lv_timer_handler(); /* let the GUI do its work */
    delay(5);
}

int main(){
    setup();
    while (1){
        loop();
    }
}