#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
	int 					sock;
	struct addrinfo	hints, *result;
	char					buf[80], *command = "HEAD / HTTP/1.0\r\n\r\n";
	memset(buf, 0, sizeof(buf));
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo("www.eb.waseda.ac.jp", "http", &hints, &result);

	if(connect(sock, result->ai_addr, result->ai_addrlen) < 0)
		exit(1);
	freeaddrinfo(result);

	send(sock, command, strlen(command), 0);
	while(recv(sock, buf, sizeof(buf) - 1, 0) > 0){
		printf("%s", buf);
		memset(buf, 0, sizeof(buf));
	}
	close(sock);
}
