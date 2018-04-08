#include "mysock.h"

ssize_t
recv_n (int sock, void *buf, size_t n)
{
    size_t	nleft = n;	/* 未受信のバイト数*/
    ssize_t	nread;		/* 受信済のバイト数*/
    char 	*ptr = buf;	/* 未受信部分の先頭アドレス(ポインタ) */

    while (nleft > 0) {
	if ((nread = recv (sock, ptr, nleft, 0)) < 0) {
	    if (errno == EINTR)	/* 割り込みが発生した*/
		nread = 0;	/* 再度recv() を呼び出す*/
	    else
		return (-1);	/* エラーが発生*/
	} else if (nread == 0)	/* ソケットが閉じられた(EOFを受信した) */
	    break;		/* whileループを抜ける */

	nleft -= nread;		/* 残りのバイト数を減らす*/
	ptr += nread;		/* 受信した分だけポインタを進める*/
    }

    return (n - nleft);
}
