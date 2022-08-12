#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

void *producer(void *arg); //生产者线程
void *consumer(void *arg); //消费者线程

struct Queue
{
    int capacity;
    int start;
    unsigned size;
    int *data;
    sem_t full;
    sem_t empty;
    pthread_mutex_t mutex;
} queue;

int main(int argc, char *argv[])
{
    unsigned capacity = 5;
    queue.capacity = capacity;
    queue.start = 0;
    queue.size = 0;
    queue.data = (int *)malloc(capacity * (sizeof(int)));

    pthread_mutex_init(&(queue.mutex), NULL);
    sem_init(&(queue.empty), 0, capacity);
    sem_init(&(queue.full), 0, 0);

    pthread_t cons_t, prod_t;
    if (pthread_create(&cons_t, NULL, consumer, NULL) != 0)
    {
        printf("consumer thread create fail!");
    }
    if (pthread_create(&prod_t, NULL, producer, NULL) != 0)
    {
        printf("producer thread create fail!");
    }

    if (pthread_join(cons_t, NULL) != 0)
    {
        printf("fail");
    }
    if (pthread_join(prod_t, NULL) != 0)
    {
        printf("fail");
    }

    printf("finished!");
    pthread_mutex_destroy(&(queue.mutex));
    // sem_destroy(&empty);
    // sem_destroy(&full);
    free(queue.data);
}

void *producer(void *arg)
{
    for (int i = 0; i < 100; i++)
    {
        sem_wait(&(queue.empty));
        pthread_mutex_lock(&(queue.mutex));
        int index = (queue.start + queue.size) % queue.capacity;
        queue.data[index] = i;
        printf("produce: %d at: %d\n", i, index);
            queue.size += 1;
        pthread_mutex_unlock(&(queue.mutex));
        sem_post(&(queue.full));
    }
}
void *consumer(void *arg)
{
    for (int i = 0; i < 100; i++)
    {
        sem_wait(&(queue.full));
        pthread_mutex_lock(&(queue.mutex));
        printf("consume: %d at: %d\n", queue.data[queue.start], queue.start);
        queue.size -= 1;
        queue.start = (queue.start+1)%queue.capacity;
        pthread_mutex_unlock(&(queue.mutex));
        sem_post(&(queue.empty));
    }
}
