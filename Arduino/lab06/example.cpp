#include "ButtonHandler.h"
#include <LiquidCrystal_I2C.h>

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define DEBOUNCING_PERIOD 100

#define LED_RED 6
#define LED_BLUE 3

LiquidCrystal_I2C lcd(0x27, 16, 2);

ButtonHandler button1(RED_BUTTON);
ButtonHandler button2(GREEN_BUTTON);

int button1_short_press_count = 0;
int button1_long_press_count = 0;

int button2_short_press_count = 0;
int button2_long_press_count = 0;

void button1_short_press()
{
    button1_short_press_count += 1;
    digitalWrite(LED_RED, !digitalRead(LED_RED));
}

void button1_long_press()
{
    button1_long_press_count += 1;
    digitalWrite(LED_RED, !digitalRead(LED_RED));
}

void button2_short_press()
{
    button2_short_press_count += 1;
    digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
}

void button2_long_press()
{
    button2_long_press_count += 1;
    digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
}

void setup()
{
    Serial.begin(9600);

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    button1.begin();
    button1.set_short_press_callback(button1_short_press);
    button1.set_long_press_callback(button1_long_press);

    button2.begin();
    button2.set_short_press_callback(button2_short_press);
    button2.set_long_press_callback(button2_long_press);

    lcd.init();
    lcd.backlight();
    lcd.clear();

    ButtonHandler::instances[0]->set_debounce_time(50);
    ButtonHandler::instances[1]->set_debounce_time(250);
}

void loop()
{
    lcd.setCursor(0, 0);
    lcd.print("Red S:");
    lcd.print(button1_short_press_count);
    lcd.print(" L:");
    lcd.print(button1_long_press_count);

    lcd.setCursor(0, 1);
    lcd.print("Grn S:");
    lcd.print(button2_short_press_count);
    lcd.print(" L:");
    lcd.print(button2_long_press_count);
}