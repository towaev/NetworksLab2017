#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string.h>
//readn func
int readn(int fd, int n){
	
	int temp = 0;
	int counter= 0;
	char buf[256];
	bzero(buf, 256);
	
	while (counter < n) {
		temp = read(fd, buf, 255);
		if (temp < 0) {
       			 perror("ERROR reading from socket");
       				return(-1);
    		}		
		counter += temp;		
		if (n < counter){ 
			for (int i =n;i< counter;i++){
			buf[i]=' ';}
			printf("\n %s \n", buf);
			return 0;
		}			
	}	
	return 0;
}
int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    uint16_t portno;
    unsigned int clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    ssize_t n;
    int err;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here process will
       * go in sleep mode and will wait for the incoming connection
    */

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    /* If connection is established then start communicating */
    bzero(buffer, 256);
    n = 10;
    err = readn(newsockfd, n); // recv on Windows

    if (err < 0) {
        //perror("ERROR reading from socket");
        exit(1);
    }
//printf("Here is the message: %s\n", buffer);
    

    /* Write a response to the client */
    n = write(newsockfd, "I got your message", 18); // send on Windows

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    shutdown(sockfd,SHUT_RDWR);
    close(sockfd);
    shutdown(newsockfd,SHUT_RDWR);
    close(newsockfd);
    return 0;
}
