#!/usr/bin/python3.10
import socket
import sys
#import threading as th
import os
#from os import getcwd
if __name__=="__main__":
    argumentos=sys.argv
    HOST_SERVER = "192.168.42.13"
    PORT_SERVER = 0
    HOST_CLIENT = "0.0.0.0"
    PORT_CLIENT = 5005
    FILE_SIZE = 200
    myServer=(HOST_SERVER,PORT_SERVER)
    myClient=(HOST_CLIENT,PORT_CLIENT)

    with socket.socket(socket.AF_INET,socket.SOCK_DGRAM) as mySocket:
        mySocket.bind(myClient)

#        msj="[+]Coneccion Establecida con ["+HOST_CLIENT+"]"
#        mySocket.sendto(msj.encode(),myServer)
        #Make Directory
        data,addr = mySocket.recvfrom(FILE_SIZE)
        newDir = data.decode()
        parent_dir=os.getcwd()+"/saves"
        path=os.path.join(parent_dir, newDir)
        os.mkdir(path)
        #get number of files
        data,addr = mySocket.recvfrom(FILE_SIZE)
        nFiles=int(data.decode())
        for n in range(nFiles):
            #get name of files
            data,addr = mySocket.recvfrom(FILE_SIZE)
            msj="saves/"+newDir+"/"+data.decode()
            with open(msj,"wb") as f:
                while True:
                    #get data files
                    data,addr = mySocket.recvfrom(FILE_SIZE)
                    if not data:
                        break
                    f.write(data)
                f.close()
        mySocket.close()
