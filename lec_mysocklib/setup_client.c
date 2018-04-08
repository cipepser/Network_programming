#include "mysock.h"

int
setup_client (const char *server, const char *port)
{
    int sock;
    struct addrinfo hints, *result;

    sock = socket (PF_INET, SOCK_STREAM, 0);

    /* ここの部分を各自で作成すること (参考: sample3.c) */

    /*
     * [仕様]
     *	1. エラーの際には -1 を返す
     */

    return sock;    /* connect済のソケットを返す */
}
