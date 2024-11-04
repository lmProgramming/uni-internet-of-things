#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(uint8_t pin, unsigned long debounce_time)
    : _pin(pin), _debounce_time(debounce_time), _last_press_time(0),
      _short_press_callback(nullptr), _long_press_callback(nullptr),
      _button_state(false), _last_button_state(false) {}

void ButtonHandler::begin()
{
    pinMode(_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_pin), [this]()
                    { this->handle_interrupt(); }, CHANGE);
}

void ButtonHandler::set_short_press_callback(void (*callback)())
{
    _short_press_callback = callback;
}

void ButtonHandler::set_long_press_callback(void (*callback)())
{
    _long_press_callback = callback;
}

void ButtonHandler::handle_interrupt()
{
    unsigned long current_time = millis();
    bool current_state = digitalRead(_pin) == LOW;

    if (current_state != _last_button_state)
    {
        _last_press_time = current_time;
    }

    if ((current_time - _last_press_time) > _debounce_time)
    {
        if (current_state != _button_state)
        {
            _button_state = current_state;

            if (_button_state == LOW)
            {
                if ((current_time - _last_press_time) > 1000)
                {
                    if (_long_press_callback)
                        _long_press_callback();
                }
                else
                {
                    if (_short_press_callback)
                        _short_press_callback();
                }
            }
        }
    }

    _last_button_state = current_state;
}