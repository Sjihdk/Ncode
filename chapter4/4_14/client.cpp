//TCP通信客户端
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>//read ,sleep
#include<stdlib.h>//exit
#include<string.h>
#include<bits/stdc++.h>
using namespace std;
int main(){
    //1.创建套接字
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }

    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family =AF_INET;
    inet_pton(AF_INET,"218.197.196.183",&serveraddr.sin_addr.s_addr); //字符串转成整数
    serveraddr.sin_port = htons(9999);
    int ret = connect(fd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    if(ret == -1){
        perror("connect");
        exit(-1);
    }

    //3.通信
    char recvBuf[1024]={0};
    // const char *data =" hello ,i am client";
    while(1){
        //给服务端发送数据
        char data[1024]={0};
        cin.getline(data,50);
        write(fd,data,sizeof(data));
        sleep(1);
        //接受服务端发来的数据
        int len = read(fd,recvBuf,sizeof(recvBuf));
        if(len == -1){
            perror("read");
            exit(-1);
        }
        else if(len>0){
            printf("recv server data : %s\n",recvBuf);
        }else if(len == 0){
            //表示客户端断开连接
            printf("server closed...\n");
            break;
        }
    }
 
    //关闭连接
    close(fd); //只有一个通信套接字
    return 0;
}