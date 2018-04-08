#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAXLEN 4096

/*
// MAXLEN文字分だけ一気に読む
// 空になったら再読込
*/
static ssize_t recv_buf(int sock, char *ptr){
   static int recv_count = 0;
   static char *recv_ptr;
   static char recv_buf[MAXLEN];
   
   if(recv_count <= 0){
      again:
      if((recv_count = recv(sock, recv_buf, sizeof(recv_buf), 0)) < 0){
         if(errno == EINTR) goto again;
         return(-1);
      }else if(recv_count == 0) return(0);
      recv_ptr = recv_buf;
   }
   recv_count--;
   *ptr = *recv_ptr++;
   return(1);
}


/*
// 改行文字まで読み込む
*/

ssize_t recv_line(int sock, void *vptr, size_t maxlen){
   ssize_t num, count;
   char c, *ptr;
   
   ptr = vptr;

   for(num = 1; num < maxlen; num++){
      if((count = recv_buf(sock, &c)) == 1){
         *ptr++ = c;
         if(c == '\n') break;
     }else if(count == 0){
         if(num == 1){
            return(0);
         }else{
            break;
         }
     }else{
        return(-1);
     }
  }
   
  *ptr = 0;
  return(num);
}
