#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>

#define PORT 5000
#define MAX 100

int main()
{
    int sockfd, connfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t len;

    int base = 0, windowSize;
    int received[MAX] = {0};
    int frame;

    srand(time(NULL));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(sockfd, 5);

    printf("Waiting for client...\n");

    len = sizeof(clientAddr);
    connfd = accept(sockfd, (struct sockaddr*)&clientAddr, &len);

    printf("Client connected\n");

    printf("Enter window size: ");
    scanf("%d", &windowSize);

    while (1)
    {
        int n = recv(connfd, &frame, sizeof(frame), 0);
        if (n <= 0) break;

        if (frame == -1)
        {
            printf("Client exited\n");
            break;
        }

        printf("Received Frame %d\n", frame);

        // LOWER LOSS → important fix
        if (rand() % 5 == 0)
        {
            printf("Frame %d LOST\n", frame);
            continue;
        }

        if (frame >= base && frame < base + windowSize)
        {
            if (!received[frame])
            {
                received[frame] = 1;
                printf("Accepted %d\n", frame);
            }

            // ALWAYS send ACK
            send(connfd, &frame, sizeof(frame), 0);
            printf("ACK %d Sent\n", frame);

            while (received[base])
                base++;
        }
        else
        {
            // IMPORTANT FIX: send last ACK again
            int ack = base - 1;
            send(connfd, &ack, sizeof(ack), 0);
            printf("Out of window → Resent ACK %d\n", ack);
        }
    }

    
    close(connfd);
    close(sockfd);
}