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

sem_t full, empty, producer_lock, consumer_lock;

//Você deve fazer as alterações necessárias nesta função e na função
//consumidor_func para que usem semáforos para coordenar a produção
//e consumo de elementos do buffer.
void *produtor_func(void *arg) {
    int max = *((int*)arg);
    
    for (int i = 0; i <= max; ++i) {
    
        int produto;

        // Não consegui fazer funcionar usando -1 como último produto
        // Então criei uma lógica no consumidor pra que consumissem
        // apenas a quantidade que foi produzida, então aqui ao invés
        // de produzir -1 eu dou um break nas threads produtoras. 
        // Acho que isso diminui o paralelismo do programa em algum nível
        // mas minhas outras soluções todas falharam miseravelmente
        if (i == max) {
            break;
        } else {
            produto = produzir(i);
        } 
        
        sem_wait(&empty);
        sem_wait(&producer_lock);
        indice_produtor = (indice_produtor + 1) % tamanho_buffer;
        buffer[indice_produtor] = produto;
        sem_post(&consumer_lock);
        sem_post(&full);
    }

    return NULL;
}

void *consumidor_func(void *arg) {
    // Aqui é a lógica que eu falei no comentário da função do produtor
    // Cada consumidor vai receber vai receber o número máximo de produtos
    // Que esse consumidor deve consumir. Esse número é calculado no main mas 
    // basicamente é uma distribuição mais ou menos igual de produtos pra
    // cada consumidor consumir. Com a excessão do último consumidor que recebe 
    // o resto dos produtos faltantes

    int max = *((int*) arg);
    int i = 0;

    while (i != max) {
        i += 1;

        sem_wait(&full);
        sem_wait(&consumer_lock);
        indice_consumidor = (indice_consumidor + 1) % tamanho_buffer;
        int produto = buffer[indice_consumidor];
        sem_post(&empty);
        sem_post(&producer_lock);

        // Como não temos mais o -1 como produto final, não precisa de checagem aqui
        consumir(produto);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Uso: %s tamanho_buffer itens_produzidos n_produtores n_consumidores \n", argv[0]);
        return 0;
    }

    tamanho_buffer = atoi(argv[1]);
    
    int itens = atoi(argv[2]);
    int n_produtores = atoi(argv[3]);
    int n_consumidores = atoi(argv[4]);

    printf("itens=%d, n_produtores=%d, n_consumidores=%d\n",
	   itens, n_produtores, n_consumidores);

    indice_produtor = 0;
    indice_consumidor = 0;
    buffer = malloc(sizeof(int) * tamanho_buffer);

    sem_init(&full, 0, 0);
	sem_init(&empty, 0, tamanho_buffer);
    sem_init(&producer_lock, 0, 1);
	sem_init(&consumer_lock, 0, 1);

    pthread_t producers[n_produtores], consumers[n_consumidores];

    for (int i = 0; i < n_produtores; i++) {
        pthread_create(&producers[i], NULL, produtor_func, (void*) &itens);
    }

    int total = itens * n_produtores;  
    int max_consumer = total / n_consumidores;  
    int consumer_param[n_consumidores];

    for (int i = 0; i < n_consumidores; i++) {
        
        if (i == n_consumidores - 1) {
            consumer_param[i] = max_consumer + (total % n_consumidores); 
        } else {
            consumer_param[i] = max_consumer;
        }
        
        pthread_create(&consumers[i], NULL, consumidor_func, (void*) &consumer_param[i]);
    }

    for (int i = 0; i < n_produtores; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < n_consumidores; i++) {
        pthread_join(consumers[i], NULL);
    }
    
    free(buffer);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&producer_lock);
    sem_destroy(&consumer_lock);

    return 0;
}

