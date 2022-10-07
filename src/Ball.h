#ifndef ESP32_TFT_GAME_BALL_H
#define ESP32_TFT_GAME_BALL_H


#include "LGFX_TFT_eSPI.h"
#include "ballBuffer.h"

class Ball {
public:
    const int32_t width = 16;
    const int32_t height = 16;
    int32_t x = 20;
    int32_t y = 20;
    int32_t x1 = 10;
    int32_t y1 = 10;
    uint32_t speed = 0;
    uint32_t direction = 0;
    uint32_t display_width = 0;
    uint32_t display_height = 0;

    TFT_eSprite* sprite{};

    explicit Ball();
    virtual ~Ball();

    void Move();

};


#endif //ESP32_TFT_GAME_BALL_H
