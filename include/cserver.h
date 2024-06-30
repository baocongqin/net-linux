#ifndef _CSERVER_H
#define _CSERVER_H
#include <x-socket.h>
#include <map>

typedef map<int,PSOCKADDRIN> CLIENTSOCKETLIST;

class CTcpServer
{
    protected:

        int m_listenfd;
        in_port_t m_port;
        char m_bind_ip[20];
        CLIENTSOCKETLIST m_client_socket_list;
       




    public:
        CTcpServer(const char* bind_ip,in_port_t port);
        virtual ~CTcpServer();
        void init();
        virtual void run() = 0;

        int lfd();
        


};


class CSelectServer:public CTcpServer{
    public:

        CSelectServer(const char* bind_ip,in_port_t port):CTcpServer(bind_ip,port){}
        void run();



};



#endif