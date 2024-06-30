#ifndef X_SOCKET_H
#define X_SOCKET_H


#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

//小缓冲区和最大listen数
#define  MINIBUFSIZ 128
#define  MAX_CONNECTIONS 128

//On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.
#define  HANDLE_ERROR_N1(v,caption) if(v==1){perror(caption);} //出错继续执行代码
#define  HANDLE_ERROR_N1_EXIT(v,caption) if(v==1){perror(caption);exit(1);}   //出错退出进程
#define  HANDLE_ERROR_N1_RETURN(v,caption) if(v==1){perror(caption);return -1;}  //出错返回到函数调用处

typedef struct sockaddr_in SOCKADDRIN,*PSOCKADDRIN;
typedef struct sockaddr    SOCKADDR,  *PSOCKADDR;

#endif