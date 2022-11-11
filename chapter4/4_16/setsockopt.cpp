/*

    #include<sys/types.h>
    #include<sys/socket.h>
    int setsockopt(int sockfd,int level,int optname,const void*optval,socklen_t optlen);
    参数：
        -sockfd；要操作的文件描述符
        -level；级别，SOL_SOCKET(端口复用级别)
        -optname；选项名称
            -SO_REUSEADDR 复用地址
            -SO_REUSEPORT  复用端口
        -optval：端口复用的值（整型）
            -1：可以复用
            -0；不可
        -optlen：optval参数的大小
       
       
        端口复用需要在绑定之前用！
*/

