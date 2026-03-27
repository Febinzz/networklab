#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

void sendData(int sockfd, int totalFrames, int windowSize)
{
    int base = 0;
    int nextSeqNum = 0;
    int ack;
    int frame;
    int n;

    struct timeval timeout;
    timeout.tv_sec = 3;   // 3 second timeout
    timeout.tv_usec = 0;

<<<<<<< HEAD
    
=======
>>>>>>> 5a40aa9a550daa5b14da0e1e6e5f1c4044df0b9f
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,
               (const char *)&timeout, sizeof(timeout));

    while (base < totalFrames)
    {
        // Send frames inside window
        while (nextSeqNum < base + windowSize &&
               nextSeqNum < totalFrames)
        {
            frame = nextSeqNum;
            send(sockfd, &frame, sizeof(frame), 0);

            printf("Frame %d Sent\n", frame);
            nextSeqNum++;
        }

        // Wait for ACK
        n = recv(sockfd, &ack, sizeof(ack), 0);

        if (n > 0)
        {
            printf("ACK Received: %d\n", ack);
            base = ack + 1;   // Slide window
        }
        else
        {
            printf("Timeout! Resending from Frame %d\n", base);
            nextSeqNum = base;   // Go back
        }
    }

    // Send exit signal
    int exitSignal = -1;
    send(sockfd, &exitSignal, sizeof(exitSignal), 0);

    printf("All Frames Sent Successfully\n");
}

int main()
{
    int sockfd;
    int totalFrames, windowSize;
    struct sockaddr_in serverAddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket Created\n");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&serverAddr,
            sizeof(serverAddr));
    printf("Connected Successfully\n");

    printf("Enter total number of frames: ");
    scanf("%d", &totalFrames);

    printf("Enter window size: ");
    scanf("%d", &windowSize);

    sendData(sockfd, totalFrames, windowSize);

    close(sockfd);
    return 0;
}