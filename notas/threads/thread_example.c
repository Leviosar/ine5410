#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void *thread_function(void *arg) {
    printf("thread_function está executando \n");
    pthread_exit("Obrigado pelo tempo de CPU! \n");
}

int main(int argc, char const *argv[])
{
    int res;
    pthread_t a_thread;
    void* thread_result;


    res = pthread_create(&a_thread, NULL, thread_function, NULL);

    res = pthread_join(a_thread, &thread_result);

    printf("thread joined, retornou %s", (char*) thread_result);

    // printf("mensagem agora é: %s", message);

    exit(EXIT_SUCCESS);

    return 0;
}
