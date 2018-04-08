#include <stdio.h>
#include <string.h>

#include "mysock.h"

int
main (int argc, char *argv)
{
    char    buf[1024];
    int	    sock;
    size_t  len;

    sock = setup_client ("localhost", "10007");

    while (1) {
	if (fgets (buf, sizeof (buf), stdin) == NULL)
	    break;
	len = strlen (buf);
	send_n (sock, buf, len);
	memset (buf, 0, sizeof (buf));
	recv_n (sock, buf, len);
	fputs (buf, stdout);
    }
    close (sock);
}
