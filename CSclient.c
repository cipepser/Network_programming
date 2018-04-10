#include "./mysocklib/mysock.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

// extern ssize_t recv_line(int, void *, size_t);

int main(int argc, char *argv[]) {
  extern char *optarg;
  extern int optind, opterr, optopt;
  int c, portNo, sock, maxfd, i, end_of_input = 0;
  fd_set rfd, rfd_keep;
  struct addrinfo hints, *result;
  char send_buf[1024], recv_buf[1024], *server = "localhost", *port = "10010";

  opterr = 0;
  while ((c = getopt(argc, argv, "p:s:h")) != -1) {
    switch (c) {
    case 'p':
      /* 一度数値に変換して妥当かどうか調べてみる */
      portNo = atoi(optarg);
      if (portNo <= 0) {
        fprintf(stderr, "Port No. must be integer!\n");
        exit(1);
      } else if (portNo > 65535) {
        fprintf(stderr, "Port No. must be less than 65535!\n");
        exit(1);
      }
      port = optarg;
      break;
    case 's':
      server = optarg;
      break;
    case 'h':
      fprintf(stderr, "%s [-p port No.] [-s server name] [-h]\n", argv[0]);
      exit(1);
    }
  }

  sock = socket(PF_INET, SOCK_STREAM, 0);

  FD_ZERO(&rfd_keep);
  FD_SET(0, &rfd_keep); // 標準出力
  FD_SET(sock, &rfd_keep);
  maxfd = sock + 1;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo(server, port, &hints, &result);

  if (connect(sock, result->ai_addr, result->ai_addrlen) < 0) {
    fprintf(stderr, "Can't connect to %s\n", server);
    exit(1);
  }
  freeaddrinfo(result);

  while (1) {
    rfd = rfd_keep;
    if (select(maxfd, &rfd, NULL, NULL, NULL) > 0) {
      if (FD_ISSET(sock, &rfd)) {
        if (recv(sock, recv_buf, sizeof(recv_buf), 0) == 0) {
					memset(recv_buf, 0, sizeof(recv_buf));
          if (end_of_input == 1)
            break;
          else {
            fprintf(stderr, "Connection closed by server.\n");
            exit(1);
          }
        }
        fputs(recv_buf, stderr);
      }

      if (FD_ISSET(0, &rfd)) {
        if (fgets(send_buf, sizeof(send_buf), stdin) == NULL) {
          end_of_input = 1;
          shutdown(sock, SHUT_WR);
          FD_CLR(0, &rfd_keep);
          continue;
        }
        send(sock, send_buf, strlen(send_buf), 0);
      }
    }
  }

  fprintf(stderr, "\nShutting down client");

  for (i = 0; i < 3; i++) {
    fprintf(stderr, ".");
    sleep(1);
  }

  fprintf(stderr, "done!\n");
  close(sock);
}