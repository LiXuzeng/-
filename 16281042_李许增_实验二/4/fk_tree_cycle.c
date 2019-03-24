#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
int main()
{
    pid_t fpid = fork();
    if(fpid == 0)
    {
        while(1){
            printf("我是子进程 P3 ，我的pid为：%d,我的ppid为：%d\n",getpid(),getppid());
            sleep(2);
	}
    }
    else
    {
        fpid = fork();
        if(fpid == 0)
        {
            
            fpid = fork();
            if(fpid == 0)
            {
                while(1){
                    printf("我是孙子进程 P4， 我的pid为：%d,我的ppid为：%d\n",getpid(),getppid());
                    sleep(2);
		        }
            }
            else
            {
                fpid = fork();
                if(fpid == 0)
                {
                    while(1){
                        printf("我是孙子进程 P5 ,我的pid为：%d,ppid为：%d\n",getpid(),getppid());
                    	sleep(2);
		            }
                }
                else
                {

                }
            }
            while(1){
                printf("我是子进程 P2 ，我的pid为：%d,我的ppid为：%d\n",getpid(),getppid());
            	sleep(2);
                 int *ptr = (int *)0;
                *ptr = 100;
	        }
        }
        else
        {
           
        }
        while(1){
            printf("我是父进程 P1 ，我的pid为：%d,我的ppid为：%d\n",getpid(),getppid());
            sleep(2);
	    }
    }
    sleep(20);
    return 0;
}
