#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netpacket/packet.h>
#include <stdlib.h>
#include <net/ethernet.h>


void Detect(unsigned char* buffer, struct sockaddr_ll* socket, int l) {

    if (socket->sll_pkttype == PACKET_MULTICAST) {
        printf("Multicast\n");
    }
    else if (socket->sll_pkttype == PACKET_HOST) {
        printf("Incoming:\n");
    }
    else if (socket->sll_pkttype == PACKET_OUTGOING) {
        printf("Outgoing\n");
    }
    else if (socket->sll_pkttype == PACKET_BROADCAST) {
        printf("Broadcast\n");
    }
    int i = 0;
    while (i < l) {
        printf("%.2x", buffer[i]);
        i++;
    }
    printf("\n\n");
}



int main() {
    int sockfd;
    struct sockaddr_ll phAddr;
    char buffer[2048];
    sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0) {
        printf("can't make socket.");
        return -1;
    }
    int l = sizeof(struct sockaddr_ll);
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int byte_recv = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&phAddr, &l);
        if (byte_recv < 0) {
            printf("Packet receive failed.");
            break;
        }
        printf("Received a packet with size: %d bytes\n", byte_recv);
        Detect(buffer, &phAddr, byte_recv);
    }
    close(sockfd);
    return 0;
}

