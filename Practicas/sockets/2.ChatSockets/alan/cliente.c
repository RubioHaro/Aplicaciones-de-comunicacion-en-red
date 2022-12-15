#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(int argc, char* argv[]){
    char buffer[100];
    
    //Paso 0. Validar el numero de entradas
    if (argc < 3){
        printf("\nError: Verificar que se introdujo la direccion ip y puerto");
        exit(EXIT_FAILURE);
    }
    int puerto = atoi(argv[2]);
    //Paso 1. Creacion del socket
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket < 0){
        printf("\nError al abrir el socket");
        exit(EXIT_FAILURE);
    }

    //Paso 2. Establecer la conexion hacia el servidor
    struct sockaddr_in servidor;
    servidor.sin_family = AF_INET; //Familia más grandes
    servidor.sin_port = htons(puerto);
    servidor.sin_addr.s_addr = inet_pton(AF_INET, argv[1], &(servidor.sin_addr)); //Convertir de char a un formato valido de red 
    //char str[INET_ADDRSTRLEN];
    // servidor.sin_addr = inet_nton(AF_INET, &(servidor.sin_addr), str, INET_ADDRSTRLEN); //Converte de formato de red a char
    int isConnect = connect(mySocket, (struct sockaddr*)(&servidor), sizeof(servidor));
    if (isConnect == -1){
        printf("\nNo se establecio la conexion con el servidor");
        exit(EXIT_FAILURE);
    }
    //Si por alguna razon el servidor no esta disponible entonces se vuelve a intentar la conexion (con while)
    
    //Paso 3. fase de comunicacion
    strcpy(buffer, "Hola desde el cliente");
    send(mySocket, buffer, strlen(buffer), 0); //3er parametro: tamaño del mensaje, NO del buffer 

    //recibir un mensaje
    while (1){
        int tam_mensaje = recv(mySocket, buffer, sizeof(buffer), 0);
        buffer[tam_mensaje] = '\0';

        if (strcmp(buffer,"Fin de la comunicacion") == 1){
            printf("\nel servidor cerro el canal.");
            close(mySocket);
            break;
        }
        
        printf("\nMensaje recibido: %s\n",buffer);
    }
    return 0;
}


/*
Socket crudo 

Es la unidad más basico de un protocolo solamente que no hay validación de información perdida, nosotros tenemos que implementar una funcionalidad para pérdida de información.
*/