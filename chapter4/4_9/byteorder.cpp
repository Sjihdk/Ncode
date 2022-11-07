/*
    字节序 字节在内存中的存储顺序
    小端字节序：数据高位字节存储在内存的高位
    大端字节序：数据高位字节存储在内存的低位
*/
#include<stdio.h>
#include<bits/stdc++.h>
using namespace std;
int main(){

    union tt
    {
        short value;//2字节
        char c[sizeof(short)];
    }test;
    test.value = 0x0102;
    if(test.c[0]==1&&test.c[1]==2){
        cout<<"大端字节序"<<endl;
    }  
    else  if(test.c[0]==2&&test.c[1]==1){
        cout<<"小端字节序"<<endl;
    }  
    return 0;
}