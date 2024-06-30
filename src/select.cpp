//IO多路复用

#include <x-socket.h>



#define  SERVER_PORT 9999




//绑定IP和PORT到SOCKADDR结构中
int bind_sockaddr(int fd,in_port_t port,const char* ip,PSOCKADDRIN psockaddr){

    //初始化psockaddr指向的内存 全置为 ascii 0
    BZERO(psockaddr,sizeof(SOCKADDRIN));
    

    psockaddr->sin_family = AF_INET; //默认IPV4
    psockaddr->sin_port = htons(port);
    if (ip == NULL)  //ip为NULL 表示IP自动选择
        psockaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    else
        {
            inet_pton(AF_INET,ip,&psockaddr->sin_addr.s_addr);

        }
    
    return bind(fd,(PSOCKADDR)psockaddr,sizeof(SOCKADDRIN));

}


int main(){


    //定义变量
    int lfd; //文件描述符 监听用的
    int ret; //作函数返回值用
    fd_set fd_set_io,fd_set_i;  //io用作返回 i用作传入的样本
    int maxfd;
    socklen_t addrlen;
    SOCKADDRIN server_sockaddr,client_sockaddr;

    //创建监听socket 并修改socket属性
    lfd = socket(AF_INET,SOCK_STREAM,0);
    //setsockopt(ldf,SO_REUSEADDR
    //如果出错 退出进程
    HANDLE_ERROR_N1_EXIT(lfd,"create socket");  
    //绑定IP 和 PORT
    ret = bind_sockaddr(lfd,SERVER_PORT,NULL,&server_sockaddr);
    //如果出错 退出进程
    HANDLE_ERROR_N1_EXIT(ret,"bind");

    //设置listen的上限
    ret = listen(lfd,MAX_CONNECTIONS);
    //如果出错 退出进程
    HANDLE_ERROR_N1_EXIT(ret,"listen");

    //使用select来监听网络
    /*
    int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);

    */
    FD_ZERO(&fd_set_i);
    FD_SET(lfd,&fd_set_i);
    fd_set_io = fd_set_i;
    maxfd = lfd;  //设置最大fd

    ret = select(maxfd+1,&fd_set_io,NULL,NULL,NULL); //阻塞式监听

    //如果出错 退出进程
    HANDLE_ERROR_N1_EXIT(ret,"select");

    //工作代码
    int cfd = accept(lfd,(PSOCKADDR)&client_sockaddr,&addrlen);
    char buf[MINIBUFSIZ] = {0};
    ret = read(cfd,buf,MINIBUFSIZ);
    write(STDOUT_FILENO,buf,ret);
    close (cfd);
    close (lfd);

    //结束
    std::cout << "finish my job!\n";





    return 0;

}