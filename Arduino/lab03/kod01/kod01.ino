/*
Napisz program,  który wykorzysta oba przyciski do zwiększania/zmniejszania pewnego licznika.

Licznik startuje z wartością 0.
Naciśnięcie przycisku zielonego zwiększa licznik o 1 i wyświetla aktualną wartość na Serial Monitor.
Naciśnięcie przycisku czerwonego zmniejsza licznik o 1 i wyświetla aktualną wartość na Serial Monitor.
Program powinien obsługiwać także wciśnięcie obu przycisków na raz, co sygnalizowane będzie mignięciem 
diody tyle razy ile wynosi aktualna wartość licznika. Odpowiedni komunikat wyświetli się także na Serial 
Monitor. Wartość licznika w trakcie obsługi wciśnięcia obu przycisków nie powinna ulec zmianie.
Należy zadbać o pozbycie się drgań z przycisków
*/

#include <LiquidCrystal_I2C.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

LiquidCrystal_I2C lcd(0x27, 16, 2);

int counter = 0;

void initRGB()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void initButtons()
{
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void initLCD()
{
    lcd.init();
    lcd.backlight();
}

void blink(int times){    
    for (int i = 0; i < times; i++)
    {
        digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(100);                      // wait for a second
        digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
        delay(900);
    }
}

void setup()
{
    initRGB();

    initButtons();
    
    initLCD();
}

void check_button(int first_button, int second_button, int counter_change)
{
    if (digitalRead(GREEN_BUTTON) == LOW)
    {
        delay(1000);
        if (digitalRead(RED_BUTTON) == LOW)
        {
            blink(counter);
        }
        else
        {
            counter += 1;
        }
    }
}

void loop()
{
    delay(10);
    if (digitalRead(GREEN_BUTTON) == LOW)
    {
        delay(1000);
        if (digitalRead(RED_BUTTON) == LOW)
        {
            blink(counter);
        }
        else
        {
            counter += 1;
        }
    }
    if (digitalRead(RED_BUTTON) == LOW){
        if (digitalRead(GREEN_BUTTON) == LOW)
        {
            blink(counter);
        }
        else
        {
            counter -= 1;
        }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(counter);
}