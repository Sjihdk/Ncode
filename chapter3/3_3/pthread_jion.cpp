/*
       #include <pthread.h>

       int pthread_join(pthread_t thread, void **retval);
       功能；和一个已经终止的线程连接,回收子线程的资源
            该函数是阻塞的，调用一次只能回收一个子线程
         -参数：
            -thread：需要回收的子线程的ID
            -retval：接受子线程退出时的值
        -返回值
        0：成功
        非0：设置错误号
*/

#include<stdio.h>
#include<pthread.h>
#include<string.h> //strerror()
#include<unistd.h>
int value =10; //设置成全局变量，局部变量在栈中，子线程结束后回收，再访问该地址的值为随机的
void * callback(void * arg){
    printf("child process ID %ld\n",pthread_self());
    // sleep(3);
    // return NULL;

    pthread_exit((void *)&value);

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
    //主线程调用pthread_join()回收子线程的资源
    int * thread_retval;
    ret = pthread_join(tid,(void **)&thread_retval);//不需要返回值，设置NULL
    if(ret != 0){
        char * errstr = strerror(ret);
        printf("error:%s\n",errstr);
    }
    printf("回收成功\n");
    printf("返回值%d\n",*thread_retval);
    //让主线程退出，其他线程不会受到影响
    pthread_exit(NULL);

    return 0;
}