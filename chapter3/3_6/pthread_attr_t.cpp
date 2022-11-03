/*
    int pthread_attr_init(pthread_attr_t * attr);
        -初始化线程属性变量
    int pthread_attr_destroy(pthread_attr_t *attr);
        -释放线程属性的资源
    int pthread_attr_getdetachstate(const pthread_attr_t *attr);
        -获取线程分离的属性状态
    int pthread_attr_setdetachstate(pthread_attr_t * attr,int detachstate);
        -设置线程分离的属性状态
    int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);

    int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
*/
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h> 
void *callback(void *arg){
    printf("child tid:%ld\n",pthread_self());
    return NULL;
}

int main(){
    //创建线程属性变量
    pthread_attr_t attr;
    //初始化属性变量
    pthread_attr_init(&attr);
    //设置属性
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//设置了线程分离
 
    //创建了一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid,&attr,callback,NULL);
    if(ret!=0){
        char * erro = strerror(ret);
        printf("error:%s\n",erro);
    }
    //获取线程的栈的大小
    size_t size;
    pthread_attr_getstacksize(&attr,&size);
    printf("size:%ld\n",size);
    //输出
    printf("tid:%ld,main thread id:%ld\n",tid,pthread_self());
    //释放属性资源
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
    return 0;
}