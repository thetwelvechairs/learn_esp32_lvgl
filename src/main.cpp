#include <iostream>
#include <lvgl.h>
#include <TFT_eSPI.h>


static const uint16_t screenWidth  = 170;
static const uint16_t screenHeight = 320;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
unsigned long targetTime = 0; // Used for testing draw times
const uint16_t WAIT = 500;

static lv_style_t style_btn;
static lv_style_t style_btn_pressed;
static lv_style_t style_btn_red;

void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}

void setup(){
    lv_init();

    tft.init();
//    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    lv_disp_draw_buf_init( &draw_buf, buf, nullptr, screenWidth * 10 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /* Create simple label */
    lv_obj_t *label = lv_label_create( lv_scr_act() );
    lv_label_set_text( label, "LVGL");
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

    /*Create a button and use the new styles*/
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    /* Remove the styles coming from the theme
     * Note that size and position are also stored as style properties
     * so lv_obj_remove_style_all will remove the set size and position too */
//    lv_obj_remove_style_all(btn);
    lv_obj_set_pos(btn, 10, 10);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_add_style(btn, &style_btn, 0);
    lv_obj_add_style(btn, &style_btn_pressed, LV_STATE_PRESSED);

    /*Add a label to the button*/
//    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);}

void loop(){
    lv_timer_handler(); /* let the GUI do its work */
    delay( 5 );

//    targetTime = millis();
//
//    // First we test them with a background colour set
//    tft.setTextSize(1);
//    tft.fillScreen(TFT_BLACK);
//    tft.setTextColor(TFT_GREEN, TFT_BLACK);
//
//    tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
//    tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 2);
//    tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 32, 2);
//    tft.drawString("abcdefghijklmnopqrstuvw", 0, 48, 2);
//    int xpos = 0;
//    xpos += tft.drawString("xyz{|}~", 0, 64, 2);
//    tft.drawChar(127, xpos, 64, 2);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BLACK);
//    tft.setTextColor(TFT_GREEN, TFT_BLACK);
//
//    tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
//    tft.drawString("/0123456789:;", 0, 26, 4);
//    tft.drawString("<=>?@ABCDE", 0, 52, 4);
//    tft.drawString("FGHIJKLMNO", 0, 78, 4);
//    tft.drawString("PQRSTUVWX", 0, 104, 4);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BLACK);
//    tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
//    tft.drawString("defghijklmno", 0, 26, 4);
//    tft.drawString("pqrstuvwxyz", 0, 52, 4);
//    xpos = 0;
//    xpos += tft.drawString("{|}~", 0, 78, 4);
//    tft.drawChar(127, xpos, 78, 4);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BLACK);
//    tft.setTextColor(TFT_BLUE, TFT_BLACK);
//
//    tft.drawString("012345", 0, 0, 6);
//    tft.drawString("6789", 0, 40, 6);
//    tft.drawString("apm-:.", 0, 80, 6);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BLACK);
//    tft.setTextColor(TFT_RED, TFT_BLACK);
//
//    tft.drawString("0123", 0, 0, 7);
//    tft.drawString("4567", 0, 60, 7);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BLACK);
//    tft.drawString("890:.", 0, 0, 7);
//    tft.drawString("", 0, 60, 7);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BLACK);
//    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
//
//    tft.drawString("0123", 0, 0, 8);
//    tft.drawString("4567", 0, 72, 8);
//    delay(WAIT);;
//
//    tft.fillScreen(TFT_BLACK);
//    tft.drawString("890:.", 0, 0, 8);
//    tft.drawString("", 0, 72, 8);
//    delay(WAIT);;
//
//    tft.setTextSize(2);
//    tft.fillScreen(TFT_BLACK);
//    tft.setTextColor(TFT_GREEN, TFT_BLACK);
//
//    tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
//    tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 32, 2);
//    tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 64, 2);
//    tft.drawString("abcdefghijklmnopqrstuvw", 0, 96, 2);
//    xpos = 0;
//    xpos += tft.drawString("xyz{|}~", 0, 128, 2);
//    tft.drawChar(127, xpos, 128, 2);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BLACK);
//    tft.setTextColor(TFT_GREEN, TFT_BLACK);
//
//    tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
//    tft.drawString("/0123456789:;", 0, 52, 4);
//    tft.drawString("<=>?@ABCDE", 0, 104, 4);
//    tft.drawString("FGHIJKLMNO", 0, 156, 4);
//    tft.drawString("PQRSTUVWX", 0, 208, 4);
//    delay(WAIT);
//    tft.fillScreen(TFT_BLACK);
//    tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
//    tft.drawString("defghijklmno", 0, 52, 4);
//    tft.drawString("pqrstuvwxyz", 0, 104, 4);
//    xpos = 0;
//    xpos += tft.drawString("{|}~", 0, 156, 4);
//    tft.drawChar(127, xpos, 156, 4);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BLACK);
//    tft.setTextColor(TFT_BLUE, TFT_BLACK);
//
//    tft.drawString("01234", 0, 0, 6);
//    tft.drawString("56789", 0, 80, 6);
//    tft.drawString("apm-:.", 0, 160, 6);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BLACK);
//    tft.setTextColor(TFT_RED, TFT_BLACK);
//
//    tft.drawString("0123", 0, 0, 7);
//    tft.drawString("4567", 0, 120, 7);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BLACK);
//    tft.drawString("890:.", 0, 0, 7);
//    tft.drawString("", 0, 120, 7);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BLACK);
//    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
//
//    tft.drawString("0123", 0, 0, 8);
//    tft.drawString("4567", 0, 144, 8);
//    delay(WAIT);;
//
//    tft.fillScreen(TFT_BLACK);
//    tft.drawString("890:.", 0, 0, 8);
//    tft.drawString("", 0, 144, 8);
//    delay(WAIT);;
//
//    tft.setTextColor(TFT_MAGENTA, TFT_BROWN);
//
//    tft.drawNumber(millis() - targetTime, 0, 180, 4);
//    delay(4000);;
//
//    // Now test them with transparent background
//    targetTime = millis();
//
//    tft.setTextSize(1);
//    tft.fillScreen(TFT_BROWN);
//    tft.setTextColor(TFT_GREEN);
//
//    tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
//    tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 2);
//    tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 32, 2);
//    tft.drawString("abcdefghijklmnopqrstuvw", 0, 48, 2);
//    xpos = 0;
//    xpos += tft.drawString("xyz{|}~", 0, 64, 2);
//    tft.drawChar(127, xpos, 64, 2);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BROWN);
//    tft.setTextColor(TFT_GREEN);
//
//    tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
//    tft.drawString("/0123456789:;", 0, 26, 4);
//    tft.drawString("<=>?@ABCDE", 0, 52, 4);
//    tft.drawString("FGHIJKLMNO", 0, 78, 4);
//    tft.drawString("PQRSTUVWX", 0, 104, 4);
//
//    delay(WAIT);
//    tft.fillScreen(TFT_BROWN);
//    tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
//    tft.drawString("defghijklmno", 0, 26, 4);
//    tft.drawString("pqrstuvwxyz", 0, 52, 4);
//    xpos = 0;
//    xpos += tft.drawString("{|}~", 0, 78, 4);
//    tft.drawChar(127, xpos, 78, 4);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BROWN);
//    tft.setTextColor(TFT_BLUE);
//
//    tft.drawString("012345", 0, 0, 6);
//    tft.drawString("6789", 0, 40, 6);
//    tft.drawString("apm-:.", 0, 80, 6);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BROWN);
//    tft.setTextColor(TFT_RED);
//
//    tft.drawString("0123", 0, 0, 7);
//    tft.drawString("4567", 0, 60, 7);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BROWN);
//    tft.drawString("890:.", 0, 0, 7);
//    tft.drawString("", 0, 60, 7);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BROWN);
//    tft.setTextColor(TFT_YELLOW);
//
//    tft.drawString("0123", 0, 0, 8);
//    tft.drawString("4567", 0, 72, 8);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BROWN);
//    tft.drawString("890:.", 0, 0, 8);
//    tft.drawString("", 0, 72, 8);
//    delay(WAIT);
//
//    tft.setTextSize(2);
//    tft.fillScreen(TFT_BROWN);
//    tft.setTextColor(TFT_GREEN);
//
//    tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
//    tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 32, 2);
//    tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 64, 2);
//    tft.drawString("abcdefghijklmnopqrstuvw", 0, 96, 2);
//    xpos = 0;
//    xpos += tft.drawString("xyz{|}~", 0, 128, 2);
//    tft.drawChar(127, xpos, 128, 2);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BROWN);
//    tft.setTextColor(TFT_GREEN);
//
//    tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
//    tft.drawString("/0123456789:;", 0, 52, 4);
//    tft.drawString("<=>?@ABCDE", 0, 104, 4);
//    tft.drawString("FGHIJKLMNO", 0, 156, 4);
//    tft.drawString("PQRSTUVWX", 0, 208, 4);
//    delay(WAIT);
//    tft.fillScreen(TFT_BROWN);
//    tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
//    tft.drawString("defghijklmno", 0, 52, 4);
//    tft.drawString("pqrstuvwxyz", 0, 104, 4);
//    xpos = 0;
//    xpos += tft.drawString("{|}~", 0, 156, 4);
//    tft.drawChar(127, xpos, 156, 4);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BROWN);
//    tft.setTextColor(TFT_BLUE);
//
//    tft.drawString("01234", 0, 0, 6);
//    tft.drawString("56789", 0, 80, 6);
//    tft.drawString("apm-:.", 0, 160, 6);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BROWN);
//    tft.setTextColor(TFT_RED);
//
//    tft.drawString("0123", 0, 0, 7);
//    tft.drawString("4567", 0, 120, 7);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BROWN);
//    tft.drawString("890:.", 0, 0, 7);
//    tft.drawString("", 0, 120, 7);
//    delay(WAIT);
//
//    tft.fillScreen(TFT_BROWN);
//    tft.setTextColor(TFT_YELLOW);
//
//    tft.drawString("0123", 0, 0, 8);
//    tft.drawString("4567", 0, 144, 8);
//    delay(WAIT);;
//
//    tft.fillScreen(TFT_BROWN);
//    tft.drawString("890:.", 0, 0, 8);
//    tft.drawString("", 0, 144, 8);
//    delay(WAIT);;
//
//    tft.setTextColor(TFT_MAGENTA);
//
//    tft.drawNumber(millis() - targetTime, 0, 180, 4);
//    delay(4000);;
}