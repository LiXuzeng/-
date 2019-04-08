#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int ticketcount = 1000;
int temp;
sem_t * mysem = NULL;

void *saller() {
    int i = 500;
    while(i > 0)
    {
        sem_wait(mysem);
        temp = ticketcount;
        pthread_yield();
        temp = temp - 1;
        pthread_yield();
        ticketcount = temp;
        i--;
        sem_post(mysem);
    }
    return NULL;
}

void *refund_ticket(){
    int i = 500;
    while(i > 0)
    {
        sem_wait(mysem);
        temp = ticketcount;
        pthread_yield();
        temp = temp + 1;
        pthread_yield();
        ticketcount = temp;
        i--;
        sem_post(mysem);
    }
    return NULL;
}

int main()
{
	mysem = sem_open("csem",O_CREAT,0666,1);
	pthread_t p1, p2;
	pthread_create(&p1, NULL, saller, NULL);
	pthread_create(&p2, NULL, refund_ticket, NULL);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	printf("火车票余量 : %d\n", ticketcount);
	sem_close(mysem);
	unlink("csem");
	return 0;
}
 

