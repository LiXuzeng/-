#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
sem_t *a_sem, *b_sem, *c_sem, *d_sem;
int main()
{
    a_sem = sem_open("asem", O_CREAT, 0666, 0);
    b_sem = sem_open("bsem", O_CREAT, 0666, 0);
    c_sem = sem_open("csem", O_CREAT, 0666, 0);
    pid_t fork_pid;//fork函数返回值
    printf("我是进程P1\n");
    sleep(1);
    sem_post(a_sem);
    fork_pid = fork();
    if(fork_pid == 0)
    {
        sem_wait(a_sem);
        printf("我是进程P2\n");
        sleep(1);
        sem_post(a_sem);
        sem_post(b_sem);
    }
    else
    {
        fork_pid = fork();
        if(fork_pid == 0)
        {
            sem_wait(a_sem);
            printf("我是进程P3\n");
            sleep(1);
            sem_post(a_sem);
            sem_post(c_sem);
        }
        else
        {
            fork_pid = fork();
            if(fork_pid == 0)
            {
                sem_wait(b_sem);
                sem_wait(c_sem);
                printf("我是进程P4\n");
                sleep(1);
                sem_post(b_sem);
                sem_post(c_sem);
            }
            else
            {}
        }
    }
    sem_close(a_sem);
    sem_close(b_sem);
    sem_close(c_sem);
    unlink("asem");
    unlink("bsem");
    unlink("csem");
    return 0;
}
