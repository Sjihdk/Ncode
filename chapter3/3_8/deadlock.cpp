

/*



*/

#include<stdio.h>
#include<unistd.h>//usleep
#include<pthread.h>
#include<string.h>
int tickets =100;
pthread_mutex_t mutex,mutex1;
void *workA(void *arg){
    pthread_mutex_lock(&mutex);
    sleep(1);
    pthread_mutex_lock(&mutex1);

    printf("workA..\n");
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex);

    return NULL;
}
void *workB(void *arg){
    pthread_mutex_lock(&mutex1);
    sleep(1);
    pthread_mutex_lock(&mutex);

    printf("workB..\n");
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex1);
    
    return NULL;
}
int main(){
    //初始化互斥量
    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&mutex1,NULL);
    //创建3个子线程
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,workA,NULL);
    pthread_create(&tid2,NULL,workB,NULL);

    //回收子线程的资源
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    //设置线程分离
    // pthread_detach(tid1);
    // pthread_detach(tid1);
    // pthread_detach(tid1);
    
    pthread_exit(NULL);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex1);
    return 0;    
}
