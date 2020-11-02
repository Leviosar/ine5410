#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int contador_global = 0;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("n_threads é obrigatório!\n");
        printf("Uso: %s n_threads n_loops\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);
    int n_loops = atoi(argv[2]);

    for (size_t i = 0; i < n_threads; i++)
        for (size_t i = 0; i < n_loops; i++)
            contador_global += 1;

    printf("Contador: %d\n", contador_global);
    printf("Esperado: %d\n", n_threads * n_loops);

    return 0;
}
