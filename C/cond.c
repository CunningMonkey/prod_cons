#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct Queue
{
    int *datas;
    pthread_cond_t condProd;
    pthread_cond_t condCons;
    pthread_mutex_t lock;
    int length;
    int count;
    int start;
} queue;

void producer()
{
    for (int i = 0; i < 100; i++)
    {
        pthread_mutex_lock(&queue.lock);
        if (queue.count == queue.length)
        {
            pthread_cond_wait(&queue.condProd, &queue.lock);
        }
        int index = (queue.start + queue.count + 1) % queue.length;
        queue.datas[index] = i;
        queue.count += 1;
        printf("Put: %d, length: %d, count: %d , start: %d \n", i, queue.length, queue.count, queue.start);
        pthread_mutex_unlock(&queue.lock);
        pthread_cond_signal(&queue.condCons);
    }
    printf("Producer finish\n");
}

void consumer()
{
    for (int i = 0; i < 100; i++)
    {
        pthread_mutex_lock(&queue.lock);
        if (queue.count == 0)
        {
            pthread_cond_wait(&queue.condCons, &queue.lock);
        }
        queue.start = (queue.start + 1) % queue.length;
        int res = queue.datas[queue.start];
        queue.count -= 1;
        printf("Get: %d, length: %d, count: %d , start: %d \n", res, queue.length, queue.count, queue.start);
        pthread_mutex_unlock(&queue.lock);
        pthread_cond_signal(&queue.condProd);
    }
    printf("Consumer finish\n");
}

void main()
{
    pthread_mutex_init(&queue.lock, 0);
    pthread_cond_init(&queue.condProd, 0);
    pthread_cond_init(&queue.condCons, 0);

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