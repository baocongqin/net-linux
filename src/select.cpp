// IO多路复用

#include <x-socket.h>

#define SERVER_PORT 9999

// 绑定IP和PORT到SOCKADDR结构中
int bind_sockaddr(int fd, in_port_t port, const char *ip, PSOCKADDRIN psockaddr)
{

    // 初始化psockaddr指向的内存 全置为 ascii 0
    BZERO(psockaddr, sizeof(SOCKADDRIN));

    psockaddr->sin_family = AF_INET; // 默认IPV4
    psockaddr->sin_port = htons(port);
    if (ip == NULL) // ip为NULL 表示IP自动选择
        psockaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    else
    {
        inet_pton(AF_INET, ip, &psockaddr->sin_addr.s_addr);
    }

    return bind(fd, (PSOCKADDR)psockaddr, sizeof(SOCKADDRIN));
}

int main()
{

    // 定义变量
    int lfd, cfd;               // 文件描述符 监听用的  另外一个用于接受客户端
    int ret;                    // 作函数返回值用
    fd_set fd_set_io, fd_set_i; // io用作返回 i用作传入的样本
    int maxfd, n_read,opt=1;
    socklen_t addrlen;
    SOCKADDRIN server_sockaddr, client_sockaddr;

    // 套接字对应sockaddr
    SOCKADDRIN client_sockaddr_array[20] = {0};

    // 创建监听socket 并修改socket属性
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    ret = setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    HANDLE_ERROR_N1(ret,"setsockopt");
    // 如果出错 退出进程
    HANDLE_ERROR_N1_EXIT(lfd, "create socket");
    // 绑定IP 和 PORT
    ret = bind_sockaddr(lfd, SERVER_PORT, NULL, &server_sockaddr);
    // 如果出错 退出进程
    HANDLE_ERROR_N1_EXIT(ret, "bind");

    // 设置listen的上限
    ret = listen(lfd, MAX_CONNECTIONS);
    // 如果出错 退出进程
    HANDLE_ERROR_N1_EXIT(ret, "listen");

    // 使用select来监听网络
    /*
    int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);

    */

    maxfd = lfd; // 设置最大fd
    FD_ZERO(&fd_set_i);
    FD_SET(lfd, &fd_set_i);

    while (1)
    {

        fd_set_io = fd_set_i;
        ret = select(maxfd + 1, &fd_set_io, NULL, NULL, NULL); // 阻塞式监听

        HANDLE_ERROR_N1_EXIT(ret < 0 ? -1 : 1, "select"); // 出错退出进程

        if (FD_ISSET(lfd, &fd_set_io))
        {

            addrlen = sizeof(SOCKADDRIN);
            cfd = accept(lfd, (PSOCKADDR)&client_sockaddr, &addrlen);
            HANDLE_ERROR_N1_EXIT(cfd, "accept");
            FD_SET(cfd, &fd_set_i);
            maxfd = (cfd > maxfd ? cfd : maxfd);
            client_sockaddr_array[cfd] = client_sockaddr; // 保存客户端信息
            if (ret == 1)
                continue;
        }

        for (int i = lfd + 1; i <= maxfd; i++)
        {

            if (FD_ISSET(i, &fd_set_io))
            {

                char buf[2*MINIBUFSIZ] = {0};
                n_read = read(i, buf, sizeof(buf));
                if (n_read == 0) // 收到客户端关闭信号
                {
                    FD_CLR(i, &fd_set_i);
                    close(i); // 关闭这个套接字
                    continue;
                }
                else if (n_read == -1)
                {

                    // 出现错误
                    HANDLE_ERROR_N1(n_read, "read");
                    continue;
                }

                // 读取数据 并打印到标准输出 并回写
                if (buf[n_read-1] == '\n')
                        buf[n_read-1] = 0;
                char sip[MINIBUFSIZ] = {0};
                inet_ntop(AF_INET, &client_sockaddr_array[i].sin_addr.s_addr, sip, sizeof(sip));
                printf("%s@%d: %s\n", sip, ntohs(client_sockaddr_array[i].sin_port), buf); // 打印客户端IP PORT 及内容

                //追加文本
                strcat(buf," recevied\n");
                //回寫給客戶端
                write(i,buf,sizeof(buf));


            }
        }
    }


close(lfd);

return 0;
}