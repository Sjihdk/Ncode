/*
       #include <pthread.h>

       int pthread_cancel(pthread_t thread);
       功能：让线程终止
       取消某个线程，终止运行，
       但是并不是立马终止，而是当子线程执行到一个取消点才回终止，取消点是系统规定好的一些系统调用，可以粗略地认为是用户区到内核区的切换
       参数：线程id
       返回值：


*/
#include<pthread.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
void *callback(void *arg){
    printf("child tid:%ld\n",pthread_self());
    for(int i=0;i<5;i++){
        printf("%d\n",i);
    }
    return NULL;
}

int main(){
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,callback,NULL);
    if(ret!=0){
        char * erro = strerror(ret);
        printf("error:%s\n",erro);
    }
    pthread_cancel(tid);
    for(int i=0;i<5;i++){
        printf("%d\n",i);
    }
    pthread_exit(NULL);
    return 0;
}