# Controle de concorrência

Como visto antes, quando mais de uma tarefa (pode ser processo ou thread) acessa uma recurso específico ao mesmo tempo, podem ocorrer inconsistências no resultado final do código, as chamadas condições de corrida.

Pra lidar com isso, podemos criar mecanismos de controle de concorrência, pra evitar que mais de uma tarefa acesse um __dado crítico__ em específico, restringindo o acesso e criando uma __região crítica__. Pode ser uma área de memória compartilhada, um arquivo compartilhado e etc.

Uma tarefa que deseja acessar uma região crítica, primeiramente precisa solicitar acesso, caso concedido ela poderá fazer suas operações, e ao final a tarefa PRECISA dizer que saiu da região crítica, do contrário a região continuará bloqueada.

![Fluxo de um acesso a região crítica](https://imgur.com/uvBRDo0.png)

Se duas tarefas tentam acessar uma região crítica simultaneamente, uma delas ganhará a vez de acesso, enquanto a outra fica com execução suspensa até que a tarefa inicial pare de utilizar a região crítica.

![Timeline da execução de 2 tarefas que acessam a mesma região crítica](https://imgur.com/hGMv0Zd.png)

Esse tipo de mecanismo pode diminuir a concorrência do programa, suponha que duas threads precisam acessar a mesma região crítica, a `thread 1` começa utilizando essa região porém perde acesso a CPU antes de finalizar e liberar a thread, mesmo que a `thread 2` ganhe acesso a CPU ela não poderá executar pois a `thread 1` continua ocupando a região crítica.

### Mecanismos

- [Mutex (*Mut*ual *Ex*clusion) ou lock](./../mutexes/)
    - Garante que apenas uma tarefa acessa a região crítica por vez, utilizando fila de acesso
- Semáforos
    - Limita o número de tarefas que acessam a região crítica simultaneamente, utilizando uma fila de acesso quando o limite for excedido
- Monitores
    - Delimita regiões críticas no código, impedindo o acesso concorrente nelas