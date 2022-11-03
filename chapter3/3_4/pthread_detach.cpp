/*
       #include <pthread.h>

       int pthread_detach(pthread_t thread);
       功能：分离线程，分离的线程在终止时会自动释放资源返回给系统
        1.不能多次分离，会产生无可预料行为
        2.不能连接已经分离的线程，会报错
        -参数:线程ID
        -返回值
            成功：0
            失败：返回错误号

*/
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<stdio.h>
void * callback (void *){
    printf("child thread id:%ld\n",pthread_self());
    return NULL;
}
int main(){
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,callback,NULL);
    if(ret != 0){
        char * err=strerror(ret);
        printf("error:%s\n",err);
    }
    //主线程与子线程的Id
    printf("tid: %ld,main thread id:%ld\n",tid,pthread_self());
    //设置子线程分离，之后不需要手动释放子线程的资源
    pthread_detach(tid);
    pthread_exit(NULL);
    //设置分离后，连接会报错
    // ret = pthread_join(tid,NULL);
    // if(ret != 0){
    //     char * err=strerror(ret);
    //     printf("error:%s\n",err); //error:Invalid argument
    // }
    return 0;
}