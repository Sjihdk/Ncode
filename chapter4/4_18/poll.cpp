/*
    #include<poll.h>

    struct pollfd{
        int fd;
        short events;
        short revernts;
    }
    int poll(struct pollfd * fds, nfd_t nfds,int timeout);
    -参数:
        -fds:是一个struct pollfd 结构体数组，这是一个需要检测的文件描述符的几何
        -nfds: 最后一个有效下标+1;
        -timeout:阻塞时长 0不阻塞  -1阻塞直到检测的文件描述符有变化
    返回：
    -1失败
    >0(n):有n个文件描述符发生变化
*/

#include<poll.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<bits/stdc++.h>
#include<string.h>
#include<unistd.h>
int main(){
    //创建socket
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in saddr;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_family = AF_INET;
    saddr.sin_port=htons(9999);

    //绑定
    bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));

    //监听
    listen(lfd,8);

    //创建一个fd_set的集合 
    // fd_set rdset,temp;
    // FD_ZERO(&rdset);
    // FD_SET(lfd,&rdset);
    // int maxfd = lfd;

    //创建一个pollfd的数组并初始化
    struct pollfd fds[1024];
    for(int i=0;i<1024;i++){
        fds[i].fd=-1;
        fds[i].events=POLLIN;
    }
    fds[0].fd = lfd; //检查连接的套接字是否发生变化
    int nfds = 0;

    while (1)
    {
        //调用poll系统函数，让内核帮加测哪些文件描述符有数据
        int ret = poll(fds,nfds+1,-1);
        if(ret == -1){
            perror("poll");
            exit(-1);
        }
        else if(ret == 0){
            printf("不可能\n");
            continue;
        }
        else if(ret>0){
            //说明有文件描述符的数据发生了改变
            if(fds[0].revents&POLLIN){
                //表示有新的客户端连接进来了 
                struct sockaddr_in clientaddr;
                socklen_t len =sizeof(clientaddr);
                int cfd = accept(lfd,(struct sockaddr *)&clientaddr,&len); //连接客户端，创建通信文件描述符
                //将新的文件描述符加入集合中
                for(int i=0;i<1024;i++){
                    if(fds[i].fd == -1){
                        fds[i].fd=cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }

                nfds = nfds >cfd ? nfds : cfd;
            }
            for(int i=1;i<nfds+1;i++){
                if(fds[i].revents&POLLIN){
                    //说明这个文件描述符对应的客户端发来了数据
                    char buf[1024]={0};
                    int len =read(fds[i].fd,buf,sizeof(buf));
                    // printf("len :%d\n",len);
                    if(len == -1){
                        perror("read");
                        exit(-1);
                    }
                    else if(len ==0){
                        printf("client closed...\n");
                        close(fds[i].fd); //关闭通信用的文件描述符
                        fds[i].fd = -1;//清空
                    }
                    else if(len >0){
                        printf("read buf= %s\n",buf);
                        // printf("stlen:%ld\n",strlen(buf));
                        write(fds[i].fd,buf,strlen(buf));
                    }
                }
            }
        }
    }
    close(lfd);

    return 0;
}