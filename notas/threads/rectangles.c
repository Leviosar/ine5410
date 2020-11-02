#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_THREADS 2

typedef struct rectangle_t
{
    double width;
    double height;
} rectangle;

void *thread(void *arg)
{
    rectangle *r = (rectangle *) arg;
    printf("w=%.2f, h=%.2f, a=%.2f\n", r->width, r->height, r->width * r->height);
    pthread_exit(NULL);
}

int main(int argc, char **argv) 
{
    pthread_t threads[NUM_THREADS];
    rectangle *r1, *r2;

    r1 = malloc(sizeof(rectangle));
    r1->width = 3.0;
    r1->height = 5.0;

    r2 = malloc(sizeof(rectangle));
    r2->width = 32.0;
    r2->height = 100.0;

    printf("Criando a primeira thread\n");
    pthread_create(&threads[0], NULL, thread, (void *) r1);

    printf("Criando a segunda thread\n");
    pthread_create(&threads[1], NULL, thread, (void *) r2);

    printf("Aguardando o térmido das threads\n");

    for (size_t i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
    
    printf("Finalizando programa principal\n");

    return 0;
}