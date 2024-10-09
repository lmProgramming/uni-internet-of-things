#include <Arduino.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

// Napisz program, który w reakcji na przyciski będzie rozjaśniał i ściemniał wybraną diodę. 
// Na przykład, dioda zielona, po przytrzymaniu przycisku zielonego, będzie rozjaśniała się, 
// a po przytrzymaniu czerwonego będzie ściemniała się.

int chosen_diode = LED_GREEN;
int intensity = 122;

int DELAY = 15;

void initRGB()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(chosen_diode, OUTPUT);
    analogWrite(chosen_diode, intensity);
}

void initButtons()
{
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void setup()
{
    initRGB();
    initButtons();
}

void loop()
{
    if (digitalRead(GREEN_BUTTON) == LOW && intensity > 0)
    {
        intensity--;
        analogWrite(chosen_diode, intensity);
        delay(DELAY);
    }

    if (digitalRead(RED_BUTTON) == LOW && intensity < 255)
    {
        intensity++;
        analogWrite(chosen_diode, intensity);
        delay(DELAY);
    }
}
