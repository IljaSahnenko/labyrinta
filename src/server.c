#include <arpa/inet.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../config.h"

#define max(a, b) ((a) > (b) ? (a) : (b))


/* Utility function: send a NUL-terminated string on a socket. */
static ssize_t send_string(int sockfd, const char *str, int flags)
{
	return send(sockfd, str, strlen(str), flags);
}

/*
 * Filter out negative values in an array of ints.
 * Return the new array count.
 */
static int filter_out_negatives(int *fds, int count)
{
	int i;
	int new_count;

	for (i = 0, new_count = 0; i < count; i++)
	{
		if (fds[i] >= 0)
			fds[new_count++] = fds[i];
	}

	return new_count;
}

void
err_send(const char * input)
{
	printf(input);
	exit(1);
}

int
main ( int argc, char ** argv )
{
	/* listen on socket_fd */
	int socket_fd;
	int i;
	int rc;
	int portn = DEFAULT_PORT_NUM;
//	socklen_t cli_len;
	int client_count = 0;
	int clients[NUM_OF_CONNECTIONS];

	/* Message insides */
//	int mesg;
	char bufx[BUFFER_SIZE];

	/* address information: rost address and remote address */
	struct sockaddr_in serv_addr;//, cli_addr;
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
	for ( i = 0; i < 10; ++i )
	{
		bzero((char *) &serv_addr, sizeof(serv_addr));
		if ( argc == 2 ) portn = atoi(argv[1]);
		/* host byte order */
		serv_addr.sin_family = AF_INET;
		/* auto-fill with host IP */
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		/* short, network byte order */
		serv_addr.sin_port = htons(portn);
		/* zero the rest of the struct */
		memset(&(serv_addr.sin_zero), 0, 8);
		if ( bind( socket_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr) ) == -1 )
		{
			if (i < 9) printf("ERROR on %d, trying next port...", portn);
			else err_send("Binding failed\n");
			++portn;
		}
		else
		{
			printf("OK\n");
			break;
		}
	}

	i = 0;

	/* Accepting incoming, new connection on new_socket_fd */
//	cli_len = sizeof(cli_addr);
	/*	int new_socket_fd; */

	/* Listener */
	printf("Listening...\t");
	if (listen( socket_fd, NUM_OF_CONNECTIONS ) == -1 )
		err_send("ERROR\n");
	else
		printf("OK, on port %d\n", portn);


	for ( ;; )
	{
		/*
		 * Listen for activity on the server socket.  It will be readable
		 * when a client attempts to connect.
		 *
		 * The nfds argument needs to be the
		 * highest-numbered file descriptor we supply, plus one.
		 */
		int nfds;
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(socket_fd, &readfds);
		nfds = socket_fd + 1;
		/* Listen for activity on any client sockets. */
		for (i = 0; i < client_count; i++)
		{
			FD_SET(clients[i], &readfds);
			nfds = max(nfds, clients[i] + 1);
		}
		/* Wait for activity from one or more of the sockets specified above. */
		rc = select(nfds, &readfds, NULL, NULL, NULL);
		if (rc < 0)
		{
			warn("select");
			continue;
		}

		/* Check for activity on client sockets. */
		for (i = 0; i < client_count; i++)
		{
			if (FD_ISSET(clients[i], &readfds))
			{
				/*
				 * If the client sends us
				 * a long message, it's not a big deal.
				 */
				ssize_t readlen;

				readlen = recv(clients[i], bufx, sizeof(bufx), 0);
				if (readlen < 0)
				{
					warn("recv");
				}
				else if (readlen == 0)
				{
					/* Client closed the connection. */
					if (close(clients[i]) < 0)
						err(1, "close (1)");

					/*
					 * Set client socket to -1.  We'll remove it
					 * at the end of this loop.
					 */
					clients[i] = -1;
				}
				else
				{
					if (send_string(clients[i], "message received by server.\n", 0) < 0)
						warn("send (2)");
					//			if (send(clients[i], bufx, readlen, 0) < 0)
					//				warn("send (3)");
					printf("someone said: %s\n", bufx);
				}
			}
		}

		/* Filter out closed clients. */
		client_count = filter_out_negatives(clients, client_count);

		/*
		 * If there is activity on the server socket, it means someone
		 * is trying to connect to us.
		 */
		if (FD_ISSET(socket_fd, &readfds))
		{
			int client;

			client = accept(socket_fd, NULL, NULL);
			if (client < 0)
				err(1, "accept");

			if (client_count < NUM_OF_CONNECTIONS)
			{
				clients[client_count++] = client;

				if (send_string(client, "welcome!\n", 0) < 0)
					err(1, "send (4)");
			}
			else
			{
				if (send_string(client, "too many connections\n", 0) < 0)
					err(1, "send (5)");
				if (close(client) < 0)
					err(1, "close (2)");
			}
		}





		/* Accepting incoming connection
		int new_socket_fd = accept( socket_fd, (struct sockaddr *) &cli_addr, &cli_len );
		if ( new_socket_fd < 0 )
			err_send("\nACCEPT ERROR\n");
		else
			printf("New client!\n");

		for ( ;; )
		{
			bzero(bufx, BUFFER_SIZE);

			* Reading incoming message
			mesg = read( new_socket_fd, bufx, BUFFER_SIZE - 1 );
			if ( mesg < 0 )

				err_send("Error in receiving!\n");
			printf("Accepted message: %s", bufx);

			if ( bufx[0] == '1' )
			{
				printf("\nSPECIAL ACTION GOES HERE\n");
			}

			char * tmp[BUFFER_SIZE];
			strcpy( tmp, bufx );

			sprintf( bufx, "Accepted %s", &tmp );

			mesg = write( new_socket_fd, bufx, strlen(bufx) + 1 );

		}
		close(new_socket_fd);
		printf("Client is disconnected.\n"); */
	}

	/* Finishing work */
	printf("Closing connections...");
	close(socket_fd);


	return 0;
}
