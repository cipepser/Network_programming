#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAXLEN	4096

/*
 * ���٤�MAXLENʸ���ޤǥХåե���recv()�����Хåե������ˤʤ�ޤǤϤ�������
 * ��ʸ�������֤����Хåե������ˤʤä������recv()����
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
 * ����ʸ���ޤǤΰ��ʬ��Хåե����ɤߤ���
 */
ssize_t
recv_line (int sock, void *vptr, size_t maxlen)
{
    ssize_t	num, count;
    char	c, *ptr;

    ptr = vptr;

    /* �Ǹ�ν�üʸ��(NULL)��ʬ������Τǡ������󥿤�1����Ϥ�� */
    for (num = 1; num < maxlen; num++) {
	if ((count = recv_buf (sock, &c)) == 1) {
	    *ptr++ = c;
	    if (c == '\n')
		break;	    /* ���Ԥޤǳ�Ǽ�����Ȥ����for�롼�פ�ȴ���� */
	} else if (count == 0) {
	    if (num == 1)
		return (0); /* EOF����, �ǡ����ʤ� */
	    else
		break;	    /* EOF����, �ǡ�������(for�롼�פ�ȴ����) */
	} else
	    return (-1);    /* ���顼ȯ�� */
    }

    *ptr = 0;	/* �Хåե���NULL��ü���� */
    return (num);
}
