#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

static const int MAX_CLIENTS = 5;
static const char   *message = "Please input something!\r\n";

int
main (int argc, char *argv[])
{
    int	    wait_sock, cli_sock[MAX_CLIENTS], sockopt, numfd, rcv_size, i;
    struct sockaddr_in	myaddr;
    fd_set  rfd, rfd_keep;
    struct timeval  timeout;
    char    buf[1024];

    memset (buf, 0, sizeof (buf));

    wait_sock = socket (PF_INET, SOCK_STREAM, 0);

    sockopt = 1;
    setsockopt (wait_sock, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof (sockopt));

    memset (&myaddr, 0, sizeof (myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons (10007);
    myaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    bind (wait_sock, (struct sockaddr *)&myaddr, sizeof (myaddr));

    listen (wait_sock, 5);

    FD_ZERO (&rfd_keep);
    FD_SET (wait_sock, &rfd_keep);

    /* Initialize sockets for each client */
    for (i = 0; i < MAX_CLIENTS; i++)
	cli_sock[i] = -1;

    while (1) {
	/* Initialize a fd_set used by select() */
	rfd = rfd_keep;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	if ((numfd = select (FD_SETSIZE, &rfd, NULL, NULL, &timeout)) > 0) {
	    /* Check if there is a connection request */
	    if (FD_ISSET (wait_sock, &rfd)) {
		/* Search for an unused client socket */
		for (i = 0; i < MAX_CLIENTS && cli_sock[i] != -1; i++)
		    ;	/* no operation */

		/* accept() if there is an unused socket available */
		if (i < MAX_CLIENTS) {
		    cli_sock[i] = accept (wait_sock, NULL, NULL);
		    /* Set a descriptor to a fd_set variable */
		    FD_SET (cli_sock[i], &rfd_keep);

		    fprintf (stderr, "Client[%d] accepted.\n", i);
		}

		/* Skip next 'for' loop unless there are unhandled events */
		if (--numfd <= 0)
		    continue;
	    } /* if (FD_ISSET (wait_sock, ...)) */

	    /* Check for each client socket */
	    for (i = 0; i < MAX_CLIENTS; i++) {

		/* Receive data if the socket is connected and there are any data */
		if (cli_sock[i] > 0 && FD_ISSET (cli_sock[i], &rfd)) {
		    rcv_size = recv (cli_sock[i], buf, sizeof (buf) - 1, 0);

		    /* Received some data */
		    if (rcv_size > 0) {
			send (cli_sock[i], buf, rcv_size, 0); /* Send back */
			memset (buf, 0, sizeof (buf));
		    } else { /* Connection was closed */
			close (cli_sock[i]);
			FD_CLR (cli_sock[i], &rfd_keep); /* Clear a fd_set */
			cli_sock[i] = -1;	/* Make the socket reusable */

			fprintf (stderr, "Client[%d] closed.\n", i);
		    }

		    /* Stop this loop unless there are unhandled events */
		    if (--numfd <= 0)
			break;
		}
	    } /* for (i = 0; ...) */
	} else if (numfd == 0) {    /* Timeout event occured in select() */
	    for (i = 0; i < MAX_CLIENTS; i++)
		if (cli_sock[i] > 0)
		    send (cli_sock[i], message, strlen (message), 0);
	}
    } /* while (1) */
}