#include <Arduino.h>
#include <util/atomic.h>
#include <LiquidCrystal_I2C.h>

#define LED_RED 6
#define LED_BLUE 3

#define ENCODER1 A2
#define ENCODER2 A3

#define DEBOUNCING_PERIOD 100

LiquidCrystal_I2C lcd(0x27, 16, 2);

void printResults(int val)
{
    char buffer[40];
    sprintf(buffer, "Encoder: %3d", val);
    lcd.setCursor(2, 0);
    lcd.print(buffer);
}

void myAction(int val)
{
    printResults(val);
    analogWrite(LED_RED, val);
    analogWrite(LED_BLUE, 255 - val);
}

void setup()
{
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(ENCODER1, INPUT_PULLUP);
    pinMode(ENCODER2, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();
    myAction(0);

    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT10);
}

volatile int encoder1 = HIGH;
volatile int encoder2 = HIGH;
volatile unsigned long encoder_timestamp = 0UL;

ISR(PCINT1_vect)
{
    encoder1 = digitalRead(ENCODER1);
    encoder2 = digitalRead(ENCODER2);
    encoder_timestamp = millis();
}

int encoderValue = 0;
int lastEn1 = LOW;
unsigned long last_change_timestamp = 0UL;
void loop()
{

    int en1;
    int en2;
    unsigned long timestamp;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        en1 = encoder1;
        en2 = encoder2;
        timestamp = encoder_timestamp;
    }

    if (en1 == LOW && timestamp > last_change_timestamp + DEBOUNCING_PERIOD)
    {
        if (en2 == HIGH)
        {
            if (encoderValue < 255)
                encoderValue += 15;
        }
        else
        {
            if (encoderValue > 0)
                encoderValue -= 15;
        }
        last_change_timestamp = timestamp;

        myAction(encoderValue);
    }
    lastEn1 = en1;
}