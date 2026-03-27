#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define PORT 5000
#define MAX 100
#define TIMEOUT 2

void func(int sockfd, int totalFrames, int windowSize)
{
    int base = 0, next = 0, ack;
    int acked[MAX] = {0};
    time_t send_time[MAX];

    struct timeval tv = {1, 0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (base < totalFrames)
    {
        while (next < base + windowSize && next < totalFrames)
        {
            send(sockfd, &next, sizeof(next), 0);
            printf("Frame %d Sent\n", next);
            send_time[next] = time(NULL);
            next++;
        }

        int n = recv(sockfd, &ack, sizeof(ack), 0);

        if (n > 0)
        {
            printf("ACK %d Received\n", ack);
            acked[ack] = 1;

            while (acked[base])
                base++;
        }

        
        for (int i = base; i < next; i++)
        {
            if (!acked[i] && difftime(time(NULL), send_time[i]) >= TIMEOUT)
            {
                printf("Timeout → Resending %d\n", i);
                send(sockfd, &i, sizeof(i), 0);
                send_time[i] = time(NULL);
            }
        }
    }

    int end = -1;
    send(sockfd, &end, sizeof(end), 0);
}

int main()
{
    int sockfd;
    struct sockaddr_in serveraddr;

    sockfd=socket(AF_INET,SOCK_STREAM,0);

    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(PORT);
    serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

    printf("Connected to server\n");

    int nf,ws;

    printf("Enter number of frames: ");
    scanf("%d",&nf);

    printf("Enter window size: ");
    scanf("%d",&ws);

    func(sockfd,nf,ws);

    close(sockfd);
}