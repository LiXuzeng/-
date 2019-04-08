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
void *saller() {
    int i = 500;
    while(i > 0)
    {
        temp = ticketcount;
        pthread_yield();
        temp = temp - 1;
        pthread_yield();
        ticketcount = temp;
        i--;
    }
    return NULL;
}

void *refund_ticket(){
    int i = 500;
    while(i > 0)
    {
        temp = ticketcount;
        pthread_yield();
        temp = temp + 1;
        pthread_yield();
        ticketcount = temp;
        i--;
    }
    return NULL;
}

int main()
{
	pthread_t p1, p2;
	pthread_create(&p1, NULL, saller, NULL);
	pthread_create(&p2, NULL, refund_ticket, NULL);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	printf("火车票余量 : %d\n", ticketcount);
	return 0;
}
 
