import paho.mqtt.client as mqtt
import os

os.environ["DISPLAY"] = "0.0"

'''Proszę przygotować zestaw dwóch programów wykorzystujących protokół MQTT do komunikacji między sobą.
Pierwszy to program obsługujący czytnik kart RFID. Program ten, jako wydawca (publisher) protokołu MQTT, będzie
wysyłał informację o identyfikatorze użytej karty i dokładnym czasie jej użycia. Proszę zadbać, aby karta RFID przyłożona
do czytnika była odczytywana jeden raz, jeśli jest stale przyłożona do czytnika. Proszę sygnałem dźwiękowym i wizualnym
poinformować użytkownika karty, że została ona odczytana.
Drugi program to klient, który jako subskrybent protokołu MQTT będzie odbierał informacje o użyciu kart RFID i zapisywał
fakt użycia.'''

# The terminal ID - can be any string.
terminal_id = "T0"
# The broker name or IP address.
broker = "localhost"
# broker = "127.0.0.1"
# broker = "10.0.0.1"

# The MQTT client.
client = mqtt.Client()

def call_worker(worker_name):
    client.publish("worker/name", worker_name + "." + terminal_id,)


def connect_to_broker():
    # Connect to the broker.
    client.connect(broker)
    # Send message about conenction.
    call_worker("Client connected")
    print("connected to broker")


def disconnect_from_broker():
    # Send message about disconenction.
    call_worker("Client disconnected")
    # Disconnet the client.
    client.disconnect()


def run_sender():
    connect_to_broker()

    input()

    disconnect_from_broker()


if __name__ == "__main__":
    run_sender()
