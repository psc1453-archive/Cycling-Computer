#include "LCD1602.h"

LCD lcd(D12,D11,D10,A4,A5,A6,A7);
Serial acce(D1, D0);

Ticker displayToLCD;

int speed, pedal = 102;
char rawData[12];
char fullToken[12];
char counter = 0;

void showDataToLCD()
{
    lcd.erase();
    lcd.printf("   Speed:%4d\n   Pedal:%4d", speed, pedal);
}

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
    }
}

int main()
{
    lcd.initial();
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
            wait(0.1);
        }
    }
}