/*
    1.创建
    int lfd =  socket (AF_LOCAL,SOCK_STREAM,0);
    2.绑定
    3.监听
    4.等待接受，创建通信文件描述符
    5.通信
    6.关闭连接
// 本地套接字通信的流程 - tcp
// 服务器端
1. 创建监听的套接字
    int lfd = socket(AF_UNIX/AF_LOCAL, SOCK_STREAM, 0);
2. 监听的套接字绑定本地的套接字文件 -> server端
    struct sockaddr_un addr;
    // 绑定成功之后，指定的sun_path中的套接字文件会自动生成。
    bind(lfd, addr, len);
3. 监听
    listen(lfd, 100);
4. 等待并接受连接请求
    struct sockaddr_un cliaddr;
    int cfd = accept(lfd, &cliaddr, len);
5. 通信
    接收数据：read/recv
    发送数据：write/send
6. 关闭连接
    close();
// 客户端的流程
1. 创建通信的套接字
    int fd = socket(AF_UNIX/AF_LOCAL, SOCK_STREAM, 0);
2. 监听的套接字绑定本地的IP 端口
    struct sockaddr_un addr;
    // 绑定成功之后，指定的sun_path中的套接字文件会自动生成。
    bind(lfd, addr, len);
3. 连接服务器
    struct sockaddr_un serveraddr;
    connect(fd, &serveraddr, sizeof(serveraddr));
4. 通信
    接收数据：read/recv
    发送数据：write/send
5. 关闭连接
    close(); 


*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/un.h>

int main(){

    unlink("client.sock");
    //1创建监听的套接字
    int cfd = socket(AF_LOCAL,SOCK_STREAM,0);
    if(cfd == -1){
        perror("socket");
        exit(-1);
    }
    //2 绑定本地套接字文件
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path,"client.sock");
    int ret = bind(cfd,(struct sockaddr *)&addr,sizeof(addr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    //3.连接服务器
    struct sockaddr_un seraddr;
    seraddr.sun_family = AF_LOCAL;
    strcpy(seraddr.sun_path,"server.sock");
    ret = connect(cfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
    if(ret == -1){
        perror("connect");
        exit(-1);
    }
    printf("client socket filename :%s\n",seraddr.sun_path);
    //通信
    int num=0;
    while (1)
    {  
         //发送数据
        char buf[128]={0};
        sprintf(buf,"hello, i am client %d\n",num++);
        send(cfd,buf,strlen(buf),0);
        printf("client say:%s\n",buf);
       
        //接收数据
        int len = recv(cfd,buf,sizeof(buf),0);
        if(len == -1){
            perror("recv");
            exit(-1);
        }
        else if(len == 0){
            printf("client closed... \n");
            break;
        }
        else if(len >0){
            printf("client say :%s\n",buf);
        }
        sleep(1);
        /* code */
    }
    close(cfd);
    return 0;
}
