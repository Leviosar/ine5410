# Semáforos

Criados pelo corno do Dijkstra nos anos 60, que resolveu que era uma boa ideia usar termos em holandês pra exemplificar o conceito. Também funciona como um tipo abstrato de dados com dois principais atributos: `value` e `queue`. A semântica é semelhante a dos mutexes, mas com a diferença de que podemos ter N tarefas dentro da região crítica.

Se em mutexes temos `lock` e `unlock`, em semáforos temos `wait` e `post`, sendo que o `wait` solicita uma posição na região crítica, enquanto `post` abre uma posição na região. No original as operações eram respectivamente `proberen` e `verhogen`.

Da mesma forma que nos mutexes, existe um fluxo necessário a ser implementado para acessar uma região crítica, demonstrado abaixo:

![Fluxo de acesso de um semáforo](https://imgur.com/o4FcPgd.png)

O atributo `value` é iniciado como o número máximo de tarefas que poderão acessar a região do semáforo, esse valor é decrementado toda vez que uma tarefa entra na região, caso o valor chegue a 0 nenhuma tarefa poderá entrar.

Dessa forma, o que a operação `wait` faz é decrementar o contador `value` caso ele seja maior que 0. Enquanto `post` irá incrementar o contador.

![Máquina de estados que representa um semáforo](https://imgur.com/hnu3qLZ.png)

### Semáforo binário

Um semáforo binário pode ser comparado a um mutex, pois ele só deveria permitir uma tarefa acessar a região crítica. Mas ainda assim existem diferenças, suponha que um semáforo foi iniciado com `value` = 1, mas nenhuma tarefa executou um `wait` até o momento. Caso em algum ponto do código você execute um `post`, o contador será incrementado para 2, permitindo a entrada de 2 tarefas.

Em um mutex, apenas a tarefa que entrou na região pode liberar a mesma região, mas na implementação de semafóro o mesmo não é verdade.

### Implementação de semáforo POSIX

Biblioteca: `semaphore.h`

- Declaração de um semáforo

```c
sem_t semaforo;
```

- Inicialização de um semáforo

```c
sem_t semaforo;
sem_init(&semaforo, compart_filhos, valor_inicial);
```

- Tentativa de entrar na região crítica (entrando em fila caso necessário)

```c
sem_t semaforo;
sem_wait(&semaforo);
```

- Tentativa de entrar na região crítica (sem entrar em file)

```c
sem_t semaforo;
sem_trywait(&semaforo);
```

- Retorna o valor do contador do semáforo

```c
sem_t semaforo;
sem_getvalue(&semaforo);
```

- Libera uma posição do semáforo

```c
sem_t semaforo;
sem_post(&semaforo);
```

- Destrói o semáforo

```c
sem_t semaforo;
sem_destroy(&semaforo);
```

### Exemplo de implementação: uso concorrente de N licença de software

Suponha que existe uma licença de uso de software que permite que você só use ela em N tarefas simultaneamente (que licença idiota). Você quer escrever um programa concorrente que utiliza de alguma forma essa licença em cada uma de suas threads. 

A primeira solução seria criar N mutexes e controlar separadamente o acesso a cada licença, mas além de gerar repetição de código, essa implementação teria problemas de performance.

A segunda solução é criar um semáforo que permite N entradas, muito mais bonito e eficiente. A implementação está abaixo para consulta. Nesse caso, a lógica do uso de licença foi abstraída para um simples `sleep(4)` apenas por motivos didáticos (e preguiça).

```c
#define NUM_THREADS 10
#define NUM_LICENSES 2

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t licenses;

void* thread(void* arg) {
    int id = *((int *) arg);
    sem_wait(&licenses);
    printf("Thread %d is using license\n", id);
    sleep(4);
    printf("Thread %d is releasing license\n", id);
    sem_post(&licenses);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    sem_init(&licenses, 0, NUM_LICENSES);

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread, (void *) &ids[i]);
    }
    
    for (size_t i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&licenses);    

    return 0;
}

```