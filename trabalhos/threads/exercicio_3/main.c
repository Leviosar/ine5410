#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

double* load_vector(const char* filename, int* out_size);

void avaliar(double* a, double* b, int size, double prod_escalar);

typedef struct operation_t
{
    double* a;
    double* b;
    double* results;
    int lower_bound;
    int upper_bound;
    int index;
} operation;

void* thread(void* argv) {
    operation* op = (operation*) argv;
    
    op->results[op->index] = 0;
    
    for (size_t i = op->lower_bound; i < op->upper_bound; i++)
        op->results[op->index] += op->a[i] * op->b[i];
    
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    // Verifica a quantidade de argumentos necessários
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n", 
               argv[0]);
        return 1;
    }
  
    // Verifica a quantidade de threads a serem usadas
    int n_threads = atoi(argv[1]);
    
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }

    // Aloca os vetores presentes no arquivo para um vetor com malloc
    int a_size = 0, b_size = 0;
    
    double* a = load_vector(argv[2], &a_size);
    
    if (!a) {
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    
    double* b = load_vector(argv[3], &b_size);
    
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    // Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }

    gettimeofday(&end, NULL);
    printf("Inicialização (serial): %f seconds\n", (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 0.000001);
    gettimeofday(&start, NULL);

    double result = 0;

    pthread_t threads[n_threads];
    operation ops[n_threads];
    int operations_per_thread = (int) a_size / (int) n_threads;

    if (n_threads > a_size) n_threads = a_size;
    double* c = malloc(n_threads * sizeof(double));

    for (size_t i = 0; i < n_threads; i++)
    {   
        ops[i].a = a;
        ops[i].b = b;
        ops[i].results = c;
        ops[i].index = i;
        ops[i].lower_bound = i * operations_per_thread;
        ops[i].upper_bound = i * operations_per_thread + operations_per_thread;

        if (i == n_threads - 1)
            ops[i].upper_bound = a_size;
    }

    operation* op_pointer = ops;

    for (size_t i = 0; i < n_threads; i++)
        pthread_create(&threads[i], NULL, thread, op_pointer + i);

    for (size_t i = 0; i < n_threads; i++)
        pthread_join(threads[i], NULL);

    gettimeofday(&end, NULL);
    printf("Multiplicação vetorial (paralela): %f seconds\n", (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 0.000001);
    gettimeofday(&start, NULL);

    for (size_t i = 0; i < n_threads; i++)
        result += c[i];    
    
    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, a_size, result);

    //Libera memória
    free(a);
    free(b);
    free(c);

    gettimeofday(&end, NULL);
    printf("Avaliação (serial): %f seconds\n", (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 0.000001);

    return 0;
}
