#include<stdlib.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<bits/stdc++.h>
#include<string.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<fcntl.h> //设置cfd阻塞
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
    //创建一个epoll实例
    int epfd= epoll_create(100);
    //将需要监听的文件描述符添加并设置
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;

    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&epev);
    struct epoll_event epevs[1024];
    while(1){
        int ret = epoll_wait(epfd,epevs,1024,-1);
        if(ret == -1){
            perror("epoll_wait");
            exit(-1);
        }
        printf("ret = %d\n",ret);
        for(int i=0;i<ret;i++){
            int curfd = epevs[i].data.fd;
            if(curfd == lfd){
                //表明监听的文件描述符有数据到达，有客户端要连接
                struct sockaddr_in clienaddr;
                socklen_t len = sizeof(clienaddr);
                int cfd = accept(lfd,(struct sockaddr * )&clienaddr,&len);
                //设置cfd属性为阻塞
                int flag = fcntl(cfd,F_GETFL);
                flag |=O_NONBLOCK;
                fcntl(cfd,F_SETFL,flag); //重新设置

                struct epoll_event epev;
                epev.events = EPOLLIN |EPOLLET;//设置了边缘触发
                epev.data.fd = cfd;
                epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&epev);

            }
            else { //不是监听的fd，而是用于通信的其他fd
                if(epevs[i].events&EPOLLOUT){
                    //如果检测到写操作就不处理
                    continue;
                }
                //一次性读出所有的数据（采用的方法是循环，因为他不会一直通报）
                char buf[5]={0};
                int len =0;

                while((len = read(epevs[i].data.fd,buf,sizeof(buf))) > 0){
                    //打印读到的数据
                    printf("recv data:%s\n",buf);
                    write(epevs[i].data.fd,buf,len);//读到多少写多少
                }
                if(len == 0){
                    printf("client closed...\n");
                    epoll_ctl(epfd,EPOLL_CTL_DEL,epevs[i].data.fd,NULL);
                    close(epevs[i].data.fd); //关闭通信用的文件描述符
                }
                else if(len == -1){
                    if(errno == EAGAIN){
                        printf("data over...\n");//数据是读完了
                    }
                    else {
                        perror("read");
                        exit(-1);
                    }
                }
            }
        }

    }
    close(lfd);
    close(epfd);
    return 0;
}