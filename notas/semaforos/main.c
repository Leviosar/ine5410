#define NUM_THREADS 10
#define NUM_LICENSES 2

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t licenses;

void* thread(void* arg) {
    int id = *((int *) arg);
    sem_wait(&licenses);
    printf("Thread %d is using license\n", id);
    sleep(4);
    printf("Thread %d is releasing license\n", id);
    sem_post(&licenses);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    sem_init(&licenses, 0, NUM_LICENSES);

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread, (void *) &ids[i]);
    }
    
    for (size_t i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&licenses);    

    return 0;
}
