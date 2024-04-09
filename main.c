/*
 ============================================================================
 Name        : Producer.c
 Author      : Goran Vujnovic
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>


sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutexBuffer;

int buffer[10];
int count = 0;


void* producer(void* args)
{
    while (1)
    {
        // Produce numbers in the range 0 - 10.
        int x = rand() % 10;
        sleep(1);
        sem_wait(&semEmpty); //wait until empty > 0 and then decrement 'empty'
        pthread_mutex_lock(&mutexBuffer); // acquire lock
        buffer[count] = x;
        count++;
        printf("Produced: %d\n", x);
        pthread_mutex_unlock(&mutexBuffer); // release lock
        sem_post(&semFull); // increment 'full'
    }
}

void* consumer(void* args)
{
    while (1)
    {
        int y;

        // Remove from the buffer
        sem_wait(&semFull); // wait until full > 0 and then decrement 'full'
        pthread_mutex_lock(&mutexBuffer); // acquire lock
        y = buffer[count - 1];
        printf("Got: %d\n", y);
        count--;
        pthread_mutex_unlock(&mutexBuffer); // release lock
        sem_post(&semEmpty); // increment 'Empty'
        sleep(1);
    }
}

int main(int argc, char* argv[])
{
    srand(time(NULL));

    pthread_t th_producer;
    pthread_t th_consumer;

    pthread_mutex_init(&mutexBuffer, NULL);

    sem_init(&semEmpty, 0, 10);
    sem_init(&semFull, 0, 0);

    pthread_create(&th_producer, NULL, &producer, NULL);
    pthread_create(&th_consumer, NULL, &consumer, NULL);

    pthread_join(th_producer, NULL);
    pthread_join(th_consumer, NULL);

    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);

    return 0;
}
