# Mutex (**Mut**ual **Ex**clusion)

O mutex é um tipo abstrato de dados, que possui como atributos principais um `estado` e uma `fila`. O estado indica se essa região está livre ou bloqueada. A fila é realmente uma implementação de `Queue` que guarda os identificadores das tarefas que esperam pra acessar a região crítica.

Mutexes devem ser usados com cuidado, já que ele limita muito a concorrência e impõe uma execução serial, diminuindo o desempenho do programa. Também é MUITO importante lembrar de destravar o mutex ao deixar de usar a região crítica, do contrário a região ficará bloqueada.

### Implementação de Mutex com `pthread`

É uma boa ideia declarar os mutexes com escopo global, para que todas as threads consigam acessar ele.

Declaração de um mutex

```c
pthread_mutex_t mutex;
```

Inicialização

```c
pthread_mutex_t mutex;
pthread_mutex_init(&mutex, attr);
```

Aquisição da trava

```c
pthread_mutex_t mutex;
pthread_mutex_lock(&mutex);
```

Tentativa de aquisição da trava, sem bloqueio

```c
pthread_mutex_t mutex;
pthread_mutex_trylock(&mutex);
```

Liberação da trava

```c
pthread_mutex_t mutex;
pthread_mutex_unlock(&mutex);
```

Finalização do mutex

```c
pthread_mutex_t mutex;
pthread_mutex_destroy(&mutex);
```

### Exemplo de implementação: contador

Esse exemplo é uma revisita ao exercício 1 do trabalho sobre threads, onde tinhamos um problema de condição de corrida, onde várias threads implementavam uma contagem sobre um contador global, ao aumentar o número de threads e de iterações por thread, existia uma diferença crescente entre a contagem final e a contagem esperada.

```c
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

int contador_global = 0;
pthread_mutex_t mutex;

void* thread(void* argv) {
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
        pthread_create(&threads[i], NULL, thread, (void *) &n_loops);
    
    for (size_t i = 0; i < n_threads; i++)
        pthread_join(threads[i], NULL);

    printf("Contador: %d\n", contador_global);
    printf("Esperado: %d\n", n_threads*n_loops);
    
    pthread_mutex_destroy(&mutex);

    return 0;
}
```

Utilize o [`Makefile`](./Makefile) junto com o comando `make` para compilar o programa localizado [aqui](./main.c)

Agora, preste atenção no que estamos fazendo, criamos um programa onde a única operação é incrementar um contador N vezes em M threads, gerando uma contagem final com valor `N * M`. Perceba que nossa única operação repetida muitas vezes é realizada dentro da região crítica criada, então será que seria mais rápido usar uma thread só? Claro que sim. Criar a trocar entre threads tem um custo. Abaixo deixarei um benchmark da versão utilizando apenas threads (onde existe um erro no cálculo final), da versão utilizando threads e mutex (onde o resultado está correto) e da versão puramente serial.

#### Versão com mutexes

![Versão com mutexes](https://imgur.com/IWgxk7S.png)

#### Versão com threads sem controle de concorrência

![Versão com threads sem controle de concorrência](https://imgur.com/5pJbR6U.png)

#### Versão serial

![Versão serial](https://imgur.com/FSOLXM8.png)

A versão serial foi em média 10 vezes mais rápida do que as outras versões. Isso significa que concorrência é inútil, ruim e não devo aprender? Não, apenas significa que o problema não é um bom exemplo para resolução concorrente.