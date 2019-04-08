#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
char buff[10];
int i = 0, j = 9;
//sem_t * mysem = NULL;
void *work1()
{
	while(1)
    {
        scanf("%c\n",&buff[i]);
        i++;
        i = i%10;
    }
	return NULL;
}
void *work2()
{
	while(1)
	{
        if(((j + 1)% 10) != i)
        {
            printf("我是输出：%c\n",buff[(j + 1) % 10]);
            sleep(1);
            j++;
            j = j%10;
        }
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	//mysem = sem_open("csem",O_CREAT,0666,1);
	pthread_t p1, p2;
	pthread_create(&p1, NULL, work1, NULL);
	pthread_create(&p2, NULL, work2, NULL);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	//sem_close(mysem);
	//unlink("csem");
	return 0;
}

