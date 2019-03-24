#include<unistd.h>
#include<stdio.h>
int main()
{
    pid_t fpid = fork();
    if(fpid == 0)
    {
        printf("我是子进程 P3 ，我的pid为：%d,我的ppid为：%d\n",getpid(),getppid());
    }
    else
    {
        printf("我是父进程 P1 ，我的pid为：%d,我的ppid为：%d\n",getpid(),getppid());
        fpid = fork();
        if(fpid == 0)
        {
            printf("我是子进程 P2 ，我的pid为：%d,我的ppid为：%d\n",getpid(),getppid());
            fpid = fork();
            if(fpid == 0)
            {
                printf("我是孙子进程 P4， 我的pid为：%d,我的ppid为：%d\n",getpid(),getppid());
            }
            else
            {
                fpid = fork();
                if(fpid == 0)
                {
                    printf("我是孙子进程 P5 ,我的pid为：%d,ppid为：%d\n",getpid(),getppid());
                }
                else
                {

                }
            }
        }
        else
        {
            
        }
    }
    sleep(20);
    return 0;
}
