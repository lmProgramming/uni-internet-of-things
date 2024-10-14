#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

/*
Napisz program, który będzie migał trzema kolorami diody RGB.

Każdy kolor diody ma mieć zmienny czas migania wyrażony w milisekundach.
Program startuje z czasami R-0,9s, G-1,0s, B-1,1s.
Proszę wykorzystać przyciski do zmiany wartości czasów migania. Można przyjąć, że przycisk czerwony 
wybiera aktualną diodę do zmiany wartości, a zielony zmienia wartość migania cyklicznie w przedziale 0,5s - 2,0s z krokiem co 0,1s.
Aktualna nastawa czasu jest wyświetlana na wyświetlaczu LCD.
Program przygotować tak, aby nie blokować przełączania diod wzajemnie, nie blokować wykonania programu.
*/

// Initial blink times in milliseconds
unsigned long blinkTimes[3] = {900, 1000, 1100};
unsigned long previousMillis[3] = {0, 0, 0};
int ledPins[3] = {LED_RED, LED_GREEN, LED_BLUE};
int currentLedIndex = 0;

// Button states
bool redButtonState = LOW;
bool greenButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    // Initialize LED pins
    for (int i = 0; i < 3; i++) {
        pinMode(ledPins[i], OUTPUT);
    }

    // Initialize button pins
    pinMode(RED_BUTTON, INPUT);
    pinMode(GREEN_BUTTON, INPUT);

    // Initialize LCD
    lcd.begin(16, 2);
    lcd.print("R: 0.9s G: 1.0s B: 1.1s");
}

void displayBlinkTime(int index, char ledChar, float time) {
    if (currentLedIndex == index) {
        lcd.print((char)toupper(ledChar));
    } else {
        lcd.print((char)tolower(ledChar));
    }
    lcd.print(": ");
    lcd.print(time);
    lcd.print("s ");
}

bool is_button_pressed(int button)
{
    static int debounced_button_state = HIGH;
    static int previous_reading = HIGH;
    static int blocker = 0;
    bool isPressed = false;

    int current_reading = digitalRead(button);

    if (previous_reading != current_reading)
    {
        blocker = 1000;
    }

    if (blocker == 0)
    {
        if(current_reading != debounced_button_state)
        {
            if (debounced_button_state == HIGH && current_reading == LOW)
            {
                isPressed = true;
            }
            debounced_button_state = current_reading;
        }
    }
    else
    {
        --blocker;
    }

    previous_reading = current_reading;

    return isPressed;
}

void loop() {
    unsigned long currentMillis = millis();

    if (is_button_pressed(RED_BUTTON)){
        currentLedIndex = (currentLedIndex + 1) % 3;
    }

    // Check green button state
    //int readingGreen = digitalRead(GREEN_BUTTON);
    //if (readingGreen != greenButtonState) {
    //    lastDebounceTime = currentMillis;
    //}
    //if ((currentMillis - lastDebounceTime) > debounceDelay) {
    //    if (readingGreen == HIGH) {
    //        blinkTimes[currentLedIndex] += 100;
    //        if (blinkTimes[currentLedIndex] > 2000) {
    //            blinkTimes[currentLedIndex] = 500;
    //        }
    //    }
    //}
    //greenButtonState = readingGreen;

    for (int i = 0; i < 3; i++) {
        if (currentMillis - previousMillis[i] >= blinkTimes[i]) {
            previousMillis[i] = currentMillis;
            digitalWrite(ledPins[i], !digitalRead(ledPins[i]));
            break;
        }
    }    

    lcd.setCursor(0, 0);
    displayBlinkTime(0, 'r', blinkTimes[0] / 1000.0);
    displayBlinkTime(1, 'g', blinkTimes[1] / 1000.0);

    lcd.setCursor(0, 1);
    displayBlinkTime(2, 'b', blinkTimes[2] / 1000.0);
}