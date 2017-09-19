#include <string.h>
#include <stdio.h>  
#include <winsock2.h>
#pragma comment (lib, "Ws2_32.lib")
//readn func
int readn(int fd, int n){
	
	int temp = 0;
	int counter= 0;
	char buf[256];	
	memset(buf, 0, 256);
	while (counter < n) {
		temp = recv(fd, buf, 255, 0);
		
		if (temp < 0) {
       			 perror("ERROR reading from socket");
				 WSACleanup();
       		     return(-1);
    		}		
		counter += temp;		
		if (n <= counter){ 
			for (int i =n;i< counter;i++){
			buf[i]=' ';}
			printf("\n %s \n", buf);
			return 0;
		}			
	}	
	return 0;
}
int main() {
	WSADATA wsaData;   // if this doesn't work
					   //WSAData wsaData; // then try this instead
					   // MAKEWORD(1,1) for Winsock 1.1, MAKEWORD(2,0) for Winsock 2.0:


	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) { printf("WSAStartup failed.\n"); return -1; }
	
    int sockfd, newsockfd;
    size_t portno;
    unsigned int clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    size_t n;
    int err;
	int rc;
    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
		WSACleanup();
        exit(1);
    }

    /* Initialize socket structure */
   // bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
		WSACleanup();
        exit(1);
    }

    /* Now start listening for the clients, here process will
       * go in sleep mode and will wait for the incoming connection
    */

    rc = listen(sockfd, 5);
	if (rc)
	{
		perror("listen call failed");
		WSACleanup();
		return(3);
	}
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0) {
        perror("ERROR on accept");
		WSACleanup();
        exit(1);
    }
	printf("Connection established");
    /* If connection is established then start communicating */
   memset(buffer, 0, 256);
    n = 12;
    err = readn(newsockfd, n); // recv on Windows

    if (err < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }
//printf("Here is the message: %s\n", buffer);
    

    /* Write a response to the client */
    n = send(newsockfd, "I got your message", 18, 0); // send on Windows

    if (n < 0) {
        perror("ERROR writing to socket");
		WSACleanup();
        exit(1);
    }
	if (WSACleanup() != 0) { printf("WSACleanup failed.\n"); return -1;	}
    shutdown(sockfd,SD_BOTH);
    closesocket(sockfd);
    shutdown(newsockfd,SD_BOTH);
    closesocket(newsockfd);	
	getch();
    return 0;
}
