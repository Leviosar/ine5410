#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

int produzir(int value);
void consumir(int produto);
void *produtor_func(void *arg);
void *consumidor_func(void *arg);

int indice_produtor, indice_consumidor, tamanho_buffer;
int* buffer;

sem_t full, empty;

void *produtor_func(void *arg) {
    int max = *(int *) arg;

    for (int i = 0; i <= max; ++i) {
        sem_wait(&empty);
        int produto;

        if (i == max)
            produto = -1;
        else
            produto = produzir(i);

        indice_produtor = (indice_produtor + 1) % tamanho_buffer;
        buffer[indice_produtor] = produto;
        sem_post(&full);
    }

    return NULL;
}

void *consumidor_func(void *arg) {
    while (1) {
        sem_wait(&full);
        
        indice_consumidor = (indice_consumidor + 1) % tamanho_buffer;
        int produto = buffer[indice_consumidor];

        if (produto >= 0) {
            consumir(produto);
        } else {
            break;
        }

        sem_post(&empty);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s tamanho_buffer itens_produzidos\n", argv[0]);
        return 0;
    }

    tamanho_buffer = atoi(argv[1]);
    int n_itens = atoi(argv[2]);
    printf("n_itens: %d\n", n_itens);

    indice_produtor = 0;
    indice_consumidor = 0;
    buffer = malloc(sizeof(int) * tamanho_buffer);

    sem_init(&full, 0, 0);
	sem_init(&empty, 0, tamanho_buffer);

    pthread_t producer, consumer;

    pthread_create(&producer, NULL, produtor_func, (void*) &n_itens);
    pthread_create(&consumer, NULL, consumidor_func, NULL);
    pthread_join(consumer, NULL);
    pthread_join(producer, NULL);
    
    free(buffer);
    sem_destroy(&full);
    sem_destroy(&empty);
    
    return 0;
}

