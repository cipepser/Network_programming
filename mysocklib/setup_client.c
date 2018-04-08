#include "mysock.h"

int setup_client (const char *server, const char *port) {
    int sock;
    struct addrinfo hints, *result;

    sock = socket (PF_INET, SOCK_STREAM, 0);
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(server, port, &hints, &result);
    
    if ((connect(sock, result->ai_addr, result->ai_addrlen) <= 0)) {
      return(-1);
   }else{
      return(sock);    /* connect済のソケットを返す */
   } 
    freeaddrinfo(result);
}
