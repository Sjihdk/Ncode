/*
      void pthread_exit(void *retval);
      -功能：终止一个线程，哪个调用终止哪个
      参数：
        retval：需要传递一个指针，作为一个返回值，可在pthread_join()中查到

    pthread_t pthread_self(void)
        -功能：返回当前线程的pthread_t 
    int pthread_equal(pthread_t t1  pthread_t t2)
        功能：比较两个线程ID是否相等
        不同操作系统的pthread_t类型是不一样的  

        
*/
#include<stdio.h>
#include<pthread.h>
#include<string.h> //strerror()
#include<unistd.h>
void * callback(void * arg){
    printf("child process ID %ld\n",pthread_self());
    return NULL;
}
int main(){
    //创建一个线程
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,callback,NULL);
    if(ret != 0){
        char * errstr = strerror(ret);
        printf("error:%s\n",errstr);
    }
    for(int i=0;i<1000;i++){
        printf("%d\n",i);
    }

    printf("tid: %ld process ID %ld\n",tid,pthread_self());
    //让主线程退出，其他线程不会受到影响
    pthread_exit(NULL);

    return 0;
}