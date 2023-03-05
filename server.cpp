/*
 * 此程序用于演示多线程的socket通信服务端
*/
#include "ServerClass.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

CTcpServer TcpServer;

//SIGINT和SIGTERM的处理函数
void EXIT(int sig)
{
    printf("收到了信号%d,程序退出\n",sig);
    
    //这里可以添加释放资源的代码
    close(TcpServer.m_listenfd);

    exit(0);    //程序退出
}

//与客户端通信的主函数
void *pth_main(void *arg);

int main(int argc, char *argv[])
{
    if (argc!=2){
        printf("Using:./server port\nExample:./server 5005\n\n"); 
        return -1;
    }

    for(int i=0;i<99;i++)
        // sighandler_t signal(int signum, sighandler_t handler);
        //SIG_IGN：忽略参数signum所指的信号。
        signal(i, SIG_IGN);//屏蔽所有的信号

    //SIGINT 键盘中断Ctrl+c
    signal(SIGINT, EXIT);

    // SIGTERM 采用“kill   进程编号”或“killall 程序名”通知程序。
    signal(SIGTERM, EXIT);

    if(TcpServer.InitServer(atoi(argv[1]))==false)
    {
        printf("客户端初始化失败\n");
        return -1;
    }

    while(1)
    {
        if(TcpServer.Accept()==false)
            continue;

        
        
        pthread_t pthid;    //创建一线程，与新连接上来的客户端通信
        
        //线程资源的回收
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);  // 设置线程的属性。
        // int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
        // 参数thread为为指向线程标识符的地址。
        // 参数attr用于设置线程属性，一般为空，表示使用默认属性。
        // 参数start_routine是线程运行函数的地址，填函数名就可以了。
        // 参数arg是线程运行函数的参数。新创建的线程从start_routine函数的地址开始运行，该函数只有一个无类型指针参数arg。若要想向start_routine传递多个参数，可以将多个参数放在一个结构体中，然后把结构体的地址作为arg参数传入，但是要非常慎重，程序员一般不会这么做。
        if(pthread_create(&pthid,NULL,pth_main,(void *)((long)TcpServer.m_clientfd))!=0)
        {
            printf("创建线程失败，程序退出\n");
            return -1;
        }
        printf("与客户端通信的线程已创建。\n");
    }
}

void *pth_main(void *arg)
{
    int clientfd=(long) arg;    //arg参数为新客户端的socket
    
    char sendBuffer[1024];
        char recvBuffer[1024];
        int ret;

        //5 接受和发送数据
        while(1)
        {
            memset(sendBuffer, 0, sizeof(sendBuffer));
            memset(recvBuffer, 0, sizeof(recvBuffer));
            //recv函数<=0代表失败
            if((ret = recv(clientfd, recvBuffer,sizeof(recvBuffer), 0))<=0){
                printf("ret = %d\n", ret);
                break;
            }
            
            printf("client: %s\n", recvBuffer);

            strcpy(sendBuffer, "ok");

            //send函数<=0代表失败
            if(send(clientfd, sendBuffer, strlen(sendBuffer), 0)<=0){
                perror("send");
                break;
            }        
            printf("server: %s\n", sendBuffer);
            
       }

    printf("客户端已断开连接\n");
    close(clientfd);//关闭客户端连接
    pthread_exit(0);
}