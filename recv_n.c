#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

ssize_t recv_n(int sockfd, void *buf, size_t n, int flags){
	size_t    nleft = n;
	ssize_t   nread;
	char      *ptr = buf;
	
	while(nleft > 0){
		if((nread = recv(sockfd, ptr, nleft, flags)) < 0){
			if(errno == EINTR){
				nread = 0;
			}else{
				return(-1);
			}
		}else if(nread == 0){
			break;
		}
		
		nleft -= nread;
		ptr   += nread;
	}
	
	return(n - nleft);
}