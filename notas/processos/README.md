# Processos - 💻

Softwares são executados como processos.

### Ciclo de vida de um processo

1. Novo: o processo está sendo criado.

2. Pronto: o processo foi criado e está pronto para ser executado.

3. Executando: o processo está sendo executado pela CPU.

4. Esperando: o processo foi interrompido durante a execução por um evento externo ou interno, pode ser a necessidade de acesso aos dispositivos IO ou outro evento qualquer.

5. Terminado: o processo finalizou sua execução.

Esse ciclo de vida pode ser demonstrado em uma máquina de estados finitos, como a que está logo abaixo. 

![Ciclo de vida do processo: FSM](https://imgur.com/0LTVNHq.png)

### PCB - Process control block

Cada sistema operacional tem sua maneira de armazenar as informações básicas de um processo utilizando um PCB (Process control block), uma estrutura de dados em memória, e apesar de cada SO tratar esses dados de maneiras diferentes, existem dados comuns a todos os SOs.

- Estado do processo
- Identificador do processor (PID)
- Program counter
- Registradores alocados
- Limites de memória
- Informações de escalonamento
- Arquivos e descritores abertos


Exemplo do PCB do Linux: `task_struct`

```c
    pid t_pid; /* identificador do processo */
    long state; /* estado do processo */
    unsigned int time_slice; /* informação para escalonamento */
    struct task_struct* parent; /* pai do processo */
    struct list_head children; /* filhos do processo */
    struct files_struct* files; /* lista de arquivos abertos */
    struct mm_struct* mm; /* espaço de endereço do processo */
```

### Escalonamento de processo

O escalonamento é o "processo" em que um programa chamado `escalonador` define  quais outros processos serão executados em uma CPU.

O objetivo principal do escalonamento é manter um bom uso de CPU, ou seja, fazer com que o tempo de processamento ocioso seja reduzido e que os processos que visam utilizar a CPU sejam "bem atendidos", alternando entre diversos processos com um conceito interno de "justiça" entre processos.

Processos podem perder acesso a CPU a qualquer momento, devido a critérios definidos pelo próprio escalonador, exemplos de motivos da perda de acesso são: chamadas a dispositivos IO, excesso de tempo de processamento, criação de processos filhos.

### Troca de contexto

Um processo possui um estado atual, dados e variáveis que são pertinentes ao contexto no qual o processo está executado. Se por exemplo um processo `P1` está computando uma contagem de 0 a 100000 e é interrompido para que um processo `P2` seja executado, pode ser que seja desejável salvar o valor atual da contagem, assim como ponteiros para instruções e outros dados.

Ao final da execução de `P2`, o sistema operacional irá salvar também o contexto de `P2`, antes de restaurar o último estado conhecido de `P1` para continuar com sua execução. É importante ressaltar que o programador não tem controle sobre essa troca de contexto, ela é executada somente pelo SO.

Essa troca de contexto em si também toma tempo de processamento, que vamos chamar de Δt, quando Δt assume valores muito próximos ao tempo médio de execução de processos, dizemos que a troca de contexto está cara.

## Hierarquia de processos

Quando o SO inicia, um processo privilegiado é criado, ele roda no modo kernel e vai ser responsável por inicializar diversos outros processos importantes para o SO. No Linux, o primeiro processo a ser iniciado é o `init`, no Windows é o `smss.exe`.

Isso gera uma hierarquia de processos, onde um processo pode criar `n` processos, e cada um desses `n` processos podem criar outros também. O processo criador, é denonimado o `pai` dos `n` processos `filhos`.

Ao criar um processo filho, o processo pai pode executar simultâneamente ao filho ou então "pausar" sua execução para esperar a finalização do filho. 

Com relação ao espaço de endereçamento, um processo pode ser cópia do seu pai (quando ele é chamado utilizando `fork()`) ou então um programa completamente diferente (quando é criado com as chamadas do tipo `exec`)

> Tipos de exec são: `execl()`, `execle()`, `execlp()`, `execv()`, `execve()`, `execvp()`

### Fork

- Novo processo é cópia do pai
- Pai e filho seguem executando ao mesmo tempo
- Processo pai recebe o PID do filho
- Processo filho recebe 0

A utilização do `fork` pode falhar, retornando um código de erro condizente.

### Exec

- Carrega um novo arquivo binário na memória
- Com `exec` o processo filho pode rodar um programa diferente do pai

### Término de processos

Um processo pode ser terminado pelo programador, chamando `exit()` e declarando que sua execução foi finalizada, nesse caso o sistema irá destruir todas as informações que o processo estava utilizando. Caso o processo pai esteja esperando (com o `wait()`) o final da execução de um filho e este filho chamar `exit`, o processo pai recebe o PID do filho que executou essa chamada.

> exit e wait são primitivas de sincronização

Além disso, um processo pode ser finalizado externamente, por exemplo um processo pai pode finalizar um de seus filhos a qualquer momento. Alguns SOs não permitem a existência de processos órfãos, então o SO mata os filhos quando um pai é finalizado.

Você pode ver um exemplo de código utilizando esses conceitos [aqui](./first_example.c)

### Outras chamadas relacionadas a processos

Mais algumas chamadas de funções que podem ser úteis ao trabalhar com processos:

- `pid_t waitpid(pid_t pid, int* status, int options)`: espera por um processo filho específico
- `pid_t getpid()`: retorna o PID do processo atual
- `pid_t getppid()`: retorna o PID do processo pai