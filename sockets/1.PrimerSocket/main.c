#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>


int main(int argc, char *argv)
{

    if (argc < 3)
    {
        printf("The ip and port are required");
        exit(-1);
    }

     // init the buffer
    char buffer[1024];

    int port = atoi(argv[2]);
    // Step 1: Initialize the socket
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket == -1)
    {
        printf("Error: socket() failed");
        exit(-1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    server.sin_addr.s_addr = inet_pton(AF_INET, argv[1], &(server.sin_addr));

    int isConnect = connect(mySocket, (struct sockaddr *)&server, sizeof(server));

    // Step 3. Communicate Phase
    strcpy(buffer, "Hola desde el cliente\r");
    send(mySocket, buffer, strlen(buffer), 0); // Send the message to the server

    // Step 4. Get the response from the server
    int bufferSize = sizeof(buffer);
    int bytesReceived = recv(mySocket, buffer, bufferSize, 0);
    buffer[bytesReceived] = '\0';
    printf("Server response: %s\n", buffer);

    // Step 5. Close the socket
    close(mySocket);

    return 0;
}