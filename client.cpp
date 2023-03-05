/*
 * 程序名：client.cpp，用于测试多线程的socket通信客户端
*/
#include <cstddef>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "ClientClass.h"

int main(int argc, char *argv[])
{   
    CTcpClient TcpClient;

    if (argc!=3){
        printf("Using:./client ip port\nExample:./client 127.0.0.1 5005\n\n"); 
        return -1;
    }

    if(TcpClient.ConnectToServer(argv[1], atoi(argv[2]))==false){
        printf("TcpClient.ConnectToServer(%s,%s) failed,exit...\n",argv[1],argv[2]); 
        return -1;
    }

    char sendBuffer[1024];
    char recvBuffer[1024];
    char end1[20]="end";
    char end2[20]="quit";
    char end3[20]="bye";

    //3 收发消息
    while(1){
        memset(sendBuffer, 0, sizeof(sendBuffer));
        memset(recvBuffer, 0, sizeof(recvBuffer));

        printf("请输入你要发送的信息：\n");
        scanf("%s", sendBuffer);

        if(strncmp(sendBuffer, end1, 3)==0||strncmp(sendBuffer, end2, 3)==0||strncmp(sendBuffer, end3, 3)==0)
        {
            printf("thank for using\n");
            break;
        }

        //send函数 返回值<=0代表失败
        if(TcpClient.Send(sendBuffer, strlen(sendBuffer))<=0){
            perror("send");
            break;
        }
        printf("client: %s\n", sendBuffer);

        //recv函数 返回值<=0代表失败
        if(TcpClient.Recv(recvBuffer, sizeof(recvBuffer))<=0){
            break;
        }
        printf("server: %s\n", recvBuffer);
    }
    

    return 0;
}
 