/*
    条件变量的类型pthread_cond_t
    int pthread_cond_init(pthread_cond_t * restrict cond,const pthread_condattr_t *restrict attr);
    int pthread_cond_destroy(pthread_cond_t * cond);
    int pthread_cond_wait(pthread_cond_t * restrict cond,pthread_mutex_t * restrict mutex);
        -等待 调用会阻塞线程
    int pthread_cond_timewait(pthread_cond_t * restrict cond,pthread_mutex_t *restrict mutex,const struct timespec *restirct abstime);
        -等待多长时间，线程会阻塞直到时间结束
    int pthread_cond_signal(pthread_cond_t * cond);
        -唤醒一个或者多个线程
    int pthread_cond_broadcast(pthread_cond_t * cond);
        -唤醒所有线程
*/  
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<memory.h>wa
#include<stdlib.h>//malloc函数
pthread_mutex_t mutex;
pthread_cond_t cond;
typedef struct Node
{
    Node* next;
    int num;
    /* data */
};
Node * head = (Node *) malloc(sizeof(Node));
void * producer(void * arg){
    //不断创建新的节点添加到链表中
    while (1)
    {   
        pthread_mutex_lock(&mutex);//加锁
        Node * newNode = (Node *) malloc(sizeof(Node));
        newNode->next=head;
        head=newNode;
        newNode->num=rand()%1000;
        printf("add node,num:%d,tid:%ld\n",newNode->num,pthread_self());

        //只要生产了一个,就通知消费者去消费
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex); //解锁
        usleep(100);

    }
    
    return NULL;
}
void * customer(void * arg){
    while (1)
    {   
        pthread_mutex_lock(&mutex);//加锁
        //保存头结点指针
        if(head!=NULL){
            //有数据
            Node * temp = head;
            head = head->next;
            printf("del node,num:%d,tid:%ld\n",temp->num,pthread_self());
            free(temp);
            pthread_mutex_unlock(&mutex); //解锁
            usleep(100);
        }
        else{
            //没有数据，等待
            pthread_cond_wait(&cond,&mutex);
        }

    }
    return NULL;
}
int main(){
    //初始化
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    //创建5个生产者的线程，5个消费者线程
    pthread_t ptids[5],ctids[5];
    for(int i=0;i<5;i++){
        pthread_create(&ptids[i],NULL,producer,NULL);
        pthread_create(&ctids[i],NULL,customer,NULL);
    }
    for(int i=0;i<5;i++){
        pthread_detach(ptids[i]);
        pthread_detach(ctids[i]);
    }
    while(1){
        sleep(10); //为了能先销毁锁再退出
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
    
    return 0;
}
