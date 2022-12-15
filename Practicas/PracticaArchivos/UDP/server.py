import os
import socket
import select


UDP_IP = "127.0.0.1"
IN_PORT = 8080
timeout = 3
buffer = 200


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, IN_PORT))

while True:
    datos = []
    
    while True:
        data, addr = sock.recvfrom(buffer)
        if data:
            # print("File name:", data)
            if data == b'Ok':
                break
            datos.append(data.decode('UTF-8'))
            # file_name = data.strip()

    os.makedirs(f'{datos[0]}', exist_ok=True)
    
    for i in range(1, len(datos)):
        f = open(f'./{datos[0]}/{datos[i]}', 'wb')

        while True:
            # ready = select.select([sock], [], [], timeout)
            # if ready[0]:
            data, addr = sock.recvfrom(buffer)
            if data == b'Ok':
                print("%s Finish!" % datos[i])
                f.close()
                break
            f.write(data)
            # else:
            #     print("%s Finish!" % datos[i])
            #     f.close()
            #     break