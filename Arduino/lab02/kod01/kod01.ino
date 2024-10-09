#include <Arduino.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

// Po naciśnięciu przycisku zielonego zmień kolor świecącej diody. 
// Przycisk czerwony włącza i wyłącza świecenie.

int DELAY_AFTER_PRESS = 50;

int leds[] = {LED_RED, LED_GREEN, LED_BLUE};
int led_index = 0;

void initRGB()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    
    for (int led : leds)
    {
        pinMode(led, OUTPUT);
        digitalWrite(led, LOW);
    }
}

void initButtons()
{
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

bool on = false;

void setup()
{
    initRGB();
    initButtons();
}

bool pressingGreen = false;
bool pressingRed = false;

void check_green_button()
{
    if (digitalRead(GREEN_BUTTON) != LOW)
    {
        pressingGreen = false;
        return;
    }
    
    if (pressingGreen || !on)
    {
        delay(DELAY_AFTER_PRESS);
        return;
    }
    
    digitalWrite(leds[led_index], LOW);

    led_index = (led_index + 1) % 3;

    digitalWrite(leds[led_index], HIGH);
    pressingGreen = true;
    delay(DELAY_AFTER_PRESS);
}

void check_red_button(){
    if (digitalRead(RED_BUTTON) != LOW)
    {        
        pressingRed = false;
        return;
    }

    if (pressingRed)
    {
        delay(DELAY_AFTER_PRESS);
        return;
    }
    
    on = !on;

    if (on)
    {
        digitalWrite(leds[led_index], HIGH);
    }
    else
    {
        digitalWrite(leds[led_index], LOW);
    }

    pressingRed = true;
    delay(DELAY_AFTER_PRESS);
}

void loop()
{
    check_green_button();

    check_red_button();
}
