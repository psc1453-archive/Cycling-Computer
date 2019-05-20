#include "LCD1602.h"

<<<<<<< HEAD
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
=======
LCD lcd(D12,D11,D10,A4,A5,A6,A7);
Serial acce(D1, D0);

Ticker displayToLCD;

int speed, pedal = 102;
char rawData[12];
char fullToken[12];
char counter = 0;
>>>>>>> f27872cc6644a786d95593cb8d49d827df48cbb6

void showDataToLCD()
{
    lcd.erase();
    lcd.printf("   Speed:%4d\n   Pedal:%4d", speed, pedal);
}

<<<<<<< HEAD
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
        speed=gps.getc();
        
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
        speed = rawSpeed * 1.85;
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
=======
void getFullToken()
{
    rawData[counter] = acce.getc();
    if (counter == 0 && rawData[0] != 0x55)
        return;
    counter++;
    if (counter == 11)
    {
        counter = 0;
        rawData[11] = '\0';
        strcpy(fullToken, rawData);
>>>>>>> f27872cc6644a786d95593cb8d49d827df48cbb6
    }
}

int main()
{
    lcd.initial();
<<<<<<< HEAD
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
=======
    displayToLCD.attach(showDataToLCD,0.5);

    acce.baud(9600);
    acce.attach(getFullToken);

    char stableDataToken[12];
    int calculateTemp;
    while(1)
    {
        strcpy(stableDataToken, fullToken);
        
        if (stableDataToken[1]==0x52)
        {
            calculateTemp = ((short)stableDataToken[7]<<8|stableDataToken[6])*2000/32768;
            speed = calculateTemp > 2000 ? calculateTemp - 4000 : calculateTemp;
>>>>>>> f27872cc6644a786d95593cb8d49d827df48cbb6
            wait(0.1);
        }
    }
}