#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>

#define PORT 8080

int main() 
{
    int sockfd, connfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t len;

    int expected = 0;
    int frame;
    int ack;

    srand(time(NULL));   // Random ACK loss

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Server Socket Created\n");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("Bind Successful\n");

    listen(sockfd, 5);
    printf("Waiting for client...\n");

    len = sizeof(clientAddr);
    connfd = accept(sockfd, (struct sockaddr*)&clientAddr, &len);
    printf("Client Connected\n\n");

    while (1) 
    {
        int n = recv(connfd, &frame, sizeof(frame), 0);
        if (n <= 0)
            break;

        if (frame == -1) 
        {
            printf("\nClient Exited\n");
            break;
        }

        printf("Received Frame: %d\n", frame);

        if (frame == expected) 
        {
            printf("Frame %d Accepted\n", frame);

            if (rand() % 3 == 0) 
            {
                printf("ACK %d LOST (simulated)\n\n", frame);
                continue;
            }

            ack = frame;
            send(connfd, &ack, sizeof(ack), 0);
            printf("ACK %d Sent\n\n", ack);

            expected++;
        } 

        else 
        {
            printf("Frame %d Discarded (Expected %d)\n", frame, expected);

            ack = expected - 1;
            send(connfd, &ack, sizeof(ack), 0);
            printf("Resent ACK %d\n\n", ack);
        }
    }

    close(connfd);
    close(sockfd);

    return 0;
}