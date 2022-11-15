#include"http_conn.h"

//向epoll添加
void addfd(int epollfd,int fd, bool one_shot){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLRDHUP; //想要边缘触发的可以改成 | EPOLLET 
    //EPOLLRDHUP是断开连接时的触发事件，就不用等到read 得到0 来判断了
    if(one_shot){
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event); //添加到epoll
}

//从epoll删除
void removefd(int epollfd,int fd){
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
}

//修改文件描述符,重置socket上的EPOOLONESHOT事件，确保下一次EPOLLIN可以触发
void modfd(int epollfd,int fd,int ev){
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLONESHOT |EPOLLHUP;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd, &event);

}

//初始化连接的fd与socketaddress
void http_conn::init(int sockfd,const sockaddr_in & addr){
    m_sockfd = sockfd;
    m_address = addr;
    //设置端口复用
    int reuse = 1;
    setsockopt(m_sockfd,SOL_SOCKET,SO_REUSEPORT,&reuse,sizeof(reuse));
    //添加到epoll对象中
    addfd(m_epollfd,sockfd,true);
    m_user_count++;//总用户数还要加1
}