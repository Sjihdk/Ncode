/*
    int sem_init(sem_t *sem，int pshared,unsigned int value);
        初始化信号量
            sem:信号量变量的地址
            pshared：0 用在线程 非0 用在进程间
            value：信号量的值
    int sem_destroy(sem_t *sem);
        销毁信号量
    int sem_wait(sem_t *sem);
        信号量的值减一,如果是0，则会阻塞，直到值不为0就不阻塞了
    int sem_trywait(sem_t *sem);
        尝试减一
    int sem_timedwait(sem_t *sem,const struct timespec *abs_timeout);
        设置时间的wait
    int sem_post(sem_t *sem);
        信号量的值加一，如果
        返回值成功为0，-1失败
    int sem_getvalue(sem_t *sem,int * sval);


*/
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<memory.h>
#include<stdlib.h>//malloc函数
#include<semaphore.h> //信号量的头文件！！
//创建互斥量
pthread_mutex_t mutex;
//创建两个信号量
sem_t psem,csem;

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
        sem_wait(&psem); //调用一次生产者值减一，准备生产

        pthread_mutex_lock(&mutex);//加锁
        Node * newNode = (Node *) malloc(sizeof(Node));
        newNode->next=head;
        head=newNode;   
        newNode->num=rand()%1000;
        printf("add node,num:%d,tid:%ld\n",newNode->num,pthread_self());
        
        pthread_mutex_unlock(&mutex); //解锁
        sem_post(&csem); //消费者信号加一，只有当此时生产完了才会调用消费者加一
        usleep(100);

    }
    
    return NULL;
}
void * customer(void * arg){
    while (1)
    {   
        sem_wait(&csem);//准备消费，消费者信号值减一
        pthread_mutex_lock(&mutex);//加锁
        //保存头结点指针
     
            //有数据
            Node * temp = head;
            head = head->next;
            printf("del node,num:%d,tid:%ld\n",temp->num,pthread_self());
            free(temp);
            pthread_mutex_unlock(&mutex); //解锁
            //消费完了，生产者值加一
            sem_post(&psem);
            usleep(100);

    }
    return NULL;
}
int main(){
    //初始化互斥锁
    pthread_mutex_init(&mutex,NULL);

    //
    sem_init(&psem,0,8);
    sem_init(&csem,0,0);
    // 创建5个生产者的线程，5个消费者线程
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
    
    pthread_exit(NULL);
    
    return 0;
}
