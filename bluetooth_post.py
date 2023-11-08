import serial
import requests
from datetime import datetime


# Nombre del puerto serie virtual creado por rfcomm 
serial_port = '/dev/rfcomm0'

# Configura la comunicación serial
ser = serial.Serial(serial_port, baudrate=9600, timeout=1)

# URL de la API a la que se enviarán los datos 
api_url = ''
headers = {
    'Content-Type': 'application/json',
}


# Función para leer datos del puerto serie y enviarlos a la API
def read_data_and_send_request():
    data = ser.readline().decode('utf-8').strip()
    
    # Si hay datos recibidos, divídelos en tres partes
    if data:
        print(data)
        data_parts = data.split(",")
        
        # Asegúrate de que haya al menos tres partes de datos
        if len(data_parts)== 3:
            data1, data2, data3 = data_parts[0], data_parts[1], data_parts[2]
        
            
            
            # Crea un diccionario con los datos para enviar en el cuerpo de la solicitud
            payload = {
                "idDevice": 1,
                "date": (datetime.now()).strftime('%d/%m/%Y %H:%M'),
                "voltage": float(data1),
                "amps": float(data2),
                "power": float(data3)
            }
    
            try:
                # Envía los datos a la API utilizando una solicitud HTTP POST
                response = requests.post(api_url, json=payload, headers=headers)
                
                print(response.json())
            except Exception as e:
                print("Error:", str(e))

# Bucle principal para leer datos y enviar solicitudes a la API
while True:
    read_data_and_send_request()