#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

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

LiquidCrystal_I2C lcd(0x27, 16, 2);

int counter = 0;
bool pressing_red = false;
bool pressing_green = false; 

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

void blink(int times)
{    
    for (int i = 0; i < times; i++)
    {
        digitalWrite(LED_BUILTIN, HIGH); 
        delay(100);                      
        digitalWrite(LED_BUILTIN, LOW);  
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
    if (digitalRead(first_button) == LOW)
    {
        while (digitalRead(first_button) == LOW)
        {
            if (digitalRead(second_button) == LOW)
            {
                blink(counter);
                if (counter <= 0)
                {
                    delay(100);
                }
                return;
            }
        }

        counter += counter_change;
    }
}

void loop()
{
    check_button(GREEN_BUTTON, RED_BUTTON, 1);
    check_button(RED_BUTTON, GREEN_BUTTON, -1);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(counter);

    delay(50);
}