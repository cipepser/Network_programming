#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	pid_t 					pid;
	int 						listenfd, connfd, rcv_size, sockopt;
	struct sockaddr_in 	myaddr;
	char 						buf[1024];
		
	listenfd = socket(PF_INET, SOCK_STREAM, 0);
	
	sockopt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));
	
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port   = htons(10007);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(listenfd, (struct sockaddr *)&myaddr, sizeof(myaddr));
	listen(listenfd, 5);
	
	while(1){
		connfd = accept(listenfd, NULL, NULL);
		
		if((pid = fork()) == 0) {
			close(listenfd);
			
			// 子プロセスで行う処理
			while((rcv_size = recv(connfd, buf, sizeof(buf) - 1, 0)) > 0){
				send(connfd, buf, rcv_size, 0);
				memset(buf, 0, sizeof(buf));
			}

			close(connfd);
			exit(0);
		}
		close(connfd);
		
	}
}

