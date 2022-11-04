/*
    
    读写锁的调用函数 
    int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,const pthread_rwlockattr_t *restrict attr);
    int pthread_rwlock_destroy(pthread_rwlock_t * rwlock);
    int pthread_rwlock_rdlock(pthread_rwlock_t * rwlock);
    int pthread_rwlock_tryrdlock(pthread_rwlock_t * rwlock);
    int pthread_rwlock_wrlock(pthread_rwlock_t * rwlock);
    int pthread_rwlock_trywrlock(pthread_rwlock_t * rwlock);
    int pthread_rwlock_unlock(pthread_rwlock_t * rwlock);
   
    案例：8个线程操作同一个全局变量
    3个线程不定时写一个全局变量。5个线程不定时地读变量
*/

#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
int num=0;
// pthread_mutex_t mutex;
pthread_rwlock_t rwlock; //读写锁
void * writeNum(void *arg){
    while (1)
    {   
        pthread_rwlock_wrlock(&rwlock); //加写锁
        num++;
        printf("write:num=%d,tid:%ld\n",num,pthread_self());/* code */
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    
    return NULL;
}
void * readNum(void *arg){
    while(1){
        pthread_rwlock_rdlock(&rwlock);
        printf("read:num=%d,tid:%ld\n",num,pthread_self());
        pthread_rwlock_unlock(&rwlock);
        usleep(100);

    }
    return NULL;
}
int main(){
    //初始化锁
    pthread_rwlock_init(&rwlock,NULL);

    //创建三个写线程。5个读线程
    pthread_t wtids[3],rtids[5];
    for(int i=0;i<3;i++){
        pthread_create(&wtids[i],NULL,writeNum,NULL);
    }
    for(int i=0;i<5;i++){
        pthread_create(&rtids[i],NULL,readNum,NULL);
    }
    //设置线程分离
    for(int i=0;i<3;i++){
        pthread_detach(wtids[i]);
    }
    pthread_exit(NULL);
    pthread_rwlock_destroy(&rwlock);
    return 0;
}