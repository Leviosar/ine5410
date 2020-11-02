#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int gValue = 0;
pthread_mutex_t gMtx;

// Função imprime resultados na correção do exercício -- definida em helper.c
void imprimir_resultados(int n, int** results);

// Função escrita por um engenheiro
void compute(int arg) {
    // Solução: compute() é chamada apenas dentro de
    // compute_thread() que já trava o mutex antes de chamar compute()
    // então chamar dentro da função novamente iria fazer com que a thread
    // esperasse pra sempre o final da própria execução, travando o programa.

    // Removendo as chamadas dessa função resolvemos o problema pelo primeiro
    // caminho citado no moodle, mas não consegui entender quais seriam as 
    // consequências negativas dessa abordagem. Pode me explicar nos comentários
    // da correção? 
    if (arg < 2) {
        gValue += arg;
    } else {
        compute(arg - 1);
        compute(arg - 2);
    }
}

// Função wrapper que pode ser usada com pthread_create() para criar uma 
// thread que retorna o resultado de compute(arg
void* compute_thread(void* arg) {
    int* ret = malloc(sizeof(int));
    pthread_mutex_lock(&gMtx);
    gValue = 0;
    compute(*((int*)arg));
    *ret = gValue;
    pthread_mutex_unlock(&gMtx);
    return ret;
}


int main(int argc, char** argv) {
    // Temos n_threads?
    if (argc < 2) {
        printf("Uso: %s n_threads x1 x2 ... xn\n", argv[0]);
        return 1;
    }
    // n_threads > 0 e foi dado um x para cada thread?
    int n_threads = atoi(argv[1]);
    if (!n_threads || argc < 2+n_threads) {
        printf("Uso: %s n_threads x1 x2 ... xn\n", argv[0]);
        return 1;
    }

    //Inicializa o mutex
    pthread_mutex_init(&gMtx, NULL);

    int args[n_threads];
    int* results[n_threads];
    pthread_t threads[n_threads];
    //Cria threads repassando argv[] correspondente
    for (int i = 0; i < n_threads; ++i)  {
        args[i] = atoi(argv[2+i]);
        pthread_create(&threads[i], NULL, compute_thread, &args[i]);
    }
    // Faz join em todas as threads e salva resultados
    for (int i = 0; i < n_threads; ++i)
        pthread_join(threads[i], (void**)&results[i]);

    // Não usaremos mais o mutex
    pthread_mutex_destroy(&gMtx);

    // Imprime resultados na tela
    // Importante: deve ser chamada para que a correção funcione
    imprimir_resultados(n_threads, results);

    // Faz o free para os resultados criados nas threads
    for (int i = 0; i < n_threads; ++i) 
        free(results[i]);
    
    return 0;
}
