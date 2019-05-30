#include "LCD1602.h"
#include <cmath>

LCD lcd(D12,D11,D10,A4,A5,A6,A3);
Serial acce(D1, D0);
Serial gps(A7, A2);
DigitalOut led(LED1);

Ticker displayToLCD;
InterruptIn musicButton(D6);

int speed, pedal;//information to display
float rawSpeed;//raw speed data from gps
char zhenW[12], zhenV[70];//the raw data from sensors directly
char tokeW[12];//a copy of zhenW, to prevent conflict
char sign = 0;// 0 token processed, can read; 1 blocked
char counterW = 0, counterV = 0;// the byte coonter
int gpsSymble = 0;// to determin the location of speed data
char gpsResult[8];//the found speed data in hex

//to display the speed and frequency to display dynamically
void showData()
{
    lcd.erase();
    lcd.printf(" Speed:%3d KM/H\n Pedal:%3d  RPM", speed, pedal);
}

//dynamically read data from accelaration sensor
void getWinfo()
{
    //read a byte
    zhenW[counterW] = acce.getc();

    //return if blocked
    if (sign)
        return;

    //return if isn't head
    if (counterW == 0 && zhenW[0] != 0x55)
        return;
    
    //return if not accelaretion
    if (counterW == 1 && zhenW[1] != 0x52)
    {
        counterW = 0;
        return;
    }
    counterW++;

    //when full package received, copy to tokeW
    //sign set to 1 to prevent read
    if (counterW == 11)
    {
        counterW = 0;
        zhenW[11] = '\0';
        strcpy(tokeW, zhenW);
        sign = 1;
    }
}

//get gps data
void getVinfo()
{
    //read a byte
    zhenV[counterV] = gps.getc();

    //return if not head
    if (counterV == 0 && zhenV[0] != '$')
        return;

    //return if not package contains speed
    if (counterV == 3 && zhenV[3] != 'R')
    {
        counterV = 0;
        return;
    }
    
    //find place contains speed
    if (zhenV[counterV] == ',')
        ++gpsSymble;

    //read speed
    if (gpsSymble == 7)
    {   
        for (int i = 0; i < 8; ++i)
        {
            gpsResult[i] = gps.getc();
            if (gpsResult[i] == ',')
            {
                gpsResult[i] = '\0';
                break;
            }
        }
        
        //store rawspeed to gpsresult
        sscanf(gpsResult, "%f", &rawSpeed);

        //calculate speed in km/h
        speed = 1.85 * rawSpeed;
        counterV = 0;
        gpsSymble = 0;
        return;
    }
    counterV++;
}

void switchMusic()
{
    //default instruction code
    char a[8] = {0x7e, 0xff, 0x06, 0x17, 0x00, 0x00, 0x00, 0xef};
    //determine the playlist by pedal frequency
    if (abs(pedal) <= 10)
    {
        a[6] = 0x01;
    }
    else if (abs(pedal) > 10 && abs(pedal) <= 20)
    {
        a[6] = 0x02;
    }
    else
    {
        a[6] = 0x03;
    }
    //send instruction
    for (int i = 0; i < 8; ++i)
    {
        acce.putc(a[i]);
    }
}

int main()
{
    //initial lcd
    lcd.initial();
    //set baud
    acce.baud(9600);
    gps.baud(9600);
    //attach UART interrupt function
    acce.attach(getWinfo);
    gps.attach(getVinfo);
    //set ticker, automatically display data as a background task
    displayToLCD.attach(showData,0.25);
    //set music button
    musicButton.mode(PullDown);
    musicButton.fall(switchMusic);
    //a copy of acceleration data
    char finalWData[12];
    while(1)
    {
        strcpy(finalWData, tokeW);
        sign = 0;
        if (finalWData[1]==0x52)
        {
            pedal = ((short)(finalWData[7] << 8) | finalWData[6]) * 2000.0 / 32768.0 / 6.0;
            wait(0.1);
        }
    }
}