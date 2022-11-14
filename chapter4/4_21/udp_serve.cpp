/*
    #include<sys/types.h>
    #include<sys/socket.h>
    ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen);
        - 参数：
            - sockfd : 通信的fd
            - buf : 要发送的数据
            - len : 发送数据的长度
            - flags : 0
            - dest_addr : 通信的另外一端的地址信息
            - addrlen : 地址的内存大小
                
        ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen);
        - 参数：
            - sockfd : 通信的fd
            - buf : 接收数据的数组
            - len : 数组的大小
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

int main(){
    //1.创建一个套接字
    int fd = socket(PF_INET,SOCK_DGRAM,0);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }
    struct sockaddr_in addr;
    addr.sin_family =AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;
    //绑定
    int ret = bind(fd,(struct sockaddr *)&addr,sizeof(addr));
    if(ret== -1){
        perror("bind");
        exit(-1);
    }
    //3.通信

    while(1){
        //接受数据
        char buf[1024]={0};
        char ipbuf[16]={0};
        struct  sockaddr_in clientaddr ;
        socklen_t len =sizeof(clientaddr);
        int num = recvfrom(fd,buf,sizeof(buf),0,(struct sockaddr *)&clientaddr,&len);
        printf("clent IP :%s,port %d\n",inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ipbuf,sizeof(ipbuf)),ntohs(clientaddr.sin_port));
        printf("client say: %s\n",buf);
        //发送数据
        sendto(fd,buf,strlen(buf),0,(struct sockaddr *)&clientaddr,sizeof(clientaddr));

    }
    close(fd);
    return 0;
}