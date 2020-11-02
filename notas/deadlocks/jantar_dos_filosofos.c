#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5
#define LEFT(i) (i + N - 1) % N
#define RIGHT(i) (i + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

pthread_mutex_t mutex;
sem_t philosophers_semaphores[N];
int states[N];

void eat(int index) {
    printf("Filósofo %d começou a comer\n", index);
    fflush(stdout);
    sleep(2);
}

void think(int index) {
    printf("Filósofo %d começou a pensar\n", index);
    fflush(stdout);
    sleep(3);
}

void test(int index) {
    printf("O filósofo %d vai tentar pegar os 2 garfos\n", index);

    if (states[index] == HUNGRY && states[LEFT(index)] != EATING && states[RIGHT(index)] != EATING)
    {
        printf("O filósofo %d conseguiu pegar os garfos e vai comer!!!\n", index);
        states[index] = EATING;
        sem_post(&philosophers_semaphores[index]);
    } else {
        printf("O filósofo %d não conseguiu pegar os garfos e vai ficar com fome!!!\n", index);
    }

}

void take_forks(int index) {
    pthread_mutex_lock(&mutex);
    
    states[index] = HUNGRY;
    test(index);
    
    pthread_mutex_unlock(&mutex);

    sem_wait(&philosophers_semaphores[index]);
}

void put_forks(int index) {
    printf("O filósofo %d já encheu a pança e vai soltar os garfos pra pensar novamente\n", index);
    pthread_mutex_lock(&mutex);
    
    states[index] = THINKING;
    test(LEFT(index));
    test(RIGHT(index));

    pthread_mutex_unlock(&mutex);
}

void* philosopher(void* arg) 
{
    int index = *((int *) arg);

    while(1) {
        think(index);
        take_forks(index);
        eat(index);
        put_forks(index);
    }
}

int main () {
    pthread_mutex_init(&mutex, NULL);

    pthread_t philosophers[N];
    int args[N];
    
    for(int i = 0; i < N; i++)
        sem_init(&philosophers_semaphores[i], 0, 0);

    for(int i = 0; i < N; i++) {
        args[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, (void*) &args[i]);
    }

    for(int i = 0; i < N; i++)
        pthread_join(philosophers[i], NULL);
    
    for(int i = 0; i < N; i++)
        sem_destroy(&philosophers_semaphores[i]);
    
    return 0;
}