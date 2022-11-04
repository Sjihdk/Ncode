/*
    int pthread_mutex_init(pthread_mutex_t* restrict mutex,const pthread_mutexattr_t* restrict attr);
        -初始化互斥量:
        -参数：
            -mutex:需要初始化的互斥量变量
            -attr：互斥量相关的属性，NULL
        restrict :C语言的修饰符,被修饰的指针，不能由另外的一个指针进行操作，相当于唯一的指针
    int pthread_mutex_destroy(pthread_mutex_t *mutex);
        -释放互斥量的资源

    int pthread_mutex_lock(pthread_mutex_t *mutex);
        -加锁 阻塞的，一个线程加锁了，其他线程只能阻塞等待
    int pthread_mutex_trylock(pthread_mutex_t *mutex);
        -尝试加锁，如果加锁失败，不会阻塞，会直接返回,注意它不会阻塞！！！！需要后面写判断条件处理错误或继续
    int pthread_mutex_unlock(pthread_mutex_t *mutex);
        -解锁
*/
#include<stdio.h>
#include<unistd.h>//usleep
#include<pthread.h>
#include<string.h>
int tickets =100;
pthread_mutex_t mutex;
void *sellticket(void *arg){
    //卖票 
    //加锁
    
    while (1)
    {   
        pthread_mutex_lock(&mutex);
        if(tickets>0){
            printf("%ld正在卖第%d张门票\n",pthread_self(),tickets--);    
        }
        else {
            pthread_mutex_unlock(&mutex);
            break;
        }
        //解锁
        pthread_mutex_unlock(&mutex);
        usleep(6000);
    }
    
    return NULL;
}
int main(){
    //初始化互斥量
    pthread_mutex_init(&mutex,NULL);
    //创建3个子线程
    pthread_t tid1,tid2,tid3;
    pthread_create(&tid1,NULL,sellticket,NULL);
    pthread_create(&tid2,NULL,sellticket,NULL);
    pthread_create(&tid3,NULL,sellticket,NULL);
    //回收子线程的资源
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    //设置线程分离
    // pthread_detach(tid1);
    // pthread_detach(tid1);
    // pthread_detach(tid1);
    
    pthread_exit(NULL);
    pthread_mutex_destroy(&mutex);
    return 0;    
}
