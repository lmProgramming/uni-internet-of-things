/*
Przygotuj własną bibliotekę, która będzie służyła do sterowania przyciskiem podłączonym do Arduino.

Samodzielnie zaprojektuj interfejs programistyczny tej biblioteki, tak aby pozwalała podać, do których pinów podłączony jest przycisk,
oraz aby pozwalała ustawić wartości konfiguracyjne (np. debounce time, etc.).

Zastanów się nad implementacją w wykorzystaniem przerwać zazmiast aktywnego odpytywania stanu pinu. Biblioteka powinna wspierać obsługę
krótkiego wciśnięcia przycisku oraz długiego wciśnięcia. Obie operacje uruchmiają inną funkcjonalność, która ma być konfigurowana przez programistę.

Na laboratorium należy zaprezentować działanie biblioteki na obu przyciskach jednocześnie. Rozwiazanie w oparciu o przerwania będzie oceniane wyżej.

Zadbaj o kolorowanie składni w Arduino IDE.
*/

#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <Arduino.h>

class ButtonHandler
{
public:
    static ButtonHandler *instances[2];

    ButtonHandler(uint8_t pin, unsigned long debounce_time = 50, unsigned long long_press_time = 1000);
    void begin();
    void set_short_press_callback(void (*callback)());
    void set_long_press_callback(void (*callback)());
    void handle_interrupt();
    void set_debounce_time(float new_time);
    void set_long_press_time(float new_time);

    static void handle_interrupt_1()
    {
        ButtonHandler::instances[0]->handle_interrupt();
        Serial.println("1");
    }

    static void handle_interrupt_2()
    {
        ButtonHandler::instances[1]->handle_interrupt();
        Serial.println("2");
    }

private:
    uint8_t _pin;
    unsigned long _debounce_time;
    unsigned long _last_change_time;
    void (*_short_press_callback)();
    void (*_long_press_callback)();
    volatile int _button_state;
    volatile int _last_button_state;
    unsigned long _long_press_time;
};

#endif