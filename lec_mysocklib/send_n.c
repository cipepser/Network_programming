#include "mysock.h"

ssize_t
send_n (int sock, const void *msg, size_t n)
{
    size_t	nleft = n;	/* 未送信のバイト数*/
    ssize_t	nwritten;	/* 送信済のバイト数*/
    const char	*ptr = msg;	/* 未送信部分の先頭アドレス(ポインタ) */

    while (nleft > 0) {
	if ((nwritten = send (sock, ptr, nleft, 0)) <= 0) {
	    if (errno == EINTR)	/* 割り込みが発生した*/
		nwritten = 0;	/* 再度send() を呼び出す*/
	    else
		return (-1);	/* エラーが発生*/
	}

	nleft -= nwritten;	/* 残りのバイト数を減らす*/
	ptr += nwritten;	/* 送信した分だけポインタを進める*/
    }
    return (n);
}
