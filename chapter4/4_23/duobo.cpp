/*


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
    //设置多播属性
    struct in_addr imr_multiaddr;
    inet_pton(AF_INET,"239.0.0.10",&imr_multiaddr.s_addr);
    setsockopt(fd,IPPROTO_IP,IP_MULTICAST_IF,&imr_multiaddr,sizeof(imr_multiaddr));

    //初始化客户端信息
    struct sockaddr_in caddr;
    caddr.sin_family =AF_INET;
    caddr.sin_port = htons(9999);
    inet_pton(AF_INET,"239.0.0.10",&caddr.sin_addr.s_addr);
 
    //3.通信
    int num=0;
    while(1){
        char buf[1024]={0};
        sprintf(buf,"hello ,client ...%d\n",num++);
        //发送数据
        sendto(fd,buf,strlen(buf)+1 ,0,(struct sockaddr *)&caddr,sizeof(caddr));
        printf("组播的数据%s\n",buf);
        sleep(1);
    }
    close(fd);
    return 0;
}