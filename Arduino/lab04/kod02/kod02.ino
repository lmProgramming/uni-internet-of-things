#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <limits.h>

#define LED_RED 6
#define LED_BLUE 3
#define POTENTIOMETER A0

/*
Napisz program, który będzie mierzy napięcie jakie otrzymujemy z potencjometru na wejście GPIO.
Wynik pomiaru powinien być wyświetlony na wyświetlaczu LCD przedstawiając aktualne napięcie w Voltach
oraz liczbową reprezentację ADC odczytaną z wejścia GPIO. Napięcie ma być wyrównane do lewej, natomiast
ADC do prawej strony wyświetlacza. Upewnij się, że wyświetlane wartości na wyświtlaczu LCD nie migają w
trakcie zmiany nastawy. Poniżej przykład poprawnie rozwiązanego zadania:
Miernik A0
V=2.60   ADC=532

Dodatkowo wykorzystaj Serial Plotter w środowisku Arduino IDE do przedstawienia wykresu napięcia w czasie. Aktualna wartość ADC także ma być na wykresie.
Dodatkowo użyj sprintf do formatowania napięcia do postaci z dwoma miejscami po przecinku.
*/

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
    pinMode(LED_RED, OUTPUT);
    analogWrite(LED_RED, 0);
    pinMode(LED_BLUE, OUTPUT);
    analogWrite(LED_BLUE, 0);

    lcd.init();
    lcd.backlight();

    Serial.begin(9600);
}

int numPlaces(int n)
{
    int r = 1;
    if (n < 0)
        n = (n == INT_MIN) ? INT_MAX : -n;
    while (n > 9)
    {
        n /= 10;
        r++;
    }
    return r;
}

int value = -1;

void loop()
{
    int new_value = analogRead(POTENTIOMETER);

    if (new_value == value)
    {
        return;
    }

    value = new_value;

    Serial.println(value);

    double voltage = value * (5.0 / 1023.0);

    char voltageStr[10];
    dtostrf(voltage, 4, 2, voltageStr);

    lcd.setCursor(0, 0);
    lcd.print("Miernik A0");

    lcd.setCursor(0, 1);
    lcd.print("V=");
    lcd.print(voltageStr);

    int adc_start_position = 12 - numPlaces(value);
    lcd.setCursor(adc_start_position, 1);
    lcd.print("ADC=");
    lcd.print(value);

    Serial.print("Voltage:");
    Serial.print(voltage);
    Serial.print(",");
    Serial.print("ADC:");
    Serial.println(value);

    delay(100);
}