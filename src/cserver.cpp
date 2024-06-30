#include <cserver.h>

CTcpServer::CTcpServer(const char* bind_ip,in_port_t port){

    BZERO(m_bind_ip,20);
    if (bind_ip==NULL){

            strcat(m_bind_ip,"0.0.0.0");
    }
    else
            strcat(m_bind_ip,bind_ip);

   // cout << m_bind_ip;

    m_port = port;



}


//服务器初始化工作  socket bind listen
void CTcpServer::init(){

   int op =1,ret;
    SOCKADDRIN server_sockaddr={0};
    //创建socket
    m_listenfd = socket(AF_INET,SOCK_STREAM,0);
    HANDLE_ERROR_N1_EXIT(m_listenfd,"socket");
    ret = setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));
    HANDLE_ERROR_N1_EXIT(ret,"setsockopt");
    //bind
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(m_port);
   // cout << server_sockaddr.sin_port;
    inet_pton(AF_INET,m_bind_ip,&server_sockaddr.sin_addr.s_addr);
    ret = bind(m_listenfd,(PSOCKADDR)&server_sockaddr,sizeof(SOCKADDRIN));
    HANDLE_ERROR_N1_EXIT(ret,"bind");
    //listen
    ret = listen(m_listenfd,MAX_CONNECTIONS);

}


int CTcpServer::lfd(){
    return m_listenfd;
}

CTcpServer::~CTcpServer(){

    close(m_listenfd);
    cout  << "服务器已关闭!\n";
}