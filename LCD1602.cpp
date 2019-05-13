//
// Created by 彭世辰 on 2019-04-09.
//

#include "mbed.h"
#include "LCD1602.h"
#include <cstdlib>
#include <cstdio>

LCD::LCD(PinName rs, PinName rw, PinName e, PinName d4, PinName d5,
         PinName d6, PinName d7):RS(rs), RW(rw), E(e),
                                 DATA(d4, d5, d6, d7){}

void LCD::pulse_E()
{
    E = 1;
    wait_ms(1);
    E = 0;
    wait_ms(1);
}

void LCD::initial()
{
    wait_ms(20);
    RS = 0;
    E = 0;

    //settings
    DATA = 0x2;
    pulse_E();
    DATA = 0x8;
    pulse_E();
    wait_ms(5);
    DATA = 0x2;
    pulse_E();
    DATA = 0x8;
    pulse_E();
    wait_ms(5);
    DATA = 0x2;
    pulse_E();
    DATA = 0x8;
    pulse_E();
    wait_ms(5);

    //off
    DATA = 0x0;
    pulse_E();
    DATA = 0x8;
    pulse_E();

    //clear
    DATA = 0x0;
    pulse_E();
    DATA = 0x1;
    pulse_E();

    //mode
    DATA = 0x0;
    pulse_E();
    DATA = 0x6;
    pulse_E();

    //on
    DATA = 0x0;
    pulse_E();
    DATA = 0xc;
    pulse_E();
}

void LCD::erase()
{
    RS = 0;
    DATA = 0x0;
    pulse_E();
    DATA = 0x1;
    pulse_E();
}

int LCD::display(const char *output, int mode)
{
    //write
    int len = strlen(output);
    RS = 1;
    bool alreadyRowSecond = false;
    for (int i = 0; i < len; ++i)
    {
        if (i == 16 && !alreadyRowSecond)
        {
            if (mode)
            {
                RS = 0;
                DATA = 0xc;
                pulse_E();
                DATA = 0x0;
                pulse_E();

                alreadyRowSecond = true;

                RS = 1;
            }
            else
            {
                return 16;
            }
        }
        
        if (output[i] != '\n')
            {
                DATA = output[i] >> 4;
                pulse_E();
                DATA = output[i];
                pulse_E();

                alreadyRowSecond = true;
            }
        else
        {
            RS = 0;
            DATA = 0xc;
            pulse_E();
            DATA = 0x0;
            pulse_E();

            RS = 1;
        }
    }
    return len;
}

int LCD::rolling(const char *output)
{
    int len = strlen(output);
    if (len <= 16)
    {
        display(output);  
    }
    else
    {
        int new_len = strlen(output) + 20;
        char *buffer = new char[new_len + 1];
        strcpy(buffer, output);
        strcat(buffer, "     ");
        strncat(buffer, output, 15);
        while(1)
        {
            for (int i = 0; i < len + 5; ++i)
            {
                erase();
                display(buffer + i, 0);
                wait(0.2);
            }
        }
        delete [] buffer;
    }
    return len;
}