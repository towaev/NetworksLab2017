#include <string.h>
#include <stdio.h>  
#include <winsock2.h>
#pragma comment (lib, "Ws2_32.lib")

int main(int argc, char *argv[]) {
	WSADATA wsaData;   // if this doesn't work
					   //WSAData wsaData; // then try this instead
					   // MAKEWORD(1,1) for Winsock 1.1, MAKEWORD(2,0) for Winsock 2.0:


	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) { printf("WSAStartup failed.\n"); return -1; }

    int sockfd, n;
    size_t portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
		WSACleanup();
        exit(0);
    }

    portno = (size_t) atoi(argv[2]);

    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
		WSACleanup();
        exit(1);
    }

    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
		WSACleanup();
        exit(0);
    }

    //bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy( (char *) &serv_addr.sin_addr.s_addr, server->h_addr, (size_t) server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
		WSACleanup();
        exit(1);
    }

    /* Now ask for a message from the user, this message
       * will be read by server
    */

    printf("Please enter the message: ");
    memset(buffer,0, 256);
    fgets(buffer, 255, stdin);

    /* Send message to the server */
    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0) {
        perror("ERROR writing to socket");
		WSACleanup();
        exit(1);
    }

    /* Now read server response */
    memset(buffer,0, 256);
    n = read(sockfd, buffer, 255);

    if (n < 0) {
        perror("ERROR reading from socket");
		WSACleanup();
        exit(1);
    }

    printf("%s\n", buffer);
	if (WSACleanup() != 0) { printf("WSACleanup failed.\n"); return -1; }
   // printf("%s\n", buffer);
	closesocket(sockfd);
    shutdown(sockfd,SD_BOTH);

    return 0;
}