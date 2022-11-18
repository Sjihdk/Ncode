#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h> //
#include<string.h>
#include<errno.h> //错误号
#include<sys/socket.h>
#include<fcntl.h> //文件描述符属性
#include<sys/epoll.h> //epoll
#include"locker.h" //锁相关
#include"threadpool.h"//线程池
#include<signal.h> //信号
#include"http_conn.h"

#define MAX_FD  65535 //最大文件描述符个数
#define MAX_EVENT_NUMBER 10000 //最大监听的个数
//添加信号捕捉
void addsig(int sig,void(handler)(int)){
    struct sigaction sa;
    memset(&sa,'\0',sizeof(sa));
    sa.sa_handler = handler;
    sigfillset(&sa.sa_mask);//临时阻塞集
    sigaction(sig,&sa,NULL);
} 

//添加文件描述符到epoll
extern void addfd(int epollfd,int fd, bool one_shot);

//从epoll中删除文件描述符
extern void removefd(int epollfd,int fd);

//修改文件描述符
extern void modfd(int epollfd,int fd,int ev);

int main(int argc,char* argv[]){
    if(argc <= 1){
        printf("按照如下格式运行：%s port_number\n",basename(argv[0]));
        exit(-1);
    }

    //获取端口号
    int port = atoi(argv[1]);
    
    //对SIGPIPE信号处理
    addsig(SIGPIPE,SIG_IGN);

    //创建线程池，初始化
    threadpool<http_conn> * pool = NULL;
    try{
        pool = new threadpool<http_conn>;
    }
    catch(...){
        exit(-1);
    }
    //创建一个数组用于保存所有的客户信息
    http_conn * users = new http_conn[MAX_FD];
    
    //创建监听的套接字
    int listenfd = socket(PF_INET,SOCK_STREAM,0);

    //设置端口复用
    int reuse = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEPORT,&reuse,sizeof(reuse));

    //绑定
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    bind(listenfd,(struct sockaddr *)&address,sizeof(address));

    //监听
    listen(listenfd,5);

    //创建epoll对象，事件数组，添加
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);

    //将监听的文件描述符添加到epoll对象中
    addfd(epollfd,listenfd,false);//不需要设置epollshot
    http_conn::m_epollfd = epollfd; //所有http连接用到的同一个epoll对象
    
    while(1){
        int num =epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        if(num < 0 && errno != EINTR){
            printf("epoll failure \n");
            break;
        }
        //循环遍历
        for(int i=0;i< num;i++){
            int sockfd = events[i].data.fd;
            if(sockfd == listenfd){
                //有客户端连接进来
                struct sockaddr_in client_address;
                socklen_t client_addlen = sizeof(client_address);
                int connfd = accept(listenfd,(struct sockaddr *)&client_address,&client_addlen);
                if(http_conn::m_user_count >= MAX_FD){
                    //表明目前的连接数满了
                    //给客户端写一个信息：服务器正忙
                    close(connfd);
                    continue;
                }    
                //将新的客户的数据初始化放入到数组中
                users[connfd].init(connfd,client_address);
            }
            else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) //对方异常断开或者错误等事件
            {
                users[sockfd].close_conn(); //关闭
            }
            else if(events[i].events & EPOLLIN){ //有读的数据
                if(users[sockfd].read()){
                    //一次性读完所有的数据
                    pool->append(users + sockfd);
                }
                else{
                    users[sockfd].close_conn(); //读失败了
                }
            }
            else if(events[i].events & EPOLLOUT){ //有写的数据，一次性写完所有数据
                if(!users[sockfd].write()){
                    users[sockfd].close_conn(); 
                }
            }
        }
    }
    close(epollfd);
    close(listenfd);
    delete [] users;
    delete pool;

    return 0;
}