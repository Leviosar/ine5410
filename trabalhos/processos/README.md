# AF2.1 - Processos (Prática)

Essa atividade é dividida em três exercícios sobre processos.

## Exercício 1

Você deve escrever um programa em C em que o processo pai crie 2 processos filhos (fork()). 

Para cada processo filho criado, o processo pai deve imprimir "Processo pai criou XXX", onde XXX é o PID do processo criado.

Cada processo filho deve apenas imprimir "Processo filho XXX criado", onde XXX é o PID do processo corrente (use a função getpid() para isso).

O processo pai (e apenas ele) deve imprimir "Processo pai finalizado!", somente após os filhos terminarem (use a função wait() para aguardar que os filhos terminem de executar).

## Exercício 2

Você deve escrever um programa C em que:

- O processo principal crie 2 processos filhos.
- Cada um dos processos filhos deve, por sua vez, criar mais três processos.
- Cada processo filho (tanto do processo principal quanto dos criados no passo anterior) deve imprimir "Processo XXX, filho de YYY", onde XXX é o PID do processo em questão e YYY o PID do processo que o criou (use as funções getpid() e getppid() para isso).
- Os processos netos (filhos dos filhos do processo principal) devem, após imprimir esta mensagem, aguardar 5 segundos antes de terminar (use a função sleep() para isso) 
- Os processos que criaram filhos devem aguardar que seus filhos terminem de executar (utilize a função wait()).
- Todos os processos filhos devem imprimir, ao finalizar, "Processo XXX finalizado", onde XXX é o PID do processo em questão. O processo principal deve imprimir "Processo principal XXX finalizado", onde XXX é o PID do processo principal.

## Exercício 3

Uma turma de alunos tem se comportado muito mal. Logo, o professor de "HIST0666 -  História dos Talheres" escreveu um programa para constantemente alternar o conteúdo da Wikipedia sobre utensílios culinários no império romano, de modo a confundir os alunos e semear o caos. O programa deve alternar as palavras silver e adamantium, em um texto, toda vez que for executado.

O programa C deve iniciar imprimindo "Processo pai iniciado" e criar dois processos filhos:

- O primeiro filho (sed) deve:
    - Imprimir "sed PID XXX iniciado", onde XXX é o PID desse processo filho
    - Trocar seu binário (função execlp()) pelo binário sed, de modo a executar o comando sed -i -e s/silver/axamantium/g;s/adamantium/silver/g;s/axamantium/adamantium/g text, onde text é um arquivo texto incluído no .tar.gz inicial
- O segundo filho (grep) deve:
    - Ser iniciado pelo pai apenas após o término do sed
    - Imprimir "grep PID XXX iniciado", onde XXX é o PID desse processo filho
    - Trocar seu binário (função execlp()) pelo binário grep, de modo a executar o comando grep adamantium text, onde text é um arquivo texto incluído no .tar.gz inicial

O pai deve aguardar pelo término dos processos filhos. Não é necessário avaliar o código de retorno (exit status) do sed. No entanto, é necessário avaliar o código de retorno (exit status) do grep. Caso o grep termine com código 0, o processo pai deve imprimir "grep retornou com código 0, encontrou adamantium"; caso contrário, deve imprimir "grep retornou com código XXX, não encontrou adamantium", onde XXX é código de saída do processo filho (grep).

O arquivo text fornecido contém a palavra silver. Na primeira execução do sed, ela será substituída por adamantium e o grep imprimirá a frase contendo adamantium e retornará com código zero. Na segunda execução, o sed trocará adamantium por silver e o grep não imprimirá nenhuma frase, apenas retornará o código 1. Se o arquivo text for removido ou renomeado, grep retornará 2 e imprimirá uma mensagem de erro.

### Atenção - 🚨

- Você deve fazer uma chamada da função wait() ou waitpid() para cada processo filho criado. Esta função deve ser chamada somente depois de todos os filhos do processo em questão terem sido criados. 
- O valor de stat em wait(&stat) ou waitpid(pid, &stat, 0) é um aglomerado de outras variáveis. Use macros como WIFEXITED e WEXITSTATUS, descritas na documentação.
- Garantam que os printfs estão como solicitado e na ordem solicitada
- Há uma lista dos printfs no começo de cada arquivo .c
- A função printf não imprime imediatamente na saída, mas sim em um buffer. Se a saída está conectada em um terminal, cada \n causa um fflush(stdio) O script de correção redireciona a saída do seu programa, portanto não há flush a cada \n.
- Em combinação com fork() , prints podem ocorrer mais de uma vez!
- Os scripts corretores vão avisar quando você esquecer do fflush() e vão dizer onde você deve colocá-lo (antes do fork()).

### Dicas

- Perdido com o fork()? Consulte essa mini-animação aqui.
- Há desenhos em ASCII dentro dos arquivos .c iniciais que complementam a especificação
- A função exit(<status>) pode ser usada para encerrar um processo em qualquer ponto do código. O uso dessa função não é obrigatório
- Para tornar o código mais legível, crie funções separadas para cada tipo de processo filho (uma função para filhos do processo principal, outra função para filhos dos filhos do processo principal)

Você vai precisar das seguintes bibliotecas em seu código:

```c
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
```

### Correção automática

O script de correção (grade-processes.sh) está dentro do esqueleto e deve ser executado na própria pasta onde está. Leia os enunciados com atenção e realize os printfs exatamente como solicitado. Você pode corrigir cada exercício de maneira independente usando o grader dentro daquele exercício.

### Entrega do Exercício

Crie um arquivo .tar.gz criado usando o comando make submission e depois envie o arquivo criado pelo Moodle. O uso do esqueleto fornecido é obrigatório. Em especial, você deve criar suas soluções editando os arquivos dentro das pastas exercicio_.