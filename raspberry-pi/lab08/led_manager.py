from config import *
import RPi.GPIO as GPIO
import time

'''Wykorzystując treść opisaną w lab09.pdf proszę o wykonanie zadania:

Napisz program, który pozwala regulować przy pomocy enkodera jasność świecenia diody LED 1/2/3/4
w module niebieskich diod świecących. Do obsługi enkodera wykorzystaj zdarzenia (events). W
danym czasie świeci się tylko jedna dioda LED. Wybór diody z modułu odbywa się poprzez przyciski.
Enkoder zmienia jedynie intensywność świecenia. Proszę na konsoli wyświetlać aktualną wartość intensywności.
'''

intensity: int = 0
leds: list = [led1, led2, led3, led4]
diodes: list[GPIO.PWM] = [map(lambda x: GPIO.PWM(x, 50), leds)]

current_led: int = 0


def encoder_callback(channel, change):
    global intensity

    if change > 0:
        intensity = min(100, intensity + change)
    else:
        intensity = max(0, intensity - change)

    diodes[current_led].ChangeDutyCycle(intensity)


def change_led(channel, change):
    global current_led
    GPIO.output(leds[current_led], False)
    current_led = (current_led + change) % 4
    GPIO.output(leds[current_led], True)


def setup():
    diode1 = GPIO.PWM(led1, 50)

    dutyCycle = 0
    diode1.start(dutyCycle)

    GPIO.add_event_detect(encoderLeft, GPIO.FALLING,
                          callback=encoder_callback, bouncetime=200)

    GPIO.add_event_detect(encoderRight, GPIO.FALLING,
                          callback=encoder_callback, bouncetime=200)


def loop():
    print(intensity)


def main():
    setup()

    while True:
        loop()


if __name__ == "__main__":
    main()
    GPIO.cleanup()
