from config import *
import RPi.GPIO as GPIO
import time

'''Wykorzystując treść opisaną w lab09.pdf proszę o wykonanie zadania:

Napisz program, który pozwala regulować przy pomocy enkodera jasność świecenia diody LED 1/2/3/4
w module niebieskich diod świecących. Do obsługi enkodera wykorzystaj zdarzenia (events). W
danym czasie świeci się tylko jedna dioda LED. Wybór diody z modułu odbywa się poprzez przyciski.
Enkoder zmienia jedynie intensywność świecenia. Proszę na konsoli wyświetlać aktualną wartość intensywności.
'''

intensity: int = 50
leds: list = [led1, led2, led3, led4]
diodes: list[GPIO.PWM] = []
encoder_change: int = 5
current_led: int = 0


def encoder_callback(channel, change: int) -> None:
    global intensity

    intensity = max(0, min(100, intensity + change))

    diodes[current_led].ChangeDutyCycle(intensity)
    print(f"Current intensity: {intensity}")


def change_led(channel, change: int) -> None:
    global current_led
    GPIO.output(leds[current_led], False)
    current_led = (current_led + change) % 4
    GPIO.output(leds[current_led], True)


def setup() -> None:
    for led in leds:
        diode = GPIO.PWM(led, 50)
        diode.start(0)
        diodes.append(diode)

    GPIO.add_event_detect(encoderLeft, GPIO.FALLING,
                          callback=lambda channel: encoder_callback(channel, encoder_change), bouncetime=200)
    GPIO.add_event_detect(encoderRight, GPIO.FALLING,
                          callback=lambda channel: encoder_callback(channel, -encoder_change), bouncetime=200)
    GPIO.add_event_detect(buttonGreen, GPIO.FALLING,
                          callback=lambda channel: change_led(channel, 1), bouncetime=200)
    GPIO.add_event_detect(buttonRed, GPIO.FALLING,
                          callback=lambda channel: change_led(channel, -1), bouncetime=200)

    print(f"Current intensity: {intensity}")


def loop() -> None:
    time.sleep(0.1)


def main() -> None:
    setup()

    try:
        while True:
            loop()
    except KeyboardInterrupt:
        pass
    finally:
        GPIO.cleanup()


if __name__ == "__main__":
    main()
