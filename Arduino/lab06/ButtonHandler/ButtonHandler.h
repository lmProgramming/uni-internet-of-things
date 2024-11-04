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
        Serial.println("Interrupt 1 handled");
    }

    static void handle_interrupt_2()
    {
        ButtonHandler::instances[1]->handle_interrupt();
        Serial.println("Interrupt 2 handled");
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