
/*
 * [仕様]
 *	1. エラーの際は -1 を返す
 *	2. 一度使用したポートもすぐに再利用できるようにする
 */

#include "mysock.h"

int setup_server(const char *port) {
  int wait_sock, sockopt;
  struct sockaddr_in myaddr;

  wait_sock = socket(PF_INET, SOCK_STREAM, 0);

  sockopt = 1;
  setsockopt(wait_sock, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));

  memset(&myaddr, 0, sizeof(myaddr));
  myaddr.sin_family = AF_INET;
  myaddr.sin_port = htons((u_int16_t)atoi(port));
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  bind(wait_sock, (struct sockaddr *)&myaddr, sizeof(myaddr));

  if (listen(wait_sock, 5) < 0) {
    return -1;
  } else {
    return wait_sock; /* listen()済の待ち受け用ソケットを返す */
  }
}
