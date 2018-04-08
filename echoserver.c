#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int 						wait_sock, cli_sock, rcv_size;
	struct sockaddr_in 	myaddr;
	char 						buf[1024];
		
	wait_sock = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port   = htons(10007);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(wait_sock, (struct sockaddr *)&myaddr, sizeof(myaddr));
	listen(wait_sock, 5);
	
	while(1){
		cli_sock = accept(wait_sock, NULL, NULL);
		while((rcv_size = recv(cli_sock, buf, sizeof(buf) - 1, 0)) > 0){
			send(cli_sock, buf, rcv_size, 0);
			memset(buf, 0, sizeof(buf));
		}
		close(cli_sock);
	}
	close(wait_sock);
}

