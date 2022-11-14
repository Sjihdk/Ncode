/*
    // 设置广播属性的函数
int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t 
optlen);
    - sockfd : 文件描述符
    - level : SOL_SOCKET
    - optname : SO_BROADCAST
    - optval : int类型的值，为1表示允许广播
    - optlen : optval的大小

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
    //设置广播属性
    int op = 1;
    setsockopt(fd,SOL_SOCKET,SO_BROADCAST,&op,sizeof(op));

    //创建一个广播的地址
    struct sockaddr_in caddr;
    caddr.sin_family =AF_INET;
    caddr.sin_port = htons(9999);
    inet_pton(AF_INET,"218.197.199.255",&caddr.sin_addr.s_addr);
 
    //3.通信
    int num=0;
    while(1){
        char buf[1024]={0};
        sprintf(buf,"hello ,client ...%d\n",num++);
        //发送数据
        sendto(fd,buf,strlen(buf)+1 ,0,(struct sockaddr *)&caddr,sizeof(caddr));
        printf("广播的数据%s\n",buf);
        sleep(1);
    }
    close(fd);
    return 0;
}