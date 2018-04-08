#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[]){
	struct addrinfo		hints, *result;
	struct sockaddr_in	saddr;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags 	= AI_CANONNAME;
	hints.ai_family 	= AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	getaddrinfo("www.google.com", "https", &hints, &result);
	memcpy(&saddr, result->ai_addr, result->ai_addrlen);
	
	printf("Host: %s (%s)\n", result->ai_canonname, inet_ntoa(saddr.sin_addr));
	printf("Port: %d\n", ntohs(saddr.sin_port));
	
	freeaddrinfo(result);
}