#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<wait.h>//waitpid
#include<errno.h>//errno
void recycleChild(int arg){
    while(1){
        int ret = waitpid(-1,NULL,WNOHANG);
        if(ret == -1){
            //所有子进程回收完了
            break;
        }
        else if(ret == 0){
            //还有子进程活着
            break;
        }
        else if(ret >0){
            //被回收了
            printf("子进程%d 被回收了\n",ret);
        }
    }
}
int main(){
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler=recycleChild;
    //信号捕捉,通过这个来回收子进程
    sigaction(SIGCHLD,&act,NULL);
    
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
    //不断循环等待客户端连接
    while (1)
    {
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        //接受连接
        
        int cfd = accept(lfd,(struct  sockaddr *)&clientaddr,&len); //创建通信描述符，并把接受到的客户端信息存储起来
        if(cfd == -1){
            if(errno == EINTR){ //如果是被软中断后回来accept会产生错误,这时候要手动判断继续执行
                continue;
            }
            perror("accept");
            exit(-1);
        }
        //每一个连接进来，创建一个子进程进行通信
        pid_t pid = fork();
        if(pid>0){
            close(cfd);
        }
        if(pid==0){
            //子进程，获取客户端信息
            char cliIP[16];
            inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,cliIP,sizeof(cliIP)); //获取IP
            unsigned short clientPort = ntohs(clientaddr.sin_port);//获取端口
            printf("client IP is:%s,port is %d\n",cliIP,clientPort);
            //接受客户端发来的数据
            char recvBuf[1024];
            while(1){
                int len = read(cfd,&recvBuf,sizeof(recvBuf));
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
                write(cfd,recvBuf,strlen(recvBuf));//全部写入

            }
            close(cfd);
            exit(0);
        }//子进程结束

    }
}
