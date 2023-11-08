#!/bin/bash

# Iniciar el servicio Pi-bluetooth
sudo service bluetooth start

# Esperar unos segundos para asegurarse de que el servicio se haya iniciado correctamente
sleep 5

connected=false

# Intentar emparejarse con el dispositivo a través del puerto serial 
while [ "$connected" != true ]; do
    sudo rfcomm connect hci0 MAC_ESP32
    if [ $? -eq 0 ]; then
        echo "Emparejamiento Bluetooth exitoso. Conectado al ESP32."
        connected=true
    else
        echo "Emparejamiento Bluetooth fallido. Reintentando en 5 segundos..."
        sleep 5
    fi
done