#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

#define RED_BUTTON 2
#define DEBOUNCING_PERIOD 100

/*
Proszę napisać program, który na wyświetlaczu LCD będzie nawigował po dwupoziomowym menu.
Nawigacja po elementach menu ma zostać zrealizowana z wykorzystaniem enkodera oraz przycisków.
Enkoder przesuwa się po elementach menu z tego samego poziomu, natomiast przycisk wchodzi do podmenu
lub uruchamia akcję z wybraną operacją z menu.

Menu ma mieć poniższą postać:

LED options
    Power [ON | OFF]
    Red
    Green
    Blue
Display
    Backlight [ON | OFF]
    Selector [> | - | <custom char>]
Temperature
    Sensor IN
    Sensor OUT
    Units [C | F]
About
Poniżej przykład akcji jakie są uruchamiane dla poszczególnych opcji menu:

Option	Action
LED options->Power	Turns on and off the RGB LED. Proper state should be displayed on LCD
LED options->Red	Changes intensity of red LED colour
LED options->Green	Changes intensity of green LED colour
LED options->Blue	Changes intensity of blue LED colour
Display->Backlight	Turns on and off the LCD backlight. Proper state should be displayed on LCD
Display->Selector	In this option user can select char that is pointing selected option on LCD. For a larger number of points, a custom character must be defined in the display memory, e.g. an arrow
Temperature->Sensor IN
Displays only some mocked temperature value. Don’t need to read a real temperature from sensor.

Temperature->Sensor OUT
Displays only some mocked temperature value. Don’t need to read a real temperature from sensor.

Temperature->Units	Units: changes temperature units between C and F. Currently select unit should be displayed when executing mock of temperature sensors.
About	Displays Name and Surname author of program

Na najwyższą ocenę należy wykorzystać przerwania do obsługi enkodera. Sposób wyświetalnia menu na wyświetlacze należy zaprojektować samemu. Ważne jest, aby było czytelnie i intuicyjnie.
*/

LiquidCrystal_I2C lcd(0x27, 16, 2);

int pressCounter = 0;
volatile unsigned long buttonTimestamp = 0UL;
unsigned long previousButtonTimestamp = 0UL;
unsigned long elapsedStartTimestamp = 0UL;

void interruptAction()
{
    buttonTimestamp = millis();
}

void printResults(int count, unsigned long time)
{
    char buffer[40];
    sprintf(buffer, "Press count%5d", count);
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    sprintf(buffer, "Time [ms]%7lu", time);
    // sprintf(buffer, "Time [s]%4lu.%03d", time / 1000, time % 1000);
    lcd.setCursor(0, 1);
    lcd.print(buffer);
}

void setup()
{
    pinMode(RED_BUTTON, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();
    printResults(0, 0UL);

    attachInterrupt(digitalPinToInterrupt(RED_BUTTON), interruptAction, FALLING);
}

void loop()
{
    noInterrupts();
    unsigned long localButtonTimestamp = buttonTimestamp;
    interrupts();

    if (localButtonTimestamp != previousButtonTimestamp && millis() > localButtonTimestamp + DEBOUNCING_PERIOD)
    {
        if (digitalRead(RED_BUTTON) == LOW)
        {
            pressCounter++;

            unsigned long elapsedTime = localButtonTimestamp - elapsedStartTimestamp;
            elapsedStartTimestamp = localButtonTimestamp;

            printResults(pressCounter, elapsedTime);
        }
        previousButtonTimestamp = localButtonTimestamp;
    }
}