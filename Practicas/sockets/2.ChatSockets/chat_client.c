#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#define PORT 4444
#define BUFFER_SIZE 2000

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
            printf("server: ");
            fputs(buffer, stdout);
            printf("\n");
        }
    }
}

int main(int argc, char **argv)
{
    struct sockaddr_in addr, cl_addr;
    int sockfd, ret;
    char buffer[BUFFER_SIZE];
    char *serverAddr;
    pthread_t rThread;

    if (argc < 2)
    {
        printf("usage: client < ip address >\n");
        exit(1);
    }

    serverAddr = argv[1];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("Error creating socket!\n");
        exit(1);
    }
    printf("Socket created...\n");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(serverAddr);
    addr.sin_port = PORT;

    ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0)
    {
        printf("Error connecting to the server!\n");
        exit(1);
    }
    printf("Connected to the server...\n");

    memset(buffer, 0, BUFFER_SIZE);
    printf("Enter your messages one by one and press return key!\n");

    //creating a new thread for receiving messages from the server
    ret = pthread_create(&rThread, NULL, receiveMessage, (void *)sockfd);
    if (ret)
    {
        printf("ERROR: Return Code from pthread_create() is %d\n", ret);
        exit(1);
    }

    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL)
    {
        ret = sendto(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&addr, sizeof(addr));
        if (ret < 0)
        {
            printf("Error sending data!\n\t-%s", buffer);
        }
    }

    close(sockfd);
    pthread_exit(NULL);

    return 0;
}