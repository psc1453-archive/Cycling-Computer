#include "LCD1602.h"

LCD lcd(D12,D11,D10,A4,A5,A6,A7);
DigitalOut led(LED1);

Ticker ticker;

int a = 1, b = 2;

void show()
{
    lcd.erase();
    lcd.printf("%d",a);
}
void ldd()
{
    led = !led;
}

int main()
{
    lcd.initial();
    ticker.attach(show,1);
    for(int i = 0; i < 100;++i)
    {
        a+=1;
        wait(0.5);
    }
}