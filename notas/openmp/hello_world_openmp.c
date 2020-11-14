#include <omp.h>
#include <stdio.h>

int main() {
    #pragma omp parallel
    printf("Hello World paralelo!\n");
    printf("Aqui voltou para a região normal\n");

    #pragma omp parallel 
    {
        printf("Primeiro!\n");
        printf("Segundo!\n");
    }
}