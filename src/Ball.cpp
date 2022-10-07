#include "Ball.h"

Ball::Ball() = default;

void Ball::Move(){
    x += x1;
    if (x <= 0){
        x = 0;
        if (x1 <= 0) x1 = -x1;
    }
    else if (x >= display_width){
        x = display_width -1;
        if (x1 > 0) x1 = -x1;
    }

    y += y1;
    if (y <= 0){
        y = 0;
        if (y1 <= 0) y1 = -y1;
    }
    else if (y >= display_height){
        y = display_height - 1;
        if (y1 > 0) y1 = -y1;
    }
}

Ball::~Ball() = default;


