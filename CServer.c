#include "mysocklib/mysock.h"
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
// #include <sys/socket.h>
// #include <sys/types.h>
#include <unistd.h>
// #include <netdb.h>
// #include <arpa/inet.h>
#include <ctype.h>

/* 定数の定義 */
static const char *port = "10007";
#define MAX_CLIENTS 5
#define MAX_BUFSIZE 1024
#define MAX_NAMELEN 16

/* クライアント用構造体 */
struct client {
  int sock;
  char name[MAX_NAMELEN];
};

/* プロトタイプ宣言 */
static int initialize_clients(struct client *client);
static int register_client(struct client *client, int num, const char *name);
static int cleanup_client(struct client *client, int num, fd_set *keep);

static int accept_client(struct client *client, int wait_sock, fd_set *keep);
static int handle_client(struct client *client, int cur);

int main(int argc, char *argv[]) {
  int wait_sock, numfd, i, len;
  struct client client[MAX_CLIENTS];
  fd_set rfd, rfd_keep;

  initialize_clients(client);
  wait_sock = setup_server(port); /* ライブラリを利用 */

  FD_ZERO(&rfd_keep);
  FD_SET(wait_sock, &rfd_keep);

  while (1) {
    rfd = rfd_keep;

    if ((numfd = select(FD_SETSIZE, &rfd, NULL, NULL, NULL)) > 0) {
      if (FD_ISSET(wait_sock, &rfd)) {
        accept_client(client, wait_sock, &rfd_keep);
        if (--numfd == 0)
          continue;
      }

      for (i = 0; i < MAX_CLIENTS; i++) {
        if (client[i].sock > 0 && FD_ISSET(client[i].sock, &rfd)) {
          len = handle_client(client, i);
          if (len <= 0) {
            cleanup_client(client, i, &rfd_keep);
          }
          if (--numfd == 0)
            break;
        }
      }
    }
  }
}

static int initialize_clients(struct client *client) {
  int i;

  for (i = 0; i < MAX_CLIENTS; i++) {
    /* 課題6 : クライアント用構造体の初期化 */
    client[i].sock = -1;
    memset(&client[i].name, 0, sizeof(client[i].name));
  }

  return 0;
}

static int register_client(struct client *client, int num, const char *name) {
  int i;
  char tmpbuf[30];

  /* 最後のNULLの1バイト分だけ短くする */
  strncpy(client[num].name, name, MAX_NAMELEN - 1);

  /* 空白文字(改行など)が後ろに付いていたらNULLにする */
  for (i = MAX_NAMELEN - 1; i >= 0; i--) {
    if (isspace(client[num].name[i]))
      client[num].name[i] = '\0';
    else if (isalnum(client[num].name[i]))
      break; /* アルファベットか数字が現われたところでループを抜ける */
  }

  /* すべてのクライアントに送信する文字列の初期化 */
  snprintf(tmpbuf, sizeof(tmpbuf), "%s connected.\n", client[num].name);
  for (i = 0; i < MAX_CLIENTS; i++) {
    if (client[i].sock > 0) /* 接続しているソケットにのみ送信 */
      send_n(client[i].sock, tmpbuf, strlen(tmpbuf));
  }

  return 0;
}

static int cleanup_client(struct client *client, int num, fd_set *keep) {
  int i;
  char tmpbuf[30];

  /* 課題6 */
  // -
  // 終了したクライアントのソケットをfd_set型変数からクリアする(FD_CLR()を呼ぶ)
  FD_CLR(client[num].sock, keep);

  // - 終了したクライアントのsockを閉じてクリアする
  close(client[num].sock);

  // - クライアントが終了したことを他のクライアントに通知する
  snprintf(tmpbuf, sizeof(tmpbuf), "%s closed.\n", client[num].name);
  for (i = 0; i < MAX_CLIENTS; i++) {
    if (client[i].sock > 0) /* 接続しているソケットにのみ送信 */
      send_n(client[i].sock, tmpbuf, strlen(tmpbuf));
  }

  // - 終了したクライントのnameをクリアする
  memset(&client[num].name, 0, sizeof(client[num].name));

  return 0;
}

static int accept_client(struct client *client, int wait_sock, fd_set *keep) {
  int i;
  char message[] = "Please register your name : ";

  /* 課題7 */
  // 受付可能なクライアント・ソケットを見つける
  // 受付可能ならaccept()し、fd_set(=rfd_keep)にソケット・ディスクリプタをセット
  // 受け付けたクライアントに名前の入力を要求する
  for (i = 0; i < MAX_CLIENTS && client[i].sock != -1; i++);
  if (i < MAX_CLIENTS) {
    client[i].sock = accept(wait_sock, NULL, NULL);
    FD_SET(client[i].sock, keep);
    send_n(client[i].sock, message, strlen(message));
  }

  return 0;
}

static int handle_client(struct client *client, int cur) {
  int i;
  ssize_t len;
  char recv_buf[MAX_BUFSIZE - MAX_NAMELEN - 4], send_buf[MAX_BUFSIZE];

  if ((len = recv_line(client[cur].sock, recv_buf, sizeof(recv_buf))) > 0) {
    /* 課題 9 */
    if (*client[cur].name != 0) {
      snprintf(send_buf, sizeof(send_buf), "%s : %s", client[cur].name,
               recv_buf);
      for (i = 0; i < MAX_CLIENTS; i++) {
        if (client[i].sock > 0) /* 接続しているソケットにのみ送信 */
          send_n(client[i].sock, send_buf, strlen(send_buf));
      }
    } else {
      register_client(client, cur, recv_buf);
    }
  }

  return len;
}
