#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
//操作共享内存需要的头文件
#include <sys/ipc.h>
#include <sys/shm.h>
//操作信号量需要的头文件
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

//用C++的方法封装socket服务端
// TCP服务端类
class CTcpServer
{
    public:
        int m_listenfd; //服务端练上来的socket
        int m_clientfd; //客户端练上来的socket

        CTcpServer();

        ~CTcpServer();

    public:
        //初始化服务器
        bool InitServer(int port);
        //等待客户端的连接
        bool Accept();

        //向对端发送报文
        int Send(const void *buf, const int buflen);
        //接受对端报文
        int Recv(void *buf, const int buflen);

        //关闭客户端socket
        void CloseClient();
        //关闭服务端socket
        void CloseListen();
};