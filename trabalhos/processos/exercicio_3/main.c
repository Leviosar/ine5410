#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

void sed() {
    pid_t sed = fork();
    
    if (sed == 0)
    {
        printf("sed PID %d iniciado\n", getpid());
        fflush(stdout);
        execlp("/bin/sed", "sed", "-i", "-e", "s/silver/axamantium/g;s/adamantium/silver/g;s/axamantium/adamantium/g", "text", NULL);
        exit(0);
    }
}

void grep() {
    pid_t grep = fork();

    if (grep == 0)
    {
        printf("grep PID %d iniciado\n", getpid());
        fflush(stdout);
        execlp("/bin/grep", "grep", "adamantium", "text", NULL);
        exit(0);
    }
}

int main(int argc, char** argv) {
    printf("Processo pai iniciado\n");
    
    sed();
    wait(NULL);

    grep();
    int grep_exit;
    wait(&grep_exit);
    int grep_status = WEXITSTATUS(grep_exit);

    if (!grep_status)
    {
        printf("grep retornou com código 0, encontrou adamantium\n");        
    } else {
        printf("grep retornou com código %d, não encontrou adamantium\n", grep_status);
    }

    return 0;
}
