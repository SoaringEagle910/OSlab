#include "kernel/types.h"
#include "user.h"
int main(int argc,char* argv[]){
  int c2f[2];//子进程向父进程通信管道
  int f2c[2];//父进程向子进程通信管道
  pipe(c2f);
  pipe(f2c);
  if(fork() == 0)//子进程
  {
    int f_pid;
    int c_pid = getpid();
    close(c2f[0]);//子进程向父进程通信的读端口在子进程中关闭
    close(f2c[1]);//父进程向子进程通信的写端口在子进程中关闭
    read(f2c[0], &f_pid, sizeof(f_pid));//获取父进程的pid
    printf("%d: received ping from pid %d\n", c_pid, f_pid);
    close(f2c[0]);//父进程向子进程通信的读端口在子进程中关闭
    write(c2f[1], &c_pid, sizeof(c_pid));//向父进程写子进程的pid
    close(c2f[1]);//子进程向父进程通信的写端口在子进程中关闭
  }
  else//父进程 
  {
    int f_pid = getpid();
    int c_pid;
    close(f2c[0]);//父进程向子进程通信的读端口在父进程中关闭
    close(c2f[1]);//子进程向父进程通信的写端口在父进程中关闭
    write(f2c[1], &f_pid, sizeof(f_pid));//向子进程写父进程的pid
    close(f2c[1]);//父进程向子进程通信的写端口在父进程中关闭
    wait(0);//等待子进程接收并处理完
    read(c2f[0], &c_pid, sizeof(c_pid));//获取子进程的pid
    printf("%d: received pong from pid %d\n", f_pid, c_pid);
    close(c2f[0]);
  }
  exit(0);//确保进程退出
}