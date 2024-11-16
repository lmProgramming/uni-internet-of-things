#include <Arduino.h>
#include <util/atomic.h>
#include <LiquidCrystal_I2C.h>
#include <Vector.h>

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
LED options->Power
Turns on and off the RGB LED. Proper state should be displayed on LCD

LED options->Red
Changes intensity of red LED colour

LED options->Green
Changes intensity of green LED colour

LED options->Blue
Changes intensity of blue LED colour

Display->Backlight
Turns on and off the LCD backlight. Proper state should be displayed on LCD

Display->Selector
In this option user can select char that is pointing selected option on LCD.
For a larger number of points, a custom character must be defined in the display memory, e.g. an arrow

Temperature->Sensor IN
Displays only some mocked temperature value. Don’t need to read a real temperature from sensor.

Temperature->Sensor OUT
Displays only some mocked temperature value. Don’t need to read a real temperature from sensor.

Temperature->Units
Units: changes temperature units between C and F. Currently select unit should be displayed when executing mock of temperature sensors.

About
Displays Name and Surname author of program

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

int leds[] = {LED_RED, LED_GREEN, LED_BLUE};
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte customChar[] = {
    B00000,
    B00000,
    B00000,
    B01010,
    B11111,
    B11111,
    B01110,
    B00100,
};

enum MenuItemType
{
    ACTION,
    SUBMENU
};

struct MenuItem
{
    const char *name;
    MenuItemType type;
    String (*action)();
    MenuItem *submenu;
    int submenuSize;
};

String toggleLEDPower();
String changeRedIntensity();
String changeGreenIntensity();
String changeBlueIntensity();
String toggleBacklight();
String selectChar1();
String selectChar2();
String selectChar3();
String displaySensorIN();
String displaySensorOUT();
String changeUnits();
String displayAbout();

MenuItem ledOptions[] = {
    {"Power [ON|OFF]", ACTION, toggleLEDPower, nullptr, 0},
    {"Red", ACTION, changeRedIntensity, nullptr, 0},
    {"Green", ACTION, changeGreenIntensity, nullptr, 0},
    {"Blue", ACTION, changeBlueIntensity, nullptr, 0}};

MenuItem selectorOptions[] = {
    {"Arrow", ACTION, selectChar1, nullptr, 0},
    {"Dash", ACTION, selectChar2, nullptr, 0},
    {"Custom char", ACTION, selectChar3, nullptr, 0}};

MenuItem displayOptions[] = {
    {"Bckled [ON|OFF]", ACTION, toggleBacklight, nullptr, 0},
    {"Selector[>|-|h]", SUBMENU, nullptr, selectorOptions, 3}};

MenuItem temperatureOptions[] = {
    {"Sensor IN", ACTION, displaySensorIN, nullptr, 0},
    {"Sensor OUT", ACTION, displaySensorOUT, nullptr, 0},
    {"Units [C | F]", ACTION, changeUnits, nullptr, 0}};

MenuItem mainMenu[] = {
    {"LED options", SUBMENU, nullptr, ledOptions, 4},
    {"Display", SUBMENU, nullptr, displayOptions, 2},
    {"Temperature", SUBMENU, nullptr, temperatureOptions, 3},
    {"About", ACTION, displayAbout, nullptr, 0}};

int main_menu_size = 4;
MenuItem *currentMenu = mainMenu;
int currentMenuSize = main_menu_size;
int currentIndex = 0;
char chosen_selector = '>';

MenuItem submenus_stack[2];
int submenus_stack_size = 0;

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

int info_refresh_time = 100;

void display_info(String information)
{
    int delay_time = 2000;
    bool waiting_for_input = true;

    if (digitalRead(RED_BUTTON) == LOW)
    {
        waiting_for_input = false;
    }

    lcd.clear();
    lcd.print(information);
    lcd.setCursor(0, 1);
    lcd.print("Back in: ");
    lcd.print((double)delay_time / 1000, 1);
    lcd.print("s");

    while (delay_time > 0)
    {
        lcd.setCursor(9, 1);
        lcd.print("           ");
        lcd.setCursor(9, 1);

        lcd.print((double)delay_time / 1000, 1);
        lcd.print("s");
        delay(info_refresh_time);
        delay_time -= info_refresh_time;

        if (digitalRead(RED_BUTTON) == LOW)
        {
            if (waiting_for_input)
            {
                return;
            }
        }
        else
        {
            waiting_for_input = true;
        }
    }
}

void updateDisplay()
{
    lcd.clear();

    int starting_index = currentIndex != currentMenuSize - 1 ? currentIndex : currentIndex - 1;

    for (int i = starting_index; i <= starting_index + 1; i++)
    {
        if (i == currentIndex)
        {
            if (chosen_selector == 'h')
            {
                lcd.write(byte(0));
            }
            else
            {
                lcd.print(chosen_selector);
            }
        }
        else
        {
            lcd.print(" ");
        }
        lcd.print(currentMenu[i].name);
        lcd.setCursor(0, 1);
    }
}

void setup()
{
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);

    pinMode(ENCODER1, INPUT_PULLUP);
    pinMode(ENCODER2, INPUT_PULLUP);

    lcd.init();
    lcd.backlight();

    Serial.begin(9600);

    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT10);

    lcd.createChar(0, customChar);

    updateDisplay();
}

void transition_to_main_menu()
{
    submenus_stack_size = 0;

    currentMenu = mainMenu;
    currentMenuSize = main_menu_size;
    currentIndex = 0;

    updateDisplay();
}

void transition_to_submenu(MenuItem *submenu, int size)
{
    MenuItem menu = {
        "", SUBMENU, nullptr, currentMenu, currentMenuSize};

    submenus_stack[submenus_stack_size] = menu;

    currentMenu = submenu;
    currentMenuSize = size;
    currentIndex = 0;

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
        delay(DEBOUNCING_PERIOD); // Debounce
        if (digitalRead(RED_BUTTON) == LOW)
        {
            if (currentMenu[currentIndex].type == ACTION)
            {
                String response_info = currentMenu[currentIndex].action();
                if (response_info != "")
                {
                    display_info(response_info);
                    updateDisplay();
                }
            }
            else if (currentMenu[currentIndex].type == SUBMENU)
            {
                transition_to_submenu(currentMenu[currentIndex].submenu, currentMenu[currentIndex].submenuSize);
                submenus_stack_size++;
            }
        }
        while (digitalRead(RED_BUTTON) == LOW)
        {
            Serial.println("Red button pressed"); // Wait for button release
        }
    }

    if (digitalRead(GREEN_BUTTON) == LOW)
    {
        delay(DEBOUNCING_PERIOD); // Debounce
        if (digitalRead(GREEN_BUTTON) == LOW)
        {
            if (submenus_stack_size > 1)
            {
                MenuItem menu = submenus_stack[submenus_stack_size--];
                transition_to_submenu(menu.submenu, menu.submenuSize);
            }
            if (submenus_stack_size == 1)
            {
                submenus_stack_size = 0;
                transition_to_main_menu();
            }
        }

        while (digitalRead(GREEN_BUTTON) == LOW)
        {
            Serial.println("Green button pressed"); // Wait for button release
        }
    }
}

String toggleLEDPower()
{
    for (int i = 0; i < 3; i++)
    {
        if (digitalRead(leds[i]) == HIGH)
        {
            for (int j = 0; j < 3; j++)
            {
                digitalWrite(leds[j], LOW);
            }
            return "All leds OFF";
        }
    }

    for (int i = 0; i < 3; i++)
    {
        digitalWrite(leds[i], HIGH);
    }
    return "All leds ON";
}

String changeRedIntensity()
{
    if (digitalRead(LED_RED) == HIGH)
    {
        analogWrite(LED_RED, 0);
        return "Red LED OFF";
    }
    else
    {
        analogWrite(LED_RED, 255);
        return "Red LED ON";
    }
}

String changeGreenIntensity()
{
    if (digitalRead(LED_GREEN) == HIGH)
    {
        analogWrite(LED_GREEN, 0);
        return "Green LED OFF";
    }
    else
    {
        analogWrite(LED_GREEN, 255);
        return "Green LED ON";
    }
}

String changeBlueIntensity()
{
    if (digitalRead(LED_BLUE) == HIGH)
    {
        analogWrite(LED_BLUE, 0);
        return "Blue LED OFF";
    }
    else
    {
        analogWrite(LED_BLUE, 255);
        return "Blue LED ON";
    }
}

bool backlight_state = true;

String toggleBacklight()
{
    backlight_state = !backlight_state;
    if (backlight_state)
    {
        lcd.backlight();
        return "Backlight ON";
    }
    else
    {
        lcd.noBacklight();
        return "Backlight OFF";
    }
}

String selectChar1()
{
    chosen_selector = '>';
    updateDisplay();

    return "";
}

String selectChar2()
{
    chosen_selector = '-';
    updateDisplay();

    return "";
}

String selectChar3()
{
    chosen_selector = 'h';
    updateDisplay();

    return "";
}

bool celsius = true;

String displaySensorIN()
{
    if (celsius)
    {
        return "15.0C";
    }
    else
    {
        return "45.0F";
    }
}

String displaySensorOUT()
{
    if (celsius)
    {
        return "65.0C";
    }
    else
    {
        return "95.0F";
    }
}

String changeUnits()
{
    celsius = !celsius;
    return celsius ? "Now using celsius" : "Now using fahrenheit";
}

String displayAbout()
{
    return "By Mikolaj Kubs";
}