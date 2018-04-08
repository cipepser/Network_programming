#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
	int 					sock;
	struct addrinfo 	hints, *result;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo("www.google.com", "https", &hints, &result);
	
	if(connetc(sock, result->ai_addr, result->ai_addrlen) < 0)
		exit(1);
	freeaddrinfo(result);
	
	shutdown(sock, SHUT_WR);

	close(sock);
}