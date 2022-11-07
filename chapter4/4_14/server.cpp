/*
    服务器端

*/
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>//read
#include<stdlib.h>//exit
#include<string.h>
int main (){
    //1.创建socket(用于监听的套接字)
    int lfd = socket(AF_INET,SOCK_STREAM,0); //流式协议TCP
    if(lfd == -1){
        perror("socket");
        exit(-1);
    }
    //2.绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    // inet_pton(AF_INET,"218.197.196.183",(void *)saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = 0 ; 
    saddr.sin_port = htons(9999);
    int ret = bind(lfd,(struct sockaddr *)& saddr,sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }
    //3.监听
    ret = listen(lfd,8);
    if(ret == -1){
        perror("listen");
        exit(-1);
    }    
    //4.接收客户端连接
    struct sockaddr_in clientadr;
    socklen_t le = sizeof(clientadr);
    int cfd = accept(lfd,(struct sockaddr * )&clientadr,&le);
    if(cfd == -1){
        perror("accept");
        exit(-1);
    }
    //输出客户端信息
    char clientIP[16];
    inet_ntop(AF_INET,&clientadr.sin_addr.s_addr,clientIP,sizeof(clientIP)); //IP
    unsigned short clientPort = ntohs (clientadr.sin_port);//端口
    printf("client IP is %s,port is %d\n",clientIP,clientPort);
  
  
   //5.获取客户端数据
    char recvBuf[1024]={0};
  
    while(1){
        int len = read(cfd,recvBuf,sizeof(recvBuf));
        if(len == -1){
            perror("read");
            exit(-1);
        }
        else if(len>0){
            printf("recv client data : %s\n",recvBuf);
        }else if(len == 0){
            //表示客户端断开连接
            printf("client closed...\n");
            break;
        }
        //给客户端发送数据
        sleep(1);
        const char *data =recvBuf;
        write(cfd,data,sizeof(data));
    }
   
    //关闭文件描述符
    close(cfd); //通信
    close(lfd); //监听
    return 0;
}