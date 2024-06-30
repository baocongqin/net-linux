#include <x-socket.h>

#define  SERVER_PORT 9999





void handle_socket_error(int ret){

    if (ret == -1) 
    {
        perror("socket error");
        exit(100);
    }

}

//tcp服务器

int main(){

    int lfd; //连接用的socket
    int cfd; //与客户端传输数据的socket
    socklen_t addrlen;
    SOCKADDRIN  server_addr,client_addr; //服务端 和 客户端 的addr
    char buf[BUFSIZ];
    char client_ip[MINIBUFSIZ]={0};
    int ret;  //用作函数返回值

    //创建连接socket
    lfd = socket(AF_INET,SOCK_STREAM,0);
    handle_socket_error(lfd);
    

    //绑定ip和port
    memset(&server_addr,0,sizeof(SOCKADDRIN));
    server_addr.sin_family =  AF_INET;
    server_addr.sin_port   =  htons(SERVER_PORT);
    server_addr.sin_addr.s_addr   =  htonl(INADDR_ANY);
    ret = bind(lfd,(PSOCKADDR)&server_addr,sizeof(SOCKADDRIN));
    handle_socket_error(ret);

    //设置listen 的上限
    ret = listen(lfd,MAX_CONNECTIONS)   ;
    handle_socket_error(ret);


    //开始监听 接受客户端的连接
    addrlen = sizeof(SOCKADDRIN);
    memset(&client_addr,0,sizeof(SOCKADDRIN));
    cfd = accept(lfd,(PSOCKADDR)&client_addr,&addrlen);
    handle_socket_error(cfd);
    
    //显示客户端的IP地址和端口号
    if( inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,client_ip,sizeof(client_ip)) == NULL)
        perror("inet_ntop");
    
    printf("client ip:%s client port:%d\n",client_ip,ntohs(client_addr.sin_port));



    //接受客户端的数据转为大小并发送
    while(1){

            memset(buf,0,BUFSIZ);  //清零

            ret = read(cfd,buf,BUFSIZ);  //阻塞式读取 有了就读
            if (ret<=0)
                break;
            write(STDIN_FILENO,buf,ret);
    }

    printf("网络会话结束!\n");
    close(cfd);
    close(lfd);



    return 0;
}