#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<fcntl.h> //文件描述符属性
#include<sys/epoll.h> //epoll
#include<signal.h> //信号
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/stat.h> //状态
#include<sys/mman.h>//内存映射
#include<stdarg.h>
#include<errno.h>
#include<sys/uio.h>
#include"locker.h"
class http_conn
{
private:
    /* data */
    int m_sockfd; //该HTTP连接的socket
    sockaddr_in m_address;//通信的socket地址
public:
    static int m_epollfd; //所有的socket上的时间都被注册到同一个epoll对象中
    static int m_user_count; //统计用户的数量
    http_conn(/* args */){};
    ~http_conn(){};
    void init(int sockfd,const sockaddr_in & addr);
    void process(); //处理客户端请求
};







#endif