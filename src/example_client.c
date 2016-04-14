#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "../config.h"

#define h_addr h_addr_list[0]


void error(const char *msg)
{
	perror(msg);
	exit(0);
}
int main(int argc, char *argv[])
{
	int sockfd, portno, n = 0;
	struct sockaddr_in serv_addr;
	struct hostent * server;
	char buffer[BUFFER_SIZE];

	portno = DEFAULT_PORT_NUM;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	server = gethostbyname(DEFAULT_HOST);
	if (server == NULL)
	{
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length );
	serv_addr.sin_port = htons(portno);

	if ( connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0 )
		error("ERROR connecting");

	for ( ;; )
	{
		printf("Please enter the message: ");
		bzero(buffer, BUFFER_SIZE);
		if ( fgets(buffer, BUFFER_SIZE - 1, stdin) )
			n = write(sockfd, buffer, strlen(buffer));
		if (n < 0)
			error("ERROR writing to socket");

		bzero(buffer,BUFFER_SIZE);
		n = read( sockfd, buffer, BUFFER_SIZE );
		if (n < 0)
			error("ERROR reading from socket");
		printf("%s",buffer);
	}

	close(sockfd);
	return 0;
}
