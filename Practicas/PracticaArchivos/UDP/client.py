import socket
import time
import sys
import os

UDP_IP = "127.0.0.1"
UDP_PORT = 8080
buf = 200
file_name = './Cliente1/A1.txt'


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Datos
datos = []

OPT = input("use the default file route ? (y/n): ")
if OPT == "y" or OPT == "Y" or OPT == "yes" or OPT == "Yes" or OPT == "": 
    DIR = "/root/dev.ubuntu/redes/PracticaArchivos/UDP"
else :
    DIR = input("Please enter the file route: ")
    
if not os.path.isdir(DIR):
    print("The folder does not exist")
    exit()
    
while True:
    if len(datos) == 0:
        data = input('user: ').encode()
    else:
        data = input(f'File: {len(datos)}: ').encode()
    if data == b'':
        sock.sendto(b'Ok', (UDP_IP, UDP_PORT))
        break
    datos.append(data.decode('UTF-8'))
    sock.sendto(data, (UDP_IP, UDP_PORT))

for i in range(1, len(datos)):
    f = open(f'./{DIR}/{datos[i]}', "rb")
    data = f.read(buf)
    while(data):
        print(data)
        if sock.sendto(data, (UDP_IP, UDP_PORT)):
            data = f.read(buf)
            time.sleep(0.02) # Give receiver a bit time to save
    sock.sendto(b'Ok', (UDP_IP, UDP_PORT))
    f.close()

sock.close()
