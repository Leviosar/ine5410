#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int children_count = 2;
int grandchildren_count = 3;

void create_child() {
    printf("Processo %d, filho de %d\n", getpid(), getppid());
    fflush(stdout);

    pid_t child = fork();

    if (child == 0)
    {
        for (size_t i = 0; i < grandchildren_count; i++)
        {
            pid_t grandchild = fork();

            if (grandchild == 0)
            {
                printf("Processo %d, filho de %d\n", getpid(), getppid());
                sleep(5);
                printf("Processo %d finalizado\n", getpid());
                exit(0);
            }
        }
        
        wait(NULL); 
        printf("Processo %d finalizado\n", getpid());
        exit(0);
    }
}

int main(int argc, char** argv) {
    for (size_t i = 0; i < children_count; i++)
    {
        create_child();
    }
    
    wait(NULL);
    printf("Processo principal %d finalizado\n", getpid());    
    return 0;    
}
