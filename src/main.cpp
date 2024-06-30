#include <cserver.h>

int main(int argc ,char** argv){

    in_port_t port;
    sscanf(argv[1],"%hu",&port);

    CSelectServer ss(NULL,port);

    ss.init();
    ss.run();



    /*
    CTcpServer server(NULL,port);
    
    server.init();
    SOCKADDRIN client_sockaddr;
    socklen_t  sockaddr_len = sizeof(SOCKADDRIN);
    
    int cfd  = accept(server.lfd(),(PSOCKADDR)&client_sockaddr,&sockaddr_len);
    
    printf("有客户端主机连上\n");

    close(cfd);
    */

    return 0;

}