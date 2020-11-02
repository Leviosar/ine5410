# Deadlocks

Quando utilizamos mecanismos de controle de concorrência no código, o mau uso deles pode vir a causar impasses na aplicação, bloqueando parcialmente ou completamente a execução do programa. A vivacidade de um programa (_liveness_) garante que a conclusão de tarefas concorrentes seja concluida em algum momento, um impasse compromete a _liveness_ do programa. Podemos nos referir a esses impasses como _deadlocks_.

Suponha que um processo P1 detenha o recurso A e solicite o recurso B, ao mesmo tempo que um processo P2 detém o recurso B e solicita o recurso A. Os dois processos ficaram bloqueados "para sempre".

## Recursos

Mas o que exatamente é um recurso? Um recurso é uma parte de um sistema, que pode ter de 1 a N instâncias. Exemplos de recursos são endereços de memória, arquivos abertos, dispostivos de entrada e saída e vários outros.

### Recursos preemptíveis

Recursos que podem ser retirados de um processo sem efeitos prejudiciais, como a memória (o conteúdo da memória pode ser salvo em disco pelo SO e depois restaurado).

### Recursos não-preemptíveis

Recursos que não podem ser retirados sem causar uma falha na computação de seu processo, como por exemplo o acesso a uma impressora. Esse tipo de recurso é o que costuma gerar _deadlocks_.

## Condições para que um _deadlock_ ocorra

1. Exclusão mútua
    - Apenas uma tarefa por vez acessando o recurso
2. Espera
    - A tarefa em questão que usa o recurso está esperando por outro recurso
3. Recurso não-preemptível
    - Recurso que não pode ser retirado a força de uma tarefa
4. Espera circular
    - Um ciclo de 2 ou mais tarefas, cada uma esperando por um recurso em uso pela próxima tarefa do ciclo

## Estados seguros e inseguros

O sistema encontra-se em um `estado seguro` se existe uma sequência <T1, T2, T3, ..., TN> de todas as tarefas do sistema tal que:

- Para cada tarefa Ti, os recursos que Ti pode requisitar podem ser satisfeitos pela união dos recursos disponíveis e dos recursos em uso por Tj, para `j < i`

Se o sistema está em um `estado seguro`, temos certeza que é impossível acontecer um _deadlock_.

Por sua vez um `estado inseguro` se dá quando a condição acima não é satisfeita. O sistema estar em um `estado inseguro` não significa exatamente que ele irá cair em um _deadlock_, mas sim que ele PODE cair em um _deadlock_.

Dessa forma, uma das maneiras de garantir que seu programa não terá _deadlocks_ é garantir que ele sempre esteja em `estados seguros`.

## Estratégias para tratar _deadlocks_

1. Ignorar completamente o problema

2. Prevenção

3. Anulação

### Ignorando o problema

Simplesmente ignore o problema, pronto.

Pegadinha, lembre-se que isso só deve ser feito em sistemas não críticos e onde a perda de dados em decorrência do _deadlock_ não é tão grande. Se você estiver trabalho com um sistema crítico PELO AMOR DE DEUS NÃO SIGA ISSO.

### Prevenção

É uma boa ideia tentar eliminar algumas das condições necessárias pra que ocorra um _deadlock_.

**1. Eliminando condição de exclusão mútua**

Se nunca tivermos uma exclusão mútua, é impossível acontecer _deadlocks_. Lembre-se que recursos _read-only_ não precisam de travas, já que o recurso em si não poderá ser alterado por nenhuma das tarefas que o usam. Dessa forma, eliminando as travas iremos impedir a ocorrência de um _deadlock_ naquele ponto do programa. 

**2. Eliminando condições de posse/espera**

Você pode exigir que um processo requisite todos os recursos que ele necessita antes de começar a executar, assim ele nunca mais irá precisar de outro recurso após iniciar a execução. Essa abordagem diminui a concorrência total do programa, além disso uma tarefa pode não saber exatamente quais os recursos que irá acessar.

**3. Eliminando a condição de não-preempção**

Se uma tarefa está usando recursos e requisita outros que não podem ser utilizados imediatamente, a tarefa simplesmente libera todos os seus recursos e pode tentar novamente mais tarde. Ou seja, transformamos todos os recursos em preemptíveis.

Essa opção também pode nos trazer problemas, alguns recursos não são preemptáveis, não importa o que você ache. Por exemplo, pense numa tarefa que está em posse de uma impressora, mas requisita um outro recurso bloqueado no meio de sua execução, segundo nosso princípio vamos agora remover a força a posse da impressora dessa tarefa. Isso pode levar a um defeito na próxima tarefa que peça acesso a impressora, já que ela pode ter começado a imprimir algo em sua tarefa anterior.

**4. Eliminando a condição de espera circular**

**Solução 1**: cada processo só pode estar em posse de 1 recurso por vez. Essa solução é bem limitada e não pode ser usada em qualquer caso, já que existem tarefas que exigirão o uso simultâneo de 2 ou mais recursos.

**Solução 2**: definimos uma regra onde cada recurso possui um identificador único global, sempre que uma tarefa requisitar o uso de um recurso, geramos um grafo de alocação de recursos e vemos se ele possui ciclos. Podemos implementar isso com duas regrinhas base.

- Impomos uma ordem total para todos os tipo de recursos e requer que as tarefas solicitem os recursos em ordem crescente.

- Para requisitar um recurso `Rj`, um processo deve primeiro liberar todos os recursos `Ri` tal que `i >= j`

### Anulação 

Anular a ocorrência de _deadlocks_ é simplesmente implementar uma das prevenções acima por todo o código, em sistemas maiores e mais complexos isso pode ser extremamente exaustivo, por isso que sistemas operacionais modernos como Linux e Windows (pfff) não implementam esse tipo de solução.

Cada uma das soluções de prevenção tem seu custo e eles devem ser levados em conta antes de implementados.

## Inanição (_starvation_)

Caso ao tentar tratar _deadlocks_ você faça alguma cagada, pode ser que cause _starvation_ em alguma tarefa, isso significa que essa tarefa nunca conseguirá acessar os recursos por algum motivo qualquer.

Podemos resolver problemas de _starvation_ de duas formas principais:

1. Um algoritmo para alocar recursos para tarefas mais curtas primeiro
    - Funciona bem para múltiplas tarefas curtas em um sistema
    - Tarefas longas podem ficar muito tempo sem serem executadas
        - Mesmo não estando bloquedas, seguem numa espécia de _starvation_

2. Quem chegar primeiro é servido primeiro (FIFO)

## _Livelock_ (precisamos começar a ser mais criativos)

Caso durante a sua execução uma tarefa não progrida devido à mudança de estados constantes em outras tarefas, prejudicando seu avanço.

A tarefa continua a executar, mas não consegue seus recursos para concluir sua execução. Em casos mais extremos pode levar a _starvation_.

Um _livelock_ é ainda pior que um _deadlock_, já que como as tarefas que não estão realmente bloqueadas, elas continuam gastando tempo de CPU sem avançar na sua computação. Além disso um _livelock_ não é detectável como um _deadlock_.

# O Jantar dos Filósofos

Problema proposto pro Djikstra como exercício de uma prova.

Suponha uma mesa circular de N filósofos, sentados a frente de N pratos e N garfos. Os filósofos podem realizar duas ações, nunca simultâneamente: `pensar` e `comer`. 

- Cada filósofo precisa de 2 garfos para comer (o spaguetti servido estava muito escorregadio)
- Ele só pode pegar um garfo por vez. 
- Após um tempo comendo, os filósofos soltam o garfo e voltam a pensar. 
- Todos os filósofos precisam eventualmente `comer` e `pensar`.

## Solução 1

Uma possível implementação (falha) de um programa multithreading do jantar dos filósofos seria:

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N 5

pthread_mutex_t garfos[N];

void comer(int posicao) {
    printf("Filósofo %d começou a comer\n", posicao);
    fflush(stdout);
    sleep(2);
}

void pensar(int posicao) {
    printf("Filósofo %d começou a pensar\n", posicao);
    fflush(stdout);
    sleep(3);
}

void* filosofo(void* arg) 
{
    int posicao = *((int *) arg); // recebe a posição do filósofo na mesa

    while(1) {
        pensar(posicao);
        pthread_mutex_lock(&garfos[posicao]);
        printf("Filósofo %d pegou o garfo %d\n", posicao, posicao);
        fflush(stdout);
        pthread_mutex_lock(&garfos[(posicao + 1) % N]);
        printf("Filósofo %d pegou o garfo %d\n", posicao, posicao + 1);
        fflush(stdout);
        comer(posicao);
        pthread_mutex_unlock(&garfos[posicao]);
        pthread_mutex_unlock(&garfos[(posicao + 1) % N]);
    }
}

int main () {
    for(int i = 0; i < N; i++)
        pthread_mutex_init(&garfos[i], NULL);

    pthread_t filosofos[N];
    int indices[N];

    for(int i = 0; i < N; i++) {
        indices[i] = i;
        pthread_create(&filosofos[i], NULL, filosofo, (void*) &indices[i]);
    }

    for(int i = 0; i < N; i++)
        pthread_join(filosofos[i], NULL);
    
    for(int i = 0; i < N; i++)
        pthread_mutex_destroy(&garfos[i]);
    
    return 0;
}
```

Só tem um probleminha: essa implementação vai te levar a um possível caso de _deadlock_, pode ser que você rode esse programa por horas e ele não trave, mas pode ser que ele trave sem ninguém comer. Bom, como caímos num caso de _deadlock_ podemos relembrar de como se resolvem esses problemas.

## Solução 2

Podemos tentar eliminar a condição de não preempção, dessa forma um filósofo tenta pegar o primeiro garfo, caso consiga, tenta pegar o segundo, nesse caso se ele falhar em pegar o segundo garfo, deve soltar o primeiro e voltar a pensar. Essa solução também possui problemas, pois pode levar a um _livelock_ onde cada filósofo está pegando e soltando um garfo sem nenhum dos filósofos consegue pegar os 2 para comer. Funciona bem para cargas de trabalho com pouca utilização do recurso.

## Solução 3

A terceira solução é um pouco mais complicada, nela precisaremos criar um `estado` para cada filósofo, assim como também precisaremos de uma maneira de acessar os filósofos vizinhos de um determinado filósofo. 

# Leitores e escritores

Esse problema traz dois atores, leitores e escritores, que podem existir de 1 a N instâncias, dependendo do enunciado. 

**1. Escritores**
    - Acessam dados para modificação
    - As modificações tem que ser feitas com acesso exclusivo

**2. Leitores**
    - Acessam dados apenas para visualização
    - Precisam de um dado consistente (que não esteja sendo escrito por escritores)
    - Vários leitores podem ler os dados ao mesmo tempo, já que esses não alteram os dados.

Uma solução possível é utilizar um semáforo binário para barrar a escrita simultânea e reutilizar esse mesmo semáforo para que o escritor não consiga escrever enquanto existem leitores acessando os dados. Dessa forma, o primeiro leitor a entrar na área crítica irá ocupar o semáforo, e o último leitor a sair irá desocupar o semáforo. Além disso, cada escritor ao começar sua execução deve ocupar a região crítica também.

Para problemas que envolvam execução infinita ou leitores infinitos, essa solução é inválida pois pode ocorrer _starvation_ do escritor caso não parem de chegar leitores. Mas para problemas com números baixos de leitores e escritores, a solução é válida.

Essa solução está implementada abaixo e no arquivo [readers_writers.c](./readers_writers.c)

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int reader_count = 0;
int data = 0;

sem_t write_lock, count_lock;

void write_f() {
    data += 1;
    sleep(1);
}

void read_f() {
    sleep(1);
}

void * reader(void* arg) {
    int index = *((int*) arg);

    sem_wait(&count_lock);

    reader_count += 1;
    
    if (reader_count == 1) {
        sem_wait(&write_lock);
    }

    sem_post(&count_lock);

    printf("Reader %d read the data: %d\n", index, data);
    read_f();

    sem_wait(&count_lock);
    
    reader_count -= 1;
    if (reader_count == 0)
        sem_post(&write_lock);        
    
    sem_post(&count_lock);
}

void * writer(void* arg) {
    int index = *((int*) arg);
    
    sem_wait(&write_lock);

    printf("Writer %d is writing data\n", index);
    write_f();

    sem_post(&write_lock);
    
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso: %s writers readers\n", argv[0]);
        return 0;
    }

    int writers_count = atoi(argv[1]);
    int readers_count = atoi(argv[2]);

    sem_init(&write_lock, 0, 1);
    sem_init(&count_lock, 0, 1);

    pthread_t readers[readers_count], writers[writers_count];
    int readers_args[readers_count], writers_args[writers_count]; 

    for (size_t i = 0; i < writers_count; i++) {
        writers_args[i] = i;
        pthread_create(&writers[i], NULL, writer, (void *) &writers_args[i]);
    }

    for (size_t i = 0; i < readers_count; i++) {
        readers_args[i] = i;
        pthread_create(&readers[i], NULL, reader, (void *) &readers_args[i]);
    }
    
    for (size_t i = 0; i < writers_count; i++)
        pthread_join(writers[i], NULL);    
    
    for (size_t i = 0; i < readers_count; i++)
        pthread_join(readers[i], NULL);    

    sem_destroy(&write_lock);
    sem_destroy(&count_lock);

    return 0;
}
```