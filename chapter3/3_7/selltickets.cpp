/*
    使用多线程实现买票的案例:三个窗口 一共是100张


*/
#include<stdio.h>
#include<unistd.h>//usleep
#include<pthread.h>
#include<string.h>
int tickets =100;
void *sellticket(void *arg){
    //卖票 
    while (tickets>0)
    {   usleep(3000);
        printf("%ld正在卖第%d张门票\n",pthread_self(),tickets--);    
    }
    return NULL;
}
int main(){
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
    return 0;    
}