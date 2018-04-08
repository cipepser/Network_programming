#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAXLEN	4096

/*
 * 一度にMAXLEN文字までバッファにrecv()し、バッファが空になるまではそこから
 * 一文字ずつ返す。バッファが空になったら再度recv()する
 */
static ssize_t
recv_buf (int sock, char *ptr)
{
    static int	recv_count = 0;
    static char	*recv_ptr;
    static char	recv_buf[MAXLEN];

    if (recv_count <= 0) {
again:
	if ((recv_count = recv (sock, recv_buf, sizeof (recv_buf), 0)) < 0) {
	    if (errno == EINTR)
		goto again;
	    return (-1);
	} else if (recv_count == 0)
	    return (0);
	recv_ptr = recv_buf;
    }
    recv_count--;
    *ptr = *recv_ptr++;
    return (1);
}

/*
 * 改行文字までの一行分をバッファに読みこむ
 */
ssize_t
recv_line (int sock, void *vptr, size_t maxlen)
{
    ssize_t	num, count;
    char	c, *ptr;

    ptr = vptr;

    /* 最後の終端文字(NULL)の分があるので、カウンタは1から始める */
    for (num = 1; num < maxlen; num++) {
	if ((count = recv_buf (sock, &c)) == 1) {
	    *ptr++ = c;
	    if (c == '\n')
		break;	    /* 改行まで格納したところでforループを抜ける */
	} else if (count == 0) {
	    if (num == 1)
		return (0); /* EOF受信, データなし */
	    else
		break;	    /* EOF受信, データあり(forループを抜ける) */
	} else
	    return (-1);    /* エラー発生 */
    }

    *ptr = 0;	/* バッファをNULL終端する */
    return (num);
}
