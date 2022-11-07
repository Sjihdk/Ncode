/*
    网络字节顺序采用的是大端排序的方式

    字节序转换函数
    h - host  主机，主机字节序
    to -  转换成什么
    n - network 网络字节序
    s - short unsigned short 
    l- long     unsigned int
    #include<arpa/inet.h>
    uint16_t htons (uint16_t hostshort)  //用来转换端口
    uint16_t ntohs (uint16_t netshort)  //
    uint32_t htonl (uint32_t hostlong)  //用来转换ip 
    uint32_t ntohs (uint16_t netlong)  //
*/

#include<stdio.h>
#include<arpa/inet.h>

int main(){
    //htons 转换端口
    unsigned short a = 0x0102;
    printf("a:%x\n",a);
    unsigned short b = htons(a);
    printf("a:%x\n",a);
    
    //htonl 转换ip
    unsigned char buf[4]= {1,0,1,0};
    unsigned int num = *(unsigned int * )buf;
    printf("%d\n",num);
    unsigned int n= htonl(num); //返回值是转换成大端的数
    printf("%d\n",n);
    unsigned char * p = (unsigned char *)&num;
    printf("%d,%d,%d,%d\n",*p,*(p+1),*(p+2),*(p+3));
        unsigned char * p1 = (unsigned char *)&n;
    printf("%d,%d,%d,%d\n",*p1,*(p1+1),*(p1+2),*(p1+3));
     
    return 0;
}