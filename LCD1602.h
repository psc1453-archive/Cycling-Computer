//
// Created by 彭世辰 on 2019-04-09.
//

#ifndef L432KC_LCD1602_H
#define L432KC_LCD1602_H

#include "mbed.h"

class LCD
{
private:
    DigitalOut RS;
    DigitalOut RW;
    DigitalOut E;
    BusOut DATA;
    char buffer[32];
    void pulse_E();

public:
    LCD(PinName rs, PinName rw, PinName e,
        PinName d4, PinName d5, PinName d6, PinName d7);
    void initial();
    void erase();
    int display(const char *output, int mode = 1);//mode1 auto next line mode0 not
    int rolling(const char *output);

    template<typename...T>
    int printf(const char *fmt,T...args)
    {
        sprintf(buffer, fmt,args...);
        return display(buffer);
    }
};

#endif //L432KC_LCD1602_H
