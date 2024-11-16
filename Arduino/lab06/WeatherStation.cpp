#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/*
Przygotuj program, który uczyni zestaw laboratoryjny małą „stacją pogodową”.
Na wyświetlaczu LCD ma prezentować temperaturę wewnętrzną (czujnik wewnątrz zestawu) i zewnętrzną (sonda na przewodzie).
Ponadto program powinien pamiętać wartość maksymalną i minimalną temperatury mierzonej przez sondę na przewodzie.
*/

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define DEBOUNCING_PERIOD 100

LiquidCrystal_I2C lcd(0x27, 16, 2);

OneWire oneWire(A1);
DallasTemperature tempSensors(&oneWire);

enum State
{
    MAIN,
    STATS_IN,
    STATS_OUT,
};

State state = MAIN;

float max_temp_in = -10000;
float min_temp_in = 10000;
float max_temp_out = -10000;
float min_temp_out = 10000;

float temp_in = 0;
float temp_out = 0;

void get_temperature()
{
    tempSensors.requestTemperatures();
    temp_in = tempSensors.getTempCByIndex(1);
    temp_out = tempSensors.getTempCByIndex(0);

    if (temp_in > max_temp_in)
    {
        max_temp_in = temp_in;
    }
    if (temp_in < min_temp_in)
    {
        min_temp_in = temp_in;
    }
    if (temp_out > max_temp_out)
    {
        max_temp_out = temp_out;
    }
    if (temp_out < min_temp_out)
    {
        min_temp_out = temp_out;
    }
}

void setup()
{
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);

    tempSensors.begin();

    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void show_stats(bool inside)
{
    lcd.setCursor(0, 0);

    char buffer[40];

    if (inside)
    {
        sprintf(buffer, "Max IN: %8s", String(max_temp_in, 4).c_str());
        lcd.print(buffer);
        sprintf(buffer, "Min IN: %8s", String(min_temp_in, 4).c_str());
        lcd.setCursor(0, 1);
        lcd.print(buffer);
    }
    else
    {
        sprintf(buffer, "Max OUT: %8s", String(max_temp_out, 4).c_str());
        lcd.print(buffer);
        sprintf(buffer, "Min OUT: %8s", String(min_temp_out, 4).c_str());
        lcd.setCursor(0, 1);
        lcd.print(buffer);
    }
}

void show_main()
{
    char buffer[40];
    sprintf(buffer, "Temp  IN: %8s", String(temp_in, 4).c_str());
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    sprintf(buffer, "Temp OUT: %8s", String(temp_out, 4).c_str());
    lcd.setCursor(0, 1);
    lcd.print(buffer);
}

void loop()
{
    get_temperature();

    if (digitalRead(GREEN_BUTTON) == LOW)
    {
        delay(DEBOUNCING_PERIOD);
        if (digitalRead(GREEN_BUTTON) == LOW)
        {
            state = state == MAIN ? STATS_IN : MAIN;
        }

        while (digitalRead(GREEN_BUTTON) == LOW)
            ;
    }

    if (digitalRead(RED_BUTTON) == LOW)
    {
        delay(DEBOUNCING_PERIOD);
        if (digitalRead(RED_BUTTON) == LOW)
        {
            state = state == MAIN ? STATS_OUT : MAIN;
        }

        while (digitalRead(RED_BUTTON) == LOW)
            ;
    }

    if (state == MAIN)
    {
        show_main();
    }
    else
    {
        show_stats(state == STATS_IN);
    }
}