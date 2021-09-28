#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

struct Queue
{
    int *datas;
    sem_t sem_prod;
    sem_t sem_cons;
    pthread_mutex_t lock;
    int length;
    int count;
    int start;
} queue;

void producer()
{
    for (int i = 0; i < 200; i++)
    {
        sem_wait(&queue.sem_prod);
        pthread_mutex_lock(&queue.lock);
        int index = (queue.start + queue.count + 1) % queue.length;
        queue.datas[index] = i;
        queue.count += 1;
        printf("Put: %d, length: %d, count: %d , start: %d \n", i, queue.length, queue.count, queue.start);
        pthread_mutex_unlock(&queue.lock);
        sem_post(&queue.sem_cons);
    }
    printf("Producer finish\n");
}

void consumer()
{
    for (int i = 0; i < 200; i++)
    {
        sem_wait(&queue.sem_cons);
        pthread_mutex_lock(&queue.lock);
        queue.start = (queue.start + 1) % queue.length;
        int res = queue.datas[queue.start];
        queue.count -= 1;
        printf("Get: %d, length: %d, count: %d , start: %d \n", res, queue.length, queue.count, queue.start);
        pthread_mutex_unlock(&queue.lock);
        sem_post(&queue.sem_prod);
    }
    printf("Consumer finish\n");
}

void main()
{
    pthread_mutex_init(&queue.lock, 0);
    sem_init(&queue.sem_cons, 0, 0);
    sem_init(&queue.sem_prod, 0, 100);

    pthread_t prod_thread, cons_thread;
    queue.length = 100;
    queue.count = 0;
    queue.start = -1;
    queue.datas = (int *)malloc(sizeof(int) * 100);
    pthread_create(&prod_thread, 0, producer, 0);
    pthread_create(&cons_thread, 0, consumer, 0);
    pthread_join(prod_thread, 0);
    pthread_join(cons_thread, 0);
    printf("finish\n");
    return;
}