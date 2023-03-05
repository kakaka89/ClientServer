#include "ServerClass.h"
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


/**************************************/
CTcpServer::CTcpServer()
{
    m_listenfd=0;
    m_clientfd=0;
}

CTcpServer::~CTcpServer(){
    if(m_listenfd!=0)
        close(m_listenfd);
    if(m_clientfd!=0)
        close(m_clientfd);
}

bool CTcpServer::InitServer(const int port){
    if(m_listenfd!=0)
    {
        close(m_listenfd);
        m_listenfd=0;
    }

    // 第1步：创建服务端的socket。
    if ( (m_listenfd = socket(AF_INET,SOCK_STREAM,0))==-1) 
    { 
        perror("socket"); 
        return false; 
    }

     //2. 进行绑定
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;     //使用ipv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  //任意ip地址均可连
    // servaddr.sin_addr.s_addr = inet_addr("127.0.0.7"); //指定ip地址
    servaddr.sin_port = htons(port);  //设置端口号
    //bind函数 成功返回0 失败返回-1
    if(bind(m_listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr))!=0){
        perror("bind");
        close(m_listenfd);
        return false;
    }

    //3 设置监听模式  listen函数 成功返回0 失败返回-1
    if(listen(m_listenfd, 20)!=0){
        perror("listen");
        close(m_listenfd);
        return false;
    }
    return true;
}

bool CTcpServer::Accept(){
    //4 接收客户端的连接
    int  socklen=sizeof(struct sockaddr_in); // struct sockaddr_in的大小
    struct sockaddr_in clientaddr;  // 客户端的地址信息。
    

    if((m_clientfd = accept(m_listenfd, 0, 0))<=0)
        return false;
    printf("客户端（%s）已连接。\n",inet_ntoa(clientaddr.sin_addr));

    return true;
}

int CTcpServer::Send(const void *buf, const int buflen){
    return send(m_clientfd, buf, buflen, 0);
}

int CTcpServer::Recv(void *buf,const int buflen)
{
  return recv(m_clientfd,buf,buflen,0);
}

void CTcpServer::CloseClient()
{
    if(m_clientfd!=0)
        close(m_clientfd);
    m_clientfd=0;
}

void CTcpServer::CloseListen()
{
    if(m_listenfd!=0)
        close(m_listenfd);
    m_listenfd=0;
}
/**************************************/
