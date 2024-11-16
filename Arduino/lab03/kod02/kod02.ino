#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define DEBOUNCE_PERIOD 10UL

/*
Napisz program, który będzie migał trzema kolorami diody RGB.

Każdy kolor diody ma mieć zmienny czas migania wyrażony w milisekundach.
Program startuje z czasami R-0,9s, G-1,0s, B-1,1s.
Proszę wykorzystać przyciski do zmiany wartości czasów migania. Można przyjąć, że przycisk czerwony
wybiera aktualną diodę do zmiany wartości, a zielony zmienia wartość migania cyklicznie w przedziale 0,5s - 2,0s z krokiem co 0,1s.
Aktualna nastawa czasu jest wyświetlana na wyświetlaczu LCD.
Program przygotować tak, aby nie blokować przełączania diod wzajemnie, nie blokować wykonania programu.
*/

unsigned long blinkTimes[3] = {900, 1000, 1100};
unsigned long previousMillis[3] = {0, 0, 0};
int ledPins[3] = {LED_RED, LED_GREEN, LED_BLUE};
int currentLedIndex = 0;

int red_debounced_button_state = HIGH;
int red_previous_reading = HIGH;
unsigned long red_last_change_time = 0UL;

int green_debounced_button_state = HIGH;
int green_previous_reading = HIGH;
unsigned long green_last_change_time = 0UL;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
    for (int i = 0; i < 3; i++)
    {
        pinMode(ledPins[i], OUTPUT);
    }

    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);

    lcd.init();
    lcd.backlight();
}

void displayBlinkTime(int index, char ledChar, float time)
{
    if (currentLedIndex == index)
    {
        lcd.print((char)toupper(ledChar));
    }
    else
    {
        lcd.print((char)tolower(ledChar));
    }
    lcd.print(": ");
    lcd.print(time);
    lcd.print("s ");
}

bool is_button_pressed(int button, int *debounced_button_state, int *previous_reading, unsigned long *last_change_time)
{
    bool isPressed = false;

    int current_reading = digitalRead(button);

    if (*previous_reading != current_reading)
    {
        *last_change_time = millis();
    }

    if (millis() - *last_change_time > DEBOUNCE_PERIOD)
    {
        if (current_reading != *debounced_button_state)
        {
            if (*debounced_button_state == HIGH && current_reading == LOW)
            {
                isPressed = true;
            }
            *debounced_button_state = current_reading;
        }
    }

    *previous_reading = current_reading;

    return isPressed;
}

void loop()
{
    unsigned long currentMillis = millis();
    if (is_button_pressed(RED_BUTTON, &red_debounced_button_state, &red_previous_reading, &red_last_change_time))
    {
        currentLedIndex = (currentLedIndex + 1) % 3;
    }
    if (is_button_pressed(GREEN_BUTTON, &green_debounced_button_state, &green_previous_reading, &green_last_change_time))
    {
        blinkTimes[currentLedIndex] += 100;
        if (blinkTimes[currentLedIndex] > 2000)
        {
            blinkTimes[currentLedIndex] = 500;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (currentMillis - previousMillis[i] >= blinkTimes[i])
        {
            previousMillis[i] = currentMillis;
            digitalWrite(ledPins[i], !digitalRead(ledPins[i]));
        }
    }

    lcd.setCursor(0, 0);
    displayBlinkTime(0, 'r', blinkTimes[0] / 1000.0);
    displayBlinkTime(1, 'g', blinkTimes[1] / 1000.0);

    lcd.setCursor(0, 1);
    displayBlinkTime(2, 'b', blinkTimes[2] / 1000.0);
}