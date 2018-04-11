/*
 * mysock.h
 */

#ifndef MYSOCK_H_INCLUDED__
#define MYSOCK_H_INCLUDED__

#include <string.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

/* libmysock.aに含まれる関数の宣言 */
extern int	setup_client (const char *server, const char *port);
extern int	setup_server (const char *port);
extern ssize_t	recv_line (int sock, void *vptr, size_t maxlen);
extern ssize_t	send_n (int sock, const void *msg, size_t n);
extern ssize_t	recv_n (int sock, void *buf, size_t n);

#endif /* !defined (MYSOCK_H_INCLUDED__) */
