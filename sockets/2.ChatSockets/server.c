#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
// import strncnp 
#include <unistd.h>

# define BUFFER_SIZE 200

void *receiveMessage(void *socket)
{
    int sockfd, ret;
    char buffer[BUFFER_SIZE];
    sockfd = (int)socket;
    memset(buffer, 0, BUFFER_SIZE);
    for (;;)
    {
        ret = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (ret < 0)
        {
            printf("Error receiving data!\n");
        }
        else
        {
			
			if ( strcmp(buffer, "exit") ) {
				fputs("Disconected", stdout);
				close(sockfd);
			}
            printf("client: ");
            fputs(buffer, stdout);
            printf("\n");
        }
    }
}


int main(int argc, char *argv)
{
    // init the buffer
    char buffer[BUFFER_SIZE];
    int port = 8080;
    char *ip = "127.0.0.1";

    if (argc < 3)
    {
        printf("You want to use the default port? (y/n)");
        char answer;
        scanf("%c", &answer);
        if (answer == 'n')
        {
            printf("Please, enter the port: ");
            scanf("%d", &port);
        }
        else
        {
            printf("Using default port: %d\n", port);
        }

        printf("You want to use the default ip? (y/n)");
        scanf("%c", &answer);
        if (answer == 'n')
        {
            printf("Please, enter the ip: ");
            scanf("%s", ip);
        }
        else
        {
            printf("Using default ip: %s\n", ip);
        }
    }
    else
    {
        port = atoi(argv[2]);
        ip = argv[1];
    }

    // int port = atoi(argv[2]);

    // Step 1: Initialize the socket
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);

    if (mySocket == -1)
    {
        printf("Error: socket() failed");
        exit(-1);
    }
    printf("Socket created\n");

    // Step 2: Making public the service
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    // server.sin_addr.s_addr = inet_pton(AF_INET, ip, &(server.sin_addr));
    server.sin_addr.s_addr = INADDR_ANY;
    // open the connection bucle 
    printf("binding...\n");
    // int isConnect = connect(mySocket, (struct sockaddr *)&server, sizeof(server));
    int isBind = bind(mySocket, (struct sockaddr *)&server, sizeof(server));

    if (isBind == -1)
    {
        printf("Error: bind() failed, connection not established\n");
        exit(-1);
    }
    printf("Binding done...\n");

    printf("Listening...\n");
    int isListen = listen(mySocket, 5);
    struct sockaddr_in clientAdd;
    int len = sizeof(clientAdd);

    int newSocket = accept(mySocket, (struct sockaddr *)&clientAdd, &len);
    if (newSocket < 0)
    {
        printf("Error: accepting new connection failed\n");
        exit(-1);
    }

    inet_ntop(AF_INET, &(clientAdd.sin_addr), buffer, 100);
    printf("Connection accepted from %s:%d\n", buffer, ntohs(clientAdd.sin_port));

    memset(buffer, 0, BUFFER_SIZE);
    printf("Enter your message (Send with return key): ");
    pthread_t thread;
    int to_return = pthread_create(&thread, NULL, receiveMessage, (void *)newSocket);

    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL)
    {
        int send = sendto(newSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAdd, sizeof(clientAdd));
        if (send < 0)
        {
            printf("Error: sending message failed\n");
            exit(-1);
        }
        memset(buffer, 0, BUFFER_SIZE);
        // int exit = strncnp(buffer, "exit", 4);
        // if (exit == 0)
        // {
        //     printf("Exiting...\n");
        //     break;
        // }
        printf("Enter your message (Send with return key): ");
    }

    


    // while (1)
    // {
    //     // Step 4. Get the response from the server
    //     int bufferSize = sizeof(buffer);
    //     int bytesReceived = recv(mySocket, buffer, bufferSize, 0);
    //     buffer[bytesReceived] = '\0';
    //     printf("Client response: %s\n", buffer);

    //     // Step 5. Accept the connection
    //     int clientSize = sizeof(server);
    //     // accept returns the identificador of the communication channel
    //     int isAccept = accept(mySocket, (struct sockaddr *)&server, clientSize);
    //     if (isAccept == -1)
    //     {
    //         printf("Error: accept() failed, connection not established");
    //         exit(-1);
    //     }

    //     // Step 6. Get message from the client
    //     int size = recv(isAccept, buffer, bufferSize, 0);
    //     buffer[size] = '\0';
    //     printf("Client response: %s\n", buffer);

    //     // Step 7. Send the response to the client
    //     strcpy(buffer, "Hola desde el servidor\r");
    //     send(isAccept, buffer, strlen(buffer), 0);

    //     // Step 8. Close the communication channel
    //     close(isAccept);
    // }

    // Step 8. Close the socket
    close(mySocket);

    return 0;
}