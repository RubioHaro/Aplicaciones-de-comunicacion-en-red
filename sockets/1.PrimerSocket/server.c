#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char *argv)
{
    // init the buffer
    char buffer[100];

    if (argc < 3)
    {
        printf("The ip and port are required");
        exit(-1);
    }

    int port = atoi(argv[2]);

    // Step 1: Initialize the socket
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);

    if (mySocket == -1)
    {
        printf("Error: socket() failed");
        exit(-1);
    }

    // Step 2: Making public the service
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    server.sin_addr.s_addr = inet_pton(AF_INET, argv[1], &(server.sin_addr));

    // int isConnect = connect(mySocket, (struct sockaddr *)&server, sizeof(server));
    int isBind = bind(mySocket, (struct sockaddr *)&server, sizeof(server));

    if (isBind == -1)
    {
        printf("Error: bind() failed, connection not established");
        exit(-1);
    }

    // Step 3. Queue Phase
    listen(mySocket, 4);
    while (1)
    {
        // Step 4. Get the response from the server
        int bufferSize = sizeof(buffer);
        int bytesReceived = recv(mySocket, buffer, bufferSize, 0);
        buffer[bytesReceived] = '\0';
        printf("Client response: %s\n", buffer);

        // Step 5. Accept the connection
        int clientSize = sizeof(server);
        // accept returns the identificador of the communication channel
        int isAccept = accept(mySocket, (struct sockaddr *)&server, clientSize);
        if (isAccept == -1)
        {
            printf("Error: accept() failed, connection not established");
            exit(-1);
        }

        // Step 6. Get message from the client
        int size = recv(isAccept, buffer, bufferSize, 0);
        buffer[size] = '\0';
        printf("Client response: %s\n", buffer);

        // Step 7. Send the response to the client
        strcpy(buffer, "Hola desde el servidor\r");
        send(isAccept, buffer, strlen(buffer), 0);

        // Step 8. Close the communication channel
        close(isAccept);
    }

    // Step 8. Close the socket
    close(mySocket);

    return 0;
}