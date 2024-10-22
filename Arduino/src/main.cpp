#include <Arduino.h>
#include <util/atomic.h>
#include <LiquidCrystal_I2C.h>

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

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define ENCODER1 A2
#define ENCODER2 A3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define DEBOUNCING_PERIOD 100

LiquidCrystal_I2C lcd(0x27, 16, 2);

enum MenuItemType
{
    ACTION,
    SUBMENU
};

struct MenuItem
{
    const char *name;
    MenuItemType type;
    void (*action)();
    MenuItem *submenu;
    int submenuSize;
};

void toggleLEDPower();
void changeRedIntensity();
void changeGreenIntensity();
void changeBlueIntensity();
void toggleBacklight();
void selectChar();
void displaySensorIN();
void displaySensorOUT();
void changeUnits();
void displayAbout();

MenuItem ledOptions[] = {
    {"Power [ON | OFF]", ACTION, toggleLEDPower, nullptr, 0},
    {"Red", ACTION, changeRedIntensity, nullptr, 0},
    {"Green", ACTION, changeGreenIntensity, nullptr, 0},
    {"Blue", ACTION, changeBlueIntensity, nullptr, 0}};

MenuItem displayOptions[] = {
    {"Backlight [ON | OFF]", ACTION, toggleBacklight, nullptr, 0},
    {"Selector [> | - | <custom char>]", ACTION, selectChar, nullptr, 0}};

MenuItem temperatureOptions[] = {
    {"Sensor IN", ACTION, displaySensorIN, nullptr, 0},
    {"Sensor OUT", ACTION, displaySensorOUT, nullptr, 0},
    {"Units [C | F]", ACTION, changeUnits, nullptr, 0}};

MenuItem mainMenu[] = {
    {"LED options", SUBMENU, nullptr, ledOptions, 4},
    {"Display", SUBMENU, nullptr, displayOptions, 2},
    {"Temperature", SUBMENU, nullptr, temperatureOptions, 3},
    {"About", ACTION, displayAbout, nullptr, 0}};

MenuItem *currentMenu = mainMenu;
int currentMenuSize = 4;
int currentIndex = 0;
bool inSubMenu = false;
char custom_char = '>';

volatile int encoder1 = HIGH;
volatile int encoder2 = HIGH;
volatile unsigned long encoder_timestamp = 0UL;
volatile unsigned long last_change_timestamp = 0UL;

ISR(PCINT1_vect)
{
    encoder1 = digitalRead(ENCODER1);
    encoder2 = digitalRead(ENCODER2);
    encoder_timestamp = millis();
}

void updateDisplay()
{
    lcd.clear();

    int starting_index = currentIndex != currentMenuSize - 1 ? currentIndex : currentIndex - 1;

    Serial.flush();

    for (int i = starting_index; i <= starting_index + 1; i++)
    {
        Serial.print(i);
        delay(1);
        Serial.print(currentMenu[i].name);
        delay(1);

        if (i == currentIndex)
        {
            lcd.print(custom_char);
        }
        else
        {
            lcd.print(" ");
        }
        lcd.print(currentMenu[i].name);
        lcd.setCursor(0, i + 1);
    }
}

void setup()
{
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(ENCODER1, INPUT_PULLUP);
    pinMode(ENCODER2, INPUT_PULLUP);
    pinMode(RED_BUTTON, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();

    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT10);

    updateDisplay();
}

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
            currentIndex = (currentIndex + 1) % currentMenuSize;
        }
        else
        {
            currentIndex = (currentIndex - 1 + currentMenuSize) % currentMenuSize;
        }
        last_change_timestamp = timestamp;
        updateDisplay();
    }

    if (digitalRead(RED_BUTTON) == LOW)
    {
        delay(50); // Debounce
        if (digitalRead(RED_BUTTON) == LOW)
        {
            if (currentMenu[currentIndex].type == ACTION)
            {
                currentMenu[currentIndex].action();
            }
            else if (currentMenu[currentIndex].type == SUBMENU)
            {
                currentMenu = currentMenu[currentIndex].submenu;
                currentMenuSize = currentMenu[currentIndex].submenuSize;
                currentIndex = 0;
                inSubMenu = true;
                updateDisplay();
            }
        }
        while (digitalRead(RED_BUTTON) == LOW)
            ; // Wait for button release
    }
}

void toggleLEDPower()
{
    // Implement LED power toggle
}

void changeRedIntensity()
{
    // Implement red LED intensity change
}

void changeGreenIntensity()
{
    // Implement green LED intensity change
}

void changeBlueIntensity()
{
    // Implement blue LED intensity change
}

void toggleBacklight()
{
    // Implement backlight toggle
}

void selectChar()
{
    // Implement character selection
}

void displaySensorIN()
{
    // Implement sensor IN display
}

void displaySensorOUT()
{
    // Implement sensor OUT display
}

void changeUnits()
{
    // Implement unit change
}

void displayAbout()
{
    lcd.clear();
    lcd.print("Author: John Doe");
    delay(2000);
    updateDisplay();
}