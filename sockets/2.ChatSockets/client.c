#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

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

    // int port = atoi(argv[2]);
    // Step 1: Initialize the socket
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket == -1)
    {
        printf("Error: socket() failed");
        exit(-1);
    }

    // printf("Socket created\n");
    // struct sockaddr_in addr, cl_addr;
    // memset(&addr, 0, sizeof(addr));
    // addr.sin_family = AF_INET;
    // addr.sin_addr.s_addr = inet_addr(serverAddr);
    // addr.sin_port = PORT;

    struct sockaddr_in server, addr;

    memset(&server, 0, sizeof(server));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(server);
    addr.sin_port = htons(port);


    // server.sin_family = AF_INET;
    // server.sin_port = htons(port);
    // server.sin_addr.s_addr = inet_pton(AF_INET, ip, &(server.sin_addr));

    int isConnect = connect(mySocket, (struct sockaddr *)&server, sizeof(server));
    if (isConnect == -1)
    {
        printf("Error: connect() failed");
        exit(-1);
    }
    printf("Connected to the server");

    // Step 3. Communicate Phase
    memset(buffer, 0, BUFFER_SIZE);
    printf("Enter your message: ");
    // send(mySocket, buffer, strlen(buffer), 0); // Send the message to the server

    // create a thread to receive messages
    pthread_t receiveThread;
    int ret = pthread_create(&receiveThread, NULL, receiveMessage, (void *)mySocket);
    if (ret != 0)
    {
        printf("Error: pthread_create() failed");
        exit(-1);
    }
    
    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL)
    {
        send(mySocket, buffer, strlen(buffer), 0); // Send the message to the server
        memset(buffer, 0, BUFFER_SIZE);
    }
    // int bufferSize = sizeof(buffer);
    // int bytesReceived = recv(mySocket, buffer, bufferSize, 0);
    // buffer[bytesReceived] = '\0';
    // printf("Server response: %s\n", buffer);

    // Step 5. Close the socket
    close(mySocket);
    // close the thread
    pthread_exit(NULL);

    return 0;
}