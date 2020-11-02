#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

int contador_global = 0;
pthread_mutex_t mutex;

void* incrementor(void* argv) {
    int* n = (int*) argv;

    pthread_mutex_lock(&mutex);
    
    for (size_t i = 0; i < *n; i++)
        contador_global += 1;
    
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("n_threads é obrigatório!\n");
        printf("Uso: %s n_threads n_loops\n", argv[0]);
        return 1;
    }

    pthread_mutex_init(&mutex, NULL);

    int n_threads = atoi(argv[1]);
    int n_loops = atoi(argv[2]);
    pthread_t threads[n_threads];

    for (size_t i = 0; i < n_threads; i++)
        pthread_create(&threads[i], NULL, incrementor, (void *) &n_loops);
    
    
    for (size_t i = 0; i < n_threads; i++)
        pthread_join(threads[i], NULL);

    printf("Contador: %d\n", contador_global);
    printf("Esperado: %d\n", n_threads*n_loops);
    
    pthread_mutex_destroy(&mutex);

    return 0;
}
