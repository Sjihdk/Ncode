/*


*/
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<memory.h>
#include<stdlib.h>//malloc函数
pthread_mutex_t mutex;
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
        Node * temp = head;
        head = head->next;
        printf("del node,num:%d,tid:%ld\n",temp->num,pthread_self());
        free(temp);
        pthread_mutex_unlock(&mutex); //解锁
        usleep(100);
    }
    return NULL;
}
int main(){
    //初始化
    pthread_mutex_init(&mutex,NULL);
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
    pthread_exit(NULL);
    
    return 0;
}