#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<pthread.h>
struct sockInfo
{
        int fd; //通信文件描述符
        pthread_t tid;
        struct sockaddr_in in_addr;
    /* data */
};
struct sockInfo  sockinfos[128];
void * working(void * arg){
    //子线程与客户端通信
    //子进程，获取客户端信息
    //转换指针
    struct sockInfo * pinfo = (struct sockInfo *)arg;
    char cliIP[16];
    inet_ntop(AF_INET,&pinfo->in_addr.sin_addr.s_addr,cliIP,sizeof(cliIP)); //获取IP
    unsigned short clientPort = ntohs(pinfo->in_addr.sin_port);//获取端口
    printf("client IP is:%s,port is %d\n",cliIP,clientPort);
    //接受客户端发来的数据
    char recvBuf[1024];
    while(1){
        int len = read(pinfo->fd,&recvBuf,sizeof(recvBuf));
        if(len == -1){
            perror("read");
            exit(-1);
        }    
        else if(len >0){
            printf("recv client data: %s\n",recvBuf);
        }
        else {
            printf("client closed...\n");
            break;
        }
        write(pinfo->fd,recvBuf,strlen(recvBuf));//全部写入

    }
    close(pinfo->fd);
    return NULL;
}
int main(){
    //创建套接字
    int lfd = socket(AF_INET,SOCK_STREAM,0);;
    if(lfd == -1){
        perror("socket");
        exit(-1);
    }
    //绑定IP、端口至文件描述符
    struct sockaddr_in saddr;
    saddr.sin_family  = AF_INET;
    saddr.sin_port = htons (9999);
    saddr.sin_addr.s_addr = INADDR_ANY;
    
    int ret = bind(lfd,(struct sockaddr *) &saddr,sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }
    //监听
    ret = listen(lfd,128);
    if(ret == -1){
        perror("listen");
        exit(-1);        
    }
    //初始化全局变量结构体
    for(int i=0;i<128;i++){
        bzero(&sockinfos[i],sizeof(sockinfos[i]));
        sockinfos[i].fd = -1; //表明文件描述符可用
        sockinfos[i].tid = -1;
    }

    //循环等待连接,每有一个连接，产生一个线程处理
    while(1){
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        //接受连接  
        int cfd = accept(lfd,(struct  sockaddr *)&clientaddr,&len); //创建通信描述符，并把接受到的客户端信息存储起来
        if(cfd == -1){
            perror("accept");
            exit(-1);
        }
        //创建子线程
        struct sockInfo * pinfo;
        for(int i=0;i<128;i++){
            //从数组中找到一个可以用的变量
            if(sockinfos[i].fd == -1){
                pinfo  = &sockinfos[i];
                break;
            }
            if(i == 127){
                sleep(1);
                i--;
            }
        }   
        pinfo->fd = cfd; //拷贝用于通信文件描述符
        memcpy(&pinfo->in_addr,&clientaddr,len); //拷贝客户端地址信息
        pthread_create(&(pinfo->tid),NULL,working,pinfo); //记得最后传入我们所需的结构体参数指针
        pthread_detach(pinfo->tid); //设置线程分离，自己释放资源
    }
    close(lfd);
    return 0;
}