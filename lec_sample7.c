#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>

static const char   *server = "localhost";
static const char   *port   = "10007";

/* Prototype declaration of recv_line() */
extern ssize_t recv_line (int, void *, size_t);

int
main (int argc, char *argv[])
{
    int	    sock, maxfd, i, end_of_input = 0;
    fd_set  rfd, rfd_keep;
    struct addrinfo hints, *result;
    char    send_buf[1024], recv_buf[1024];

    /*
     * getaddrinfo() accepts both the server name and the port number as 
     * character strings (char *), so we can use the variables in "argv[]"
     * as is.
     */
    if (argc == 2) {
	server = argv[1];
    } else if (argc == 3) {
	server = argv[1];
	port = argv[2];
    }

    sock = socket (PF_INET, SOCK_STREAM, 0);

    FD_ZERO (&rfd_keep);
    FD_SET (0, &rfd_keep);	/* For keyboard input (stdin) */
    FD_SET (sock, &rfd_keep);	/* For socket */
    maxfd = sock + 1;

    memset (&hints, 0, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo (server, port, &hints, &result);

    if (connect (sock, result->ai_addr, result->ai_addrlen) < 0) {
	fprintf (stderr, "Can't connect to %s\n", server);
	exit (1);
    }
    freeaddrinfo (result);

    while (1) {
	rfd = rfd_keep;

	/*
	 * Here, we set the timeout value as NULL to block program at select()
	 * until either keyboard input or socket becomes available.
	 * Of course, we can use a certain value.
	 */
	if (select (maxfd, &rfd, NULL, NULL, NULL) > 0) {
	    if (FD_ISSET (sock, &rfd)) {

		/* Data from socket is received by recv_line() */
		if (recv_line (sock, recv_buf, sizeof (recv_buf)) == 0) {
		    if (end_of_input = 1)
			break;
		    else {
			fprintf (stderr, "Connection closed by server.");
			exit (1);
		    }
		}
		fputs (recv_buf, stdout);
	    }

	    if (FD_ISSET (0, &rfd)) {
		if (fgets (send_buf, sizeof (send_buf), stdin) == NULL) {
		    /* "EOF" has been input */
		    end_of_input = 1;
		    shutdown (sock, SHUT_WR);
		    FD_CLR (0, &rfd_keep);
		    continue;
		}
		send (sock, send_buf, strlen (send_buf), 0);
	    }
	}
    }

    fprintf (stderr, "\nShutting down client");
    for (i = 0; i < 3; i++) {
	fprintf (stderr, ".");
	sleep (1);
    }
    fprintf (stderr, " done!\n");

    close (sock);
}
