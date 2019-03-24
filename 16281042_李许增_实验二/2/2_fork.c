#include<unistd.h>
#include<stdio.h>
int main()
{
	pid_t fork_pid;//fork_pid为fork函数返回值
	fork_pid = fork();
	if(fork_pid < 0)//当返回值为负值时表示出现错误
	{
		printf("fork error");
	}
	else if(fork_pid == 0)//当返回值为0时表示创建的子进程在运行
	{
		printf("我是子进程，我的pid为：%d\n",getpid());
		if((execlp("vi","vi","/home/os-2019/test",NULL))<0)//注意execlp参数使用
			printf("execlp error\n");
	}
	else//当返回值为其他正值时表示父进程在运行
	{
		printf("我是父进程，我的pid为：%d\n",getpid());
		while(1){};
	}
	return 0;
}
