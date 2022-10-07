#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <Adafruit_NeoPixel.h>
#include <random>

class LGFX : public lgfx::LGFX_Device{
    lgfx::Panel_ST7789  _panel_instance;
    lgfx::Bus_SPI       _bus_instance;

public:
    LGFX(){
        auto busCfg = _bus_instance.config();
        auto panelCfg = _panel_instance.config();

        busCfg.freq_write = 27000000;
        busCfg.pin_sclk = 5;
        busCfg.pin_mosi = 19;
        busCfg.pin_dc   = 33;
        _bus_instance.config(busCfg);
        _panel_instance.setBus(&_bus_instance);

        panelCfg.pin_cs           =    32;
        panelCfg.pin_rst          =    -1;
        panelCfg.panel_width      =   240;
        panelCfg.panel_height     =   320;
        panelCfg.invert           =  true;
        _panel_instance.config(panelCfg);

        setPanel(&_panel_instance);
    }
};

#include <LGFX_TFT_eSPI.h>
#include "Ball.h"
#include "Flipper.h"

static TFT_eSPI tft;

static Ball ball;
static Flipper flipperL;
static Flipper flipperR;

static TFT_eSprite buffer1(&tft);
static TFT_eSprite buffer2(&tft);
static TFT_eSprite *screenBuffers[2] = {&buffer1, &buffer2};

static TFT_eSprite ballSprite;

static const size_t frameCount = 2;

static uint32_t sec, psec;
static size_t fps = 0, frame_count = 0;

static uint32_t display_width;
static uint32_t display_height;

std::vector<TFT_eSprite> sprites{4};

void setupPixel(){
    Adafruit_NeoPixel pixels(1, 0, NEO_GRB + NEO_KHZ800);
    pixels.begin();
    pixels.setPixelColor(0, 1, 0, 0);
    pixels.show();
}

void frames(){
    ++frame_count;
    sec = lgfx::millis() / 1000;
    if (psec != sec) {
        psec = sec;
        fps = frame_count;
        frame_count = 0;
    }
}

void drawTarget(int x, int y){
    tft.drawLine(x-5, y-5, x+5, y+5, TFT_WHITE);
    tft.drawLine(x-5, y+5, x+5, y-5, TFT_WHITE);
}

void setup(){
    try{
        setupPixel();

        tft.init();
        tft.setRotation(2);
        tft.setColorDepth(16);
        tft.setSwapBytes(true);
        tft.fillScreen(TFT_ORANGE);

        display_width = tft.width();
        display_height = tft.height();

        tft.setPivot(0, 0);

        screenBuffers[0]->setColorDepth(tft.getColorDepth());
        screenBuffers[0]->setFont(&fonts::Font2);
        screenBuffers[0]->createSprite(display_width, display_height);
        screenBuffers[1]->setColorDepth(tft.getColorDepth());
        screenBuffers[1]->setFont(&fonts::Font2);
        screenBuffers[1]->createSprite(display_width, display_height);

        ballSprite.setSwapBytes(true);
        ballSprite.createSprite(ball.width, ball.height);
        ballSprite.pushImage(0, 0, ball.width, ball.height, ballBuffer);
        ball.sprite = &ballSprite;
        ball.display_width = display_width;
        ball.display_height = display_height;

        for (int n = 0; n < 4; n++) {
            sprites.at(n).setSwapBytes(true);
            sprites.at(n).createSprite(flipperL.width, flipperL.height);
            auto temp = flipperLD;
            switch (n) {
                case 0:
                    temp = flipperLD;
                    break;
                case 1:
                    temp = flipperLU;
                    break;
                case 2:
                    temp = flipperRD;
                    break;
                case 3:
                    temp = flipperRU;
                    break;
                default:
                    temp = flipperLD;
                    break;
            }

            sprites.at(n).pushImage(0, 0, flipperL.width, flipperL.height, temp);
        }

        tft.startWrite();
    }
    catch (std::exception const &e){
        tft.print(e.what());
    }
}

void loop(){
    try {
        ball.Move();

        screenBuffers[0]->clear();
        drawTarget(display_width >> 1, (display_height >> 1) + (display_height >> 2));
        drawTarget(display_width >> 1, display_height >> 1);
        drawTarget(display_width >> 1, display_height >> 2);

        drawTarget(40, 40);
        drawTarget(40, 120);
        drawTarget(40, 200);
        drawTarget(40, 280);
        drawTarget(200, 40);
        drawTarget(200, 120);
        drawTarget(200, 200);
        drawTarget(200, 280);

        sprites.at(random(0,2)).pushSprite(screenBuffers[0], 40, 240, TFT_BLACK);
        sprites.at(random(2,4)).pushSprite(screenBuffers[0], 136, 240, TFT_BLACK);

        screenBuffers[0]->setCursor(0, 0);
        screenBuffers[0]->setFont(&fonts::Font4);
        screenBuffers[0]->setTextColor(0xFFFFFFU);
        screenBuffers[0]->printf("FPS: %d", fps);

        ball.sprite->pushSprite(screenBuffers[0], ball.x, ball.y, TFT_BLACK);

        screenBuffers[0]->pushSprite(&tft, 0, 0);

        tft.display();

        frames();

    }
    catch (std::exception const &e){
        tft.print(e.what());
    }
}