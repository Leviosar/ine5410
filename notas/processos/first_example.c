#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

// Esse código será executado tanto pelo processo pai quanto pelo processo filho
int main() {
    pid_t pid;
    pid = fork();

    // Ao ser executado pela primeira vez pelo processo pai, o PID retornado por
    // `fork` será o do processo filho, fazendo com que a condicional abaixo caia no "else".

    // Ao ser executado como filho que foi criado na primeira execução do processo, o valor de
    // pid será 0, que joga para a condicional do else if.

    // Por fim, na ocorrência de algum erro, o programa entrará na primeira condicional if
    if(pid < 0) {
        // Caso ocorra algum problema na execução do fork, 
        // o PID retornado será um código de erro negativo
        fprintf(stderr, "Falha na execução do fork");
        return 1;
    } else if(pid == 0) {
        // Aqui, o processo filho ao ser executado iá criar um novo processo
        // dessa vez com exec, chamando o programa `ls`
        printf("O processo filho %d está executando\n", getpid());
        fflush(stdout);
        execlp("/bin/ls", "ls", NULL);
    } else {
        // Aqui, o processo pai aguardará o final da execução do processo filho.
        wait(NULL);
        printf("Filho, processo número: %d terminou a tarefa\n", pid);
    }

    return 0;
}