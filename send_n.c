#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

ssize_t send_n(int sockfd, const void *msg, size_t n, int flags){
	size_t 	nleft = n; // 未送信のバイト数
	ssize_t  nwritten; // 送信済みのバイト数
	const char*ptr = msg; // 未送信部分の先頭アドレス

	while(nleft > 0){
		if((nwritten = send(sockfd, ptr, nleft, flags)) <= 0){
			if(errno == EINTR){ // 割り込みが発生した
				nwritten = 0;    // 送信済みのバイト数を0にして、再度send()を呼び出す
			}else{
				return(-1); // エラーが発生
			}
		}
		nleft -= nwritten; // 残りのバイト数を減らす
		ptr   += nwritten; // 送信した分だけポインタを進める
	}
	return(n);
}