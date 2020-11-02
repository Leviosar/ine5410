#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_THREADS 10

void *factorial_thread(void *arg)
{
    int* n = (int*) arg;
    printf("Nova thread em execução, fatorial de %d \n", *n);
    
    int factorial = 1;
	for (size_t i = 1; i <= *n; i++)
		factorial = factorial * i;
    
    pthread_exit(factorial);
}

int main(int argc, char **argv) 
{
    pthread_t threads[NUM_THREADS];
    long* values[NUM_THREADS];
    void* result[NUM_THREADS];

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        values[i] = malloc(sizeof(long));
        *values[i] = i;
        printf("Thread (%ld) sendo criada \n", i);
        pthread_create(&threads[i], NULL, factorial_thread, (void*) values[i]);
    }

    printf("Aguardando término das threads\n");

    for (size_t i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], &result[i]);
    
    for (size_t i = 0; i < NUM_THREADS; i++)
        printf("Valor do retorno de factorial(%ld) é: %d \n", i, (int) result[i]);

    return 0;
}