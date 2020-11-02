#include <time.h>
#include <stdatomic.h>
#include <stdio.h>

//    +============================+
//    |                            |
// ~~~| NÃO MODIFIQUE ESSE ARQUIVO |~~~
//    |                            |
//    +============================+

atomic_int gProduct = 0;

//    +============================+
//    |                            |
// ~~~| NÃO MODIFIQUE ESSE ARQUIVO |~~~
//    |                            |
//    +============================+

//Produz um elemento
int produzir(int value) {
    // equivalente a um gProduct++ protegido por mutex, mas pode ser
    // mais rápido pois o memory_order_relaxed interfere menos com o
    // paralelismo de outras instruções que não o incremento dessa
    // variável
    //
    // AVISO: entender essa função não é necessário para resolver o
    // exercício, não perca muito tempo aqui
    int prod = atomic_fetch_add_explicit(&gProduct, 1, memory_order_relaxed);
    struct timespec ts = {0, (value%50)*1000*1000};
    nanosleep(&ts, NULL);
    return prod;
}

//    +============================+
//    |                            |
// ~~~| NÃO MODIFIQUE ESSE ARQUIVO |~~~
//    |                            |
//    +============================+

//Consome um elemento.
void consumir(int produto) {
    printf("Consumindo %d\n", produto);
    struct timespec ts = {0, (produto%50)*1000*1000};
    nanosleep(&ts, NULL);
}

//    +============================+
//    |                            |
// ~~~| NÃO MODIFIQUE ESSE ARQUIVO |~~~
//    |                            |
//    +============================+
