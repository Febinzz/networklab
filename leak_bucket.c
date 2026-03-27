#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define NOF_PACKETS 10

int randnum(int a)   // renamed from rand()
{
    int rn = (random() % 10) % a;
    return rn == 0 ? 1 : rn;
}

int main()
{
    int packet_sz[NOF_PACKETS], i, clk, b_size, o_rate;
    int p_sz_rm = 0, p_time, op;

    printf("\n=========== LEAKY BUCKET SIMULATION ===========\n");

    for(i = 0; i < NOF_PACKETS; ++i)
        packet_sz[i] = randnum(6) * 10;

    printf("\nGenerated Packets:\n");
    for(i = 0; i < NOF_PACKETS; ++i)
        printf("Packet[%d] : %d bytes\n", i, packet_sz[i]);

    printf("\nEnter the Output rate: ");
    scanf("%d", &o_rate);

    printf("Enter the Bucket Size: ");
    scanf("%d", &b_size);

    for(i = 0; i < NOF_PACKETS; ++i)
    {
        printf("\n\n========================================");
        printf("\nProcessing Packet %d", i);
        printf("\n========================================\n");

        if((packet_sz[i] + p_sz_rm) > b_size)
        {
            if(packet_sz[i] > b_size)
                printf("\nPacket size (%d bytes) exceeds Bucket Capacity (%d bytes). Packet Rejected.\n",
                       packet_sz[i], b_size);
            else
                printf("\nBucket Overflow! Packet Rejected.\n");
        }
        else
        {
            p_sz_rm += packet_sz[i];

            printf("Incoming Packet size : %d bytes\n", packet_sz[i]);
            printf("Current Bucket Size  : %d bytes\n", p_sz_rm);

            p_time = randnum(4) * 10;
            printf("Transmission Time    : %d units\n", p_time);

            printf("\n--- Transmitting ---\n");

            for(clk = 10; clk <= p_time; clk += 10)
            {
                sleep(1);

                if(p_sz_rm > 0)
                {
                    if(p_sz_rm <= o_rate)
                        op = p_sz_rm, p_sz_rm = 0;
                    else
                        op = o_rate, p_sz_rm -= o_rate;

                    printf("Transmitted : %d bytes | Remaining in Bucket : %d bytes\n",
                           op, p_sz_rm);
                }
                else
                {
                    printf("No packets left to transmit.\n");
                }
            }
        }
    }

<<<<<<< HEAD
    
=======
>>>>>>> 5a40aa9a550daa5b14da0e1e6e5f1c4044df0b9f
    printf("\n\n=========== SIMULATION COMPLETED ===========\n");
    return 0;
}