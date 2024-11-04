#include "ButtonHandler.h"

/*
Przygotuj własną bibliotekę, która będzie służyła do sterowania przyciskiem podłączonym do Arduino.

Samodzielnie zaprojektuj interfejs programistyczny tej biblioteki, tak aby pozwalała podać, do których pinów podłączony jest przycisk,
oraz aby pozwalała ustawić wartości konfiguracyjne (np. debounce time, etc.).

Zastanów się nad implementacją w wykorzystaniem przerwać zazmiast aktywnego odpytywania stanu pinu. Biblioteka powinna wspierać obsługę
krótkiego wciśnięcia przycisku oraz długiego wciśnięcia. Obie operacje uruchmiają inną funkcjonalność, która ma być konfigurowana przez programistę.

Na laboratorium należy zaprezentować działanie biblioteki na obu przyciskach jednocześnie. Rozwiazanie w oparciu o przerwania będzie oceniane wyżej.

Zadbaj o kolorowanie składni w Arduino IDE.
*/

ButtonHandler *ButtonHandler::instances[2] = {nullptr, nullptr};

ButtonHandler::ButtonHandler(uint8_t pin, unsigned long debounce_time, unsigned long long_press_time)
    : _pin(pin), _debounce_time(debounce_time), _last_change_time(0),
      _short_press_callback(nullptr), _long_press_callback(nullptr),
      _button_state(HIGH), _last_button_state(HIGH), _long_press_time(long_press_time) {}

void ButtonHandler::begin()
{
    pinMode(_pin, INPUT_PULLUP);
    Serial.println(_pin);

    switch (_pin)
    {
    case 2:
        attachInterrupt(digitalPinToInterrupt(_pin), handle_interrupt_1, CHANGE);
        Serial.println(22);

        ButtonHandler::instances[0] = this;
        break;
    case 4:
        attachInterrupt(digitalPinToInterrupt(_pin), handle_interrupt_2, CHANGE);
        Serial.println(44);

        ButtonHandler::instances[1] = this;
        break;
    }
}

void ButtonHandler::set_short_press_callback(void (*callback)())
{
    _short_press_callback = callback;
}

void ButtonHandler::set_long_press_callback(void (*callback)())
{
    _long_press_callback = callback;
}

void ButtonHandler::set_debounce_time(float new_time)
{
    _debounce_time = new_time;
}

void ButtonHandler::set_long_press_time(float new_time)
{
    _long_press_time = new_time;
}

void ButtonHandler::handle_interrupt()
{
    unsigned long current_time = millis();
    int current_reading = digitalRead(_pin);

    if ((current_time - _last_change_time) > _debounce_time)
    {
        if (current_reading != _button_state)
        {
            _button_state = current_reading;

            if (_button_state == HIGH)
            {
                unsigned long press_duration = current_time - _last_change_time;

                if (press_duration > _long_press_time)
                {
                    if (_long_press_callback)
                    {
                        _long_press_callback();
                    }
                }
                else
                {
                    if (_short_press_callback)
                    {
                        _short_press_callback();
                    }
                }
            }
        }
    }

    if (_last_button_state != current_reading)
    {
        _last_change_time = current_time;
    }

    _last_button_state = current_reading;
}