#include <x-socket.h>

#define MAXEPOLLSIZE 1024

int socket_list_bind(const char* ip_bind,const in_port_t port){

    int op =1,ret,lfd;
    SOCKADDRIN server_sockaddr={0};
    //创建socket
    lfd = socket(AF_INET,SOCK_STREAM,0);
    HANDLE_ERROR_N1_EXIT(lfd,"socket");
    ret = setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));
    HANDLE_ERROR_N1_EXIT(ret,"setsockopt");
    //bind
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);
    inet_pton(AF_INET,ip_bind,&server_sockaddr.sin_addr.s_addr);
    ret = bind(lfd,(PSOCKADDR)&server_sockaddr,sizeof(SOCKADDRIN));
    HANDLE_ERROR_N1_EXIT(ret,"bind");
    //listen
    ret = listen(lfd,MAX_CONNECTIONS);
    HANDLE_ERROR_N1_EXIT(ret,"listen");

    return lfd;
    

}


int main(int argc,char** argv){

    in_port_t port ;
    sscanf(argv[2],"%hu",&port);

    //创建监听用的socket描述符lfd
    int lfd = socket_list_bind(argv[1],port);

    //创建epoll根结点描述符
    int epfd =  epoll_create(MAXEPOLLSIZE);

    HANDLE_ERROR_N1_EXIT(epfd,"epoll_create");

    epoll_event event ;
    event.data.fd = lfd;
    event.events = EPOLLIN;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&event);

    //开始监听
    epoll_event events[MAXEPOLLSIZE];
    ret = epoll_wait(epfd,events,MAXEPOLLSIZE,-1); 
    HANDLE_ERROR_N1_EXIT(epfd,"epoll_wiat");

    

    
    


    return 0;
}