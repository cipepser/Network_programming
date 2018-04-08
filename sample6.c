#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

// #include <netdb.h>
// #include <arpa/inet.h>

static const int MAX_CLIENTS = 5;

int main (int argc, char *argv[]) {
	int wait_sock, cli_sock[MAX_CLIENTS], sockopt, numfd, rcv_size, i;
	struct sockaddr_in	myaddr;
	fd_set	rfd, rfd_keep;
	char	buf[1024];
	
	memset(buf, 0, sizeof(buf));
	
	wait_sock = socket(PF_INET, SOCK_STREAM, 0);
	
	sockopt = 1;
	setsockopt(wait_sock, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));
	
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port   = htons(10007);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(wait_sock, (struct sockaddr *)&myaddr, sizeof(myaddr));
	
	listen(wait_sock, 5);
		
	FD_ZERO(&rfd_keep);
	FD_SET(wait_sock, &rfd_keep);
	
	// クライアント用socketを初期化
	for(i = 0; i < MAX_CLIENTS; i++) {
		cli_sock[i] = -1;
	}
	
	while(1) {
		// select()を呼び出すとrfdがクリアされるので初期化する
		rfd = rfd_keep;
		
		// イベントが発生したか
		if((numfd = select(FD_SETSIZE, &rfd, NULL, NULL, NULL)) > 0) {

			// 接続要求があるか
			if(FD_ISSET(wait_sock, &rfd)) {
				// 接続可能なsocketまで進める
				for(i = 0; i < MAX_CLIENTS && cli_sock[i] != -1; i++);
				
				// 接続可能な場合
				if(i < MAX_CLIENTS) {
					cli_sock[i] = accept(wait_sock, NULL, NULL);
					FD_SET(cli_sock[i], &rfd_keep);
					
					fprintf(stderr, "Client[%d] accepted.\n", i);
				}
				
				// イベントが残っていない場合は次のループへ
				if(--numfd <= 0){
					continue;
				}
			}
			
			for(i = 0; i < MAX_CLIENTS; i++){
				// cli_sock[i]が接続済み、かつ、データを受信
				if(cli_sock[i] > 0 && FD_ISSET(cli_sock[i], &rfd)){
					// 受信処理
					rcv_size = recv(cli_sock[i], buf, sizeof(buf) - 1, 0);
					
					if(rcv_size > 0){ // 届いていたらechoする
						send(cli_sock[i], buf, rcv_size, 0);
						memset(buf, 0, sizeof(buf));
					} else { // 切断処理
						close(cli_sock[i]);
						FD_CLR(cli_sock[i], &rfd_keep);
						cli_sock[i] = -1;
						
						fprintf(stderr, "Client[%d] closed.\n", i);
					}
					// イベントが残っていない場合は次のループへ
					if(--numfd <= 0){
						break;
					}
				}
			}
			
		}
	}
}