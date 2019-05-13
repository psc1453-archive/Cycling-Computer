#include "LCD1602.h"

LCD lcd(D12,D11,D10,A4,A5,A6,A7);

Ticker displayToLCD;

int speed = 12, pedal = 102;

void showData()
{
    lcd.erase();
    lcd.printf("   Speed:%4d\n   Pedal:%4d", speed, pedal);
}

int main()
{
    lcd.initial();
    displayToLCD.attach(showData,0.5);

    while(1)
    {
        speed = rand()%50;
        pedal = rand()%120;
        wait(0.5);
    }
}