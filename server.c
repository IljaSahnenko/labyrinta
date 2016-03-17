#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "server.h"

int main ( int argc, char ** argv )
{
	int socket_fd;
	int new_socket_fd;
	int portn = DEFAULT_PORT_NUM;

	socklen_t cli_len;

	struct sockaddr_in serv_addr, cli_addr;
	/* man 7 ip */

	/* Creating socket */
	socket_fd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( socket_fd < 0 ) { printf( "SOCKET() error.\n" ); exit(1); }

	/* Binding */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	if ( argc == 2 ) portn = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portn);

	/* Listener */
	listen( socket_fd, NUM_OF_CONNECTIONS );
	cli_len = sizeof(cli_addr);
	printf("Listening on port %d...\n", portn);

	/* Accepting incoming */
	new_socket_fd = accept( socket_fd, (struct sockaddr *) &cli_addr, &cli_len );
	if ( new_socket_fd < 0 ) { printf("ACCEPT() error. \n"); exit(1); }


	for (;;sleep(10))
	{
		printf("tick...");
	} /* Here goes actual loop */


	/* Finishing work */
	close(new_socket_fd);
	close(socket_fd);

	return 0;
}
