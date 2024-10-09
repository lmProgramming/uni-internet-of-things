#include <Arduino.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

// Napisz program, który w sposób łagodny będzie zmieniał kolory diody RGB w cyklu, 
// na przykład, czerwony > zielony > niebieski > czerwony ..., wyświetlając kolory pośrednie.

// Składowe kolorów RGB powinny się mieszać ze sobą tworząc różne, pośrednie kolory. 
// Na najwyższą ocenę należy zadbać, aby dla każdego z kolorów RGB można było zmieniać intensywność.

int leds[] = {LED_RED, LED_GREEN, LED_BLUE};
float led_intensity_multipliers[] = {1, 1, 1};
int led_index = 0;

int DELAY = 10;

void initRGB()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    for (int led : leds)
    {
        pinMode(led, OUTPUT);
        analogWrite(led, 0);
    }
}

void setup()
{
    initRGB();
}

void loop(){
    int intensity = 255;
    while (intensity != 0)
    {
        analogWrite(leds[led_index], intensity);
        analogWrite(leds[(led_index + 1) % 3], (255 - intensity));
        intensity = intensity - 1;
        delay(DELAY);
    }
    led_index = (led_index + 1) % 3;
}