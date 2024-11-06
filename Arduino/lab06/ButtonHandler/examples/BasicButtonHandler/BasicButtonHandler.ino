#include <ButtonHandler.h>
#include <Arduino.h>

#define BUTTON_PIN 2
#define LED_PIN 13

ButtonHandler button(BUTTON_PIN);

void shortPress()
{
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void longPress()
{
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        digitalWrite(LED_PIN, LOW);
        delay(200);
    }
}

void setup()
{
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    button.begin();
    button.set_short_press_callback(shortPress);
    button.set_long_press_callback(longPress);
}

void loop()
{
    // The button handling is interrupt-driven
    // so the loop can be empty or do other tasks
}
