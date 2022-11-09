/*
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // 包含了这个头文件，上面两个就可以省略


int socket(int domain, int type, int protocol);
- 功能：创建一个套接字
- 参数：
- domain: 协议族
AF_INET : ipv4
AF_INET6 : ipv6
AF_UNIX, AF_LOCAL : 本地套接字通信（进程间通信）
- type: 通信过程中使用的协议类型
SOCK_STREAM : 流式协议
SOCK_DGRAM : 报式协议
- protocol : 具体的一个协议。一般写0
- SOCK_STREAM : 流式协议默认使用 TCP
- SOCK_DGRAM : 报式协议默认使用 UDP
- 返回值：
- 成功：返回文件描述符，操作的就是内核缓冲区。
- 失败：-1


int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); // socket命
名
- 功能：绑定，将fd 和本地的IP + 端口进行绑定
- 参数：
- sockfd : 通过socket函数得到的文件描述符
- addr : 需要绑定的socket地址，这个地址封装了ip和端口号的信息
- addrlen : 第二个参数结构体占的内存大小


int listen(int sockfd, int backlog); // /proc/sys/net/core/somaxconn
- 功能：监听这个socket上的连接
- 参数：
- sockfd : 通过socket()函数得到的文件描述符
- backlog : 未连接的和已经连接的和的最大值， 5


int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
- 功能：接收客户端连接，默认是一个阻塞的函数，阻塞等待客户端连接
- 参数：
- sockfd : 用于监听的文件描述符
- addr : 传出参数，记录了连接成功后客户端的地址信息（ip，port）
- addrlen : 指定第二个参数的对应的内存大小
- 返回值：
- 成功 ：用于通信的文件描述符
- -1 ： 失败


int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
- 功能： 客户端连接服务器
- 参数：
- sockfd : 用于通信的文件描述符
- addr : 客户端要连接的服务器的地址信息
- addrlen : 第二个参数的内存大小
- 返回值：成功 0， 失败 -1
ssize_t write(int fd, const void *buf, size_t count); // 写数据
ssize_t read(int fd, void *buf, size_t count);  //读数据


*/