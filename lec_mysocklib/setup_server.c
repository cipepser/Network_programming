#include "mysock.h"

int
setup_server (const char *port)
{
    int wait_sock, sockopt;
    struct sockaddr_in	myaddr;

    wait_sock = socket (PF_INET, SOCK_STREAM, 0);

    /* ここの部分を各自で作成すること (参考: sample4-2.c 〜 sample6-2.c)

    /*
     * [仕様]
     *	1. エラーの際は -1 を返す
     *	2. 一度使用したポートもすぐに再利用できるようにする
     */

    return wait_sock;	/* listen()済の待ち受け用ソケットを返す */
}
