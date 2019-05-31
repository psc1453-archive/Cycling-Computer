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
    void pulse_E();

public:
    //pin definition
    LCD(PinName rs, PinName rw, PinName e,
        PinName d4, PinName d5, PinName d6, PinName d7);
    //use this method first
    void initial();
    //clear the display
    void erase();
    //display static content
    int display(const char *output, int mode = 1);//mode1 auto next line mode0 not
    //rolling display
    int rolling(const char *output);
    //use like printf
    template<typename...T>
    int printf(const char *fmt,T...args)
    {
        char buffer[32];
        sprintf(buffer, fmt,args...);
        return display(buffer);
    }
};

#endif //L432KC_LCD1602_H
