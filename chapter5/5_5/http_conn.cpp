#include"http_conn.h"
#include<memory.h>//meset初始化数组
int http_conn::m_epollfd = -1; //所有的socket上的时间都被注册到同一个epoll对象中
int http_conn::m_user_count = 0;

//设置非阻塞
void setnonblocking (int fd){
    int old_flag = fcntl(fd,F_GETFL);
    old_flag |= O_NONBLOCK;
    fcntl(fd,F_SETFL,old_flag);
}

//向epoll添加
void addfd(int epollfd,int fd, bool one_shot){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLRDHUP | EPOLLET; //想要边缘触发的可以改成 | EPOLLET 
    //EPOLLRDHUP是断开连接时的触发事件，就不用等到read 得到0 来判断了
    if(one_shot){
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event); //添加到epoll
    //设置文件描述符非阻塞
    setnonblocking(fd);
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
    event.events = ev | EPOLLONESHOT | EPOLLHUP;
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
    init();
}

void http_conn::init(){
    m_check_state = CHECK_STATE_REQUESTLINE; //初始化状态为解析请求首行
    m_checked_index = 0;//每行当前解析的字符位置
    m_start_line = 0;
    m_read_index = 0;
    m_method = GET;
    m_url = 0;
    m_version = 0;
    memset(m_read_buf,0,sizeof(m_read_buf));
}

void http_conn::close_cone(){//关闭连接

    if(m_sockfd != -1){
        removefd(m_epollfd,m_sockfd);
        m_sockfd = -1;
        m_user_count-- ; //清除epoll中的值和减少客户
    }

}
 
//非阻塞的读写,循环读取客户数据直到没有数据可以读
bool http_conn::read(){
    if(m_read_index >= READ_BUFFER_SIZE){ //
        return false;
    }
    //读取到的字节
    int byte_read = 0;
    while(true){
        byte_read = recv(m_sockfd,m_read_buf+m_read_index,READ_BUFFER_SIZE-m_read_index,0);
        if(byte_read == -1){
            if(errno == EAGAIN || errno  == EWOULDBLOCK){
                //表示没有数据
                break;
            }
            return false;            
        }
        else if(byte_read == 0){
            //对方关闭连接
            return false;
        }
        m_read_index += byte_read; //改变读的位置
    }
    printf("读取到的数据%s\n",m_read_buf);
    return true;
};

bool http_conn::write(){
    printf("一次性写完数据\n");
    return true;
};

//由线程池中的工作线程调用，这是处理HTTP请求的入口函数
void http_conn::process(){
    //解析HTTP请求
    HTTP_CODE read_ret = process_read();
    if(read_ret == NO_REQUEST){
        modfd(m_epollfd,m_sockfd,EPOLLIN);//重置EPOLLSHOT使得下次能检测到epollin
        return ;
    }

    
    printf("parse request, create response\n");
    //生成响应

}

//主状态机，解析HTTP请求
http_conn::HTTP_CODE http_conn::process_read(){
    LINE_STATUS line_status = LINE_OK;
    HTTP_CODE ret = NO_REQUEST;
    char * text = 0;
    while (((m_check_state == CHECK_STATE_CONTENT) && (line_status == LINE_OK)) || ((line_status = parse_line()) == LINE_OK))
    {   //解析到了一行完成的数据，或者解析到了请求体，并且从状态也是LINE_OK
        //获取一行数据
         text = get_line();
         m_start_line = m_checked_index ;
         printf("got 1 http line :%s\n",text);
         switch (m_check_state)
         {
             case CHECK_STATE_REQUESTLINE:
             {
                ret = parse_request_line(text);
                if(ret == BAD_REQUEST){
                    return BAD_REQUEST;
                }
                break;
             }
             case CHECK_STATE_HEADER:
             {
                ret  = parse_request_headers(text);
                if(ret == BAD_REQUEST){
                    return BAD_REQUEST;
                }
                else if(ret == GET_REQUEST){
                    return do_request();
                }
             }
             case CHECK_STATE_CONTENT:
             {
                ret  = parse_request_content(text);
                if(ret == GET_REQUEST){ //成功
                    return do_request();
                }
                line_status = LINE_OPEN;//不完整
                break;
             }              
         
             default:
             {
                return INTERNAL_ERROR;
             }
       
         }

    }
    
    return NO_REQUEST;
};

//解析HTTP请求首行,获得请求方法、目标URL、HTTP版本
http_conn::HTTP_CODE http_conn::parse_request_line(char * text){}
http_conn::HTTP_CODE http_conn::parse_request_headers(char * text){};//解析HTTP请求头
http_conn::HTTP_CODE http_conn::parse_request_content(char * text){};//解析HTTP请求体
 
 
 
 //解析行
http_conn::LINE_STATUS http_conn::parse_line(){
    char temp;
    for(;m_checked_index < m_read_index; ++m_checked_index){
        temp = m_read_buf[m_checked_index];
        if(temp == '\r'){
            if((m_read_index+1) == m_read_index){
                return LINE_OPEN;
            }
            else if(m_read_buf[m_checked_index + 1] == '\n'){
                m_read_buf[m_checked_index++] = '\0';
                m_read_buf[m_checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD; //其余情况就是有问题
        }
        else if(temp == '\n'){
            if((m_checked_index >1)&&(m_read_buf[m_checked_index-1] == '\r')){
                m_read_buf[m_checked_index-1] = '\0';
                m_read_buf[m_checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}

http_conn::HTTP_CODE http_conn::do_request(){}