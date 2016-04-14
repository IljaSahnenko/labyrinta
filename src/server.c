#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../config.h"

void
err_send(const char * input)
{
	printf(input);
	exit(1);
}

int
main ( int argc, char ** argv )
{
	/* listen on socket_fd, new connection on new_socket_fd */
	int socket_fd;
	int new_socket_fd;
	int portn = DEFAULT_PORT_NUM;
	socklen_t cli_len;

	/* Message insides */
	int mesg;
	char bufx[BUFFER_SIZE];

	/* address information: rost address and remote address */
	struct sockaddr_in serv_addr, cli_addr;
	/* man 7 ip */


	/* Creating socket */
	printf("Socket...\t");
	socket_fd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( socket_fd < 0 )
		err_send("ERROR\n");
	else
		printf("OK\n");


	/* Binding */
	printf("Binding...\t");
//	bzero((char *) &serv_addr, sizeof(serv_addr));
	if ( argc == 2 ) portn = atoi(argv[1]);
	/* host byte order */
	serv_addr.sin_family = AF_INET;
	/* auto-fill with host IP */
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	/* short, network byte order */
	serv_addr.sin_port = htons(portn);
	/* zero the rest of the struct */
	memset(&(serv_addr.sin_zero), 0, 8);
	if ( bind(socket_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1 )
		err_send("ERROR\n");
	else
		printf("OK\n");


	/* Listener */
	printf("Listening...\t");
	if (listen( socket_fd, NUM_OF_CONNECTIONS ) == -1 )
		err_send("ERROR\n");
	else
		printf("OK, on port %d\n", portn);


	/* Accepting incoming */
	cli_len = sizeof(cli_addr);

	for ( ;; sleep(1) )
	{
		new_socket_fd = accept( socket_fd, (struct sockaddr *) &cli_addr, &cli_len );
		if ( new_socket_fd < 0 )
			err_send("\nACCEPT ERROR\n");
		else
			printf("New client!\n");

		for ( ;; sleep(1) )
		{
			bzero(bufx, BUFFER_SIZE);
			mesg = read( new_socket_fd, bufx, BUFFER_SIZE - 1 );
			if ( mesg < 0 )
				err_send("Error in receiving!\n");
			printf("Accepted message: %s", bufx);
			if ( !strcmp(bufx, "exit") )
			{
				printf("Initiated disconnect...\n");
				break;
			}
		}
		close(new_socket_fd);
		printf("Client is disconnected.\n");
	}

	/* Finishing work */
	printf("Closing connections...");
//	close(new_socket_fd);
	close(socket_fd);


	return 0;
}
