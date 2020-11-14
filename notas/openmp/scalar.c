#define VECTOR_SIZE 20000

#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void generate_random_array(int* arr) {
    srand(time(NULL));
    
    for (size_t i = 0; i < VECTOR_SIZE; i++){
        arr[i] = rand() % 20; 
    }
}

int main(int argc, char const *argv[])
{
    int a[VECTOR_SIZE] = {0}, b[VECTOR_SIZE] = {0};
    
    generate_random_array(a);
    generate_random_array(b);

    clock_t begin = clock();

    int sum = 0;
    #pragma omp parallel for shared(a, b) schedule(static) reduction(+:sum)
    for (size_t i = 0; i < VECTOR_SIZE; i++){
        sum += a[i] * b[i];
    }

    clock_t end = clock();

    float time_spent = (float) (end - begin) / CLOCKS_PER_SEC;
    printf("O produto escalar paralelo resultou em %d e levou %f pra rodar\n", sum, time_spent);

    clock_t serial_begin = clock();

    int serial_sum = 0;
    for (size_t i = 0; i < VECTOR_SIZE; i++){
        serial_sum += a[i] * b[i];
    }

    clock_t serial_end = clock();
    float serial_time_spent = (float) (serial_end - serial_begin) / CLOCKS_PER_SEC;

    printf("O produto escalar serial resultou em %d e levou %f pra rodar\n", serial_sum, serial_time_spent);
    
    return 0;
}
