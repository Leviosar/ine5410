#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);

// Avalia o resultado no vetor c. Assume-se que todos os ponteiros (a, b, e c)
// tenham tamanho size.
void avaliar(double* a, double* b, double* c, int size);

typedef struct operation_t
{
    double* a;
    double* b;
    double* c;
    int lower_bound;
    int upper_bound;
} operation;

void* thread(void* argv) {
    operation* op = (operation*) argv;

    for (size_t i = op->lower_bound; i < op->upper_bound; i++)
    {
        op->c[i] = op->a[i] + op->b[i];
    }
    
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    // Gera um resultado diferente a cada execução do programa
    // Se **para fins de teste** quiser gerar sempre o mesmo valor
    // descomente o srand(0)
    srand(time(NULL)); //valores diferentes
    srand(0);        //sempre mesmo valor

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n",
               argv[0]);
        return 1;
    }
  
    //Quantas threads?
    int n_threads = atoi(argv[1]);
    
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }
    
    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    
    double* a = load_vector(argv[2], &a_size);
    
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    
    double* b = load_vector(argv[3], &b_size);
    
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    //Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }

    //Cria vetor do resultado 
    double* c = malloc(a_size*sizeof(double));

    pthread_t threads[n_threads];
    operation ops[n_threads];
    int sums_per_thread = (int) a_size / (int) n_threads;

    if (n_threads > a_size) n_threads = a_size;

    for (size_t i = 0; i < n_threads; i++)
    {   
        ops[i].a = a;
        ops[i].b = b;
        ops[i].c = c;
        ops[i].lower_bound = i * sums_per_thread;
        ops[i].upper_bound = i * sums_per_thread + sums_per_thread;

        if (i == n_threads - 1)
            ops[i].upper_bound = a_size;
    }

    operation* op_pointer = ops;

    for (size_t i = 0; i < n_threads; i++)
        pthread_create(&threads[i], NULL, thread, op_pointer + i);

    for (size_t i = 0; i < n_threads; i++)
        pthread_join(threads[i], NULL);

    avaliar(a, b, c, a_size);

    free(a);
    free(b);
    free(c);

    return 0;
}
