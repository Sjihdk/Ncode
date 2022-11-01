
/*
  #include <pthread.h>
     一般情况下，main函数所在的线程称之为主线程(main线程)，其他为子线程

       int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
                          -功能：创建一个子线程
                          -参数：
                            -thread:传出参数，线程创建成功后，子线程的ID被写到改变量中
                            -attr：设置线程的属性，一般使用Null
                            -start_routine: 函数指针,子线程需要处理的逻辑代码
                            -arg:传参
                        -返回值：成功0失败返回错误号，与之前errno不一样
                            获取错误号的信息: strerror()
*/

#include<pthread.h>
#include<bits/stdc++.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h> //sleep
void * callback(void * arg){
    printf("child thread ---\n");
    printf("arg value:%d\n",*(int *)arg);
}
int main(){
    pthread_t tid;
    int num = 10;
    int ret = pthread_create(&tid,NULL,callback,(void *)&num);
    if(ret != 0){
        char* errstr = strerror(ret);
        printf("error:%s\n",errstr);
    }
    for(int i=0;i<5;i++){
        printf("%d\n",i);
    }
    sleep(1);
    return 0;
}