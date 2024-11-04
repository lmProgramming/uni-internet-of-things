#include "ButtonHandler.h"

const int BUTTON1_PIN = 2;
const int BUTTON2_PIN = 3;
const int LED1_PIN = 13;
const int LED2_PIN = 12;

ButtonHandler button1(BUTTON1_PIN);
ButtonHandler button2(BUTTON2_PIN);

bool led1_state = false;
bool led2_state = false;
unsigned long blink_interval = 500;
unsigned long last_blink_time = 0;
bool is_blinking = false;

void button1_short_press()
{
    led1_state = !led1_state;
    digitalWrite(LED1_PIN, led1_state);
}

void button1_long_press()
{
    is_blinking = !is_blinking;
    if (!is_blinking)
    {
        digitalWrite(LED1_PIN, LOW);
        led1_state = false;
    }
}

void button2_short_press()
{
    led2_state = !led2_state;
    digitalWrite(LED2_PIN, led2_state);
}

void button2_long_press()
{
    blink_interval = (blink_interval == 500) ? 200 : 500;
}

void setup()
{
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);

    button1.begin();
    button1.set_short_press_callback(button1_short_press);
    button1.set_long_press_callback(button1_long_press);

    button2.begin();
    button2.set_short_press_callback(button2_short_press);
    button2.set_long_press_callback(button2_long_press);
}

void loop()
{
    if (is_blinking)
    {
        unsigned long current_time = millis();
        if (current_time - last_blink_time >= blink_interval)
        {
            led1_state = !led1_state;
            digitalWrite(LED1_PIN, led1_state);
            last_blink_time = current_time;
        }
    }
}
