#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  extern char *optarg;
  extern int optind, opterr, optopt;
  int c, portNo;
  char *server_name = "localhost", *port_string = "10010";

  opterr = 0;
  while ((c = getopt(argc, argv, "p:s:h")) != -1) {
    switch (c) {
    case 'p':
      /* 一度数値に変換して妥当かどうか調べてみる */
      portNo = atoi(optarg);
      if (portNo == 0) {
        fprintf(stderr, "Port No. must be integer!\n");
        exit(1);
      } else if (portNo > 65535) {
        fprintf(stderr, "Port No. must be less than 65535!\n");
        exit(1);
      }
      port_string = optarg;
      break;
    case 's':
      server_name = optarg;
      break;
    case 'h':
      fprintf(stderr, "%s [-p port No.] [-s server name] [-h]\n", argv[0]);
      exit(1);
    }
  }

  fprintf(stderr, "Server Name : %s\n", server_name);
  fprintf(stderr, "Port No. : %s\n", port_string);
}
