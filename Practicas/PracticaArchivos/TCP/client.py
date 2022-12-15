import socket
import os

IP = socket.gethostbyname(socket.gethostname())
PORT = 8080

ADDR = (IP, PORT)

SIZE = 1024
FORMAT = "utf-8"

DISCONNECTED_MSG = "!DISCONNECT"

OPT = input("use the default file route ? (y/n): ")
if OPT == "y" or OPT == "Y" or OPT == "yes" or OPT == "Yes" or OPT == "": 
    DIR = "/root/dev.ubuntu/redes/PracticaArchivos/TCP"
    
else :
    DIR = input("Please enter the file route: ")
    
if not os.path.isdir(DIR):
    print("The folder does not exist")
    exit()


def ana_dir(dir):
    cont = os.listdir(dir)

    files = []

    for file in cont:
        if os.path.isfile(os.path.join(dir, file)):
            files.append(file)

    files.append(DISCONNECTED_MSG)

    return files


def main():

    files = ana_dir(DIR)

    print("The files in the folder are: ")
    for file in files:
        print("\t|-", file)

    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    client.connect(ADDR)

    print(f"[CONNECTED] Client connected to server at {IP}:{PORT}")

    connected = True

    while connected:
        for file in files:

            # Se envia el nombre del archivo
            msgS = file
            client.send(msgS.encode(FORMAT))

            # Se recibe confirmacion
            msgR = client.recv(SIZE).decode(FORMAT)
            # print(f"[Server] {msgR}")

            if msgS == DISCONNECTED_MSG:
                connected = False
            else:
                with open(os.path.join(DIR, msgS), "rb") as f:
                    while True:
                        msgS = f.read(200)

                        if len(msgS) != 0:
                            client.send(msgS)

                            msgR = client.recv(SIZE).decode(FORMAT)
                            print(f"[Server] {msgR}")
                        else:
                            client.send(b"END")

                            msgR = client.recv(SIZE).decode(FORMAT)
                            print(f"[Server] {msgR}")

                            break


if __name__ == "__main__":
    main()
