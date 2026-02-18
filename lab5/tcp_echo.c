#include <stdio.h>
#include <sys/socket.h>     // socket(), connect(), send(), recv()
#include <arpa/inet.h>      // sockaddr_in, inet_addr()
#include <stdlib.h>         // exit(), atoi()
#include <string.h>         // memset(), strlen()
#include <unistd.h>         // close()

#define BUFSIZE 32

int main() {

    /* CREATE A TCP SOCKET */
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printf("Error in opening a socket\n");
        exit(0);
    }
    printf("Client Socket Created\n");

    /* CONSTRUCT SERVER ADDRESS STRUCTURE */
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));  
    // memset() fills memory with zeros

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8898);   // Change port if needed
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    // Specify server IP address here

    printf("Address assigned\n");

    /* ESTABLISH CONNECTION */
    int c = connect(sock, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
    printf("%d\n", c);

    if (c < 0) {
        printf("Error while establishing connection\n");
        exit(0);
    }
    printf("Connection Established\n");

    /* SEND DATA */
    printf("ENTER MESSAGE FOR SERVER (max 32 characters)\n");

    char msg[BUFSIZE];
    gets(msg);

    int bytesSent = send(sock, msg, strlen(msg), 0);
    if (bytesSent != strlen(msg)) {
        printf("Error while sending the message\n");
        exit(0);
    }
    printf("Data Sent\n");

    /* RECEIVE DATA */
    char recvBuffer[BUFSIZE];
    int bytesRecvd = recv(sock, recvBuffer, BUFSIZE - 1, 0);

    if (bytesRecvd < 0) {
        printf("Error while receiving data from server\n");
        exit(0);
    }

    recvBuffer[bytesRecvd] = '\0';
    printf("%s\n", recvBuffer);

    close(sock);
    return 0;
}
