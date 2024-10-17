#include <Arduino.h>
#include <ctype.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

/*
Napis program,  który będzie reagował na komendy wysyłane z narzędzia Serial Monitor do zestawu Arduino.
Program ma sterować diodami LED RGB z zadaną każdej diodzie jasnościa osobno. Postać komend należy zaprojektować samemu.
*/

void setup()
{
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);

    Serial.begin(9600);
    while (!Serial)
    { /* just wait */
    }
    Serial.println("Ready. To reverse color use 'red', 'green', 'blue' commands. To change value use 'red 255', 'green 0', 'blue 122'. Value must be between 0 and 255.");
}

int red_value = 0;
int green_value = 0;
int blue_value = 0;

void get_color_address(int color, int *&value)
{
    if (color == LED_RED)
    {
        value = &red_value;
    }
    else if (color == LED_GREEN)
    {
        value = &green_value;
    }
    else if (color == LED_BLUE)
    {
        value = &blue_value;
    }
}

void reverse_color(int *value)
{
    *value = 255 - *value;
}

int color_name_to_int(String color)
{
    if (color == "red")
    {
        return LED_RED;
    }
    else if (color == "green")
    {
        return LED_GREEN;
    }
    else if (color == "blue")
    {
        return LED_BLUE;
    }
    return -1;
}

bool is_number(String &str)
{
    for (char const &c : str)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

void loop()
{
    if (Serial.available() == 0)
    {
        return;
    }

    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();

    int space_index = command.indexOf(' ');
    String chosen_color = (space_index != -1) ? command.substring(0, space_index) : command;

    int color = color_name_to_int(chosen_color);

    if (color == -1)
    {
        Serial.println(String("Unknown color '") + chosen_color + "'");
        return;
    }

    int *value;

    get_color_address(color, value);

    if (space_index == -1)
    {
        reverse_color(value);
    }
    else
    {
        String chosen_value = command.substring(space_index + 1);

        if (!is_number(chosen_value))
        {
            Serial.println(String("Expected int between 0 and 255 '") + chosen_value + "'");
            return;
        }

        *value = chosen_value.toInt();

        if (*value < 0 || *value > 255)
        {
            Serial.println(String("Expected int between 0 and 255 '") + *value + "'");
            return;
        }
    }

    Serial.println(String("Setting color '") + chosen_color + "' to value '" + *value + "'");
    analogWrite(color, *value);
}