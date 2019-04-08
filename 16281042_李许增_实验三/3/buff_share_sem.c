#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
char buff[10];
int i = 0, j = 0;
sem_t * sem_place = NULL;
sem_t * sem_data = NULL;
void *work1()
{
	while(1)
    {
        sem_wait(sem_place);//消耗空间资源wait操作
        scanf("%c\n",&buff[i]);
        sem_post(sem_data);//释放data资源signal操作
        i++;
        i = i%10;
    }
	return NULL;
}
void *work2()
{
	while(1)
	{
        sem_wait(sem_data);//消耗data资源wait操作
        printf("我是输出：%c\n",buff[j]);
        sem_post(sem_place);//释放空间资源signal操作
        sleep(1);
        j++;
        j = j%10;
	}
	return NULL;
}

int main()
{
	sem_place = sem_open("placesem",O_CREAT,0666,10);//空间资源初始化为10
    sem_data = sem_open("datasem",O_CREAT,0666,0);//data资源初始化为0
	pthread_t p1, p2;
	pthread_create(&p1, NULL, work1, NULL);
	pthread_create(&p2, NULL, work2, NULL);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	sem_close(sem_place);
    sem_close(sem_data);
	unlink("placesem");
    unlink("datasem");
	return 0;
}
