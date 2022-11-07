/*
    #include<bits/socket.h>
    struct sockaddr{    
        sa_family_t sa_family;
        char sa_data[14];
    };

    typedef unsigned short int sa_family_t;
  
    #include<arpa/inet.h>
    p:点分十进制的IP字符串，n：表示network，网络字节序的整数
    int intet_pton(int af,const char *src,void * dst);

    const char * inet_ntop(int af,const void *src ,char * dst,socklen_t size);

  

*/
#include<stdio.h>
#include<arpa/inet.h>
#include<string.h>

int main(){
    //创建一个IP字符串，点分十进制的IP字符串
    char buf[] = "1.0.1.0";
 
    unsigned int num = 0;
    //转换成数字
    inet_pton(AF_INET,buf,&num);
    printf("%ld\n",num);
    unsigned char * p = (unsigned  char *)&num;
    printf("%d,%d,%d,%d\n",*p,*(p+1),*(p+2),*(p+3));
    
    //转换成字符串
    char ip[16]="";
    const char * str = inet_ntop(AF_INET,&num,ip,16);
    printf("%s\n",str);

    return 0;
}