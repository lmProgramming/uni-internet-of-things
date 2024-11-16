import RPi.GPIO as GPIO
import time

'''Wykorzystując treść opisaną w lab09.pdf proszę o wykonanie zadania:

Napisz program, który pozwala regulować przy pomocy enkodera jasność świecenia diody LED 1/2/3/4 
w module niebieskich diod świecących. Do obsługi enkodera wykorzystaj zdarzenia (events). W 
danym czasie świeci się tylko jedna dioda LED. Wybór diody z modułu odbywa się poprzez przyciski. 
Enkoder zmienia jedynie intensywność świecenia. Proszę na konsoli wyświetlać aktualną wartość intensywności.
'''

LED_PINS: list[int] = [17, 18, 27, 22]
BUTTON_PINS: list[int] = [5, 6, 13, 19]
ENCODER_PIN_A = 23
ENCODER_PIN_B = 24

GPIO.setmode(GPIO.BCM)
for pin in LED_PINS + BUTTON_PINS + [ENCODER_PIN_A, ENCODER_PIN_B]:
    GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

led_pwms: list[PWM] = [GPIO.PWM(pin, 1000) for pin in LED_PINS]
for pwm in led_pwms:
    pwm.start(0)

current_led = 0
brightness = 0


def button_callback(channel):
    global current_led
    current_led = BUTTON_PINS.index(channel)
    print(f"Selected LED: {current_led + 1}")


def encoder_callback(channel):
    global brightness
    if GPIO.input(ENCODER_PIN_A) == GPIO.input(ENCODER_PIN_B):
        brightness = min(brightness + 10, 100)
    else:
        brightness = max(brightness - 10, 0)
    led_pwms[current_led].ChangeDutyCycle(brightness)
    print(f"LED {current_led + 1} brightness: {brightness}")


for pin in BUTTON_PINS:
    GPIO.add_event_detect(
        pin, GPIO.FALLING, callback=button_callback, bouncetime=300)

GPIO.add_event_detect(ENCODER_PIN_A, GPIO.BOTH, callback=encoder_callback)

try:
    while True:
        time.sleep(0.1)
except KeyboardInterrupt:
    pass
finally:
    for pwm in led_pwms:
        pwm.stop()
    GPIO.cleanup()
