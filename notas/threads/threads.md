# Threads

Threads são tarefas de um processo a serem executadas, possivelmente de forma concorrente. Um programa ao ser inicializado cria um processo, que cria uma única thread. Novas threads podem ser criadas dentro do código do programa, assim podemos ter múltiplas threads dentro de um mesmo processo.

Em sistemas com múltiplos núcleos de processamento, um processo pode ter várias threads ativas ao mesmo tempo, utilizando-se de paralelismo real.

### Compartilhamento de recursos

As threads de um mesmo processo compartilham:

- Espaço de endereçamento
- Variáveis globais
- Arquivos abertos
- Sinais
- Informações de contabilidade do processo

São coisas únicas para cada thread:

- Program counter
- Registradores
- Stack
- Estado

### Troca de contexto

Uma troca de contexto acontece quando uma thread deixa de ser executada para que outra tome o seu lugar, existem trocas de contexto parciais e completas. 

Durante uma troca de contexto parcial, duas threads de um mesmo processo trocam de estado de execução, quando isso acontece registradores, PC e a pilha devem ser salvos. A troca de contexto parcial é muito mais leve que a troca completa.

Durante uma troca de contexto completa, duas threads de processos diferentes irão atuar, quando isso acontece TODOS os atributos do processo antigo devem ser salvos, e todos os atributos do processo novo carregados.

### Escalonamento por thread vs Escalonamento por processo

Um SO pode abordar dois tipos de escalonamento, por thread ou por processo. Ao fazer um escalonamento por processo, temos uma arquitetura mais fixa quanto a ordem de execução de threads, já que apenas threads do processo que está sendo executado no momento podem ganhar tempo de CPU.

![Escalonamento por processo](https://imgur.com/kxRKVA9.png)

Ao optar por um escalonamento por threads, qualquer thread de qualquer processo pode a qualquer momento ganhar tempo de CPU para executar.

![Escalonamento por thread](https://imgur.com/CDxfmr2.png)

### Suporte para threads

Em relação ao SO, threads podem ser criadas nativamente em sistemas operacionais modernos, mais especificamente para sistemas Linux a partir do kernel 2.6, e para o Windows desde a versão 95;

Quanto a bibliotecas de desenvolvimento, existem diversos fornecedores de APIs, sendo uma das mais famosa POSIX threads.

Por último, você precisará de uma linguagem de programação compatível com a criação de múltiplas threads como C, Java, Go e outras tantas, já que a maioria das linguagens modernas possui essa funcionalidade.

# POSIX (Portable Operating System Interface)

POSIX é um padrão aberto de interface de operação utilizado mundialmente, mantido pela IEEE e reconhecido pela ISO e ANSI, pode possuir operações nas mais diversas linguagens, por exemplo na linguagem C temos a implementação `pthread.c`.

### Principais funções da biblioteca `pthread.c`

Para criar uma thread:

```c
    pthread_create(<thread>, <atrib>, <rotina>, <args>);
```

Para obter identificação da thread

```c
    pthread_self();
```

Para suspender a execução de uma thread

```c
    pthread_delay_np(<tempo>);
```

Para definir um ponto de junção (aguardar o término de outra thread)

```c
    pthread_join(<thread>, <retorno_thread>);
```

Para finalizar a thread

```c
    pthread_exit(<retorno>);
```

### Exemplo de funcionamento

No código do arquivo [thread_example.c](./thread_example.c), você pode ver o fluxo de criação, execução, espera e finalização de uma thread, algo que segue o fluxograma a seguir:

![Fluxograma de execução de thread_example.c](https://imgur.com/MZMyLCn.png)

### Escopo de variáveis

```c
#include <stdio.h>

int x; // variável global, pode ser usada em ambas as threads

void *func_thread1 (void *arg) {
    int t1; // não é visível dentro de func_thread2
    pthread_exit(NULL);
}

void *func_thread2 (void *arg) {
    int t2; // não é visível dentro de func_thread1
    pthread_exit(NULL);
}
```

### Threads no Windows

Sim, você leu certo, as POSIX threads não funcionam no Windows, então se você quiser desenvolver no Windows a API vai ser diferente.

Para criar uma thread

```c
CreateThread(<atribs>, <tam_stack>, <rotina>, <params>, <flags>, <thread_id>)
```

Obter identificação da thread

```c
GetCurrentThreadId()
```

Suspender execução de thread

```c
SuspendThread(<thread>)
```

Finalizar a thread

```c
ExitThread(<retorno>)
```

### Fatorial em threads

No exemplo que está no arquivo [multithread.c](./multithread.c), podemos ver uma implementação que calcula os fatoriais de 0 a N utilizando N threads diferentes, apesar dessa não ser uma boa aplicação para threads, podemos abstrair bastante coisas dela.