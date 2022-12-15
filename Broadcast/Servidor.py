#!/usr/bin/python3.10
import socket
import threading as th
import os
from os import getcwd

def conection(dirName,mySocketP,fSize,Broad):
    myFiles = getFilesNames(ruta=getcwd()+"/src/"+dirName)
#    data,myClient = mySocketP.recvfrom(fSize)
#    msj = data.decode()
#    print("\n"+msj)
    #Enviar nombre de Carpeta
    print("Carpeta a enviar: "+dirName)
    print(myFiles)
    mySocketP.sendto(dirName.encode(),Broad)
    #enviar numero de archivos
    mySocketP.sendto(str(len(myFiles)).encode(),Broad)
    for i in range(len(myFiles)):
        #enviar Nombre de archivos
        msj=myFiles[i]
        mySocketP.sendto(msj.encode(),Broad)
        with open("src/"+dirName+"/"+myFiles[i],"rb") as f:
            while True:
                data = f.read(fSize)
                mySocketP.sendto(data,Broad)
                if not data:
                    break
            f.close()
    print("[-]Finalizado")

def getDirNames(ruta = getcwd()+"/src"):
    return [dir.name for dir in os.scandir(ruta) if dir.is_dir()]

def getFilesNames(ruta = getcwd()+"/src"):
    return [arch.name for arch in os.scandir(ruta) if arch.is_file()]

HOST_Broad = "10.10.20.255"
PORT_Broad = 5005
FILE_SIZE=200
myBroad=(HOST_Broad,PORT_Broad)


with socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP) as mySocket:
    mySocket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST,1)
    mySocket.setsockopt(socket.SOL_SOCKET, socket.SO_BINDTODEVICE, str("tap0" + '\0').encode('utf-8'))
    dirNames=getDirNames()
    directoriesTH=list()
    for i in range(len(dirNames)):
        conection(dirNames[i],mySocket,FILE_SIZE,myBroad)
