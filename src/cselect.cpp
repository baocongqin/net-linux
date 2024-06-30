#include <cserver.h>


void CSelectServer::run(){

    int maxfd = m_listenfd,n_fds;
    fd_set readfs,readfs_bak;

    FD_ZERO(&readfs_bak);
    FD_SET(m_listenfd,&readfs_bak);

    while(1)
    {

         readfs = readfs_bak;
         int n_fds= select(maxfd+1,&readfs,NULL,NULL,NULL); // 阻塞
         HANDLE_ERROR_N1_EXIT(n_fds,"select");

        //先判断是不是有客户端连接上来
        if (FD_ISSET(m_listenfd,&readfs)){
            
        }

    }
   


}