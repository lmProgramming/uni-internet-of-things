#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

// Po naciśnięciu przycisku zielonego zmień kolor świecącej diody. 
// Przycisk czerwony włącza i wyłącza świecenie.

void initRGB()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, LOW);

    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_GREEN, LOW);

    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, HIGH);
}

void initButtons()
{
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

bool on = true;
int current_color = LED_BLUE;

void setup()
{
    initRGB();
    initButtons();
}

void loop()
{
    if (digitalRead(GREEN_BUTTON) == LOW)
        if (current_color == LED_RED){
          current_color = LED_GREEN;
        }
        else if (current_color == LED_GREEN){
          current_color = LED_BLUE;
        }
        else{
          current_color = LED_RED;
        }
        delay(20);

    if (digitalRead(RED_BUTTON) == LOW)
    {
        on = !on;
        if (on){
            digitalWrite(current_color, HIGH);
        }
        else{
            digitalWrite(current_color, LOW);
        }
        delay(20);
    }
}
