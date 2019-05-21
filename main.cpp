#include "LCD1602.h"

LCD lcd(D12,D11,D10,A4,A5,A6,A3);
Serial acce(D1, D0);
Serial gps(A7, A2);
DigitalOut led(LED1);

Ticker displayToLCD;


int speed, pedal;
float rawSpeed;
char zhenW[12], zhenV[70];
char tokeW[12];
char sign = 0, counterW = 0, counterV = 0;
int gpsSymble = 0;
char gpsResult[8];

void showData()
{
    lcd.erase();
    lcd.printf(" Speed:%3d KM/H\n Pedal:%3d  RPM", speed, pedal);
}

void getWinfo()
{
    zhenW[counterW] = acce.getc();
    if (counterW == 0 && zhenW[0] != 0x55)
        return;
    
    if (counterW == 1 && zhenW[1] != 0x52)
    {
        counterW = 0;
        return;
    }
    counterW++;
    if (counterW == 11)
    {
        counterW = 0;
        zhenW[11] = '\0';
        strcpy(tokeW, zhenW);
    }
}

void getVinfo()
{
    zhenV[counterV] = gps.getc();
    if (counterV == 0 && zhenV[0] != '$')
        return;
    if (counterV == 3 && zhenV[3] != 'R')
    {
        counterV = 0;
        return;
    }
    
    if (zhenV[counterV] == ',')
        ++gpsSymble;

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
        
        sscanf(gpsResult, "%f", &rawSpeed);
        speed = 1.85 * rawSpeed;
        counterV = 0;
        gpsSymble = 0;
        return;
    }
    counterV++;
}

void music()
{
    char a[8] = {0x7e, 0xff, 0x06, 0x01, 0, 0, 0, 0xef};
    for (int i = 0; i < 8; ++i)
    {
        acce.putc(a[i]);
    }
}

int main()
{
    lcd.initial();
    displayToLCD.attach(showData,0.5);
    acce.baud(9600);
    acce.attach(getWinfo);
    gps.baud(9600);
    gps.attach(getVinfo);

    char finalWData[12];
    int temp;
    music();
    while(1)
    {
        strcpy(finalWData, tokeW);
        if (finalWData[1]==0x52)
        {
            pedal = ((short)(finalWData[7] << 8) | finalWData[6]) * 2000.0 / 32768.0 / 6.0;
            wait(0.1);
        }
    }
}