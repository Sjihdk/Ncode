/*




*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<string.h>
#include<bits/stdc++.h>
using namespace std;
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
    fd_set rdset,temp;
    FD_ZERO(&rdset);
    FD_SET(lfd,&rdset);
    int maxfd = lfd;
    while (1)
    {
        //调用select系统函数，让内核帮加测哪些文件描述符有数据
        temp = rdset; //让内核处理临时的fd_set 这样不会改变原本我们想要检查的文件描述符
        int ret = select(maxfd +1,&temp,NULL,NULL,NULL);
        if(ret == -1){
            perror("select");
            exit(-1);
        }
        else if(ret == 0){
            printf("不可能\n");
            continue;
        }
        else if(ret>0){
            //说明有文件描述符的数据发生了改变
            if(FD_ISSET(lfd,&temp)){
                //表示有新的客户端连接进来了 
                struct sockaddr_in clientaddr;
                socklen_t len =sizeof(clientaddr);
                int cfd = accept(lfd,(struct sockaddr *)&clientaddr,&len); //连接客户端，创建通信文件描述符
                //将新的文件描述符加入集合中
                FD_SET(cfd,&rdset); //这里不用改temp,因为刚创建，没有检查过是否接收数据
                maxfd = maxfd >cfd ? maxfd : cfd;
            }
            for(int i=lfd+1;i<maxfd+1;i++){
                if(FD_ISSET(i,&temp)){
                    //说明这个文件描述符对应的客户端发来了数据
                    char buf[1024]={0};
                    int len =read(i,buf,sizeof(buf));
                    // printf("len :%d\n",len);
                    if(len == -1){
                        perror("read");
                        exit(-1);
                    }
                    else if(len ==0){
                        printf("client closed...\n");
                        close(i); //关闭通信用的文件描述符
                        FD_CLR(i,&rdset);//清空的rdset
                    }
                    else if(len >0){
                        printf("read buf= %s\n",buf);
                        // printf("stlen:%ld\n",strlen(buf));
                        write(i,buf,strlen(buf));
                    }
                }
            }
        }
    }
    close(lfd);

    return 0;
}