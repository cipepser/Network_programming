#include <stdio.h>

#include "mysock.h"

int
main (int argc, char *argv)
{
    int wait_sock, cli_sock;
    char    buf[1024];
    ssize_t len;

    wait_sock = setup_server ("10007");

    while (1) {
	cli_sock = accept (wait_sock, NULL, NULL);

	while ((len = recv_line (cli_sock, buf, sizeof (buf))) > 0) {
	    send_n (cli_sock, buf, len);
	}

	close (cli_sock);
    }
}
