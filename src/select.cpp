//IO多路复用

#include <x-socket.h>



#define  SERVER_PORT 9999

//On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.
#define  HANDLE_ERROR_N1(v,caption) if(v==1){perror(caption);} //出错继续执行代码
#define  HANDLE_ERROR_N1_EXIT(v,caption) if(v==1){perror(caption);exit(1);}   //出错退出进程
#define  HANDLE_ERROR_N1_RETURN(v,caption) if(v==1){perror(caption);return -1;}  //出错返回到函数调用处


//绑定IP和PORT到SOCKADDR结构中
int bind_sockaddr(int fd,in_port_t port,const char* ip,PSOCKADDRIN psockaddr){

    //初始化psockaddr指向的内存 全置为 ascii 0
    bzero(psockaddr,sizeof(SOCKADDRIN));

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
    SOCKADDRIN server_sockaddr;

    //创建监听socket
    lfd = socket(AF_INET,SOCK_STREAM,0);
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




    return 0;

}