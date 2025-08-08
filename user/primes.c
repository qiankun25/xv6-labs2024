#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_NUM 280
#define RD 0
#define WT 1    
const uint INT_LEN = sizeof(int);
void prime(int *p);
void transmit_data(int f_p[2],int c_p[2],int prime_num);

void main(int argc, char const *argv[])
{
  int p[2];
  pipe(p);

  if (fork() == 0) {
    prime(p);
  } 
  else {
    for (int i = 2; i <= MAX_NUM; ++i) //写入初始数据
    {
      write(p[WT], &i, INT_LEN);
    }
    close(p[WT]);
    close(p[RD]);
    wait(0);
  }

  exit(0);
}

void transmit_data(int f_p[2],int c_p[2],int prime_num)
{
    
    int buff;
    while(read(f_p[RD], &buff,sizeof(int)) > 0){
        if(buff% prime_num != 0){
            write(c_p[WT],&buff, sizeof(int));
            //printf("parent: %d ->%d\n", prime_num,buff);
        }
    }
    close(f_p[RD]);
    close(c_p[WT]);
}

void prime(int *p)
{
    close(p[WT]);
    int prime_num;
    if(read(p[RD],&prime_num,sizeof(int)) > 0){
        printf("prime %d\n",prime_num);
        int c_p[2];
        //pipe(c_p);
        if(pipe(c_p) == -1){
          printf("pipe number outof edge");
        }

        
        if(fork() == 0){
          //关闭不需要的文件描述符
          

          close(p[RD]);
          close(c_p[WT]);
          prime(c_p);
        }
        else{
           close(c_p[RD]);
            transmit_data(p,c_p,prime_num);
            
            
            wait(0);
        }

    }

}

