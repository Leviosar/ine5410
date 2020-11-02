#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

void create_proccess() {
    pid_t process = fork();

    if (process < 0) {
        fprintf(stderr, "Falha ao realizar fork\n");
    }

    if (process == 0) { 
        printf("Processo filho %d criado\n", getpid());
        exit(0);
    }

    printf("Processo pai criou %d\n", process);
}

int main(int argc, char** argv) {
    for (size_t i = 0; i < 2; i++)
    {
        create_proccess();
    }

    wait(NULL);

    printf("Processo pai finalizado!\n");   

    return 0;
}