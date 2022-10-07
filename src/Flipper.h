#ifndef ESP32_TFT_GAME_FLIPPER_H
#define ESP32_TFT_GAME_FLIPPER_H

#include <vector>
#include <cstring>
#include "LGFX_TFT_eSPI.h"
#include "flipperLD.h"
#include "flipperLU.h"
#include "flipperRD.h"
#include "flipperRU.h"

class Flipper {
public:
    int_fast16_t width = 64;
    int_fast16_t height = 50;

    Flipper();
    ~Flipper();

    void Init();
    void Move();

private:
//    unsigned short bufferL [2][3200];
//    unsigned short bufferR [2][3200];
};


#endif //ESP32_TFT_GAME_FLIPPER_H
